import time
import serial
import customtkinter as ctk
from PIL import Image 


car = None
command_playlist = []
def connect_to_car():
    global car
    com_port = entry_com.get()
    baud_rate = 115200
    
    log_msg(f"⏳ Connecting to  {com_port}...")
    app.update() 
    
    try:
        car = serial.Serial(com_port, baud_rate, timeout=1)
        log_msg("✅ Successfully connected !!!")
        btn_connect.configure(state="disabled") 
        btn_disconnect.configure(state="normal") 
        label_status.configure(text="🟢 Online", text_color="#00cc66")
        
    except Exception as e:
        log_msg(f"❌ Error: {e}")
        car = None

def disconnect_car():
    global car
    try:
        if car and car.is_open:
            car.close()
        car = None
        log_msg("⚠️ Disconnected HC-05 !!!")
        
        # Cập nhật lại UI
        btn_connect.configure(state="normal")
        btn_disconnect.configure(state="disabled")
        label_status.configure(text="🔴 Offline", text_color="#ff5555")
        
    except Exception as e:
        log_msg(f"Lỗi khi ngắt kết nối: {e}")
def add_command(direction):
    try:
        speed = int(slider_speed.get())
        time_val = int(entry_time.get().strip())
    except ValueError:
        log_msg("Error: Wrong format")
        return
    cmd = f"{direction}{speed},{time_val}"
    command_playlist.append(cmd)
    update_playlist_ui()

def update_playlist_ui():
    txt_playlist.configure(state="normal")
    txt_playlist.delete("1.0", "end") 
    current_string = "+".join(command_playlist) 
    txt_playlist.insert("end", current_string)
    txt_playlist.configure(state="disabled")

def clear_playlist():
    command_playlist.clear()
    update_playlist_ui()

def send_playlist():
    if not command_playlist:
        log_msg("⚠️ Commands are empty")
        return
        
    if car is None or not car.is_open:
        log_msg("❌ Error: Not connected to HC-05 yet!")
        return
        
    commands = "+".join(command_playlist) + "\r\n"
    car.write(commands.encode('utf-8'))
    log_msg(f"🚀 Sent: {commands.strip()}")
    clear_playlist()

def log_msg(text):
    txt_log.configure(state="normal")
    txt_log.insert("end", text + "\n")
    txt_log.see("end")
    txt_log.configure(state="disabled")


ctk.set_appearance_mode("dark")
app = ctk.CTk()
app.geometry("550x750")
app.title("SmartCar Command Builder")
app.configure(fg_color="#2c3e50")

frame_header = ctk.CTkFrame(app, fg_color="transparent")
frame_header.pack(pady=15)

try:
   
    my_sticker = ctk.CTkImage(
        light_image=Image.open("download.png"), 
        dark_image=Image.open("download.png"),
        size=(60, 60)
    )
   
    label_logo = ctk.CTkLabel(frame_header, image=my_sticker, text="")
    label_logo.grid(row=0, column=0, padx=10)
except Exception:
    pass 

label_title = ctk.CTkLabel(frame_header, text="COMMAND STATION", font=('Consolas', 26, "bold"), text_color="#00ffff")
label_title.grid(row=0, column=1)


frame_conn = ctk.CTkFrame(app, corner_radius=15, border_width=2, border_color="#333333")
frame_conn.pack(pady=5, padx=20, fill="x", ipady=5)

entry_com = ctk.CTkEntry(frame_conn, width=80, font=("Arial", 14, "bold"), justify="center")
entry_com.insert(0, "COM6")
entry_com.grid(row=0, column=0, padx=20, pady=10)   

btn_connect = ctk.CTkButton(frame_conn, text="🔌 Connect", command=connect_to_car, 
                            fg_color="#27ae60", hover_color="#2ecc71", corner_radius=8, width=100)
btn_connect.grid(row=0, column=1, padx=5)

btn_disconnect = ctk.CTkButton(frame_conn, text="❌ Disconnect", command=disconnect_car, state="disabled",
                               fg_color="#c0392b", hover_color="#e74c3c", corner_radius=8, width=100)
btn_disconnect.grid(row=0, column=2, padx=5)

