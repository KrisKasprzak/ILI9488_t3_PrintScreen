<b><h2><center>Print Screen for ILI9488_t3 libraries and Touchscreen Displays</center></h1></b>

This library will let programmers add a print screen capability to their projects. There are 2 functions: 1) SaveBMP24 and 2) DrawBMP24 so you can save and draw images. The created imagtes are 24-bit BMP files
As this library requires and SD card there are 2 versions of the functions 1) for SD.h and 2) SdFat.h. The functions are simple inline and don't require any object creation. The goal was to make implementation simple such as

<b>Some examples of captured screens are:</b>

![header image](https://raw.github.com/KrisKasprzak/ILI9488_t3_PrintScreen/master/Images/PrintScreen.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9488_t3_PrintScreen/master/Images/PrintScreen3.jpg)


  <b>Tips on usage</b>
  <li>
  Many 3.5: ILI9488 displays do not have a tri-state MISO--meaning the display tends to own the MISO
  line not allowing other SPI devices (Touch and SD cards) to send data to the MCU. A special buffer may be needed
  on display MISO--may not be needed, See the diagram on github for part number and connection
  </li>
  <li>
  if you want to use a PrintScreen capabiity you must use a frame buffer (xxx.useFrameBuffer(true)), and you
  must then use xxx.updateDisplay() to send data to display
  </li>
  <li>
  the library clock speeds in the ILI9488_t3.h seem to work, you may slow the read rate if there is an issue
  </li>
  <li>
  SD cards vary so you may have to lower the write time. Currently the library uses 20 mhz
  </li>


<br>
<br>
<b>How to get on-board SD card readers working</b>
Teensy 4.0's are 3v3 hence you must adapt the 3.5" TFT to talk to the MCU. In order to get the on-board SD card working you will need to 
a) replace the 3 SD resistors with 0 ohm 
b) solder J1 on display to force 3v3 operation
 
![header image](https://raw.github.com/KrisKasprzak/ILI9488_t3_PrintScreen/master/Images/DisplayHack.jpg)

<br>
<br>

<b>Implementation</b>
#include <ILI9488_t3_PrintScreen_SD.h>
or
#include <ILI9488_t3_PrintScreen_SdFat.h>

To save a screen shot, make a call like this

SaveBMP24(&display_object, chip select pin, "the_file.bmp");

To draw a BMP file to the screen, make a call like this

DrawBMP24(&display_object, chip select pin, "the_file.bmp");


You need not initialize and SD card or even include the SD librar, all that is done in inside the functions.

While this library was created for the ILI9488_t3 and intended for Teensy-based MCU's, the library *may* be adaptable to other display drivers provide the driver has a method getPixel(). At the time of posting, I don't know of any lib other that the ILI9341_t3.h and ILI9488_t3.h that support the getPixel() method.

To get tri-state behavior for the displays MISO, a buffer will be needed
![header image](https://raw.github.com/KrisKasprzak/ILI9488_t3_PrintScreen/master/Images/MISO_Buffer.jpg)



<br>
<br>

