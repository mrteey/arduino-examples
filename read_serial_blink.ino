void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    if (Serial.available() > 0) {
        String command = Serial.readString();
        command.trim();
        
        int spaceIndex = command.indexOf(' ');
        
        if (spaceIndex > 0) {
            String mainCommand = command.substring(0, spaceIndex);
            String parameter = command.substring(spaceIndex + 1);
            
            if (mainCommand.equals("blink")) {
                int blinkCount = parameter.toInt();
                if (blinkCount > 0) {
                    for (int i = 0; i < blinkCount; i++) {
                        digitalWrite(LED_BUILTIN, HIGH);
                        delay(500);
                        digitalWrite(LED_BUILTIN, LOW);
                        delay(500);
                    }
                }
            }
        } else if (command.equals("blink")) {
            // Default to 2 blinks if no parameter
            for (int i = 0; i < 2; i++) {
                digitalWrite(LED_BUILTIN, HIGH);
                delay(500);
                digitalWrite(LED_BUILTIN, LOW);
                delay(500);
            }
        }
    }
}