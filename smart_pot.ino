# define DEBUG 0
// Necessary Libraries
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include "DHT.h"
#include <TFT_eSPI.h>
#include <JPEGDecoder.h>


// Pin Definition
#define SensorPin A0
#define LDR_pin 14
#define DHTPIN 27
#define batt 34

// DHT sensor type
#define DHTTYPE DHT11

// Sensor's Thresold Value
#define LOW_TEMP_THRESOLD 15
#define HIGH_TEMP_THRESOLD 45
#define DAY_LIGHT_THRESOLD 500
#define NIGHT_LIGHT_THRESOLD 10
#define LOW_SOIL_MOISTURE_THRESOLD 3800
#define HIGH_SOIL_MOISTURE_THRESOLD 300


// Necessary Variables
int h;
int t;
int ldr_value;
float sensorValue;


TFT_eSPI tft = TFT_eSPI();
DHT dht(DHTPIN, DHTTYPE);


//####################################################################################################
// Setup
//####################################################################################################
void setup()
{

  if (DEBUG)Serial.begin(115200);

  // Set all chip selects high to avoid bus contention during initialisation of each peripheral
  digitalWrite(22, HIGH); // Touch controller chip select (if used)
  digitalWrite(15, HIGH); // TFT screen chip select
  digitalWrite( 5, HIGH); // SD card chips select, must use GPIO 5 (ESP32 SS)

  // Setting Up SD Card
  if (!SD.begin()) {
    if (DEBUG)Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    if (DEBUG)Serial.println("No SD card attached");
    return;
  }

  if (DEBUG)Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    if (DEBUG)Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    if (DEBUG)Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    if (DEBUG)Serial.println("SDHC");
  } else {
    if (DEBUG)Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  if (DEBUG)Serial.printf("SD Card Size: %lluMB\n", cardSize);

  if (DEBUG)Serial.println("initialisation done.");

  pinMode(36, OUTPUT);
  pinMode(43, INPUT);

  dht.begin();

  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
}

//####################################################################################################
// Main loop
//####################################################################################################
void loop() {

  // Reading Sensor's Data
  reading_sensor_data();

  // Checking Batter Percentage
  Check_battery();

  // Printing Data on if(DEBUG)Serial Monitor
  if (DEBUG)Serial.print("Humidity: "); if (DEBUG)Serial.println(h);
  if (DEBUG)Serial.print("Temperature: "); if (DEBUG)Serial.println(t);
  if (DEBUG)Serial.print("Soil Moisture: "); if (DEBUG)Serial.println( sensorValue);
  if (DEBUG)Serial.print("LDR Sensor: "); if (DEBUG)Serial.println(ldr_value);


  int x = 0;
  int y = 20;

  // Display Respected Image based on Sensor's Data

  if (ldr_value <= NIGHT_LIGHT_THRESOLD)
  {
    drawSdJpeg("/night.jpg", x, y);
  }
  else if ( sensorValue >= LOW_SOIL_MOISTURE_THRESOLD)
  {
    drawSdJpeg("/thirsty.jpg", x, y);
  }
  else if (sensorValue <= HIGH_SOIL_MOISTURE_THRESOLD )
  {
    drawSdJpeg("/over.jpg", x, y);
  }
  else if ( t >= HIGH_TEMP_THRESOLD)
  {
    drawSdJpeg("/hot.jpg", x, y);
  }
  else if (t <= LOW_TEMP_THRESOLD)
  {
    drawSdJpeg("/cold.jpg", x, y);
  }
  else
  {
    drawSdJpeg("/happy.jpg", x, y);
  }

}

//####################################################################################################
// Draw a JPEG on the TFT pulled from SD Card
//####################################################################################################
// xpos, ypos is top left corner of plotted image
void drawSdJpeg(const char *filename, int xpos, int ypos) {

  // Open the named file (the Jpeg decoder library will close it)
  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library

  if ( !jpegFile ) {
    if (DEBUG)Serial.print("ERROR: File \""); if (DEBUG)Serial.print(filename); if (DEBUG)Serial.println ("\" not found!");
    return;
  }

  if (DEBUG)Serial.println("===========================");
  if (DEBUG)Serial.print("Drawing file: "); if (DEBUG)Serial.println(filename);
  if (DEBUG)Serial.println("===========================");

  // Use one of the following methods to initialise the decoder:
  bool decoded = JpegDec.decodeSdFile(jpegFile);  // Pass the SD file handle to the decoder,
  //bool decoded = JpegDec.decodeSdFile(filename);  // or pass the filename (String or character array)

  if (decoded) {
    // print information about the image to the if(DEBUG)Serial port
    jpegInfo();
    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    if (DEBUG)Serial.println("Jpeg file format not supported!");
  }
}

