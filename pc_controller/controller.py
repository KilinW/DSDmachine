import serial, time


# ser.port = "COM24"
# Baudrate: 115200,N,8,1
# ser.baudrate = 115200


# ser.bytesize = serial.EIGHTBITS         # Number of bits per bytes
# ser.parity = serial.PARITY_NONE         # Set parity check
# ser.stopbits = serial.STOPBITS_ONE      # number of stop bits

# ser.timeout = 0.5                       # Non-block read 0.5s
# ser.writeTimeout = 0.5                  # Timeout for write 0.5s
# ser.xonxoff = False                     # Disable software flow control
# ser.rtscts = False                      # Disable hardware (RTS/CTS) flow control
# ser.dsrdtr = False                      # Disable hardware (DSR/DTR) flow control

try:
    ser = serial.Serial("COM24", 115200)
except Exception as ex:
    print("open serial port error " + str(ex))
    exit()
    
while ser.isOpen():
    try:
        ser.flushInput()                # Flush input buffer
        ser.flushOutput()               # Flush output bufferaw11

        ser.write(b'test\r\n')
        time.sleep(0.5)
        received = ser.readline(20).decode()
        print(received)
        time.sleep(0.5)
        
    except Exception as e1:
        print("communication error " + str(e1))
else:
    print("open serial port error")
        