
const int inpina1 = 2;   
const int inpinb1 = 3;
const int outpin1 = 4;

const int inpina2 = 5;   
const int inpinb2 = 6;
const int outpin2 = 7;

const int inpina3 = 8;   
const int inpinb3 = 9;
const int outpin3 = 10;

const int inpina4 = 11;   
const int inpinb4 = 12;
const int outpin4 = 13;

const int gateInputs[4][2] = {
  {inpina1, inpinb1}, {inpina2, inpinb2},
  {inpina3, inpinb3}, {inpina4, inpinb4}
};
const int gateOutputs[4] = {outpin1, outpin2, outpin3, outpin4};




void setup() {
  Serial.begin(115200);
  while (!Serial);

 // Initialize pins as defined in your setup
  for (int i = 0; i < 4; i++) {
    pinMode(gateInputs[i][0], OUTPUT);
    pinMode(gateInputs[i][1], OUTPUT);
    pinMode(gateOutputs[i], INPUT); 
  }
  
  Serial.println("--- SN7447AN Automated Logic Validator ---");
}

void loop() {
  bool chipFault = false;

  for (int g = 0; g < 4; g++) {
    Serial.print("Testing Gate "); Serial.print(g + 1);
    Serial.println(":");
    
    if (!runNandTruthTable(g)) {
      chipFault = true;
    }
    Serial.println("-----------------------");
  }

  if (chipFault) {
    Serial.println("OVERALL RESULT: CHIP FAILED (Check Pins)");
  } else {
    Serial.println("OVERALL RESULT: CHIP PASSED (All Gates OK)");
  }

  delay(5000); 
}

bool runNandTruthTable(int gateIdx) {
  bool gateOk = true;
  for (int a = 0; a < 2; a++) {
    for (int b = 0; b < 2; b++) {
      digitalWrite(gateInputs[gateIdx][0], a);
      digitalWrite(gateInputs[gateIdx][1], b);
      
      // Delay to allow for physical gate propagation delay (~15ns)
      delayMicroseconds(20); 

      int actual = digitalRead(gateOutputs[gateIdx]);
      int expected = !(a && b); // NAND logic

      Serial.print("  In: "); Serial.print(a); Serial.print(b);
      Serial.print(" | Out: "); Serial.print(actual);
      
      if (actual != expected) {
        Serial.println(" -> !! FAIL !!");
        gateOk = false;
      } else {
        Serial.println(" -> OK");
      }
    }
  }
  return gateOk;
}