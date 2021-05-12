#include <DHT_U.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22 // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

// Ints for leds
const int GreenLed = 3;
const int YellowLed = 4;
const int RedLed = 5;
const int buttonPin = 7;

String statusLed;            // String for send color of led
String radiatorState;        // String for print radiator state
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

void setup() {
  // PinModes for enable to output
  pinMode (GreenLed, OUTPUT) ;
  pinMode (YellowLed, OUTPUT) ;
  pinMode (RedLed, OUTPUT) ;
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Delay betwenn each loop
  delay(1000);

  // Read the state of the pushbutton
  buttonState = digitalRead(buttonPin);

  // Add a counter on button when it's pushed
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      buttonPushCounter++;
    } else {
    }
    delay(50);
  }
  
  // Save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  // Read data for DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Check if reading is OK or not
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Calculation of temperature in celsius
  float hic = dht.computeHeatIndex(t, h, false);

  // Receive data from serial port
  if (Serial.available() > 0) {
    // read the incoming string:
    String incomingString = Serial.readString();
    radiatorState = incomingString;
    buttonPushCounter = buttonPushCounter + 1;
  }

  // Change radiator state using counter created and modified by button
  if (buttonPushCounter % 2 == 0) {
    radiatorState = "Auto";
    // Light up leds in terms of temparture
    if (t <= 18) {
      digitalWrite(GreenLed, HIGH);
      digitalWrite(YellowLed, LOW);
      digitalWrite(RedLed, LOW);
      statusLed = "Green";
    }
    else if (t > 18 || t < 25) {
      digitalWrite(GreenLed, LOW);
      digitalWrite(YellowLed, HIGH);
      digitalWrite(RedLed, LOW);
      statusLed = "Yellow";
    }
    else if (t >= 25) {
      digitalWrite(GreenLed, LOW);
      digitalWrite(YellowLed, LOW);
      digitalWrite(RedLed, HIGH);
      statusLed = "Red";
    }
  } else {
    radiatorState = "Manual";
    statusLed = "Off";
  }

  // Send data by serial port :
  Serial.print(h);
  Serial.print(" ");
  Serial.print(t);
  Serial.print(" ");
  Serial.print(statusLed);
  Serial.print(" ");
  Serial.println(radiatorState);

}