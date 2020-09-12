/*
 * Kết nối:
 *          TFT                 Arduino Uno R3
 *          VCC                      5v
 *          GND                      GND
 *          CLK                      13
 *          SDA                      11
 *          RS                       9
 *          RST                      8
 *          CS                       10
 * 
 */

#include  <Adafruit_ST7735.h>
#include  <Adafruit_GFX.h>

#include  <SPI.h>
#define TFT_CS     10
#define TFT_RST    8                      
#define TFT_DC     9
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
#define TFT_SCLK 13   
#define TFT_MOSI 11  


void setup () 
{
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK);

    //tieu de
//    printText("Hello", ST7735_RED,28,10,1);
//    printText("Nhom 1", ST7735_BLUE,70,10,1);
//
//    //vach ke 1:       //tft.width(): chiều rộng của màn hình tft
//    tft.drawFastHLine(0, 25,  35, ST7735_BLUE);
//    tft.drawFastHLine(38, 25,  52, ST7735_WHITE); //vạch trắng ở giữa
//    tft.drawFastHLine(93, 25,  35, ST7735_BLUE);
//
//    //chiều dài chiều rộng của màn hình
//    printText("Chieu rong:", ST7735_WHITE,0,30,1);
//    printText(tft.width(), ST7735_WHITE,90,30,1);
//   
//    printText("TEST OK", ST7735_GREEN,23,90,2);

    text_group = displayio.Group(max_size=10, scale=2, x=11, y=64);
text = "Hello World!";
text_area = label.Label(terminalio.FONT, text=text, color=0xFFFF00);
text_group.append(text_area); 
splash.append(text_group);


        
    Serial.begin(57600);
}

void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}



void loop () {}
