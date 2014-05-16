#pragma once
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(false);
	ofEnableSmoothing();

    // Initialize variabes, and flags
    baud = 115200;              // Set Serial baud rate
    point = 1;                  // Initialize point postiion
    angle = 20;                 // max Angle of galvos
    position = ofPoint(0,0);    // position of galvo
    resolution = 30;
    radius = 5;

    fileLoaded = false;         // No file is initially loaded
    manualControl = false;      // Manual control is initially off
    connected = false;          // No device is initially connected
    laserFire = false;          // Manual Laser is off
    camMode = false;            // Camera intially off
	bLearnBakground = true;     // Learn background first time camera turns on
	drawILDA = true;            // ILDA will draw in the GUI initially

    positionDraw = ofPoint(ofGetWidth()*.5, ofGetHeight()*.5);      //draw position of laser cursor
    ofSetCircleResolution(resolution);


    /*-------------- ILDA FILES SET UP --------------------- */
    // Update dir object with ilda file path, then sort files
	// File has not been loaded yet
	dir.listDir("ildaFiles/");
	dir.sort();
	nFiles = dir.size();
	fileLoaded = false;

    // Load in first ilda file in directory
    f_index = 0;
    inFile = dir.getName(f_index);
    ildaLoad(inFile);

    /* ------------- SERIAL COMMUNICATION SET UP ---------------*/
    bSendSerialMessage = false;                         // Initialize serial boolean variable
	portSelected = false;
	ofSetLogLevel(OF_LOG_VERBOSE);                      // Set log levels, and load fonts
	serial.listDevices();                               // Find all devices connected to computer
	deviceList = serial.getDeviceList();

    /* ------------- CAMERA VISION SET UP -----------------------*/
    // Allocate memory to store data from video frames
    xCam_max = 320; yCam_max = 240;
    colorImg.allocate(xCam_max,yCam_max);
	grayImage.allocate(xCam_max,yCam_max);
	grayBg.allocate(xCam_max,yCam_max);
	grayDiff.allocate(xCam_max,yCam_max);
    threshold = 80;                     // Set pixel difference threshold

    /* ------------- GUI GRAPHICS SET UP -----------------------*/
    gui();              // GUI initialization

    // Set grid origin slightly to the right, but center height
    gui_ctr.x = float(ofGetWidth())*(0.60);
    gui_ctr.y = float(ofGetHeight())*(0.50);

}

