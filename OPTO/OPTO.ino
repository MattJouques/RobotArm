

int IR_DETECTOR_PIN  = A15;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(IR_DETECTOR_PIN);
  //if (val > 800) {
    Serial.print("tick ");
    Serial.println(val);
    delay(250);
  //}

}
