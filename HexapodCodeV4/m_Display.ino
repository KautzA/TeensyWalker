//display
/* provide control for Display
 Requires changing Adafruit_SSD1306.cpp to change from wire.h to i2c_t3.h and commenting out compiler sections starting with "#ifndef __SAM3X8E__"
 
 display will show 21 characters per line at size 1 with 4 lines
 */

void MainDisplay(){//Display run in loop
#if defined(OLED_ENABLE)
  //Time=xxxxxxxxxxxxxxxx
  //X00Y00Z00,R:X00Y00Z00
  //B:00,E:00,00,V:000,
  //T:000
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Time = ");
  display.println((float)time_1/1000);
  
  display.print("X");
  display.print(input_x+128,HEX);
  display.print("Y");
  display.print(input_y+128,HEX);
  display.print("Z");
  display.print(input_z+128,HEX);
  display.print("R:X");
  display.print(input_rx+128,HEX);
  display.print("Y");
  display.print(input_ry+128,HEX);
  display.print("Z");
  display.println(input_rz+128,HEX);
  
  display.print("B:");
  display.print(input_buttons+128,HEX);
  display.print(",E:");
  display.print(input_extend1+128,HEX);
  display.print(",");
  display.print(input_extend2+128,HEX);
  display.print(",V:");
  display.println(current_voltage,DEC);
  
  display.print("T:");
  display.print(last_cycle_time,DEC);
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