//--------------------------------------------------------------
void testApp::update(){

//    cout<< "camMOde:" << camMode <<endl;
//    cout<< "ILDAmode: " << ILDAmode <<endl;
//    cout << "manualControl: " << manualControl<< endl;
//    cout << "bSendSerial: " << bSendSerialMessage <<endl;
//    cout << "connected: " << connected <<endl;

    // Update screen if in camera mode
    if(camMode){
        vidGrabber.update();
        bNewFrame = vidGrabber.isFrameNew();

        // Check camera for new frame information
        if (bNewFrame){
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
            grayImage = colorImg;                   // Convert pixel information to grayscale

            // Load new background
            if (bLearnBakground){
                grayBg = grayImage;		// copys pixels from grayImage into grayBg (operator overloading)
                bLearnBakground = false;
            }

            // take the abs value of the difference between background and incoming and then threshold:
            grayDiff.absDiff(grayBg, grayImage);
            grayDiff.threshold(threshold);

            // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
            // also, find holes is set to true so we will get interior contours as well....
            contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
        }
    }

    // If connected to device
    // bSendSerialMessage = Actually connected to device, connected = toggle switch on/off
    if(bSendSerialMessage && connected){

        // 1. Manual Mode
        if (manualControl){

            int i = 0;  //index of 5 coordinate
            // Form data packets to sent 5 coordinates
            while(i < 5){
                // Convert GUI Pad x,y coordinates to control signal
                dataPkt[3*i+0] = posCntSgnl(-(position.x-0.5)*2*INT16_MAX);
                dataPkt[3*i+1] = posCntSgnl(-(position.y-0.5)*2*INT16_MAX);
                if(laserFire){  //if laser is on
                    dataPkt[3*i+2] = uint8_t(1);    //fire!
                }
                else{
                    dataPkt[3*i+2] = uint8_t(0);    //don't fire!
                }
                i++;                    // Increment index
            }
            // Write dataPacket to PSoC, size is three bytes
            serial.writeBytes(dataPkt,63);
        }

        // 2. Camera Vision Mode
        if(camMode){
            // Cycle back through points after reaching last index

            int i =0;

            // Send data packets only if contour detected
            if(contourFinder.nBlobs){
                if(multiContours){ //if multiContours mode selected
                    //project Multiple contours
                    for(int j =0; j < contourFinder.nBlobs; j++){

                        // Number of points in the contour
                        int edg_Pts = contourFinder.blobs[j].nPts;
                        c_index = 0;

                        while(c_index < edg_Pts){
                            i = 0;  // Reinitialize index

                            // Form data packets to sent 5 coordinates
                            while((i < 5) && (c_index < edg_Pts)){
                                dataPkt[3*i+0] = uint8_t((-contourFinder.blobs[j].pts[c_index].x/(xCam_max+0.5))*UINT8_MAX);
                                dataPkt[3*i+1] = uint8_t((-contourFinder.blobs[j].pts[c_index].y/(yCam_max+0.5))*UINT8_MAX);

                                // If at last point in contour, turn off laser
                                if(c_index == (edg_Pts-1)){
                                    dataPkt[3*i+2] = uint8_t(0);
                                }
                                else{
                                   dataPkt[3*i+2] = uint8_t(1);
                                }

                                c_index++;           // Increment index
                                i++;                    // Increment index
                            }
                            // Write dataPacket to PSoC, size is four bytes
                            serial.writeBytes(dataPkt,63);
                        }
                    }
                }
                else {          //Output single/first contour

                    int edg_Pts = contourFinder.blobs[0].nPts;      // Number of points in the contour
                    c_index = 0;

                    while(c_index < edg_Pts){
                        i = 0;  // Reinitialize index

                        // Form data packets to sent 5 coordinates
                        while((i < 5) && (c_index < edg_Pts)){
                            dataPkt[3*i+0] = uint8_t((-contourFinder.blobs[0].pts[c_index].x/(xCam_max+0.5))*UINT8_MAX);
                            dataPkt[3*i+1] = uint8_t((-contourFinder.blobs[0].pts[c_index].y/(yCam_max+0.5))*UINT8_MAX);
                            dataPkt[3*i+2] = uint8_t(1);
                            c_index++;           // Increment index
                            i++;                    // Increment index
                        }
                        // Write dataPacket to PSoC, size is four bytes
                        serial.writeBytes(dataPkt,63);
                    }
                }
            }
        }

        // 3. ILDA File Mode
        if (ILDAmode && fileLoaded && !manualControl && !camMode){

            // Cycle back through points after reaching last index
            c_index = 0;
            int i =0;

            while(c_index < nPts){
                i = 0;  // Reinitialize index

                // Form data packets to sent 5 coordinates
                while((i < 5) && (c_index < nPts)){
                    dataPkt[3*i+0] = x_sgl[c_index];
                    dataPkt[3*i+1] = y_sgl[c_index];
                    dataPkt[3*i+2] = l_state[c_index];
                    c_index++;           // Increment index
                    i++;                 // Increment index
                }
                // Write dataPacket to PSoC, size is four bytes
                serial.writeBytes(dataPkt,63);
            }
        }
	}

}

