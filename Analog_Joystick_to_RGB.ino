/*www.elegoo.com
2018.02.09 PyroTyger
This code allows colour control of an RGB LED using a Joystick Module
on an Elegoo Uno R3 Arduino board
Rotate joystick to choose colour, then click once to set. Click again to release.
*/

// Define joystick pins
// Ensure +5V and GND pins are connected to appropriate pins on board
#define SW_pin 2 // digital pin 2 connected to switch output SW
#define X_pin 0 // analog pin A0 connected to X output VRx
#define Y_pin 1 // analog pin A1 connected to Y output VRy

// Define RGB LED Pins - connect common cathode to GND on board
#define BLUE 3 // digital pin 3 - use 220ohm resistor in series
#define GREEN 5 // digital pin 5 - use 220ohm resistor in series
#define RED 6 // digital pin 6 - use 220ohm resistor in series


void setup() {
// setup Joystick pins
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600); //output generated values to Serial Monitor (commented out below)

// setup RGB LED pins
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

// define variables

float angle; //polar coordinates
float magnitude; //polar coordinates
float X_pos; //cartesian coordinates
float Y_pos; //cartesian coordinates
double fixedMagnitude; //used to map cartesian square to polar circular magnitude
double mappingRatio; //used in calculating fixedMagnitude
int hueVal; //arbitrary remapping of angle, for convenience in RGB calculations
int redValue;
int greenValue;
int blueValue;


void loop() {

  while (digitalRead(SW_pin) == HIGH) { 
      //runs this whole schtick until the joystick button is pressed ie SW_pin goes LOW
      
    X_pos = analogRead(X_pin)-511; //read joystick position & adjust origin to (0,0)
    Y_pos = analogRead(Y_pin)-511; //this assumes a range of 0-1024 - adjust for your module
    
    angle = atan2(X_pos,Y_pos); //polar attitude coordinate
    magnitude = sqrt(sq(X_pos)+sq(Y_pos)); //polar (square) magnitude coordinate
  
    /* Map cartesian square magnitude to polar circle
     * We need the ratio between the maximum possible magnitude within the cartesian 512 square
     * at a given polar angle to the mapped polar maximum of 512
     * this will be either sin(angle) or cos(angle), depending on whether X or Y is greater
     * It allows us to use the full range of our (square) joystick range instead of clipping 
     * the corners. Bit of a hack, but it's fairly efficient
     */
    if (abs(X_pos) > abs(Y_pos)) {
      mappingRatio = sin(abs(angle));
    }
    else {
      mappingRatio = cos(abs(angle));
    }
    fixedMagnitude = magnitude*mappingRatio;
  
      
    angle = angle + 3.14159; //shifts angle from +/- to purely +ve
  
    //now make magnitude a scaling factor 0 to 1 instead of a range 0 to 512
    fixedMagnitude = abs(fixedMagnitude/512.0);
    
  /* Okay, we have attitude (angle) & magnitude scalar (fixedMagnitude), 
   *  which we can equate to Hue & Saturation for a light 
   *  (HSV or HSL, assuming we don't care about brightness)
   *  Now let's try & convert those two values into three RGB values
   *  using a colour-wheel phase space of 3*256 = 768 "degrees"
   *  This givers RGB value 2/3 of the circle, overlapping 1/3 with each
   */
  
    //First map angle (currently in range 0 to 2pi) to a value more convenient for 8bit RGB
    
    angle = angle*100;  //so we don't lose granularity to map(), which uses ints
    hueVal = map(angle, 0,628,0,767);
  
    //0-512, process for R, peak at 255 "degrees", tailing off to 0 in each direction
    if(hueVal < 512) {
      redValue = 256-abs(hueVal-255); 
     }
    else {
      redValue = 0; //prevents an old R value from carrying over if not defined above
    }
  
     //256-767, process for G, peak at 512 "degrees"
    if(hueVal > 255) {
      greenValue = 256-abs(hueVal-511);  
    }
    else {
      greenValue = 0;
    }
  
    //512-767 and 0-255, process for B, peak at 0 (768) "degrees"
    if(hueVal < 256) {
      blueValue = abs(255-hueVal);  
    }
    else if(hueVal > 511) {
      blueValue = hueVal-512;  
    }
    else {
      blueValue = 0;
    }

/*So far we've ignored magnitude. Now let's adjust the colour mix accordingly
 * - At origin (0,0) fixedMagnitude is 0 and we want White (RGB = 127,127,127)
 * - At full extension, fixedMagnitude is 1 and we want RGB as defined above, up to 256
 * We want our colour mix to be somewhere between White and the RGB we've defined above,
 * mixed so that the proportion of RGB is definied by fixedMagnitude
 */

   redValue = (redValue*fixedMagnitude) + 127*(1-fixedMagnitude);
   greenValue = greenValue*fixedMagnitude + 127*(1-fixedMagnitude);
   blueValue = blueValue*fixedMagnitude + 127*(1-fixedMagnitude);
   

/* Un-comment and use Serial Monitor to output values for debugging
  Serial.print("Switch:  ");
  Serial.println(digitalRead(SW_pin));
  Serial.print("X-axis: ");
  Serial.println(X_pos);
  Serial.print("Y-axis: ");
  Serial.println(Y_pos);
  Serial.print("Angle: ");
  Serial.println(angle);
  Serial.print("Hue Value: ");
  Serial.println(hueVal);
  Serial.print("Raw Magnitude: ");
  Serial.println(magnitude);
  Serial.print("Fixed Magnitude: ");
  Serial.println(fixedMagnitude);
  Serial.print("Mapping Ratio: ");
  Serial.println(mappingRatio);
  Serial.print("\n");
  Serial.print("RED value: ");
  Serial.println(redValue);
  Serial.print("GREEN value: ");
  Serial.println(greenValue);
  Serial.print("BLUE value: ");
  Serial.println(blueValue);
  Serial.print("\n");
  delay(1000);
*/

  //FINALLY, let's output our RGB values to the relevant pins
    
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
  }
  //Above code loops continuously until the joystick button is pressed

  delay(300); //wait a moment for the button to be released

  //now we're trapped with the last RGB setting until the button is pressed again

  while (digitalRead(SW_pin) == HIGH) {} 
    delay(300); //wait a moment for the button to be released
}
