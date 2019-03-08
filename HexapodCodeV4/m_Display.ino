//display
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
  display.println((float)time_1/1000);
  display.print("X:");
  display.print(input_x,HEX);
  display.print(",Y:");
  display.print(input_y,HEX);
  display.print(",Z:");
  display.println(input_z,HEX);
  display.print("Rx:");
  display.print(input_rx,HEX);
  display.print(",Ry:");
  display.print(input_ry,HEX);
  display.print(",Rz:");
  display.println(input_rz,HEX);
  display.print("B:");
  display.print(input_buttons,HEX);
  display.print(",E1:");
  display.print(input_extend1,HEX);
  display.print(",E2:");
  display.println(input_extend2,HEX); 
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

