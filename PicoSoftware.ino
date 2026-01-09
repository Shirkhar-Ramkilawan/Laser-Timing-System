const int greenLED = 12;
const int redLED   = 13;
const int button   = 16;
const int laserPin = 15;  
const int ldrPin   = 26;   // ADC0
const int adc1Pin  = 27;   // ADC1 for laser voltage

bool laserOn = false;
bool lastButtonState = HIGH;

unsigned long startTime = 0;
bool timing = false;
int threshold = 600;  // Default threshold, can be changed via serial
bool lastBeamBroken = false;

// Calibration variables
String inputString = "";
bool stringComplete = false;
int calibrationThreshold = 600;

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  digitalWrite(laserPin, HIGH);   // Laser OFF
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);

  Serial.begin(115200);
  
  // Reserve space for input string
  inputString.reserve(10);
  
  Serial.println("System Ready");
  Serial.println("Commands: '1'=Laser ON, '0'=Laser OFF, 'T<number>'=Set Threshold");
}

void loop() {
  // --- Serial commands ---
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    
    if (inChar == '\n') {
      stringComplete = true;
    } else if (inChar == '1') {
      laserOn = true;
      timing = false;
    } else if (inChar == '0') {
      laserOn = false;
      timing = false;
    } else if (inChar == 'T') {
      // Start of threshold command
      inputString = "";
    } else if (inChar >= '0' && inChar <= '9') {
      // Add digit to threshold command
      inputString += inChar;
    }
  }
  
  // Process complete threshold command
  if (stringComplete && inputString.length() > 0) {
    int newThreshold = inputString.toInt();
    if (newThreshold >= 0 && newThreshold <= 1023) {
      threshold = newThreshold;
      calibrationThreshold = newThreshold;
      Serial.print("Threshold set to: ");
      Serial.println(threshold);
    } else {
      Serial.println("Invalid threshold value (0-1023)");
    }
    inputString = "";
    stringComplete = false;
  }

  // --- Button toggle ---
  bool buttonState = digitalRead(button);
  if (lastButtonState == HIGH && buttonState == LOW) {
    laserOn = !laserOn;
    timing = false;
    delay(200); // debounce
  }
  lastButtonState = buttonState;

  // --- LEDs + Laser ---
  if (laserOn) {
    digitalWrite(laserPin, LOW); // active low
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
  } else {
    digitalWrite(laserPin, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
  }

  // --- LDR + Voltage ---
  int ldrValue = analogRead(ldrPin);    
  int rawVoltage = analogRead(adc1Pin); 
  float voltage = (rawVoltage / 1023.0) * 3.3;
  bool beamBroken = (ldrValue > threshold);

  // --- Timing / lap detection ---
  unsigned long elapsedNow = 0;
  if (laserOn) {
    if (beamBroken && !lastBeamBroken) {
      if (!timing) {
        startTime = millis();
        timing = true;
      } else {
        elapsedNow = millis() - startTime;
        timing = false;
      }
    }
    if (timing) elapsedNow = millis() - startTime;
  }
  lastBeamBroken = beamBroken;

  // --- Send CSV for Web Interface ---
  Serial.print(laserOn ? 1 : 0); Serial.print(",");
  Serial.print(beamBroken ? 1 : 0); Serial.print(",");
  Serial.print(ldrValue); Serial.print(",");
  Serial.print(voltage, 2); Serial.print(",");
  Serial.print(timing ? 1 : 0); Serial.print(",");
  Serial.println(elapsedNow);

  delay(50);
}
