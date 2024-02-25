 

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

const char* ssid = "Seyram";
const char* password = "seyram801570801";
const char* firebaseHost = "https://blindstick-b0501-default-rtdb.firebaseio.com/";
const char* firebaseAuth = "AIzaSyDa3FRZGPa78g-VZDGoxNydWySses-G5w4";

const int trigPin1 = D1;
const int echoPin1 = D2;
const int trigPin2 = D5;
const int echoPin2 = D6;
const int buzzerPin = D3;      // Define the pin for the buzzer
const int pushButtonPin = D7; // Define the pin for the push button
const int ledPin = D4;        // Define the pin for the LED

long duration1, duration2;
int distance1, distance2;

const int maxDistance = 200;   // Maximum distance of the ultrasonic sensor in centimeters
const int buzzerRange = 100;   // Buzzer activation range in centimeters

FirebaseData firebaseData;

void setup() {
pinMode(trigPin1, OUTPUT);     // Sets the trigPin1 as an Output
pinMode(echoPin1, INPUT);      // Sets the echoPin1 as an Input
pinMode(trigPin2, OUTPUT);     // Sets the trigPin2 as an Output
pinMode(echoPin2, INPUT);      // Sets the echoPin2 as an Input
pinMode(buzzerPin, OUTPUT);    // Sets the buzzerPin as an Output
pinMode(pushButtonPin, INPUT_PULLUP); // Sets the pushButtonPin as an Input with internal pull-up resistor
pinMode(ledPin, OUTPUT);       // Sets the ledPin as an Output

Serial.begin(9600);           // Starts the serial communication

// Connect to Wi-Fi
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println();
Serial.println("WiFi connected!");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());

// Initialize Firebase
Firebase.begin(firebaseHost, firebaseAuth);
}

void Activate1() {
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
}

void Activate2() {
digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
}

void loop() {
Activate1();
duration1 = pulseIn(echoPin1, HIGH);
distance1 = duration1 * 0.034 / 2;

Activate2();
duration2 = pulseIn(echoPin2, HIGH);
distance2 = duration2 * 0.034 / 2;

// Limit the distances to the maximum allowed by the sensor
distance1 = constrain(distance1, 0, maxDistance);
distance2 = constrain(distance2, 0, maxDistance);

// Prints the distances on the Serial Monitor
Serial.print("Obstacle Distance 1: ");
Serial.print(distance1);
Serial.print(" cm, Distance 2: ");
Serial.print(distance2);
Serial.print(" cm");

// Check if either of the distances is within the buzzer activation range
if (distance1 <= buzzerRange || distance2 <= buzzerRange) {
// Activate the buzzer and LED
digitalWrite(buzzerPin, HIGH);
digitalWrite(ledPin, HIGH);
} else {
// Turn off the buzzer and LED
digitalWrite(buzzerPin, LOW);
digitalWrite(ledPin, LOW);
}

// Check if the push button is pressed
if (digitalRead(pushButtonPin) == LOW) {
Serial.println(", Emergency!");
} else {
Serial.println();
}

// Update Firebase with the distances
String firebaseRootPath = "/blinddata";
String firebaseDistance1Path = firebaseRootPath + "/distance1";
String firebaseDistance2Path = firebaseRootPath + "/distance2";

Firebase.setInt(firebaseData, firebaseDistance1Path, distance1);
Firebase.setInt(firebaseData, firebaseDistance2Path, distance2);

// Add a delay to control the rate of sending data to Firebase
delay(2000);
}
