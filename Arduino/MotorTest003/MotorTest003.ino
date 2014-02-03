#include <SoftwareSerial.h>

#define AX_READ_DATA          0x02
#define AX_WRITE_DATA         0x03
#define AX_SYNC_WRITE         0x83
#define AX_GOAL_LENGTH        0x05
#define AX_GOAL_SPEED_LENGTH  0x07
#define AX_TORQUE_LENGTH      0x04
#define AX_MAX_TORQUE_LENGTH  0x05
#define AX_TORQUE_ENABLE      0x18
#define AX_MAX_TORQUE         0x0E
#define AX_GOAL_POSITION_L    0x1E
#define AX_GOAL_POSITION_H    0x1F
#define AX_START              0xFF
#define AX_BROADCAST          0xFE
#define AX_TWO_MOTOR_LENGTH   0x0E
#define AX_POS_SPEED_LENGTH   0x04
#define AX_ID_LENGTH          0x04
#define AX_SET_ID             0x03

#define RX 2
#define TX 3
#define DX 4

unsigned char Checksum;
SoftwareSerial port(TX, RX);
String str;
boolean start, done;
int x, y;

void setup() {
  Serial.begin(9600);
  port.begin(57600);
  pinMode(DX, OUTPUT);
  digitalWrite(DX, LOW);
  start = false;
  done = false;
  str = "";
  x = 128;
  y = 128;
}

void loop() {
  if (done) {
    processStr();
    doMotor();
    readMessage();
  }
}

void readMessage() {
  while (port.available() > 0) {
    char c = port.read();
    Serial.print(c, HEX);
  }
  Serial.println();
}

void serialEvent() {
  char c = (char) Serial.read();
  if (c == '<') {
    if (!start) {
      start = true;
    } 
    else {
      str = "";
    }
  }
  if (start) {
    str += c;
    if (c == '>') {
      done = true;
      start = false;
    }
  }
}

void processStr() {
  if (str.length() != 5) {
    str = "";
    done = false;
    Serial.println("Invalid");
    return;
  }
  if (str.charAt(0) != '<' || str.charAt(2) != ':' || str.charAt(4) != '>') {
    str = "";
    done = false;
    Serial.println("Invalid");
    return;
  }
  x = constrain(int(str.charAt(1)), 0, 99);
  y = constrain(int(str.charAt(3)), 0, 99);
  str = "";
  done = false;
}

void doMotor() {
  Serial.println(x);
  Serial.println(y);
  int x1 = map(x, 0, 99, 360, 660);
  int y1 = map(y, 0, 99, 360, 660);
  //  moveSpeed(1, x1, 100);
  //  moveSpeed(2, y1, 100);
  moveTwo(1, x1, 150, 2, y1, 150);
}

void enableTorque(unsigned char ID, unsigned char tor) {
  Checksum = (~(ID + AX_TORQUE_LENGTH + AX_WRITE_DATA + AX_TORQUE_ENABLE + tor))&0xFF;
  digitalWrite(DX, HIGH);
  port.write(AX_START);
  port.write(AX_START);
  port.write(ID);
  port.write(AX_TORQUE_LENGTH);
  port.write(AX_WRITE_DATA);
  port.write(AX_TORQUE_ENABLE);
  port.write(tor);
  port.write(Checksum);
  delayMicroseconds(400);
  digitalWrite(DX, LOW);
}

void setTorque(unsigned char ID, int tor) {
  unsigned char TorH, TorL;
  TorH = tor >> 8;
  TorL = tor;
  Checksum = (~(ID + AX_MAX_TORQUE_LENGTH + AX_WRITE_DATA + AX_MAX_TORQUE + TorL + TorH))&0xFF;
  digitalWrite(DX, HIGH);
  port.write(AX_START);
  port.write(AX_START);
  port.write(ID);
  port.write(AX_MAX_TORQUE_LENGTH);
  port.write(AX_WRITE_DATA);
  port.write(AX_MAX_TORQUE);
  port.write(TorL);
  port.write(TorH);
  port.write(Checksum);
  delayMicroseconds(400);
  digitalWrite(DX, LOW);
}

