import serial

# Configure the serial port (adjust the port name and baud rate)
ser = serial.Serial(port='COM10', baudrate=9600)

# Open a log file for writing
log_filename = 'logfile.txt'
with open(log_filename, 'w') as log_file:
    try:
        while True:
            # Read data from the serial port
            data = ser.readline().decode('utf-8')
            print(data.strip())  # Print to console (optional)

            # Write data to the log file
            log_file.write(data)
            log_file.flush()  # Flush to ensure immediate write
    except KeyboardInterrupt:
        print("\nLogging stopped. Press Ctrl+C again to exit.")

# Close the serial port
ser.close()