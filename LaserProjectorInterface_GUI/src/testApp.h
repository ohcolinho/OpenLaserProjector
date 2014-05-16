#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"

// Macros to define length of different arrays in meta data, small and large
// Macro to define number of 2 byte fields in the coordinate data structure [coordinate fields]
#define S_LEN 4
#define L_LEN 8
#define C_FLDS 4        // THIS IS ONLY GOOD FOR 3D images

// Macro function to check bits
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

// Macros to define upper/lower bounds on int data types
#define INT16_MAX   32767
#define INT16_MIN   -32767
#define UINT8_MAX   255

// Macro to define the size of the ilda GUI graphic
#define GUI_MAX 250

class testApp : public ofBaseApp{

    private:
	    /* --------- FILE DIRECTORY MEMBER VARIABLES ---------- */
	    ofDirectory dir;                    // Object to access all files in directory
        int         nFiles;                 // Number of ilda files in directory
        int         f_index;                // Current file index
        bool        fileLoaded;
        string      inFile;                 // Current input file string name
        string      fileMeta;               // string containing the metadata of the ildaFile

        /* -------- IDLA FILE MEMBER VARIABLES ---------------- */
	    char        f_head[S_LEN+1];        // Identifier for header format
	    uint8_t     f_code[S_LEN];          // Format code
	    char        f_name[L_LEN+1];        // Frame name
        char        c_name[L_LEN+1];        // Company name
        uint16_t    nPts;                   // Number of points
        uint16_t    fNum;                   // Frame number
        uint16_t    nFms;                   // Number of frames
        uint8_t     s_head;                 // Scanner head
        uint8_t     rsvd;                   // Reserved
        int16_t     *coord_data;            // Pointer to coordinate data array

        uint8_t     *x_sgl;                 // Pointer to x coordinate control signal array
        uint8_t     *y_sgl;                 // Pointer to y coordinate control signal array
        uint8_t     *color;                 // Pointer to color array
        uint8_t     *l_state;               // Pointer to laser state (ON/OFF) array
        uint8_t     dataPkt[63];            // Data packet to send to PSoC, containing information above
        int         c_index;                // Current point index


        float       *x_grd;                 // Pointer to x coordinate data used for GUI
        float       *y_grd;                 // Pointer to y coordinate data used for GUI
        ofPoint     gui_ctr;                // Center point for GUI

        int             point;              // currently selected point in the ilda file
        string          pointinfo;          // string containing point x,y, blinking
        bool            drawILDA;           // Should the ILDA be drawn to the GUI?
        bool            ILDAmode;           // flag for if ILDAmode is activated

        /* --------- SERIAL COMMUNICATION MEMBER VARIABLES -------- */
		bool		                bSendSerialMessage;     // a flag for sending serial
		ofSerial	                serial;
        bool                        connected;              // is the serial device connected?
        bool                        portSelected;           // is a port selected from dropdown?
        int                         baud;                   //baud rate of serail connection
        string                      serialInfo;             //string containing information about serial; comm port and baud rate
        vector<ofSerialDeviceInfo>  deviceList;
        vector<string>              devices;                // vector of devices that go in dropdown list
        string                      device;                 //name of device that is connected to

        /* --------- GUI MEMBER VARIABLES ------------------------- */
        ofxUICanvas         *guiConnect;
        ofxUICanvas         *guiMode;
        ofxUICanvas         *guiWebcam;
        ofxUICanvas         *guiLoad;
        ofxUICanvas         *guiPoints;
        ofxUICanvas         *guiLaser;
        ofTrueTypeFont      myFont;
        ofxUIDropDownList   *ddl;         //DropDownList pointer for modifying alter
        ofxUIToggle         *toggleManual;
        ofxUIToggle         *toggleWebcam;
        ofxUIToggle         *toggleILDA;
        ofxUITextArea       *textareaSerial;

        /* ------------ LASER PAD CONTROL MEMBER VARIABLES ---------------------- */
        bool            manualControl;      // is manual control enabled?
        ofPoint         position;           // position for manual firing of laser position.x and y float values 0-1
        bool            laserFire;          // is the laser being fired manually?
        ofPoint         positionDraw;       // value for drawing point on screen to indicate laser position
        float           angle;              // Max angle of galvos

        /* ------------ CAMERA VISION MEMBER VARIABLES ---------------------- */
        ofVideoGrabber 		    vidGrabber;         // Provide access to camera
        ofxCvColorImage	        colorImg;           // Represent each pixel as unsigned char value
        ofxCvGrayscaleImage 	grayImage;          // Condense image to grayscale
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;           // Store difference between background and new objects in frame
        ofxCvContourFinder 	    contourFinder;      // Object to find outline of person
        bool                    multiContours;      // flag for if projecting multiple contours or single contour
        bool                    bNewFrame;          // flag for new data received in frame from camera
        bool                    camMode;            // is camera on/off?
		bool				    bLearnBakground;    // flag to learn new background
		int 				    threshold;
		int                     xCam_max;           // Amount of pixels in x-direction
		int                     yCam_max;           // Amount of pixels in y-direction

         // misc stuff
        float radius;
        int resolution;

    public:
        /* --------- CORE MEMBER FUNCTIONS ---------- */
        void setup();
        void update();
        void draw();
        void exit();

        /* --------- EVENT MEMBER FUNCTIONS---------- */
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        void guiEvent(ofxUIEventArgs &e);       //gui event listener, triggers when gui events happen

        /* --------- GUI MEMBER FUNCTIONS ----------------- */
        void gui();                             //Do all the fun ofxUI initialization
        void guiUpdate();

        /* --------- FILE PROCESSING MEMBER FUNCTIONS ------------- */
        void ildaLoad(string s);                // Load ilda file
        void ildaUnload();                      // Unload ilda file

        // Deconstructor
        ~testApp();

};

/* ------------- Position control signal    -------------*/
uint8_t posCntSgnl(const int16_t &x);
