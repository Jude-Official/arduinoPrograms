// librarys
#include <LiquidCrystal.h>

// Debugging
int debugging = 0;  // enabiling this will print extra information to the serial monitor to help with debugging

// Constants
const int buzzerPin = 13;     // pin that the buzzer is connected to
const int doorButtonPin = 6;  // pin that the door button is connected to
const int redLED = A4;        // Pin that the red LED is connected to
const int greenLED = A5;      // pin that the green LED is connected to 

// Variables
int isDoorOpen = 0;     // boolean value used to state wether door is open
int buttonData;         // defining button data to read the data pin
int doorOpenClose = 1;  // counter that I am using to increment on button presses to allow for the open and close of the fridge door
unsigned long previousMillis = 0; // stores the last time the alarm sounded
unsigned long currentMillis = millis(); // total elapsed millis
unsigned long previousTime;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // defines the pins used for the lcd display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void startup() { // custom function to do some basic manadatory actions at the start of the program
  tone(buzzerPin, 2000);
  delay(200);
  tone(buzzerPin, 4000);
  delay(200);
  tone(buzzerPin, 6000);
  delay(200);
  noTone(buzzerPin); // the buzzer part above thi scomment is a form of self test to ensure the buzzer is working
  lcd.setCursor(0, 0);           // sets the cursor to the top line on the LCD
  lcd.print("Initialising...");  // Prints some text to check the LCD is working
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, HIGH);
  delay(2000);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW); // similarly to the buzzer self test I have also included an LED self test
  lcd.clear(); // this clears the lcd before the main part of the code starts
}

void doorSequence() { // this function contains just 1 if statement to check if the door is open or closed and it then runs the requred fucntion dependant on the doors state
  if (isDoorOpen == 0) {
    doorClosed();
  } else if (isDoorOpen == 1) {
    doorOpen();
  }
}

void doorClosed() { // this function is ran only when the door is closed. It will diaply a green light and print the status to the lcd screen
  lcd.print("Door Status:");
  lcd.setCursor(0, 1);
  lcd.print("CLOSED          ");
  digitalWrite(greenLED, HIGH);  // turns on the green LED
  digitalWrite(redLED, LOW);     // turns the red LED off
  currentMillis - currentMillis;
}

void doorOpen() { // this function runs when the door is open and prints the status of the door to the LCD and illuminates the red LED
  lcd.print("Door Status:");
  lcd.setCursor(0, 1); // this sets the cursor to the bottom row of the LCD
  lcd.print("OPEN              ");
  digitalWrite(greenLED, LOW);  // turns off the green LED
  digitalWrite(redLED, HIGH);   // turns on the red LED
  unsigned long prevousTime = millis();
  unsigned long elapsedTime = millis() - previousTime;
  Serial.println(elapsedTime);
  if (elapsedTime > 45000) {
    alarmSound();
  }
  else if (elapsedTime > 55000) {
    alarmSound2();
  }
  else if (elapsedTime > 65000) {
    alarmSound3();
  }
  
}
void alarmSound() {
  tone(buzzerPin, 2000);
  delay(300);
  noTone(buzzerPin);
  delay(300);
}

void alarmSound2() {
  tone(buzzerPin, 2000);
  delay(200);
  noTone(buzzerPin);
  delay(200);
}

void alarmSound3() {
  tone(buzzerPin, 2000);
  delay(200);
  tone(buzzerPin, 4000);
  delay(200);
}

void setup() {
  pinMode(13, OUTPUT);  // set the pin mode for the buzzer
  pinMode(A5, OUTPUT);  // sets the pin A5 to output
  pinMode(A4, OUTPUT);  // sets the pin A4 to output
  lcd.begin(16, 2);     // sets up the initial configyuration for the LCD
  Serial.begin(9600);   // starts the serial monitor
  startup();            // runs the custom function startup
}

void loop() { // this is the main loop
  doorSequence(); // this runs the function that checks to see if the boolean "isDoorOpen" is open or closed.
  //Serial.println(isDoorOpen);
  buttonData = digitalRead(doorButtonPin); // this reads the pin that the door button is plugged into
  if (buttonData == 1 && doorOpenClose == 1) { // this if statement checks if the button is pressed and the doorOpenClose variable is set to 1. both bust be met for it to change isDoorOpen to 1
    isDoorOpen = 1;
    doorOpenClose++; // this adds 1 to the doorOpenClose. This is basically a very small system state type fuction to allow the door to open and close from 1 button
    if (debugging == 1) {
      Serial.println(doorOpenClose); // this is a part of my debugging function. This will only run if debugging is enabled at the top of the code
    }
  } else if (buttonData == 1 && doorOpenClose == 2) { // this statement checks to see if the button is pressed and the doorOpenClose variable is set to 2. It will then change isDoorOpen to 0
    isDoorOpen = 0;
    doorOpenClose = 1;
    if (debugging == 1) { // more debugging if statements
      Serial.println(doorOpenClose);
    }
  }
  delay(100); // delay of 100ms
}
