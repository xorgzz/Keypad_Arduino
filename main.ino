const int row = 4;
const int col = 4;

char keys[row][col] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

const int pinCol[col] = {5, 4, 3, 2};
const int pinRow[row] = {9, 8, 7, 6};

bool changeMode = false;
String defaultPin = "123A";
String inputPin = "";

char readFromPad() {
  char key = 0;
  int x=0;
  for (int i=0; i<row; i++) {
    digitalWrite(pinRow[i], LOW);
    for (int j=0; j<col; j++) {
      if (digitalRead(pinCol[j]) == LOW) {
        delay(20);
        while (digitalRead(pinCol[j]) == LOW) {
          key = keys[i][j];
        }
      }
    }
    digitalWrite(pinRow[i], HIGH);
  }
  return key;
}

void setup() {
  Serial.begin(9600);
  for (int i=0; i<4; i++) {
    pinMode(pinCol[i], INPUT);
    digitalWrite(pinCol[i], HIGH);
    pinMode(pinRow[i], OUTPUT);
  }
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.print("\nPin: ");

}

bool ProperPin() {
  if (inputPin == defaultPin) return true;
  else return false;
}

void Light(int pin) {
  for (int i=0; i<3; i++) {
    digitalWrite(pin, HIGH);
    delay(250);
    digitalWrite(pin, LOW);
    delay(250);
  }
}

void loop() {
  char key = readFromPad();
  if (changeMode) {
    digitalWrite(11, HIGH);
    if (key != 0) {
      if (key == '*') {
        if (inputPin.length() >= 1) {
          Serial.println("\nPin changed successfully");
          defaultPin = inputPin;
          digitalWrite(11, LOW);
          Light(12);
        }
        else {
          digitalWrite(11, LOW);
          Serial.println("\nPin hadn't been changed");
          Light(10);
        }
        changeMode = false;
        inputPin = "";
        Serial.print("\nPin: ");
      }
      else if (key != "#") {
        inputPin += key;
        Serial.print(key);
      }
    }
    
  }
  else if (key != 0) {
    if (key == '#') {
      if (ProperPin()) {
        Serial.println("\nAcces Granted");
        digitalWrite(12, HIGH);
        delay(1000);
        digitalWrite(12, LOW);
      }
      else {
        Serial.println("\nAcces Denied");
        Light(10);
      }
      inputPin = "";
      Serial.print("\nPin: ");
    }
    else if (key == '*') {
      if (ProperPin()) {
        Serial.println("\n\nPin Overwrite Mode On");
        inputPin = "";
        changeMode = true;
        Serial.print("\nNew Pin: ");
      }
      else {
        Serial.println("\n\nPin Overwrite Mode Not Allowed");
        Light(10);
        inputPin = "";
        Serial.print("\nPin: ");
      }

    }
    else {
      Serial.print(key);
      inputPin += key;
    }
  }
}