//--------------------------------------------------------------
void testApp::draw(){

    //set background to pretty gray gradient
	ofBackgroundGradient(ofColor::black, ofColor::gray);

    //Update the GUI elements
    guiUpdate();

    ofSetColor(255);  // Set color to white

    //onscreen instructions
    if(camMode){
        string msg;
        msg += "Instructions:\n";
        msg += "Press up and down arrow keys to adjust contour contrast level\n";
        msg += "Press the space bar to reset the background\n";
        myFont.drawString(msg, 210, 20);
    }
    else if (ILDAmode){
        string msg;
        msg += "Instructions:\n";
        msg += "Press \"n\" or right arrow to go to next file, \"b\" or left arrow to go to previous file\n";
        msg += "Toggle \"Draw ILDA\" off to make the framerate faster\n";
        msg += "ILDA files are in \"data/ildaFiles\"";
        myFont.drawString(msg, 210, 20);
    }
    else if (manualControl){
        string msg;
        msg += "Instructions:\n";
        msg += "Turn laser on and off with \"Laser\" toggle\n";
        msg += "Click and drag within 2D pad to control laser point\n";
        myFont.drawString(msg, 210, 20);
    }
    else{
        string msg;
        msg += "Instructions:\n";
        msg += "Click on the \"Serial Devices\" dropdown to select your serial comm port\n";
        msg += "To rescan serial ports, click refresh\n";
        myFont.drawString(msg, 210, 20);
    }

	/* -------- DRAW CURRENT ILDA GRAPHIC ----------- */
	if(ILDAmode && drawILDA && fileLoaded && !manualControl && !camMode ){

        ofSetLineWidth(1);

        // Draw the current idla file in the center of the CUI
        for(int i = 0; i < (nPts-1); i++){

            // Draw circle around "Point" currently selected in point inspector
            if(point == (i+1)){
                ofSetColor(0,255,255);
                ofNoFill(); //draw circle outline, thus nofill
                ofCircle(gui_ctr.x+x_grd[i],gui_ctr.y-y_grd[i],10);
            }

            // If laser if on, draw red line, otherwise white line
            if(l_state[i]){
                ofSetColor(153,0,0);
                ofLine(gui_ctr.x+x_grd[i],gui_ctr.y-y_grd[i], \
                       gui_ctr.x+x_grd[i+1],gui_ctr.y-y_grd[i+1]);
            }
            else if(!l_state[i]){
                ofSetColor(150,150,150);
                ofLine(gui_ctr.x+x_grd[i],gui_ctr.y-y_grd[i], \
                       gui_ctr.x+x_grd[i+1],gui_ctr.y-y_grd[i+1]);
            }
        }

	}

    /* ------- DRAW MANUAL POINT POSITION ----------- */
    if(manualControl){

        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);

        //draw laser cursor on screen
        ofSetColor(255, 100, 100, 150);
        ofFill();
        ofCircle(positionDraw.x,positionDraw.y, radius);
        ofPopStyle();
    }

    /* --------- DRAW EDGE CONTOUR -------------------- */
    if(camMode){

        // Set coordinate for CV video feed
        int x_max = 320;
        int y_max = 240;
        int xCam_ctr = gui_ctr.x - (x_max/2);
        int yCam_ctr = gui_ctr.y;

        // Draw Color Image, All blob contours
        colorImg.draw(xCam_ctr,(yCam_ctr-1.15*y_max));
        contourFinder.draw(xCam_ctr - 0.6*x_max,yCam_ctr);

        //label webcam images
        myFont.drawString("Webcam Image", xCam_ctr+110, yCam_ctr-1.15*y_max-5 );
        myFont.drawString("Blobs", xCam_ctr - 0.6*x_max + 135, yCam_ctr - 5 );
        myFont.drawString("Contours", xCam_ctr + 0.6*x_max + 125, yCam_ctr -5 );

        // Draw first blob
        if(contourFinder.nBlobs){

            if(multiContours){
                //draw multiple blobs
                // Iterate through each blob
                for(int i = 0; i < contourFinder.nBlobs; i++){

                    int edg_pts = contourFinder.blobs[i].nPts;  // Number of points in contour

                    // Iterate through each point in each blob
                    for(int j = 0; j < (edg_pts-1); j++){
                        // Draw contour lines
                        ofLine( xCam_ctr + 0.6*x_max + contourFinder.blobs[i].pts[j].x,     \
                            yCam_ctr + contourFinder.blobs[i].pts[j].y,    \
                            xCam_ctr + 0.6*x_max + contourFinder.blobs[i].pts[j+1].x,  \
                            yCam_ctr + contourFinder.blobs[i].pts[j+1].y);      // Draw connecting lines
                    }

                }

                // Draw border lines
                ofLine( xCam_ctr + 0.6*x_max + 0,      \
                        yCam_ctr + 0,      \
                        xCam_ctr + 0.6*x_max + x_max,    \
                        yCam_ctr + 0);

                ofLine( xCam_ctr + 0.6*x_max + x_max,    \
                        yCam_ctr + 0,      \
                        xCam_ctr + 0.6*x_max + x_max,    \
                        yCam_ctr + y_max);

                ofLine( xCam_ctr + 0.6*x_max + x_max,    \
                        yCam_ctr + y_max,    \
                        xCam_ctr + 0.6*x_max + 0,      \
                        yCam_ctr + y_max);

                ofLine( xCam_ctr + 0.6*x_max + 0,      \
                        yCam_ctr + y_max,    \
                        xCam_ctr + 0.6*x_max + 0,      \
                        yCam_ctr + 0);
            }
            else{
                //draw only the first blob
                int n = contourFinder.blobs[0].nPts;
                for(int i = 0; i < (n-1); i++){

                    // Draw contour lines
                    ofLine( xCam_ctr + 0.6*x_max + contourFinder.blobs[0].pts[i].x,     \
                            yCam_ctr + contourFinder.blobs[0].pts[i].y,    \
                            xCam_ctr + 0.6*x_max + contourFinder.blobs[0].pts[i+1].x,  \
                            yCam_ctr + contourFinder.blobs[0].pts[i+1].y);      // Draw connecting lines
                }

                // Draw border lines
                ofLine( xCam_ctr + 0.6*x_max + 0,      \
                        yCam_ctr + 0,      \
                        xCam_ctr + 0.6*x_max + x_max,    \
                        yCam_ctr + 0);

                ofLine( xCam_ctr + 0.6*x_max + x_max,    \
                        yCam_ctr + 0,      \
                        xCam_ctr + 0.6*x_max + x_max,    \
                        yCam_ctr + y_max);

                ofLine( xCam_ctr + 0.6*x_max + x_max,    \
                        yCam_ctr + y_max,    \
                        xCam_ctr + 0.6*x_max + 0,      \
                        yCam_ctr + y_max);

                ofLine( xCam_ctr + 0.6*x_max + 0,      \
                        yCam_ctr + y_max,    \
                        xCam_ctr + 0.6*x_max + 0,      \
                        yCam_ctr + 0);
                }

        }
    }
}

