void setup() {
    // Initialize serial communication
    Serial.begin(9600);
}

void loop() {
    // Read the analog value from the photoresistor
    int lightValue = analogRead(A0);
    
    // Print the value to the serial monitor
    Serial.print("Light sensor value: ");
    Serial.println(lightValue);
    
    // Wait for a short period before next reading
    delay(500);
}