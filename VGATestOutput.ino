const int dataPins[] = {2, 3, 4, 5, 6, 7, 8, 9}; // Digital output pins for X, Y, character, and color
const int controlPins[] = {10, 11}; // Digital output pins for control signals
const int writePin = 12; // Digital output pin to pulse for writing data

const int numRows = 32;
const int numCols = 60;

struct Pixel {
  uint8_t color;
  char character;
};

Pixel pixelArray[numCols][numRows];

void setup() {
  pinMode(writePin, OUTPUT);
  digitalWrite(writePin, LOW);

  for (int i = 0; i < 8; i++) {
    pinMode(dataPins[i], OUTPUT);
    digitalWrite(dataPins[i], LOW);
  }

  for (int i = 0; i < 2; i++) {
    pinMode(controlPins[i], OUTPUT);
    digitalWrite(controlPins[i], LOW);
  }

  Serial.begin(9600); // Start serial communication with the PC
}

void outputData(uint8_t type, uint8_t value) {
  digitalWrite(controlPins[0], type & 0x01);
  digitalWrite(controlPins[1], (type & 0x02) >> 1);
  digitalWrite(dataPins[0], value & 0x01);
  digitalWrite(dataPins[1], (value & 0x02) >> 1);
  digitalWrite(writePin, HIGH);
  delayMicroseconds(1);
  digitalWrite(writePin, LOW);
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read();
    if (input != '\n') { // Ignore newline characters
      for (int y = 0; y < numRows; y++) {
        for (int x = 0; x < numCols; x++) {
          pixelArray[x][y].character = input;
        }
      }
    }
  }

  for (int y = 0; y < numRows; y++) {
    for (int x = 0; x < numCols; x++) {
      // Output X position
      outputData(0, x);

      // Output Y position
      outputData(1, y);

      // Output character
      outputData(2, pixelArray[x][y].character);

      // Output color
      outputData(3, pixelArray[x][y].color);
    }
  }
}