//--------------------------------------------------------------
void testApp::exit()
{
    guiLoad->saveSettings("guiSettings.xml");
    guiPoints->saveSettings("guiPointsSettings.xml");
    guiLaser->saveSettings("guiLaserSettings.xml");
    delete guiLoad;
    delete guiPoints;
    delete guiLaser;
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key){
//        // Show/Hide GUI widgets
//        case 'g':
//            guiLoad->toggleVisible();
//            guiPoints->toggleVisible();
//            guiLaser->toggleVisible();
//            // Set grid origin slightly to the center, but center height
//            gui_ctr.x = float(ofGetWidth())*(0.50);
//            gui_ctr.y = float(ofGetHeight())*(0.50);
//            break;

        // Load ILDA next file
        case 'n':
        case OF_KEY_RIGHT:
            // If a file is already loaded, first free memory
            if(fileLoaded){
                ildaUnload();
            }

            // Increment file index, wrap to beginning at last file in directory
            f_index ++;
            f_index = ((f_index % nFiles) + nFiles )% nFiles;

            // Load input file
            inFile = dir.getName(f_index);
            ildaLoad(inFile);

            break;

        // Load previous ILDA file
        case 'b':
        case OF_KEY_LEFT:
            // If a file is already loaded, first free memory
            if(fileLoaded){
                ildaUnload();
            }

            // Decrement file index, wrap to last at first file in directory
            f_index = f_index - 1;
            f_index = ((f_index % nFiles) + nFiles )% nFiles;

            // Load input file
            inFile = dir.getName(f_index);
            ildaLoad(inFile);
            break;

//        // Turn on/off camera
//        case 'v':
//            camMode = !camMode;     // Toggle mode
//            manualControl = false;  // Turn off manual mode
//
//            // Turn on/off camera, depending on mode
//            if(camMode){
//                vidGrabber.setVerbose(true);
//                vidGrabber.initGrabber(320,240);
//            }
//            else{
//                vidGrabber.close();
//            }
//
//            break;

        // Learn new background
        case ' ':
            if(camMode)
                bLearnBakground = true;
            break;

        // Increase image difference theshold
        case OF_KEY_UP:
			threshold ++;
			if (threshold > 255) threshold = 255;
			cout << threshold << endl;
			break;

        // Decrease image difference theshold
        case OF_KEY_DOWN:
			threshold --;
			if (threshold < 0) threshold = 0;
            cout << threshold << endl;
			break;

        default:
            break;
    }
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(button == 2)
    {
        cout << "x: " << x << endl;
        cout << "Y: " << y << endl;
        return;
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

testApp::~testApp(){
    // Free any memory that was allocated for an ilda file
    if(fileLoaded){
        ildaUnload();
    }
}

/* ---------------- Position control signal    ----------------*/
// The input coordinates are mapped from the int16_t range to the uint8_t range
uint8_t posCntSgnl(const int16_t &x){
    float num = x - INT16_MIN;
    float den = (INT16_MAX - INT16_MIN);
    float sgnl = (num/den)*UINT8_MAX;

    return uint8_t(sgnl);
}
