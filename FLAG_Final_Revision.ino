/*
  TB6560 Stepper Motor
*/
bool enableMtr;           // true = enabled, false = disabled
bool jogMtr;		  // true = button pressed, false = not pressed
bool labelBlockingLight;  // true if label over photoresistor, false otherwise

int photoResPin = A0;     // Photo resistor analog input pin
int photoResVal = 0;      // Value read by photoresistor

int pulseOnTime = 500;    // time to pulse motor in microseconds
int pulseOffTime = 500;   // time to pulse motor off in microseconds
int lightThreshold = 560; // photoresistor value to stop the motor
int iterations = 1000;    // end number of iterations of loop

bool timerLatch;          // true if timer has started
bool timer;               // true if timer is running
int count = 0;            // iterations of the loop

void setup() {

  pinMode(6, INPUT);       // enable switch (originally pin 4)
  pinMode(5, INPUT);       // Start motor rotation

  pinMode(8, OUTPUT);      // pin8 EN+
  pinMode(9, OUTPUT);      // pin9 CW+
  pinMode(10, OUTPUT);     // pin10 CLK+

  digitalWrite(8, LOW);    // EN+ ON
  digitalWrite(9, HIGH);   // CW+ HIGH=CW, LOW=CCW
  digitalWrite(10, HIGH);  // Pulse CLK+ off

  pinMode(13, OUTPUT);     // driver board LED
  digitalWrite(13, LOW);   // turn the LED off
  // Analog pin of the TB6560:
  pinMode(photoResPin, INPUT);      // Photoresistor
  Serial.begin(9600);
}
void loop() {

  //Serial.println(photoResVal);  //Prints photoresistor value for testing
  //Slows down code significantly

  //Serial.println(digitalRead(5));    //print jogMtr button value for testing
  //Serial.println(digitalRead(6));    //print enableMtr switch value for testing

  photoResVal = analogRead(photoResPin);  //read value from photoresistor
  enableMtr = digitalRead(6);
  jogMtr = digitalRead(5);

  if (enableMtr) {
    digitalWrite(8, HIGH);    // EN+ HIGH to Enable TB6560 Drive
  }
  else {
    digitalWrite(8, LOW);   // EN+ LOW to Disable TB6560 Drive
    digitalWrite(13, HIGH);  // turn the LED off by making the voltage LOW
  }

  if (photoResVal < lightThreshold) {      // value < lightThreshold is on a label
    labelBlockingLight = true;
  }
  else {
    labelBlockingLight = false;
  }


  if (jogMtr && !timerLatch) {
    timerLatch = true;
  }
  if (timerLatch) {
    count = count + 1;
  }
  if (timerLatch && count < iterations) {
    timer = true;
  }
  else {
    timer = false;
    count = false;
    timerLatch = false;
  }

  //Stepper motor run sequence

  if (jogMtr || labelBlockingLight || timer) {
    digitalWrite(10, LOW);   // Pulse CLK+
    digitalWrite(13, LOW);   // turn the LED on
    delayMicroseconds(pulseOnTime);   // Pulse ON after pulseOnTime microseconds
    digitalWrite(10, HIGH);  // Pulse CLK+ off
    digitalWrite(13, HIGH);  // turn the LED off
    delayMicroseconds(pulseOnTime);  // Pulse OFF after pulseOffTime microseconds

  }
}
