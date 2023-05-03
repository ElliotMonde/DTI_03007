#include "HX711.h"
#include "FastLED.h"
#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

// Loadcell 1 data-out and clock
#define LOADCELL_DOUT_PIN_1 2
#define LOADCELL_SCK_PIN_1 3

// Loadcell 2 data-out and clock
#define LOADCELL_DOUT_PIN_2 4
#define LOADCELL_SCK_PIN_2 5

// Loadcell 3 data-out and clock
#define LOADCELL_DOUT_PIN_3 8
#define LOADCELL_SCK_PIN_3 9

// Loadcell 4 data-out and clock
#define LOADCELL_DOUT_PIN_4 10
#define LOADCELL_SCK_PIN_4 11

#define NUM_LEDS 67
#define LED_PIN 7
#define COLOR_ORDER RGB

// init leds color objects
CRGB leds[NUM_LEDS];

/* lily_1 : 0-16, lily_2 : 17-33, lily_3 : 34-50, lily_4 : 51-67
    light up current and next lilypad
*/
int ledArr[4][2] = {{0, 32}, {17, 49}, {34, 66}, {51, 66}};

// init loadcell struct objects
HX711 scale_1, scale_2, scale_3, scale_4;
HX711 *scale_1_ptr = &scale_1, *scale_2_ptr = &scale_2, *scale_3_ptr = &scale_3, *scale_4_ptr = &scale_4;
HX711* loadcellArr[4] = {scale_1_ptr, scale_2_ptr, scale_3_ptr, scale_4_ptr};
bool stateArr[4] = {false};

// isPressed function : if loadcell measure greater than abs 30 -> true
void lcIsPressed(HX711* loadcellArr[4], bool stateArr[4]) {
  for (int i = 0; i < 4; i++) {
    if (abs((*loadcellArr[i]).get_units()) > 30) {
      stateArr[i] = true;
    } else {
      stateArr[i] = false;
    }
  }
}

void lightUp(bool stateArr[4], int ledArr[4][2]) {
  int color[3] = {0, 0, 0};
  for (int i = 0; i < 4; i++) {
  
    switch (i) {
      case 0:
        // pink
        color[0] = 10;
        color[1] = 230;
        color[2] = 100;
        break;
      case 1:
        // red
        color[0] =   0;
        color[1] = 180;
        color[2] = 0;
        break;
      case 2:
        // blue
        color[0] = 0;
        color[1] = 0;
        color[2] = 250;
        break;
      case 3:
        // purple
        color[0] = 250;
        color[1] = 0;
        color[2] = 0;
        break;
      default:
        // black
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        break;
    }
//
    if (stateArr[i]) {
//      Serial.println(i);
//      Serial.println(" = true");
      for (int j = ledArr[i][0]; j <= ledArr[i][1]; j++) {
        leds[j] = CRGB(color[0], color[1], color[2]);
      }
      FastLED.show();
    }
  }
}

void setup() {
  Serial.begin(9600);
//  Serial.println("HX711 scale demo");

  scale_1.begin(LOADCELL_DOUT_PIN_1, LOADCELL_SCK_PIN_1);
  scale_1.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale_1.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  scale_2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
  scale_2.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale_2.tare();

  scale_3.begin(LOADCELL_DOUT_PIN_3, LOADCELL_SCK_PIN_3);
  scale_3.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale_3.tare();

  scale_4.begin(LOADCELL_DOUT_PIN_4, LOADCELL_SCK_PIN_4);
  scale_4.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale_4.tare();

  FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  pinMode(2, INPUT);


  // test if LEDs working
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(100, 100, 100);
    FastLED.show();
    delay(10);
  }
  for (int j = NUM_LEDS - 1; j >= 0; j--) {
    leds[j] = CRGB::Black;
    FastLED.show();
    delay(10);
  }
}

void loop() {

  lcIsPressed(loadcellArr, stateArr);
  lightUp(stateArr, ledArr);

  //For debug:
//  Serial.print("Reading1: ");
//  Serial.print(scale_1.get_units()); //scale.get_units() returns a float
//  Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
//  Serial.println();
//
//  Serial.print("Reading2: ");
//  Serial.print(scale_2.get_units()); //scale.get_units() returns a float
//  Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
//  Serial.println();
//
//  Serial.print("Reading3: ");
//  Serial.print(scale_3.get_units()); //scale.get_units() returns a float
//  Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
//  Serial.println();
//
//  Serial.print("Reading4: ");
//  Serial.print(scale_4.get_units()); //scale.get_units() returns a float
//  Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
//  Serial.println();
for(int i = 66; i>=0; i--){
  leds[i] = CRGB::Black;
  FastLED.show();
  delay(10);
}
  delay(0);

}
