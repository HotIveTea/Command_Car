import time
import serial

com_port = 'COM6'
baud_rate = 115200
try:
    print("Connecting to HC-05.....")
    car = serial.Serial(com_port, baud_rate, timeout=1)
    time.sleep(2)
    print("Connected Sucessfully")
except Exception as e:
    print(f"Cannot connect to HC-05 - The true is {e}")
    exit()
try:
    while(1):
        cmd = input("\nNhap cac lenh dieu khien xe, nhap 'q' de thoat ")
        if cmd.lower() == 'q':
            break
        payload = cmd + '\r\n'
        car.write(payload.encode('utf-8'))
        if car.in_waiting > 0:
            res = car.readline().decode('utf-8').strip()
            print(f"Response be like: {res}")
except KeyboardInterrupt:
    print("Force to Stop")
finally:
    car.close()
    print("Disconnected !!!!")