label_status = ctk.CTkLabel(frame_conn, text="🔴 Offline", font=("Arial", 14, "bold"), text_color="#e74c3c")
label_status.grid(row=0, column=3, padx=(10, 20))

frame_params = ctk.CTkFrame(app, corner_radius=15, fg_color="#262626")
frame_params.pack(pady=15, padx=20, fill="x")

label_speed = ctk.CTkLabel(frame_params, text="⚡ Speed (%):", font=("Arial", 14, "bold"), text_color="#ffcc00")
label_speed.grid(row=0, column=0, padx=15, pady=10)
slider_speed = ctk.CTkSlider(frame_params, from_=0, to=200, number_of_steps=200, width=180, 
                             button_color="#ffcc00", button_hover_color="#e6b800", progress_color="#ffcc00")
slider_speed.set(60)
slider_speed.grid(row=0, column=1, padx=10)

label_time = ctk.CTkLabel(frame_params, text="⏱️ Time (ms):", font=("Arial", 14, "bold"), text_color="#ff66b2")
label_time.grid(row=1, column=0, padx=15, pady=10)
entry_time = ctk.CTkEntry(frame_params, width=180, border_color="#ff66b2")
entry_time.insert(0, "1000")
entry_time.grid(row=1, column=1, padx=10)


label_add = ctk.CTkLabel(app, text="Controls", font=("Arial", 14, "bold"))
label_add.pack(pady=(5, 0))

frame_buttons = ctk.CTkFrame(app, fg_color="transparent")
frame_buttons.pack()


btn_kwargs = {"width": 80, "height": 40, "corner_radius": 20, "font": ("Arial", 14, "bold")}

btn_f = ctk.CTkButton(frame_buttons, text="⬆️ Fwd", command=lambda: add_command('F'), fg_color="#3399ff", hover_color="#2673cc", **btn_kwargs)
btn_f.grid(row=0, column=1, pady=5, padx=5)

btn_l = ctk.CTkButton(frame_buttons, text="⬅️ Left", command=lambda: add_command('L'), fg_color="#9933ff", hover_color="#7326cc", **btn_kwargs)
btn_l.grid(row=1, column=0, pady=5, padx=5)

btn_s = ctk.CTkButton(frame_buttons, text="⏹️ STOP", command=lambda: add_command('S'), fg_color="#ff3333", hover_color="#cc0000", border_width=2, border_color="#ff9999", **btn_kwargs)
btn_s.grid(row=1, column=1, pady=5, padx=5)

btn_r = ctk.CTkButton(frame_buttons, text="➡️ Right", command=lambda: add_command('R'), fg_color="#9933ff", hover_color="#7326cc", **btn_kwargs)
btn_r.grid(row=1, column=2, pady=5, padx=5)

btn_b = ctk.CTkButton(frame_buttons, text="⬇️ Back", command=lambda: add_command('B'), fg_color="#3399ff", hover_color="#2673cc", **btn_kwargs)
btn_b.grid(row=2, column=1, pady=5, padx=5)

txt_playlist = ctk.CTkTextbox(app, width=450, height=50, fg_color="#1a1a1a", text_color="#ffff00", 
                              font=("Consolas", 16, "bold"), border_width=1, border_color="#ffff00")
txt_playlist.pack(pady=10)
txt_playlist.configure(state="disabled")

frame_action = ctk.CTkFrame(app, fg_color="transparent")
frame_action.pack(pady=5)

btn_send = ctk.CTkButton(frame_action, text="🚀 SEND TO CAR", command=send_playlist, width=250, height=45, 
                         font=("Arial", 16, "bold"), fg_color="#00cc66", hover_color="#00994c", corner_radius=22)
btn_send.grid(row=0, column=0, padx=10)

btn_clear = ctk.CTkButton(frame_action, text="🗑️ Clear", command=clear_playlist, width=90, height=45, 
                          fg_color="#4d4d4d", hover_color="#333333", corner_radius=22)
btn_clear.grid(row=0, column=1, padx=10)

txt_log = ctk.CTkTextbox(app, width=450, height=120, fg_color="#0d0d0d", text_color="#00ff00", 
                         font=("Consolas", 12), border_width=1, border_color="#00ff00")
txt_log.pack(pady=15)
txt_log.configure(state="disabled")

app.mainloop()

if car and car.is_open:
    car.close()