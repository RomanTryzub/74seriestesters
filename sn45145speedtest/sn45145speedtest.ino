const int inpinA = 2;  
const int inpinB = 3;  
const int inpinC = A0; 
const int inpinD = A1; 

const int outPins[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

int currentDelay = 100; // Start at 100 microseconds
bool systemFailed = false;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(inpinA, OUTPUT);
  pinMode(inpinB, OUTPUT);
  pinMode(inpinC, OUTPUT);
  pinMode(inpinD, OUTPUT);

  for (int i = 0; i < 10; i++) {
    pinMode(outPins[i], INPUT_PULLUP);
  }

  Serial.println("--- SN74145 Stress Test: Finding the Breaking Point ---");
}

void loop() {
  if (systemFailed) return;

  Serial.print("Testing with delay: ");
  Serial.print(currentDelay);
  Serial.println(" us");

  for (int num = 0; num < 10; num++) {
    updateBCD(num);
    delayMicroseconds(currentDelay); 
    
    // Check if the correct pin is LOW
    if (digitalRead(outPins[num]) != LOW) {
      Serial.println("\n!!! LOGIC BREAKDOWN DETECTED !!!");
      Serial.print("Failed to switch fast enough at: ");
      Serial.print(currentDelay);
      Serial.println(" us");
      systemFailed = true;
      break;
    }
  }

  if (!systemFailed) {
    currentDelay--; // Tighten the timing for the next loop
    if (currentDelay < 1) {
      Serial.println("Reached limit of Arduino delay resolution.");
      systemFailed = true;
    }
  }
}

void updateBCD(int val) {
  digitalWrite(inpinA, (val >> 0) & 0x01);
  digitalWrite(inpinB, (val >> 1) & 0x01);
  digitalWrite(inpinC, (val >> 2) & 0x01);
  digitalWrite(inpinD, (val >> 3) & 0x01);
}