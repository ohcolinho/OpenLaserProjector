#Open Source Low Cost Laser Projector

Made with love by: 
Colin Ho, Jon Gonzales, Christine Gregg, Amy Liao and Marty Cowell as part of UC Berkeley's ME235 in Spring 2014

##You'll need:
- [A Cypress PSoC 5LP Development Kit CY8CKIT-050](http://www.cypress.com/?rID=51577)
- 20kpps Laser Galvanometers with drivers (We sourced ours from ebay for $120)
- A Laser with TTL control (we sourced a 150mw red laser from ebay for $30)
- a computer 


##To run it:
run the executable called "project_GUI_final.exe" in "\LaserProjectorInterface_GUI\bin" 
follow the onscreen directions, and make sure that the usb cable is plugged into the usb comm on the PSoC, not the usb debug/program port

##To play with the source code 
The Laser Projector Interface is written in C++ using open frameworks. You'll need to generate a new project with the following add-ons
- [ofxUI](https://github.com/rezaali/ofxUI), you'll need to install this
- ofxXmlSettings
- ofxOpenCV

The PSoC code can be loaded by using Cypress's [PSoC Creator IDE](http://www.cypress.com/psoccreator/), which is awesomely free. Just open the workspace file "USB_UART01-000.Bundle01"

We did our best to make the code easy to follow with tons of comments. 

Happy Lasing!!!

