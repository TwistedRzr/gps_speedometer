#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define u8g_logo_sat_width 15
#define u8g_logo_sat_height 15

//satellite logo
const unsigned char u8g_logo_sat[] = {
0x00, 0x00, 0x0c, 0x00, 0x0e, 0x00, 0x07, 0x00, 0x02, 0xe0, 0x01, 0xf0, 0x0d, 0xe0, 0x03, 0xd0,
0x01, 0x38, 0xa8, 0x9c, 0x2c, 0x8e, 0x50, 0x00, 0x2c, 0x00, 0x10, 0x00, 0x04, 0x00
};


// The serial connection to the GPS device
#include <SoftwareSerial.h>
static const int RXPin = 13, TXPin = 14;
static const uint32_t GPSBaud = 9600;
SoftwareSerial ss(RXPin, TXPin);


//GPS Library
#include <TinyGPS++.h>
TinyGPSPlus gps;

int num_sat, gps_speed;
String heading;



void setup() {

  ss.begin(GPSBaud);
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

 
}

void loop() {

  Get_GPS(); //Get GPS data
  print_speed();
}

void print_speed() {
  
  display.clearDisplay();
  display.setTextSize(3);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(2,10);             // Start at top-left corner
  display.println(gps_speed, DEC);
  display.setTextSize(2);
  display.setCursor(55,19);             
  display.println("km/h");
  //display.setTextSize(2);
  //display.setCursor(50,1);             
  //display.println("Sat:");
  display.drawBitmap(65, 1, u8g_logo_sat, u8g_logo_sat_width, u8g_logo_sat_height, 1); // draw sattelite logo
  display.setTextSize(2);
  display.setCursor(90,1);             
  display.println(num_sat);
  display.display();
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


void Get_GPS()
{


  num_sat = gps.satellites.value();

  if (gps.location.isValid() == 1) {

    gps_speed = gps.speed.kmph();

    heading = gps.cardinal(gps.course.value());
  }
  smartDelay(1000);


  if (millis() > 5000 && gps.charsProcessed() < 10)
  {

  }



}
