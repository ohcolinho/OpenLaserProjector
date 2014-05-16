#pragma once
#include "testApp.h"

/*----------------- ILDA FILE PROCESSING   -------------------------- */
void testApp::ildaLoad(string s){
    // Define input file object, find file path, and then open file
    s = dir.getAbsolutePath() + "\\" + s.c_str();
    fstream fin;
    fin.open(s.c_str(),ios::in);

    // Store meta-data in class member variables
    fin.read(f_head,S_LEN);  f_head[S_LEN] = '\0';                  // Read in header format

    fin.read(reinterpret_cast<char *>(f_code),S_LEN);               // Read in format code; convert pointer from uint8_t * to char *

    fin.read(f_name,L_LEN);  f_name[L_LEN] = '\0';                  // Read in frame name
    fin.read(c_name,L_LEN);  c_name[L_LEN] = '\0';                  // Read in company name
    fin.read(reinterpret_cast<char *>(&nPts),sizeof(uint16_t));     // Read in number of points
    fin.read(reinterpret_cast<char *>(&fNum),sizeof(uint16_t));     // Read in frame number
    fin.read(reinterpret_cast<char *>(&nFms),sizeof(uint16_t));     // Read in number of Frames
    fin.read(reinterpret_cast<char *>(&s_head),sizeof(uint8_t));    // Read in scaner head
    fin.read(reinterpret_cast<char *>(&rsvd),sizeof(uint8_t));      // Read in reserved

    // Swap out byte order, should make this more robute, not all platforms are little-endian
    nPts = Endian16_Swap(nPts);
    fNum = Endian16_Swap(fNum);
    nFms = Endian16_Swap(nFms);

    // Read in coordinate data
    int nBlks = nPts*C_FLDS;                                                // Number of int16_t blocks to store in buffer,
                                                                            // Four int16_t blocks per coordinate
    coord_data = new int16_t [nBlks];                                       // Buffer to store coordinate data
    fin.read(reinterpret_cast<char*>(coord_data),nBlks*sizeof(int16_t));    // Read into buffer

    // Buffer to store x and y control signals to send to PSoC
    x_sgl = new uint8_t [nPts];         // Buffer to store x control signal
    y_sgl = new uint8_t [nPts];         // Buffer to store y control signal
    color = new uint8_t [nPts];         // Buffer to store color
    l_state = new uint8_t [nPts];       // Buffer to store laser status (ON/OFF)

    // Buffer to store x and y coordinates to draw to GUI
    x_grd = new float [nPts];           // Buffer to store x coordinate for GUI
    y_grd = new float [nPts];           // Buffer to store y coordinate for GUI

    // 1) Fix byte order, and 2) get on/off state and color for each coordinate
    int n = 0;                          // Counter x,y,color,l_state control signal arrays

    for(int i=0; i < nBlks; i++){

        // Increment counter every forth block, exclude first iteration
        if(((i%4)==0) && (i!=0)) {  n++; }

        // Swap first and second byte
        coord_data[i] = Endian16_Swap(coord_data[i]);

        // Compute x control signal, x GUI coordinate and store it
        if((i%4)==0){
            x_sgl[n] = posCntSgnl(-coord_data[i]);
            x_grd[n] = float(GUI_MAX)*float(coord_data[i])/float(INT16_MAX);
        }

        // Compute y control signal, y GUI coordinate and store it
        if(((i-1)%4)==0){
            y_sgl[n] = posCntSgnl(coord_data[i]);
            y_grd[n] = float(GUI_MAX)*float(coord_data[i])/float(INT16_MAX);
        }

        // Store color information on 3D image, lower 8 bits of status code block
        if(((i-3)%4)==0){
            color[n] = coord_data[i] & 0xFF;
        }

        // Determine on/off state and color for each coordinate
        // This information is stored in every forth block for 3D image
        if(((i-3)%4)==0){
            // Check 14th bit (using zero-index); Convention: 0 if ON, 1 if OFF
            // Increment counter
            if(!CHECK_BIT((coord_data[i]),14)){
                l_state[n] = 1;  // Laser is on
            }
            else{
                 l_state[n] = 0;  // Laser is off
            }
        }
    }

    // Close file
    fin.close();

    // File has been successfully load
    fileLoaded = true;
}

void testApp::ildaUnload(){

    // Free memory associate with coordinaet data from ilda file
    delete [] coord_data;

    // Free memory used to sent data packets to PSoC
    delete [] x_sgl;
    delete [] y_sgl;
    delete [] color;
    delete [] l_state;

    // Free memory used to store GUI coordinates
    delete [] x_grd;
    delete [] y_grd;

    // All memory allocated has been released, file data unloaded
    fileLoaded = false;
}
