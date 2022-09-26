/*


  Tips on usage
  1. Many 3.5: ILI9488 displays do not have a tri-state MISO--meaning the display tends to own the MISO
  line not allowing other SPI devices (Touch and SD cards) to send data to the MCU. A special buffer may be needed
  on display MISO--may not be needed, See the diagram on github for part number and connection
  2) if you want to use a PrintScreen capabiity you must use a frame buffer (xxx.useFrameBuffer(true)), and you
  must then use xxx.updateDisplay() to send data to display
  3) the library clock speeds in the ILI9488_t3.h seem to work, you may slow the read rate if there is an issue
  4) SD cards vary so you may have to lower the write time. Currently the library uses 20 mhz
  5) Teensy 4.0's are 3v3 hence you must adapt the 3.5" TFT to talk to the MCU. a) replace the 3 SD resistors with 0 ohm b) solder J1 on display to force 3v3 operation

*/

#include <ILI9488_t3.h>           // fast display driver lib
#include <XPT2046_Touchscreen.h>  // https://github.com/PaulStoffregen/XPT2046_Touchscreen
#include "SdFat.h"                // recommend using this library
#include <ILI9488_t3_PrintScreen_SdFat.h>   // my library for capturing the display and saving to a BMP file
#include <ili9488_t3_font_Arial.h>          // fancy font for this demo

#define TFT_RST   8     // rst
#define TFT_DC    9     // DC pin on LCD
#define TFT_CS    10    // chip select pin on LCD
#define LCD_PIN   A0    // lcd pin to control brightness
#define SD_CS     A9    // chip select  pin on sd card
#define T_CS       5    // chip select  pin on sd card
#define T_IRQ      6    // chip select  pin on sd card

int BtnX, BtnY, i;

// create display and DS objects
ILI9488_t3 Display = ILI9488_t3(TFT_CS, TFT_DC, TFT_RST);

XPT2046_Touchscreen Touch(T_CS, T_IRQ);

TS_Point p;

SdFat sd;
SdFile DataFile;

void setup() {
  Serial.begin(9600);

  while (!Serial) {}
  Serial.println("Starting...");
  pinMode(LCD_PIN, OUTPUT);

  digitalWrite(LCD_PIN, HIGH);
  Display.begin();

  // must have to allow MCU to read display pixel data
  Display.useFrameBuffer(true);
  delay(100);

  // fire up the touch display
  Touch.begin();

  Touch.setRotation(1);
  Display.setRotation(3);
  delay(100);
  Serial.println("drawing graphics...");
  // draw some sample graphics
  Display.fillScreen(ILI9488_BLACK);
  for (i = 0; i < 480; i += 5) {
    Display.drawLine(0, 0, i, 320, ILI9488_BLUE);
  }
  for (i = 320; i > 0; i -= 5) {
    Display.drawLine(0, 0, 480, i, ILI9488_BLUE);
  }
  Display.drawLine(5, 5, 50, 70, ILI9488_BLUE);
  Display.fillRect(5, 5, 470, 40, ILI9488_RED);
  Display.setTextColor(ILI9488_WHITE);
  Display.setFont(Arial_28);
  Display.setCursor(10, 10);
  Display.print(F("Press to Print Screen"));

  // must have to allow MCU to update the display
  Serial.println("Displaying graphics...");
  Display.updateScreen();



}

/*

  main loop function

*/

void loop() {
  Serial.println("waiting for press...");
  delay(100);
  if (Touch.touched()) {

    ProcessTouch();

    if ((BtnX > 5) & (BtnX < 470) & (BtnY > 5) & (BtnY < 40)) {
      Serial.print("Saving file...");
      SaveBMP24(&Display, SD_CS, "0PrintScreen.BMP");
      Serial.print("file saved.");

      Display.fillRect(100, 100, 200, 50, ILI9488_GREEN);
      Display.setTextColor(ILI9488_WHITE);
      Display.setFont(Arial_28);
      Display.setCursor(110, 110);
      Display.print(F("Done"));
      Display.updateScreen();

    }

  }
}



void ProcessTouch() {

  p = Touch.getPoint();

  BtnX = p.x;
  BtnY = p.y;

  /*
        Serial.print("real coordinates: ");
        Serial.print(BtnX);
        Serial.print(",");
        Serial.print (BtnY);
        Display.drawPixel(BtnX, BtnY, C_GREEN);
  */
  // x  = map(x, real left, real right, 0, 480);
  // y  = map(y, real bottom, real top, 320, 0);

  BtnX  = map(BtnX, 300, 3900, 0, 480);
  BtnY  = map(BtnY, 300, 3900, 0, 320);


  /*
    Serial.print(", Mapped coordinates: ");
    Serial.print(BtnX);
    Serial.print(",");
    Serial.println(BtnY);
    Display.fillCircle(BtnX, BtnY, 3, C_BLUE);
    Display.updateScreen();
  */
  delay(10);
}
