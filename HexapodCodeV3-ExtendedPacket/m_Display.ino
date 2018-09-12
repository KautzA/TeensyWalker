/* provide control for Display
   Requires changing Adafruit_SSD1306.cpp to change from wire.h to i2c_t3.h and commenting out compiler sections starting with "#ifndef __SAM3X8E__"
   
   display will show 21 characters per line at size 1
*/

void MainDisplay(){//Display run in loop
#if defined(OLED_ENABLE)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Time = ");
  display.println((float)Tim1/1000);
  display.print("X:");
  display.print(InputX,HEX);
  display.print(",Y:");
  display.print(InputY,HEX);
  display.print(",Z:");
  display.println(InputZ,HEX);
  display.print("Rx:");
  display.print(InputRx,HEX);
  display.print(",Ry:");
  display.print(InputRy,HEX);
  display.print(",Rz:");
  display.println(InputRz,HEX);
  display.print("B:");
  display.print(InputButtons,HEX);
  display.print(",E1:");
  display.print(InputExtend1,HEX);
  display.print(",E2:");
  display.println(InputExtend2,HEX); 
  #endif
}

/*
#include <SPI.h>
//#include <Wire.h>//Conflicts with i2c_t3
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 27
Adafruit_SSD1306 display(OLED_RESET);


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);
  
  */
