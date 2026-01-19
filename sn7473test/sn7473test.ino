const int clkpin1 = 2;  
const int clrpin1 = 3; 
const int jpin1   = 4; 
const int kpin1   = 5;

const int qpin1   = 10;
const int qnpin1  = 11;


const int clkpin2 = 6;  
const int clrpin2 = 7; 
const int jpin2   = 8; 
const int kpin2   = 9;

const int qpin2   = 12;
const int qnpin2  = 13;




int currentDelay = 100; // Start at 100 microseconds
bool systemFailed = false;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(clkpin1, OUTPUT);
  pinMode(clrpin1, OUTPUT);
  pinMode(jpin1, OUTPUT);
  pinMode(kpin1, OUTPUT);
  
  pinMode(qpin1, INPUT);
  pinMode(qnpin1, INPUT);
  
  pinMode(clkpin2, OUTPUT);
  pinMode(clrpin2, OUTPUT);
  pinMode(jpin2, OUTPUT);
  pinMode(kpin2, OUTPUT);
  
  pinMode(qpin2, INPUT);
  pinMode(qnpin2, INPUT);
  
  digitalWrite(clrpin1, HIGH); 
  digitalWrite(clkpin1, HIGH);
  digitalWrite(clrpin2, HIGH); 
  digitalWrite(clkpin2, HIGH);
  Serial.println("--- SN7473 Stress Test: Finding the Breaking Point ---");
}

void loop() {
  // Test Case 1: Set FF1, Reset FF2
  Serial.println("Testing FF1:SET, FF2:RESET...");
  testDual(1, 0, 0, 1); 

  // Test Case 2: Reset FF1, Set FF2
  Serial.println("Testing FF1:RESET, FF2:SET...");
  testDual(0, 1, 1, 0);

  // Test Case 3: Toggle Both (J=1, K=1)
  Serial.println("Testing Dual TOGGLE...");
  testDual(1, 1, 1, 1);

  delay(3000);
}

void testDual(int j1, int k1, int j2, int k2) {
  // Apply Inputs
  digitalWrite(jpin1, j1); digitalWrite(kpin1, k1);
  digitalWrite(jpin2, j2); digitalWrite(kpin2, k2);
  
  delayMicroseconds(5); // Setup time

  // Trigger both clocks on Negative Edge (HIGH -> LOW)
  digitalWrite(clkpin1, LOW); digitalWrite(clkpin2, LOW);
  delayMicroseconds(5); // Hold time
  digitalWrite(clkpin1, HIGH); digitalWrite(clkpin2, HIGH);

  delayMicroseconds(10); // Propagation delay

  reportStatus();
}

void reportStatus() {
  Serial.print("  FF1 Q: "); Serial.print(digitalRead(qpin1));
  Serial.print(" | FF2 Q: "); Serial.println(digitalRead(qpin2));
}