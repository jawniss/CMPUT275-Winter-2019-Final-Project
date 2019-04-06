#include <Arduino.h>

const int SimonSaysLEDs[3] = { 13, 12, 11 };
const int ButtonMiniG[3] = { 8, 7, 3 };

int Array[20] = {10};

// Set the pins and LEDS as inputs and outputs
void setup() {
	init();
	Serial.begin(9600);
  // config LED pins to be a digital OUTPUT
  pinMode(SimonSaysLEDs[0], OUTPUT);
	pinMode(SimonSaysLEDs[1], OUTPUT);
	pinMode(SimonSaysLEDs[2], OUTPUT);

  // configure the button pins to be a digital INPUT
  // turn on the internal pull-up resistors
  pinMode(ButtonMiniG[0], INPUT);
  digitalWrite(ButtonMiniG[0], HIGH);
  pinMode(ButtonMiniG[1], INPUT);
  digitalWrite(ButtonMiniG[1], HIGH);
	pinMode(ButtonMiniG[2], INPUT);
	digitalWrite(ButtonMiniG[2], HIGH);

	digitalWrite(SimonSaysLEDs[0], LOW);
	digitalWrite(SimonSaysLEDs[1], LOW);
	digitalWrite(SimonSaysLEDs[2], LOW);
}

void SimonSays() {
	delay(1000);
	bool failed = false;
	int counter = 1;
	int variedlight;
	for (int position = 0; position < 20; position++) {
		Array[position] = rand()%3;
	}
	for (int i = 0; i < 20; i++) {
		Serial.print(Array[i]);
	}
	Serial.println();
	digitalWrite(SimonSaysLEDs[0], HIGH);
	digitalWrite(SimonSaysLEDs[1], HIGH);
	digitalWrite(SimonSaysLEDs[2], HIGH);
	delay(1000);
	digitalWrite(SimonSaysLEDs[0], LOW);
	digitalWrite(SimonSaysLEDs[1], LOW);
	digitalWrite(SimonSaysLEDs[2], LOW);
	delay(1000);
	while (failed == false) {
		for (int whichlight = 0; whichlight < counter; whichlight++) {
			digitalWrite(SimonSaysLEDs[Array[whichlight]], HIGH);
			delay(900);
			digitalWrite(SimonSaysLEDs[Array[whichlight]], LOW);
			delay(500);
		}
		int test = 0;
		bool keepgoing = true;
		while(keepgoing == true) {
			if (test == counter) {
				counter++;
				keepgoing = false;
				delay(300);
			}
			if (digitalRead(ButtonMiniG[0]) == LOW && Array[test] == 0) {
				digitalWrite(SimonSaysLEDs[0], HIGH);
				test++;
				Serial.println(counter);
				delay(300);
				digitalWrite(SimonSaysLEDs[0], LOW);
			}
			if (digitalRead(ButtonMiniG[1]) == LOW && Array[test] == 1) {
				digitalWrite(SimonSaysLEDs[1], HIGH);
				test++;
				Serial.println(counter);
				delay(300);
				digitalWrite(SimonSaysLEDs[1], LOW);
			}
			if (digitalRead(ButtonMiniG[2]) == LOW && Array[test] == 2) {
				digitalWrite(SimonSaysLEDs[2], HIGH);
				test++;
				Serial.println(counter);
				delay(300);
				digitalWrite(SimonSaysLEDs[2], LOW);
			}
			if (digitalRead(ButtonMiniG[0]) == LOW && Array[test] != 0 ||
					digitalRead(ButtonMiniG[1]) == LOW && Array[test] != 1 ||
					digitalRead(ButtonMiniG[2]) == LOW && Array[test] != 2) {
						digitalWrite(SimonSaysLEDs[0], HIGH);
						digitalWrite(SimonSaysLEDs[1], HIGH);
						digitalWrite(SimonSaysLEDs[2], HIGH);
						delay(1000);
						digitalWrite(SimonSaysLEDs[0], HIGH);
						digitalWrite(SimonSaysLEDs[1], HIGH);
						digitalWrite(SimonSaysLEDs[2], LOW);
						delay(1000);
						digitalWrite(SimonSaysLEDs[0], HIGH);
						digitalWrite(SimonSaysLEDs[1], LOW);
						digitalWrite(SimonSaysLEDs[2], LOW);
						delay(1000);
						digitalWrite(SimonSaysLEDs[0], LOW);
						digitalWrite(SimonSaysLEDs[1], LOW);
						digitalWrite(SimonSaysLEDs[2], LOW);
						delay(1000);
						digitalWrite(SimonSaysLEDs[0], HIGH);
						digitalWrite(SimonSaysLEDs[1], HIGH);
						digitalWrite(SimonSaysLEDs[2], HIGH);
						delay(500);
						digitalWrite(SimonSaysLEDs[0], LOW);
						digitalWrite(SimonSaysLEDs[1], LOW);
						digitalWrite(SimonSaysLEDs[2], LOW);
						delay(500);
						digitalWrite(SimonSaysLEDs[0], HIGH);
						digitalWrite(SimonSaysLEDs[1], HIGH);
						digitalWrite(SimonSaysLEDs[2], HIGH);
						delay(500);
						digitalWrite(SimonSaysLEDs[0], LOW);
						digitalWrite(SimonSaysLEDs[1], LOW);
						digitalWrite(SimonSaysLEDs[2], LOW);
				Serial.println("Failed");
				failed = true;
				keepgoing = false;
				delay(300);
			}

		}

		Serial.println("DONE");

	}
}


// Main function
int main() {
    // Initialize
	setup();
	SimonSays();

Serial.flush();
	return 0;
}  // end main






/*

while (true) {
	if (digitalRead(ButtonMiniG[0]) == LOW) {
		digitalWrite(SimonSaysLEDs[0], HIGH);
	} else if (digitalRead(ButtonMiniG[0]) == HIGH) {
		digitalWrite(SimonSaysLEDs[0], LOW);
	}
	if (digitalRead(ButtonMiniG[1]) == LOW) {
		digitalWrite(SimonSaysLEDs[1], HIGH);
	} else if (digitalRead(ButtonMiniG[1]) == HIGH) {
		digitalWrite(SimonSaysLEDs[1], LOW);
	}
	if (digitalRead(ButtonMiniG[2]) == LOW) {
		digitalWrite(SimonSaysLEDs[2], HIGH);
	} else if (digitalRead(ButtonMiniG[2]) == HIGH) {
		digitalWrite(SimonSaysLEDs[2], LOW);
	}
}
*/
