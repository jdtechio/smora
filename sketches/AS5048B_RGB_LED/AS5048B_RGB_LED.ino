#include "libs/ams_as5048b.h"

//unit consts
#define U_RAW 1
#define U_TRN 2
#define U_DEG 3
#define U_RAD 4
#define U_GRAD 5
#define U_MOA 6
#define U_SOA 7
#define U_MILNATO 8
#define U_MILSE 9
#define U_MILRU 10

#define COMMON_ANODE

AMS_AS5048B mysensor;

int redPin = 5; 
int greenPin = 6; 
int bluePin = 9;

int rgb[3];
double angle;

void setup() {
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT);
  setColor(0, 0, 0);

  //Start Wire object. Unneeded here as this is done (optionnaly) by AMS_AS5048B object (see lib code - #define USE_WIREBEGIN_ENABLED)
  //Wire.begin();

  //init AMS_AS5048B object
  mysensor.begin();

  //consider the current position as zero
  mysensor.setZeroReg(); 

	//Start serial
	Serial.begin(1152000);
	//while (!Serial) ; //wait until Serial ready 
}

void loop() {
  angle = mysensor.angleR(U_DEG, true);
  Serial.print("Angle degree : ");
  Serial.println(angle, DEC);
  
  hsi2rgb(angle, 1, 1, rgb);
  setColor(rgb[0], rgb[1], rgb[2]);
}

void setColor(int red, int green, int blue) {
#ifdef COMMON_ANODE 
  red = 255 - red;
  green = 255 - green; 
  blue = 255 - blue;
#endif
  analogWrite(redPin, red); 
  analogWrite(greenPin, green); 
  analogWrite(bluePin, blue);
}

void hsi2rgb(float H, float S, float I, int* rgb) {
  int r, g, b;
  H = fmod(H,360); // cycle H around to 0-360 degrees
  H = 3.14159*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):0;
    
  // Math! Thanks in part to Kyle Miller.
  if(H < 2.09439) {
    r = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    g = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    b = 255*I/3*(1-S);
  } else if(H < 4.188787) {
    H = H - 2.09439;
    g = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    b = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    r = 255*I/3*(1-S);
  } else {
    H = H - 4.188787;
    b = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    r = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    g = 255*I/3*(1-S);
  }
  rgb[0]=r;
  rgb[1]=g;
  rgb[2]=b;
}