void moveMotor(unsigned char ID, int pos) {
  unsigned char PosH, PosL;
  PosH = pos >> 8;
  PosL = pos;
  Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + PosL + PosH))&0xFF;
  digitalWrite(DX, HIGH);
  port.write(AX_START);
  port.write(AX_START);
  port.write(ID);
  port.write(AX_GOAL_LENGTH);
  port.write(AX_WRITE_DATA);
  port.write(AX_GOAL_POSITION_L);
  port.write(PosL);
  port.write(PosH);
  port.write(Checksum);
  delayMicroseconds(400);
  digitalWrite(DX, LOW);
}

void moveSpeed(unsigned char ID, int pos, int spd) {
  unsigned char PosH, PosL;
  unsigned char SpdH, SpdL;
  PosH = pos >> 8;
  PosL = pos;
  SpdH = spd >> 8;
  SpdL = spd;
  Checksum = (~(ID + AX_GOAL_SPEED_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + PosL + PosH + SpdL + SpdH))&0xFF;
  digitalWrite(DX, HIGH);
  port.write(AX_START);
  port.write(AX_START);
  port.write(ID);
  port.write(AX_GOAL_SPEED_LENGTH);
  port.write(AX_WRITE_DATA);
  port.write(AX_GOAL_POSITION_L);
  port.write(PosL);
  port.write(PosH);
  port.write(SpdL);
  port.write(SpdH);
  port.write(Checksum);
  delayMicroseconds(400);
  digitalWrite(DX, LOW);
}

void moveTwo(unsigned char id1, int pos1, int spd1, unsigned char id2, int pos2, int spd2) {
  unsigned char PosH1, PosL1, PosH2, PosL2;
  unsigned char SpdH1, SpdL1, SpdH2, SpdL2;
  PosH1 = pos1 >> 8;
  PosL1 = pos1;
  PosH2 = pos2 >> 8;
  PosL2 = pos2;
  SpdH1 = spd1 >> 8;
  SpdL1 = spd1;
  SpdH2 = spd2 >> 8;
  SpdL2 = spd2;
  Checksum = (~(AX_BROADCAST + AX_TWO_MOTOR_LENGTH + AX_SYNC_WRITE + AX_GOAL_POSITION_L + AX_POS_SPEED_LENGTH +
    id1 + PosL1 + PosH1 + SpdL1 + SpdH1 + 
    id2 + PosL2 + PosH2 + SpdL2 + SpdH2))&0xFF;
  digitalWrite(DX, HIGH);
  port.write(AX_START);
  port.write(AX_START);
  port.write(AX_BROADCAST);
  port.write(AX_TWO_MOTOR_LENGTH);
  port.write(AX_SYNC_WRITE);
  port.write(AX_GOAL_POSITION_L);
  port.write(AX_POS_SPEED_LENGTH);
  port.write(id1);
  port.write(PosL1);
  port.write(PosH1);
  port.write(SpdL1);
  port.write(SpdH1);
  port.write(id2);
  port.write(PosL2);
  port.write(PosH2);
  port.write(SpdL2);
  port.write(SpdH2);
  port.write(Checksum);
  delayMicroseconds(400);
  digitalWrite(DX, LOW);
}

void setId(unsigned char id) {
  Checksum = (~(AX_BROADCAST + AX_ID_LENGTH + AX_WRITE_DATA + AX_SET_ID + id))&0xFF;
  digitalWrite(DX, HIGH);
  port.write(AX_START);
  port.write(AX_START);
  port.write(AX_BROADCAST);
  port.write(AX_ID_LENGTH);
  port.write(AX_WRITE_DATA);
  port.write(AX_SET_ID);
  port.write(id);
  port.write(Checksum);
  delayMicroseconds(400);
  digitalWrite(DX, LOW);
}

