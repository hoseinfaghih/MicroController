import serial
import datetime

# تنظیمات پورت سریال
ser = serial.Serial(
    port='COM10',         # نام پورت سریال را بر اساس سیستم‌عامل و دستگاه خود تنظیم کنید
    baudrate=9600,        # نرخ باود (baud rate)
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1             # زمان انتظار برای دریافت داده (در ثانیه)
)

# دریافت ساعت سیستم به فرمت HH:MM:SS
current_time = datetime.datetime.now().strftime("%H:%M:%S")
message = current_time + '\n'
# ارسال ساعت سیستم از طریق UART
ser.write(message.encode('utf-8'))

data = ser.readline().decode('utf-8')
print(data.strip())  # Print to console (optional)

# بستن پورت سریال
ser.close()
