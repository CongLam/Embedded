#include <Adafruit_GFX.h>       // include Adafruit graphics library
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library

#define TFT_CS    8      // TFT CS  pin is connected to arduino pin 8
#define TFT_RST   9      // TFT RST pin is connected to arduino pin 9
#define TFT_DC    10     // TFT DC  pin is connected to arduino pin 10
// initialize ILI9341 TFT library
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!");
  tft.begin();
  //goi ham test
  testText();
}

void loop() {
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  
  tft.setCursor(0, 0);              //set tọa độ
  tft.setTextColor(ILI9341_WHITE);  //set màu
  tft.setTextSize(1);               //set cỡ chữ
  tft.println("Hello World!");      //nội dung in ra
  
  tft.setTextColor(ILI9341_YELLOW); 
  tft.setTextSize(2);
  tft.println("25/6/2020");

  tft.setCursor(50, 50); 
  tft.setTextColor(ILI9341_RED);    
  tft.setTextSize(3);
  tft.println("LT NHUNG");
  
  tft.setTextSize(2);
  tft.println("Nhom 1");
  
  tft.setTextSize(2);
  tft.println("1. Nguyen Van Anh");
  tft.println("2. Lam Tuan Cong");
  tft.println("3. Ngo Xuan Giang");
  
  return micros() - start;
}
