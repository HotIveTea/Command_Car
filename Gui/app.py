import time
import serial
import customtkinter as ctk

com_port = 'COM6'
baud_rate = 115200

try:
    print("Connecting to HC-05.....")
    car = serial.Serial(com_port, baud_rate, timeout=1)
    time.sleep(2)
    print("Connected Sucessfully")
except Exception as e:
    print(f"Cannot connect to HC-05 - {e}")
    car = None

# Danh sách chứa các lệnh chưa gửi
command_playlist = []

def add_command(direction):
    # Lấy thông số hiện tại
    try:
        speed = int(slider_speed.get())
        time_val = int(entry_time.get().strip())
    except ValueError:
        log_msg("Lỗi: Nhập sai định dạng số!")
        return

    # Tạo lệnh đơn và nhét vào Playlist
    cmd = f"{direction}{speed},{time_val}"
    command_playlist.append(cmd)
    
    # Cập nhật màn hình UI
    update_playlist_ui()

def update_playlist_ui():
    txt_playlist.configure(state="normal")
    txt_playlist.delete("1.0", "end") # Xóa cũ
    # Nối các lệnh bằng dấu +, giống y hệt chuẩn anh em mình làm trên C
    chuoi_hien_tai = "+".join(command_playlist) 
    txt_playlist.insert("end", chuoi_hien_tai)
    txt_playlist.configure(state="disabled")

def clear_playlist():
    command_playlist.clear()
    update_playlist_ui()

def send_playlist():
    if not command_playlist:
        log_msg("⚠️ Commands are empty")
        return
        
    if car is None or not car.is_open:
        log_msg("❌ Error: Can't connect to hardware")
        return
    chuoi_tong = "+".join(command_playlist) + "\r\n"
    
    car.write(chuoi_tong.encode('utf-8'))
    log_msg(f"🚀 Commands launch: {chuoi_tong.strip()}")
    clear_playlist()

def log_msg(text):
    txt_log.configure(state="normal")
    txt_log.insert("end", text + "\n")
    txt_log.see("end")
    txt_log.configure(state="disabled")
ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

app = ctk.CTk()
app.geometry("500x650")
app.title("SmartCar Command Builder")

label_title = ctk.CTkLabel(app, text="🛠️ COMMAND STATION", font=('Arial', 22, "bold"), text_color="#00ffcc")
label_title.pack(pady=10)

frame_params = ctk.CTkFrame(app)
frame_params.pack(pady=10, padx=20, fill="x")

label_speed = ctk.CTkLabel(frame_params, text="Set Speed (%):", font=("Arial", 14))
label_speed.grid(row=0, column=0, padx=10, pady=10)
slider_speed = ctk.CTkSlider(frame_params, from_=0, to=100, number_of_steps=20, width=150)
slider_speed.set(60)
slider_speed.grid(row=0, column=1, padx=10)

label_time = ctk.CTkLabel(frame_params, text="Set Time (ms):", font=("Arial", 14))
label_time.grid(row=1, column=0, padx=10, pady=10)
entry_time = ctk.CTkEntry(frame_params, width=150)
entry_time.insert(0, "1000")
entry_time.grid(row=1, column=1, padx=10)

label_add = ctk.CTkLabel(app, text="Add more commands:", font=("Arial", 14, "bold"))
label_add.pack(pady=(10, 0))

frame_buttons = ctk.CTkFrame(app, fg_color="transparent")
frame_buttons.pack(pady=5)

btn_f = ctk.CTkButton(frame_buttons, text="⬆️ Forward", command=lambda: add_command('F'), width=70)
btn_f.grid(row=0, column=1, pady=5, padx=5)

btn_l = ctk.CTkButton(frame_buttons, text="⬅️ Lefr", command=lambda: add_command('L'), width=70)
btn_l.grid(row=1, column=0, pady=5, padx=5)

btn_s = ctk.CTkButton(frame_buttons, text="⏹️ Stop", command=lambda: add_command('S'), width=70, fg_color="#ff5555", hover_color="#cc0000")
btn_s.grid(row=1, column=1, pady=5, padx=5)

btn_r = ctk.CTkButton(frame_buttons, text="➡️ Right", command=lambda: add_command('R'), width=70)
btn_r.grid(row=1, column=2, pady=5, padx=5)

btn_b = ctk.CTkButton(frame_buttons, text="⬇️ Backward", command=lambda: add_command('B'), width=70)
btn_b.grid(row=2, column=1, pady=5, padx=5)

# --- HIỂN THỊ CHUỖI LỆNH ĐANG XẾP HÀNG ---
label_playlist = ctk.CTkLabel(app, text="Commands are available:", font=("Arial", 14, "bold"), text_color="#ffff99")
label_playlist.pack(pady=(10, 0))

txt_playlist = ctk.CTkTextbox(app, width=420, height=60, fg_color="#333333", text_color="#ffff00", font=("Consolas", 16))
txt_playlist.pack(pady=5)
txt_playlist.configure(state="disabled")

# --- NÚT GỬI VÀ XÓA ---
frame_action = ctk.CTkFrame(app, fg_color="transparent")
frame_action.pack(pady=10)

btn_send = ctk.CTkButton(frame_action, text="🚀 SEND", command=send_playlist, width=200, height=40, font=("Arial", 14, "bold"), fg_color="#00cc66", hover_color="#00994c")
btn_send.grid(row=0, column=0, padx=10)

btn_clear = ctk.CTkButton(frame_action, text="🗑️ delete", command=clear_playlist, width=100, height=40, fg_color="#737373", hover_color="#4d4d4d")
btn_clear.grid(row=0, column=1, padx=10)

# --- TERMINAL LOG ---
txt_log = ctk.CTkTextbox(app, width=420, height=100, fg_color="#1e1e1e", text_color="#00ff00", font=("Consolas", 12))
txt_log.pack(pady=10)
txt_log.configure(state="disabled")

app.mainloop()
if car and car.is_open:
    car.close()