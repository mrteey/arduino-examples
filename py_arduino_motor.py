import dotenv
import serial
import time
import os

dotenv.load_dotenv()

# Configure the serial connection
arduino = serial.Serial(os.getenv("BOARD_ID"), 9600, timeout=1)
time.sleep(2)  # Wait for connection to establish

try:
    while True:
        # Get text input from user
        text_to_send = input("Enter text to send to Arduino: ")
        
        # Send text to Arduino
        arduino.write(text_to_send.encode())
        
        # Optional: Read response from Arduino
        if arduino.in_waiting > 0:
            response = arduino.readline().decode().strip()
            print(f"Arduino response: {response}")
        
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting...")
finally:
    arduino.close()