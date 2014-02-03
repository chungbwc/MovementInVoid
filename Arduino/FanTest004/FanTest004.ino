int pins []  = {
  2, 3, 9, 4, 5, 6, 7, 8};
int idx;
boolean start, done;
String str;

void setup() {
  for (int i=0; i<8; i++) {
    pinMode(pins[i], OUTPUT);
  }
  Serial.begin(9600);
  idx = 0;
  start = false;
  done = false;
  str = "";
}

void loop() {
  if (done) {
    processStr();
    doFan();
  }
}

void serialEvent() {
  char c = (char) Serial.read();
  if (c=='<') {
    if (!start) {
      start = true;
    } else {
      str = "";
    }
  }
  if (start) {
    str += c;
    if (c=='>') {
      done = true;
      start = false;
    }
  }
}

void processStr() {
  if (str.length() != 10) {
    str = "";
    done = false;
    Serial.println("Invalid");
    return;
  }
  if (str.charAt(0) != '<' || str.charAt(9) != '>') {
    str = "";
    done = false;
    Serial.println("Invalid");
    return;
  }
}

void doFan() {
  for (int i=0; i<8; i++) {
    int v = constrain(int(str.charAt(i+1)), 0, 99);
    if (v > 50) {
      digitalWrite(pins[i], HIGH);
    } else {
      digitalWrite(pins[i], LOW);
    }
  }
  str = "";
  done = false;
}
