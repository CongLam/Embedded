#include <Adafruit_GFX.h>    // include Adafruit graphics library
#include <Adafruit_ST7735.h> // include Adafruit ST7735 display library
#include <SPI.h>             // include Arduino SPI library
#include <SD.h>              // include Arduino SD library
 
// define ST7735 TFT display connections
#define TFT_RST  5   // reset line (optional, pass -1 if not used)
#define TFT_CS   6   // chip select line
#define TFT_DC   7   // data/command line
 
#define button   2  // button pin
 
// initialize Adafruit ST7735 TFT library
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
 
void setup(void) {
  Serial.begin(9600);
 
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(button, INPUT_PULLUP);
 
  // initialize ST7735S TFT display
  tft.initR(INITR_BLACKTAB);

  //màu nền
  tft.fillScreen(ST77XX_CYAN);
 
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("failed!");
    while(1);  // stay here
  }
  Serial.println("OK!");
 
  File root = SD.open("/");  // open SD card main root
  printDirectory(root, 0);   // print all files names and sizes
  root.close();              // close the opened root
 
}

int count = 0;
void loop() {
  File root = SD.open("/");  // mo SD card main root
  while (true) {
    File entry =  root.openNextFile();  // mo file
    
    if (! entry) {
      root.close();
      return;
    }
 
    uint8_t nameSize = String(entry.name()).length();  // lấy độ dài tên ảnh
    String str1 = String(entry.name()).substring( nameSize - 4 ); //lấy ra kiểu ảnh ".bmp"

    if ( str1.equalsIgnoreCase(".bmp") )  // nếu như là .bmp thì vẽ ảnh từ vị trí 0-0
      bmpDraw(entry.name(), 0, 30);      

    entry.close();  // close the file

    if(count == 0){
        printText("Ngo Xuan Giang", ST77XX_CYAN, 22, 10, 1);
        printText("GV: TS.Pham Van Ha", ST7735_BLACK, 15, 10, 1);
    }

    if(count == 1){
        printText("GV: TS.Pham Van Ha", ST77XX_CYAN, 15, 10, 1);
        printText("Nguyen Van Anh", ST7735_BLACK, 22, 10, 1);
    }
    if(count == 2){
        printText("Nguyen Van Anh", ST77XX_CYAN, 22, 10, 1);
        printText("Lam Tuan Cong", ST7735_BLACK, 22, 10, 1);
    }
    if(count == 3){
        printText("Lam Tuan Cong", ST77XX_CYAN, 22, 10, 1);
        printText("Ngo Xuan Giang", ST7735_BLACK, 22, 10, 1);
    }
        
    if(digitalRead(button)){
        count++;  
        if(count > 3){
            count = 0;  
        } 
    }

    delay(50);
    while(digitalRead(button)); 
  }
}

void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}
 
// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.
 
#define BUFFPIXEL 20
 
void bmpDraw(char *filename, uint8_t x, uint16_t y) {
 
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
 
  if((x >= tft.width()) || (y >= tft.height())) return;
 
  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
 
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }
 
  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
 
        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);
 
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;
 
        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }
 
        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;
 
        // Set TFT address window to clipped image bounds
        tft.startWrite();
        tft.setAddrWindow(x, y, w, h);
 
        for (row=0; row<h; row++) { // For each scanline...
 
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            tft.endWrite();
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }
 
          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
              tft.startWrite();
            }
 
            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r,g,b));
          } // end pixel
        } // end scanline
        tft.endWrite();
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }
 
  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}
 
 
// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.
 
uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}
 
uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
 
 
void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
