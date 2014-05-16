#pragma once
#include "testApp.h"

//--------------------------------------------------------------
void testApp::gui()
{
    cout << "begin GUI" << endl;
    // Pixel Spacing Variables
    int ConnectHeight = 135;
    int ModeHeight = 95;
    int LoadHeight = 75;
    int PointHeight = 135;
    int LaserHeight = 270;
    int webcamHeight = 60;
    int guiSpacing = 5;

    myFont.loadFont("swisop1.ttf", 12);         //load pretty font


    /*--------------------GUI for Connecting to PSOC--------------------*/

    guiConnect = new ofxUICanvas();
    guiConnect->setDimensions(200,ConnectHeight);
    guiConnect->setPosition(5,5); //position at top left
    guiConnect->setFont("swisop1.ttf");
    guiConnect->setFontSize(OFX_UI_FONT_LARGE, 8);
    guiConnect->setFontSize(OFX_UI_FONT_MEDIUM, 8);
    guiConnect->setFontSize(OFX_UI_FONT_SMALL, 6);

    // Add GUI elements
    guiConnect->addLabel("Connect to Device");
    guiConnect->addSpacer();
    guiConnect->addLabel("Select Serial Device",OFX_UI_FONT_SMALL);

    // if there are serial devices present, add device names to device object
    if(deviceList.size()>0){
        for (int index = 0; index<deviceList.size(); index++){
            devices.push_back(deviceList[index].getDeviceName());
            cout << "index: " << index << endl;
        }
    }
    else{
        devices.push_back("none");
        device = "none";
    }

    ddl = guiConnect->addDropDownList("Serial Devices",devices);
    guiConnect->addToggle("Connect", false)->getLabelWidget()->setColorFill(ofColor(255, 0, 0));
    serialInfo = "Port: \n";
    textareaSerial = guiConnect->addTextArea("SERIALINFO",serialInfo, OFX_UI_FONT_SMALL);
    guiConnect->addLabelButton("Refresh", true);


    /*--------------------GUI for selecting modes--------------------*/

    guiMode = new ofxUICanvas();
    guiMode->setDimensions(200,ModeHeight);
    guiMode->setPosition(5,ConnectHeight+2*guiSpacing); //position below guiConnect
    guiMode->setFont("swisop1.ttf");
    guiMode->setFontSize(OFX_UI_FONT_LARGE, 8);
    guiMode->setFontSize(OFX_UI_FONT_MEDIUM, 8);
    guiMode->setFontSize(OFX_UI_FONT_SMALL, 6);

    // add GUI elements
    guiMode->addLabel("Laser Mode", OFX_UI_FONT_MEDIUM);
    guiMode->addSpacer();
    toggleManual = guiMode->addToggle("Manual", false);    //button for manually switching on laser
    toggleWebcam = guiMode->addToggle("Webcam", false);
    toggleILDA = guiMode->addToggle("ILDA", false);

    /*--------------------GUI for Manually Firing Lazor--------------------*/

    guiLaser = new ofxUICanvas();
    guiLaser->setDimensions(200,LaserHeight);
    guiLaser->setPosition(5,ConnectHeight+ModeHeight+3*guiSpacing); //position below GuiPoints
    guiLaser->setFont("swisop1.ttf");
    guiLaser->setFontSize(OFX_UI_FONT_LARGE, 8);
    guiLaser->setFontSize(OFX_UI_FONT_MEDIUM, 8);
    guiLaser->setFontSize(OFX_UI_FONT_SMALL, 6);

    //guiLaser->addToggle("Manual", false)->getLabelWidget()->setColorFill(ofColor(255, 0, 0));    //button for manually switching on laser
    guiLaser->addLabel("Manual Laser Control", OFX_UI_FONT_MEDIUM);
    guiLaser->addSpacer();
    guiLaser->add2DPad("Laserpad", ofPoint(0, 255), ofPoint(0,255), ofPoint(128,128));    //2D pad for controlling galvos
    guiLaser->addToggle("Laser", false)->getLabelWidget()->setColorFill(ofColor(255, 0, 0));    //button for manually switching on laser

    guiLaser->setVisible(false);   //initialize as hidden

    /*--------------------GUI for Webcam Contour--------------------*/

    guiWebcam = new ofxUICanvas();
    guiWebcam->setDimensions(200,webcamHeight);
    guiWebcam->setPosition(5,ConnectHeight+ModeHeight+3*guiSpacing); //position below GuiPoints
    guiWebcam->setFont("swisop1.ttf");
    guiWebcam->setFontSize(OFX_UI_FONT_LARGE, 8);
    guiWebcam->setFontSize(OFX_UI_FONT_MEDIUM, 8);
    guiWebcam->setFontSize(OFX_UI_FONT_SMALL, 6);

    //add GUI elements
    guiWebcam->addLabel("Webcam Contour",OFX_UI_FONT_MEDIUM);
    guiWebcam->addSpacer();
    guiWebcam->addToggle("Multi Contours", false)->getLabelWidget()->setColorFill(ofColor(255, 0, 0));
    guiWebcam->setVisible(false);


    /*--------------------GUI for loading file----------------*/
    guiLoad = new ofxUICanvas();
    guiLoad->setDimensions(200,LoadHeight);     //orig 200,55
    guiLoad->setPosition(5,ConnectHeight+ModeHeight+3*guiSpacing);

    //font setting
    guiLoad->setFont("swisop1.ttf");
    guiLoad->setFontSize(OFX_UI_FONT_LARGE, 8);
    guiLoad->setFontSize(OFX_UI_FONT_MEDIUM, 8);
    guiLoad->setFontSize(OFX_UI_FONT_SMALL, 8);

    // Add GUI elements
    guiLoad->addLabel("File");
    guiLoad->addSpacer();
    guiLoad->setWidgetFontSize(OFX_UI_FONT_SMALL);
    fileMeta = "File Name: \nNumber of Points:";
    guiLoad->addTextArea("FILEINFO", fileMeta, OFX_UI_FONT_SMALL);

    guiLoad->setVisible(false);   //initialize as hidden

    /*--------------------GUI for inspecting points----------------*/
    guiPoints = new ofxUICanvas();

    // GUI element formatting
    guiPoints->setDimensions(200,PointHeight);
    guiPoints->setPosition(5,ConnectHeight+ LoadHeight+ModeHeight+4*guiSpacing);
    guiPoints->setFont("swisop1.ttf");
    guiPoints->setFontSize(OFX_UI_FONT_LARGE, 8);
    guiPoints->setFontSize(OFX_UI_FONT_MEDIUM, 8);
    guiPoints->setFontSize(OFX_UI_FONT_SMALL, 6);

    guiPoints->addLabel("Point Inspector");
    guiPoints->addSpacer();
    guiPoints->setWidgetFontSize(OFX_UI_FONT_SMALL);
    guiPoints->addToggle("Draw ILDA", true)->getLabelWidget()->setColorFill(ofColor(0, 255, 0));     // toggle to toggle if drawing ILDA
    guiPoints->addLabel("Point Selection Slider",OFX_UI_FONT_SMALL);
    guiPoints->addSlider("POINT", 1, 1, point);   //add slider bar
    //guiPoints->addLabel("Point Number Text Input",OFX_UI_FONT_SMALL);
    //guiPoints->addTextInput("TEXT INPUT","1")->setAutoClear(false);
    pointinfo = "X: \nY: \nLaser:";   //initialize string for point Information text area
    guiPoints->addTextArea("POINTINFO",pointinfo , OFX_UI_FONT_SMALL);

    ofxUIWidget *w = guiPoints->getWidget("POINT"); // grab pointer reference to widget called "POINT"
    ofxUISlider *slider = (ofxUISlider *) w;        // Cast reference into reference to UISlider object
    slider->setLabelPrecision(0);                   // make the slider show integer values, thus decimal recision = 0

    guiPoints->setVisible(false);   //initialize Point Inspector GUI as hidden

    //add listeners to the GUI canvases
    ofAddListener(guiConnect->newGUIEvent,this,&testApp::guiEvent);
    ofAddListener(guiMode->newGUIEvent,this,&testApp::guiEvent);
    ofAddListener(guiWebcam->newGUIEvent,this,&testApp::guiEvent);
    ofAddListener(guiLoad->newGUIEvent,this,&testApp::guiEvent);
    ofAddListener(guiPoints->newGUIEvent,this,&testApp::guiEvent);
    ofAddListener(guiLaser->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::guiUpdate()
{
    //---------------Update guiFile-----------------------------
    //update text area with file metadata
    ofxUIWidget *w1 = guiLoad->getWidget("FILEINFO");
    ofxUITextArea *textArea = (ofxUITextArea *) w1;
    fileMeta = "File Name: " + dir.getName(f_index);
    fileMeta += "\n# of Points: " + ofToString(nPts);
    //fileMeta += "\nFrame#: " + ofToString(fNum);
    //fileMeta += "\nScanner Head:" + ofToString(s_head);
    textArea->setTextString(fileMeta);

    //---------------Update guiPoint-----------------------------
    //update point inspector slider widget
    ofxUIWidget *w = guiPoints->getWidget("POINT");
    ofxUISlider *slider = (ofxUISlider *) w;
    slider->setMax(nPts);

    // Set grid origin slightly to the right, but center height
    gui_ctr.x = float(ofGetWidth())*(0.60);
    gui_ctr.y = float(ofGetHeight())*(0.50);
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();

    if(name == "POINT"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		point = slider->getScaledValue();

		//update textarea with point info
		ofxUIWidget *w1 = guiPoints->getWidget("POINTINFO");
        ofxUITextArea *textArea = (ofxUITextArea *) w1;

        pointinfo = "\nX: " + ofToString(x_grd[point-1]);
        pointinfo += "\nY: " + ofToString(y_grd[point-1]);
        pointinfo += "\nLaser:" + ofToString(l_state[point-1]);
        textArea->setTextString(pointinfo);  //push new text to textarea
	}
	else if(name == "Draw ILDA"){

        // Create pointer to control toggle widget
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        drawILDA = toggle->getValue();

        //change label color, green if on, red if off
        if (toggle->getValue()){
            toggle->getLabelWidget()->setColorFill(ofColor(0,255,0));
        }
        else{
            toggle->getLabelWidget()->setColorFill(ofColor(255,0,0));
        }
    }

//	else if(name == "Angle"){
//
//		ofxUISlider *slider = (ofxUISlider *) e.widget;
//		angle = slider->getValue();
//		cout << "Angle Value: " << angle << endl;
//	}

	else if(name == "TEXT INPUT"){

        // Create pointer to text input widget
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;

        if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER){
            //set Point value
            point = ofToInt(ti->getTextString());

            //get Point slider object and set it to new value
            ofxUIWidget *w = guiPoints->getWidget("POINT");
            ofxUISlider *slider = (ofxUISlider *) w;

            //if input value is within bounds
            if(point <= slider->getMax() && point >= slider->getMin())  {
                slider->setValue(point);    //update slider to inputed point
            }
        }
        else if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS){
            cout << "ON FOCUS: ";
        }
        else if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS){
            cout << "ON BLUR: ";
        }
        string output = ti->getTextString();
        cout << "text input: " << output << endl;
    }

    else if(name == "Refresh"){
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        cout << button->getValue() <<endl;

        devices.clear();    //empty the vector of devices

        //rescan serial ports serial connection
        deviceList = serial.getDeviceList();

        //update Serial Devices DropDown List
        if(deviceList.size()>0) //if there are serial devices present
        {
            for (int index = 0; index<deviceList.size(); ++index)
            {
            devices.push_back(deviceList[index].getDeviceName());   //push device name into devices vector
            }
        }
        else    //otherwise only make a single entry of none
        {
            devices.push_back("none");

        }

        //update serial device list drop down menu
        ddl->clearToggles();
        ddl->addToggles(devices);
    }

    else if(name == "Connect"){

        cout << "inside connect code " << endl;

        // Create pointer to Connect widget
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        connected = toggle->getValue();
        if (connected) //if on
        {
            toggle->getLabelWidget()->setColorFill(ofColor(0,255,0));   //make green
            if(portSelected && device != "none")    //if device isn't none
            {
                //connect to serial device!!
                serial.setup(device, baud);
                cout << "Serial Device: " << device << " at baud: " << baud << endl;
                //update serialInfo text area
                //ofxUIWidget *w1 = guiLaser->getWidget("SERIALINFO");
                //ofxUITextArea *textArea = (ofxUITextArea *) w1;
                serialInfo = "Port: " + device + "\n";
                textareaSerial->setTextString(serialInfo);
                bSendSerialMessage = true;  //send Serial messages

            }
            else{   //if no devices connected, update to none connected
                cout << "no device connected";
                //ofxUIWidget *w1 = guiLaser->getWidget("SERIALINFO");
                //ofxUITextArea *textArea = (ofxUITextArea *) w1;
                serialInfo = "Port: None \n";
                textareaSerial->setTextString(serialInfo);
                cout<< "after setTextSTring"<<endl;
                bSendSerialMessage = false;  //send Serial messages
                cout<< "end of serial toggle else"<<endl;
            }

        }
        //if off disconnected
        else {
            bSendSerialMessage = false;
            toggle->getLabelWidget()->setColorFill(ofColor(255,0,0));   //make Red
            //ofxUIWidget *w1 = guiLaser->getWidget("SERIALINFO");
            //ofxUITextArea *textArea = (ofxUITextArea *) w1;
            serialInfo = "Port: \n" ;
            textareaSerial->setTextString(serialInfo);
            serial.close();
            cout << "no device connected " << endl;
        }
    }

    else if(name == "Serial Devices"){

        // Create pointer to Serial Device widget
        ofxUIDropDownList *droplist = (ofxUIDropDownList *) e.widget;
        vector<string> dev = droplist->getSelectedNames();
        cout << "Device vector size = " << dev.size()  << endl;

        // if there are devices!
        if (dev.size() > 0){
            cout<< "first element" << dev.front() << endl;
            device = dev.front();
            portSelected = true;

        }
        //otherwise there aren't any devices
        else {
            cout<< "no devices" << endl;
            portSelected = false;
        }
    }

    else if(name == "Laserpad"){

        // Create pointer to Pad widget
		ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
        if (manualControl){

            // Draw manual laser position in center of GUI
            positionDraw.x = float(GUI_MAX)*(pad->getValue().x-0.5)+gui_ctr.x;
            positionDraw.y = float(GUI_MAX)*(pad->getValue().y-0.5)+gui_ctr.y;

            // position of actual data going to laser
            // positions are from 0-1, with 0.5 being center of the pad in both x and y axes
            position.x = pad->getValue().x;
            position.y = pad->getValue().y;
        }
	}

    else if(name == "Webcam"){

        // Create pointer to Manual control toggle widget
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        camMode = toggle->getValue();   // Toggle mode

        manualControl = false;  // Turn off manual mode
        ILDAmode = false;


        guiWebcam->setVisible(camMode);

        //set other gui canvases invisible
        guiPoints->setVisible(false);
        guiLoad->setVisible(false);
        guiLaser->setVisible(false);

        //turn off other mode toggles
        toggleManual->setValue(false);
        toggleILDA->setValue(false);

        // Turn on/off camera, depending on mode
        if(camMode){
            vidGrabber.setVerbose(true);
            vidGrabber.initGrabber(320,240);
        }
        else{
            vidGrabber.close();
        }
    }
    else if(name == "Multi Contours"){

        // Create pointer to Manual control toggle widget
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        multiContours = toggle->getValue();   // Toggle mode

        //change label color, green if on, red if off
        if (toggle->getValue()){
            toggle->getLabelWidget()->setColorFill(ofColor(0,255,0));
        }
        else{
            toggle->getLabelWidget()->setColorFill(ofColor(255,0,0));
        }
    }

	else if(name == "Manual"){

        // Create pointer to Manual control toggle widget
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        manualControl = toggle->getValue();
        cout << "Manual toggle" << manualControl;

        // Turn off camera if on
        if(camMode){
            vidGrabber.close();
        }

        ILDAmode = false;
        camMode = false;

        guiLaser->setVisible(manualControl);

        //set other gui canvases invisible
        guiPoints->setVisible(false);
        guiLoad->setVisible(false);
        guiWebcam->setVisible(false);

        //turn off other mode toggles
        toggleWebcam->setValue(false);
        toggleILDA->setValue(false);



    }

    else if(name == "ILDA"){

        // Create pointer to Manual control toggle widget
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ILDAmode = toggle->getValue();
        cout << "ILDA mode" <<  ILDAmode;

        // Turn off camera if on
        if(camMode){
            vidGrabber.close();
        }

        manualControl = false;
        camMode = false;

        guiLoad->setVisible(ILDAmode);
        guiPoints->setVisible(ILDAmode);

        //set other gui canvases invisible
        guiLaser->setVisible(false);
        guiWebcam->setVisible(false);

        //turn off other mode toggles
        toggleWebcam->setValue(false);
        toggleManual->setValue(false);
    }

    else if(name == "Laser"){

        // Create pointer to Laser control widget
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        laserFire = toggle->getValue();

        //change label color of laser, green if on, red if off
        if (toggle->getValue()){
            toggle->getLabelWidget()->setColorFill(ofColor(0,255,0));
        }
        else{
            toggle->getLabelWidget()->setColorFill(ofColor(255,0,0));
        }
    }
}