//####################################################################################################
// Draw a JPEG on the TFT, images will be cropped on the right/bottom sides if they do not fit
//####################################################################################################
// This function assumes xpos,ypos is a valid screen coordinate. For convenience images that do not
// fit totally on the screen are cropped to the nearest MCU size and may leave right/bottom borders.
void jpegRender(int xpos, int ypos) {

  //jpegInfo(); // Print information from the JPEG file (could comment this line out)

  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  bool swapBytes = tft.getSwapBytes();
  tft.setSwapBytes(true);

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = jpg_min(mcu_w, max_x % mcu_w);
  uint32_t min_h = jpg_min(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // Fetch data from the file, decode and display
  while (JpegDec.read()) {    // While there is more data in the file
    pImg = JpegDec.pImage ;   // Decode a MCU (Minimum Coding Unit, typically a 8x8 or 16x16 pixel block)

    // Calculate coordinates of top left corner of current MCU
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
    else if ( (mcu_y + win_h) >= tft.height())
      JpegDec.abort(); // Image has run off bottom of screen so abort decoding
  }

  tft.setSwapBytes(swapBytes);

  showTime(millis() - drawTime); // These lines are for sketch testing only
}

//####################################################################################################
// Print image information to the if(DEBUG)Serial port (optional)
//####################################################################################################
// JpegDec.decodeFile(...) or JpegDec.decodeArray(...) must be called before this info is available!
void jpegInfo() {

  // Print information extracted from the JPEG file
  if (DEBUG)Serial.println("JPEG image info");
  if (DEBUG)Serial.println("===============");
  if (DEBUG)Serial.print("Width      :");
  if (DEBUG)Serial.println(JpegDec.width);
  if (DEBUG)Serial.print("Height     :");
  if (DEBUG)Serial.println(JpegDec.height);
  if (DEBUG)Serial.print("Components :");
  if (DEBUG)Serial.println(JpegDec.comps);
  if (DEBUG)Serial.print("MCU / row  :");
  if (DEBUG)Serial.println(JpegDec.MCUSPerRow);
  if (DEBUG)Serial.print("MCU / col  :");
  if (DEBUG)Serial.println(JpegDec.MCUSPerCol);
  if (DEBUG)Serial.print("Scan type  :");
  if (DEBUG)Serial.println(JpegDec.scanType);
  if (DEBUG)Serial.print("MCU width  :");
  if (DEBUG)Serial.println(JpegDec.MCUWidth);
  if (DEBUG)Serial.print("MCU height :");
  if (DEBUG)Serial.println(JpegDec.MCUHeight);
  if (DEBUG)Serial.println("===============");
  if (DEBUG)Serial.println("");
}

//####################################################################################################
// Show the execution time (optional)
//####################################################################################################
// WARNING: for UNO/AVR legacy reasons printing text to the screen with the Mega might not work for
// sketch sizes greater than ~70KBytes because 16 bit address pointers are used in some libraries.

// The Due will work fine with the HX8357_Due library.

void showTime(uint32_t msTime)
{
  if (DEBUG)Serial.print(F(" JPEG drawn in "));
  if (DEBUG)Serial.print(msTime);
  if (DEBUG)Serial.println(F(" ms "));
}

void Check_battery()

{
  if (DEBUG)Serial.println(analogRead(batt));
  float raw = 0;
  for (int i = 0; i < 10; i++)
  {
    raw += analogRead(batt);
  }
  raw = raw / 10;
  raw = (raw * 1.73) / 1000;

  if (DEBUG)Serial.print("Battery Voltage - "); if (DEBUG)Serial.println(raw);

  if (raw >= 3.8)
  {
    drawSdJpeg("/bat_lvl4.jpg", 2, 0);
  }
  if (3.8 > raw && raw >= 3.6)
  {
    if (DEBUG)Serial.println("P");
    drawSdJpeg("/bat_lvl3.jpg", 2, 0);
  }
  if (3.6 > raw && raw >= 3.4)
  {
    drawSdJpeg("/bat_lvl2.jpg", 2, 0);
  }
  if ( raw < 3.4)
  {
    drawSdJpeg("/bat_lvl1.jpg", 2, 0);
  }

}

void reading_sensor_data()
{
  // Reading Data from DHT11
  h = dht.readHumidity();
  t = dht.readTemperature();

  // Checking for Errors
  if (isnan(h) || isnan(t))
  {
    if (DEBUG)Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Reading Data from Soil Moisture Sensor
  sensorValue = analogRead(SensorPin);

  // Reading LDR Sensor's Value
  ldr_value = analogRead(LDR_pin);
}
