// Mapping from your code
const int inpina = 9;   
const int inpinb = 10;
const int inpinc = 11;  
const int inpind = 12;
const int inpinlt = 13; 
const int inpinbin = A0; 
const int inpinrbin = A1;

const int outpins[] = {2, 3, 4, 5, 6, 7, 8}; // a, b, c, d, e, f, g

// The Truth Table for SN7447AN (Active LOW outputs)
// Each byte represents segments g-f-e-d-c-b-a
// 0 = Segment ON (LOW), 1 = Segment OFF (HIGH)
const byte segmentTable[] = {
  0b1000000, // 0: g off
  0b1111001, // 1: a,d,e,f,g off
  0b0100100, // 2: c,f off
  0b0110000, // 3: e,f off
  0b0011001, // 4: a,d,e off
  0b0010010, // 5: b,e off
  0b0000010, // 6: b off
  0b1111000, // 7: d,e,f,g off
  0b0000000, // 8: all on
  0b0010000  // 9: e off
};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(inpina, OUTPUT); 
  pinMode(inpinb, OUTPUT);
  pinMode(inpinc, OUTPUT); 
  pinMode(inpind, OUTPUT);
  pinMode(inpinlt, OUTPUT); 
  pinMode(inpinbin, OUTPUT); 
  pinMode(inpinrbin, OUTPUT);

  for (int i = 0; i < 7; i++) {
    pinMode(outpins[i], INPUT_PULLUP); // Handle Open-Collector
  }

  // Initialize: Normal Operation (Control pins HIGH)
  digitalWrite(inpinlt, HIGH); 
  digitalWrite(inpinbin, HIGH); 
  digitalWrite(inpinrbin, HIGH);

  Serial.println("--- SN7447AN Automated Logic Validator ---");
}

void loop() {
  for (int num = 0; num < 10; num++) {
    validateDigit(num);
    delay(1500); 
  }
}

void validateDigit(int val) {
  // 1. Drive BCD
  digitalWrite(inpina, (val >> 0) & 0x01);
  digitalWrite(inpinb, (val >> 1) & 0x01);
  digitalWrite(inpinc, (val >> 2) & 0x01);
  digitalWrite(inpind, (val >> 3) & 0x01);

  delayMicroseconds(50); // Settlement time for TTL gates

  // 2. Read Physical State
  byte physicalState = 0;
  for (int i = 0; i < 7; i++) {
    if (digitalRead(outpins[i]) == HIGH) {
      physicalState |= (1 << i); // Set bit if segment is OFF
    }
  }

  // 3. Compare to Ideal Truth Table
  Serial.print("Testing BCD ["); 
  Serial.print(val); 
  Serial.print("]: ");
  
  if (physicalState == segmentTable[val]) {
    Serial.println("PASS (Logic Matches DataSheet)");
  } else {
    Serial.print("FAIL! Expected: 0b"); 
    Serial.print(segmentTable[val], BIN);
    Serial.print(" | Actual: 0b"); 
    Serial.println(physicalState, BIN);
    diagnoseError(segmentTable[val], physicalState);
  }
}

void diagnoseError(byte expected, byte actual) {
  for (int i = 0; i < 7; i++) {
    bool expBit = (expected >> i) & 0x01;
    bool actBit = (actual >> i) & 0x01;
    if (expBit != actBit) {
      Serial.print("  -> Physical Error on Segment ");
      Serial.print((char)('a' + i));
      Serial.println(actBit ? " (Stuck HIGH/Weak Pullup)" : " (Stuck LOW/Short)");
    }
  }
}