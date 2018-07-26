unsigned int snd = 0;
 
void setup() {
    Serial.begin(9600);
}
 
void loop() {
    snd = analogRead(A0);
 
    Serial.println(snd);
    delay(100);
}

