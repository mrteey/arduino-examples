// Stepper Motor Control with TB6600 Driver
// Commands: "right [steps]" or "left [steps]"
// Use -1 for indefinite movement until limit switch is triggered

#define STEP_PIN 5
#define DIR_PIN 2

#define ENABLE_PIN 8
#define LIMIT_SWITCH_PIN 7  // Connect your limit switch to pin 7

// Motor parameters
int stepDelay = 1000; // microseconds between steps (adjust for speed)

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    
    // Initialize pins
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    
    // Enable the motor driver
    digitalWrite(ENABLE_PIN, LOW);

    // Initialize limit switch pin
    pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP); // Switch pulls HIGH when pressed

    Serial.println("Stepper Motor Controller Ready");
    Serial.println("Commands: 'right [steps]' or 'left [steps]'");
    Serial.println("Use -1 for indefinite movement until limit switch is triggered");
    Serial.println("Send 'stop' during indefinite movement to manually stop");
}

void loop() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        parseAndExecuteCommand(command);
    }
}

void parseAndExecuteCommand(String command) {
    int spaceIndex = command.indexOf(' ');
    
    if (spaceIndex == -1) {
        Serial.println("Invalid command format");
        return;
    }
    
    String direction = command.substring(0, spaceIndex);
    String stepsStr = command.substring(spaceIndex + 1);
    int steps = stepsStr.toInt();
    
    if (steps == 0 && stepsStr != "0") {
        Serial.println("Invalid step count");
        return;
    }
    
    if (direction == "right") {
        if (steps == -1) {
            moveMotorIndefinite(true);
            Serial.println("Moving right indefinitely until limit switch");
        } else if (steps > 0) {
            moveMotor(true, steps);
            Serial.println("Moving right " + String(steps) + " steps");
        } else {
            Serial.println("Invalid step count. Use positive number or -1 for indefinite");
        }
    } else if (direction == "left") {
        if (steps == -1) {
            moveMotorIndefinite(false);
            Serial.println("Moving left indefinitely until limit switch");
        } else if (steps > 0) {
            moveMotor(false, steps);
            Serial.println("Moving left " + String(steps) + " steps");
        } else {
            Serial.println("Invalid step count. Use positive number or -1 for indefinite");
        }
    } else {
        Serial.println("Invalid direction. Use 'right' or 'left'");
    }
}

void moveMotor(bool clockwise, int steps) {
    // Set direction
    digitalWrite(DIR_PIN, clockwise ? HIGH : LOW);

    // Step the motor
    for (int i = 0; i < steps; i++) {
        // Check limit switch (active HIGH)
        if (digitalRead(LIMIT_SWITCH_PIN) == HIGH) {
            Serial.println("Limit switch triggered! Stopping motor after " + String(i) + " steps.");
            
            // Move 100 steps in the opposite direction
            Serial.println("Moving 100 steps in opposite direction...");
            digitalWrite(DIR_PIN, clockwise ? LOW : HIGH); // Reverse direction
            for (int j = 0; j < 100; j++) {
                digitalWrite(STEP_PIN, HIGH);
                delayMicroseconds(stepDelay);
                digitalWrite(STEP_PIN, LOW);
                delayMicroseconds(stepDelay);
            }
            Serial.println("Reverse movement complete");
            break;
        }
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(stepDelay);
    }

    Serial.println("Movement complete");
}

void moveMotorIndefinite(bool clockwise) {
    // Set direction
    digitalWrite(DIR_PIN, clockwise ? HIGH : LOW);

    // Move indefinitely until limit switch is triggered
    long stepCount = 0;
    while (true) {
        // Check limit switch (active HIGH)
        if (digitalRead(LIMIT_SWITCH_PIN) == HIGH) {
            Serial.println("Limit switch triggered! Stopping motor after " + String(stepCount) + " steps.");
            
            // Move 100 steps in the opposite direction
            Serial.println("Moving 100 steps in opposite direction...");
            digitalWrite(DIR_PIN, clockwise ? LOW : HIGH); // Reverse direction
            for (int j = 0; j < 100; j++) {
                digitalWrite(STEP_PIN, HIGH);
                delayMicroseconds(stepDelay);
                digitalWrite(STEP_PIN, LOW);
                delayMicroseconds(stepDelay);
            }
            Serial.println("Reverse movement complete");
            break;
        }
        
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(stepDelay);
        
        stepCount++;
        
        // Optional: Check for serial interrupt to allow manual stopping
        if (Serial.available() > 0) {
            String interrupt = Serial.readStringUntil('\n');
            interrupt.trim();
            if (interrupt == "stop") {
                Serial.println("Manual stop received after " + String(stepCount) + " steps.");
                break;
            }
        }
    }

    Serial.println("Indefinite movement complete");
}