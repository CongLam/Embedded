#include  <Adafruit_ST7735.h>
#include  <Adafruit_GFX.h>

#include  <SPI.h>
#define TFT_CS     10
#define TFT_RST    8                      
#define TFT_DC     9
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
#define TFT_SCLK 13   
#define TFT_MOSI 11  


int button = 0;

void setup () 
{
    //button
    pinMode(button, INPUT);
    
    tft.initR(INITR_BLACKTAB);
    
    manhinh_0();
    //manhinh_1();  //gioi thieu
    //manhinh_2(); //thong so
    //manhinh_3();
    Serial.begin(9700);
}


void manhinh_0(){
    tft.fillScreen(ST7735_CYAN);
  
    tft.fillRect(0, 0, tft.width(), 5, ST7735_RED);
    tft.fillRect(0, 155, tft.width(), 5, ST7735_RED);
    tft.fillRect(0, 0, 5, 160, ST7735_RED);
    tft.fillRect(tft.width()-5, 0, 5, 160, ST7735_RED);
    
    printText("XIN CHAO", ST7735_BLACK, 18,30,2);
    printText("1.Gioi thieu", ST7735_BLACK, 10,80,1);
    printText("2.Thong so", ST7735_BLACK, 10, 100,1);
    printText("3.Chay demo", ST7735_BLACK, 10,120,1);
}

void manhinh_3(){
    tft.fillScreen(ST7735_BLACK);
    printText("Sau day la demo ve", ST7735_WHITE,10,10,1);
    delay(100);
    printText("cac ham co ban", ST7735_WHITE,10,25,1);
    delay(1000);
    
    //ve 4 mau tu goc (0;0)
    for(int i=0; i<80; i++){
        tft.drawLine( 0, 0, 128, i, ST7735_YELLOW);     
    }
    for(int i=80; i<160; i++){
        tft.drawLine( 0, 0, 128, i, ST7735_GREEN);    
    }
    for(int i=128; i>64; i-=1){
        tft.drawLine( 0, 0, i, 160, ST7735_RED);
        //delay(2);     
    }
    for(int i=64; i>=0; i-=1){
        tft.drawLine( 0, 0, i, 160, ST7735_BLUE); 
    }

    //chuyen trang thai
    for(int i=0; i<160; i+=2){
        tft.drawFastHLine(0, i,  tft.width(), ST7735_WHITE);
    }
    for(int i=0; i<128; i+=2){
        tft.drawFastVLine(0, i,  100, ST7735_WHITE);
    }

    tft.fillScreen(ST7735_GREEN);
    //ve hinh tron
    for(int i=64; i>=0; i--){
       drawCircleMidpoint(64, 80, i, ST7735_RED );
       delay(10);
    }

    delay(100);
    tft.fillScreen(ST7735_WHITE);
    //hinh chu nhat
    tft.fillRect(10, 10, 30, 30, ST7735_GREEN);
    tft.drawRect(10, 50, 30, 30, ST7735_RED);
    //hinh tron
    tft.fillCircle(80, 25, 15, ST7735_BLUE);
    tft.drawCircle(80, 65, 15, ST7735_CYAN);
    //tam giac
    tft.fillTriangle(30, 90, 30, 140, 80, 120, ST7735_YELLOW ); 
}

void manhinh_2() {
    tft.fillScreen(ST7735_BLACK);
    printText("THONG SO KY THUAT", ST7735_GREEN,10,10,1);
    tft.drawFastHLine(0, 25,  tft.width(), ST7735_WHITE);

    printText("1.IC DriveL : ST7735", ST7735_WHITE, 0, 40,0.5);
    printText("2.Dien ap   : 3.3~5V", ST7735_WHITE, 0, 55,0.5);
    printText("3.Kich thuoc: 2 inch", ST7735_WHITE, 0, 70,0.5);
    printText("4.Phan giai : 128x160", ST7735_WHITE, 0, 85,0.5);
    printText("5.Tich hop ke the nho  MicroSD giao tiep SPI", ST7735_WHITE, 0, 100,0.5);  
}

void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}


void manhinh_1(){
    tft.fillScreen(ST7735_BLACK);
    //tieu de
    printText("LAP TRINH NHUNG", ST7735_GREEN,20,10,1);
    tft.drawFastHLine(0, 25,  tft.width(), ST7735_WHITE); 
  
    for(int i=38; i<55 ; i++){
       tft.drawFastHLine(0, i,  tft.width(), ST7735_RED); 
       delay(20);
    }
    
    printText("TFT-ST7735", ST7735_WHITE, 5, 39, 2);
   
    printText("GV: TS.Pham Van Ha", ST7735_WHITE, 2, 65, 1);

    //vach ke:
    tft.drawFastHLine(0, 80,  35, ST7735_BLUE);
    printText("Nhom 1", ST7735_RED,47, 79, 1);
    
    tft.drawFastHLine(93, 80,  35, ST7735_BLUE);
    
    //Thong tin
    printText("Thanh vien:", ST7735_WHITE,0,90,1);
    printText("1.Nguyen Van Anh", ST7735_WHITE,0,105,1);
    printText("2.Lam Tuan Cong", ST7735_WHITE,0,120,1);
    printText("3.Ngo Xuan Giang", ST7735_WHITE,0,135,1);    
}

//thuat toan midpoint
void put8pixel(int xc, int yc, int x, int y, uint16_t color)
{
  
    tft.drawPixel(x + xc, y + yc, color);
    tft.drawPixel(-x + xc, y + yc, color);
    tft.drawPixel(x + xc, -y + yc, color);
    tft.drawPixel(-x + xc, -y + yc, color);
    tft.drawPixel( y + xc, x + yc, color);
    tft.drawPixel(-y + xc, x + yc, color);
    tft.drawPixel(y + xc, -x + yc, color);
    tft.drawPixel(-y + xc, -x + yc, color);
}
void drawCircleMidpoint(int xc, int yc, int r, uint16_t color)
{
    int x = 0; int y = r;
    int f = 1 - r;
    put8pixel(xc, yc, x, y, color);
    while (x < y)
    {
        if (f < 0) f += (x << 1) + 3;
        else
        {
            y--;
            f += ((x - y) << 1) + 5;
        }
        x++;
        put8pixel(xc, yc, x, y, color);
    }
}
//end thuat toan midpoint

int count = 1;
//kỹ thuật deboucing
int buttonState = LOW;            // Trang thai phim bam o lan doc hien tai
int lastButtonState = LOW;      // Trang thai phim bam o lan doc truoc do
unsigned long lastDebounceTime = 0; // Thoi diem thay doi trang thai gan nhat
unsigned long debounceDelay = 10; 

void loop () {
  int reading = digitalRead(button);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
         buttonState = reading;
         if (buttonState == HIGH){
          //code
          if(count == 1){
              manhinh_1();
          }
          if(count == 2){
              manhinh_2();
          }
          if(count == 3){
              manhinh_3();
          }
          if(digitalRead(button)){
              count++;
              if(count > 3){
                  count = 1;  
              }  
          }
          //end-code
        }
    }
}
  lastButtonState = reading;
  //end-deboucing
}
