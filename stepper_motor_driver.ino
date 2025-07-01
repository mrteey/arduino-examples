// Stepper Motor Control with TB6600 Driver
// Commands: "right [steps]" or "left [steps]"

#define STEP_PIN 5
#define DIR_PIN 2
#define ENABLE_PIN 8

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
    
    Serial.println("Stepper Motor Controller Ready");
    Serial.println("Commands: 'right [steps]' or 'left [steps]'");
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
    
    if (steps <= 0) {
        Serial.println("Invalid step count");
        return;
    }
    
    if (direction == "right") {
        moveMotor(true, steps);
        Serial.println("Moving right " + String(steps) + " steps");
    } else if (direction == "left") {
        moveMotor(false, steps);
        Serial.println("Moving left " + String(steps) + " steps");
    } else {
        Serial.println("Invalid direction. Use 'right' or 'left'");
    }
}

void moveMotor(bool clockwise, int steps) {
    // Set direction
    digitalWrite(DIR_PIN, clockwise ? HIGH : LOW);
    
    // Step the motor
    for (int i = 0; i < steps; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(stepDelay);
    }
    
    Serial.println("Movement complete");
}