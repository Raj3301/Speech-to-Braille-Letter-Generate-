#include <Servo.h>

Servo myservoP1;
Servo myservoP2;
Servo myservoH;
String voice;
int steps = 27;
int paperLine = 250;
int stepperDelay = 500;
int servodelay = 500;
int n = 7; //number char per line

void setup() {
  pinMode(6, OUTPUT); //Enable
  pinMode(5, OUTPUT); //Step
  pinMode(4, OUTPUT); //Direction
  digitalWrite(6, LOW);
  Serial.begin(9600);
  moveBackward(1000);
  //movehead();
  movepaper(paperLine);
}

String mirrior(String myString) {
  Serial.println(myString);  
  String tempString = (String)myString.charAt(1) + myString.charAt(0) + myString.charAt(3) + myString.charAt(2) + myString.charAt(5) + myString.charAt(4);
  Serial.println("inside mirrior");
  Serial.println(tempString);
  return tempString;
}

String getBraille(char ch) {
  Serial.println("inside getBraille");
  String myBraille = "000000";
  switch (ch) {
    case 'A':
    case 'a':
      myBraille = "100000";
      break;
    case 'B':
    case 'b':
      myBraille = "101000";
      break;
    case 'C':
    case 'c':
      myBraille = "110000";
      break;
    case 'D':
    case 'd':
      myBraille = "110100";
      break;
    case 'E':
    case 'e':
      myBraille = "100100";
      break;
    case 'F':
    case 'f':
      myBraille = "111000";
      break;
    case 'G':
    case 'g':
      myBraille = "111100";
      break;
    case 'H':
    case 'h':
      myBraille = "101100";
      break;
    case 'I':
    case 'i':
      myBraille = "011000";
      break;
    case 'J':
    case 'j':
      myBraille = "011100";
      break;
    case 'K':
    case 'k':
      myBraille = "100010";
      break;
    case 'L':
    case 'l':
      myBraille = "101010";
      break;
    case 'M':
    case 'm':
      myBraille = "110010";
      break;
    case 'N':
    case 'n':
      myBraille = "110110";
      break;
    case 'O':
    case 'o':
      myBraille = "100110";
      break;
    case 'P':
    case 'p':
      myBraille = "111010";
      break;
    case 'Q':
    case 'q':
      myBraille = "111110";
      break;
    case 'R':
    case 'r':
      myBraille = "101110";
      break;
    case 'S':
    case 's':
      myBraille = "011010";
      break;
    case 'T':
    case 't':
      myBraille = "011110";
      break;
    case 'U':
    case 'u':
      myBraille = "100011";
      break;
    case 'V':
    case 'v':
      myBraille = "101011";
      break;
    case 'W':
    case 'w':
      myBraille = "011101";
      break;
    case 'X':
    case 'x':
      myBraille = "110011";
      break;
    case 'Y':
    case 'y':
      myBraille = "110111";
      break;
    case 'Z':
    case 'z':
      myBraille = "100111";
      break;
    default:
      myBraille = "000000";
      break;
  }
  //return mirrior(myBraille);
  return myBraille;
}

void loop() {
  voice = "";
  while (Serial.available())   //Check if there is an available byte to read
  {
    delay(10);                   //Delay added to make thing stable
    char c = Serial.read();      //Conduct a serial read
    if (c == '#') {
      break; //Exit the loop when the # is detected after the word
    }
    voice += c;                  //Shorthand for voice = voice + c
  }

  if (voice.length() > 0) {
    Serial.println(voice);
    //Serial.println(voice.length());
    for (int v = 0; v < voice.length();) {
      int tempSize = 0;
      if (voice.length() > (v + n)) {
        tempSize = n;
      }
      else {
        tempSize = voice.length() - v;
      }
      //Serial.println(tempSize);
      String chars [tempSize];
      for (int i = 0; i < n && v < voice.length(); i++) {
        chars[i] = getBraille(voice.charAt(v++));
        Serial.println(chars[i]);
      }

      int x = 0;
      for (int i = 0; i < 3; i++) { // print 3 row
        for (int r = 0; r < tempSize; r++) {
          for (int a = 0; a < 2; a++) {
            moveForward(steps);
            Serial.println(chars[r].charAt(x + a));
            if (chars[r].charAt(x + a) == '1') {
              Serial.println("inside if");
              movehead();
            }
          }
        }
        x += 2;
        movepaper(paperLine);
        moveBackward(steps * (2 * tempSize));
      }
      movepaper(paperLine);
      delay(1000);
      movepaper(paperLine);
    }
  }

}




void moveBackward(int mm) {
  digitalWrite(4, HIGH);
  for (int b = 0; b < mm; b++) {
    digitalWrite(5, HIGH);
    delayMicroseconds(stepperDelay);
    digitalWrite(5, LOW);
    delayMicroseconds(stepperDelay);
  }
  delayMicroseconds(stepperDelay);
}

void moveForward(int mm) {
  digitalWrite(4, LOW);
  for (int f = 0; f < mm; f++) {
    digitalWrite(5, HIGH);
    delayMicroseconds(stepperDelay);
    digitalWrite(5, LOW);
    delayMicroseconds(stepperDelay);
  }
  delayMicroseconds(stepperDelay);
}

void movepaper(int delayS) {
  myservoP1.attach(11);
  myservoP2.attach(13);
  myservoP1.write(0);
  myservoP2.write(180);
  delay(delayS);
  myservoP1.detach();
  myservoP2.detach();
}

void movehead() {
  myservoH.attach(8);
  myservoH.write(0);
  delay(servodelay);
  myservoH.write(180);
  delay(servodelay+200);
  myservoH.write(0);
  delay(servodelay);
  myservoH.detach();
}
