// Exact pin assignments from your uploaded code
const int inpins[] = {2, 4, 6, 8, 10, 12};
const int outpins[] = {3, 5, 7, 9, 11, 13};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Initialize pins based on your setup code
  for (int i = 0; i < 6; i++) {
    pinMode(inpins[i], OUTPUT);
    pinMode(outpins[i], INPUT);
  }

  Serial.println("--- SN74LS14N Hex Inverter: Physical Logic Test ---");
}

void loop() {
  bool chipStatus = true;

  for (int gate = 0; gate < 6; gate++) {
    Serial.print("Testing Gate "); Serial.print(gate + 1);
    
    // Check both logic states: LOW -> HIGH and HIGH -> LOW
    bool gatePassed = testInverter(gate);
    
    if (!gatePassed) {
      chipStatus = false;
      Serial.println(" -> FAILED");
    } else {
      Serial.println(" -> PASSED");
    }
  }

  if (chipStatus) {
    Serial.println("OVERALL: CHIP INTEGRITY OK");
  } else {
    Serial.println("OVERALL: PHYSICAL DEFECT DETECTED");
  }

  Serial.println("-------------------------------------------");
  delay(5000); 
}

bool testInverter(int idx) {
  bool passed = true;
  int states[] = {LOW, HIGH};

  for (int i = 0; i < 2; i++) {
    int inputVal = states[i];
    digitalWrite(inpins[idx], inputVal);
    
    // Wait for propagation delay and breadboard stabilization
    delayMicroseconds(20); 

    int actual = digitalRead(outpins[idx]);
    int expected = !inputVal;

    if (actual != expected) {
      passed = false;
      Serial.print(" [Error at Input "); Serial.print(inputVal);
      Serial.print(": Got "); Serial.print(actual); Serial.print("]");
    }
  }
  return passed;
}