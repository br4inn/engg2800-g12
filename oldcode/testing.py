from tkinter import *
from tkinter import ttk, messagebox
import requests
import serial
import serial.tools.list_ports  # Import tools from serial
import json
from datetime import datetime
import time_and_date  # Ensure time_and_date.py is in the same directory

# Serial port setup
baudRate = 9600
ser = None  # Initialize serial port variable

def configure_settings_frame(root):
    settings_frame = Frame(root, padx=10, pady=10)
    settings_frame.pack()

    usb_optlink = StringVar(settings_frame, "none")  # linking variable
    Radiobutton(settings_frame, text="USB", variable=usb_optlink, value="usb").grid(column=1, row=1)
    Radiobutton(settings_frame, text="Optic Link", variable=usb_optlink, value="optic link").grid(column=2, row=1)

    twelve_24 = StringVar(settings_frame, value="1")  # linking variable
    Radiobutton(settings_frame, text="12hr", variable=twelve_24, value="12").grid(column=1, row=2)
    Radiobutton(settings_frame, text="24hr", variable=twelve_24, value="24").grid(column=2, row=2)

    def save_values():
        time_and_date.params.value_usb_optic = usb_optlink.get()
        time_and_date.params.value12_24 = twelve_24.get()

    Button(settings_frame, text="Submit preferences", command=save_values).grid(column=3, row=2)

def fetch_weather_data():
    url = "https://tp-weather.uqcloud.net/weather.json"
    try:
        response = requests.get(url)
        response.raise_for_status()
        return response.json().get('weather')
    except requests.exceptions.RequestException as e:
        messagebox.showerror("Error", f"Failed to fetch weather data: {e}")
        return None

def normalise_date(date_str):
    parts = date_str.split('-')
    if len(parts) == 3:
        year, month, day = parts
        return f"{year}-{month.zfill(2)}-{day.zfill(2)}"
    return date_str

def show_weather():
    newWindow = Toplevel(main_frame)
    newWindow.title("Weather Forecast")
    newWindow.geometry("800x600")

    weatherframe = Frame(newWindow)
    weatherframe.pack(pady=20)

    saved_weather = [
        time_and_date.params.weather1,
        time_and_date.params.weather2,
        time_and_date.params.weather3,
        time_and_date.params.weather4,
        time_and_date.params.weather5,
        time_and_date.params.weather6,
        time_and_date.params.weather7
    ]

    for i, weather_data in enumerate(saved_weather):
        if i >= 7:  # Limit to 7 entries
            break
        
        date, temperature, humidity, forecast = weather_data

        Label(weatherframe, text=date, font=("Comic Sans", 14, "bold")).grid(row=i, column=0, padx=10, pady=5, sticky="w")

        if time_and_date.params.celsius_fahrenheit == "F":
            temperature = (temperature * (9 / 5)) + 32

        Label(weatherframe, text=f"Temperature: {temperature} °{time_and_date.params.celsius_fahrenheit}", font=("Comic Sans", 14)).grid(row=i, column=1, padx=10, pady=5, sticky="w")
        Label(weatherframe, text=f"Humidity: {humidity} %", font=("Comic Sans", 14)).grid(row=i, column=2, padx=10, pady=5, sticky="w")
        Label(weatherframe, text=f"Forecast: {forecast}", font=("Comic Sans", 14)).grid(row=i, column=3, padx=10, pady=5, sticky="w")

def download_weather():
    weather = fetch_weather_data()
    if weather:
        today_date = datetime.now().strftime('%Y-%m-%d')
        weather_params = [
            time_and_date.params.weather1,
            time_and_date.params.weather2,
            time_and_date.params.weather3,
            time_and_date.params.weather4,
            time_and_date.params.weather5,
            time_and_date.params.weather6,
            time_and_date.params.weather7
        ]

        display_date = False
        for i, (date, info) in enumerate(weather.items()):
            normalised_date = normalise_date(date)

            if not display_date and normalised_date >= today_date:
                display_date = True

            if display_date and i < len(weather_params):
                weather_params[i][0] = normalised_date  # Save date
                weather_params[i][1] = info['temperature']  # Save temperature
                weather_params[i][2] = info['humidity']  # Save humidity
                weather_params[i][3] = info['forecast']  # Save forecast
    else:
        messagebox.showerror("Error", "Weather data could not be retrieved.")

def show_ports():
    # Get the current list of serial ports
    ports = list(serial.tools.list_ports.comports())
    port_options = [p.device for p in ports]
    port_combobox['values'] = port_options
    if port_options:
        port_combobox.current(0)
    else:
        port_combobox.set('No ports available')

def refresh_ports():
    show_ports()
    root.after(2000, refresh_ports)  # Check for available ports every 2 seconds

def connect_port():
    selected_port = port_combobox.get()
    if selected_port and selected_port != 'No ports available':
        try:
            global ser
            ser = serial.Serial(selected_port, baudRate, timeout=1)

            if ser.is_open:
                messagebox.showinfo("Success", f"Connected to {selected_port}")
                start_serial_polling()
            else:
                messagebox.showerror("Error", f"Failed to open {selected_port}")
        except serial.SerialException as e:
            messagebox.showerror("Error", f"Failed to connect to {selected_port}: {e}")
    else:
        messagebox.showwarning("Warning", "Please select a valid port from the dropdown.")

def start_serial_polling():
    root.after(100, poll_serial)  # Start polling every 100 milliseconds

def poll_serial():
    if ser and ser.is_open:
        try:
            if ser.in_waiting > 0:  # Data is available to read
                received_data = ser.read(ser.in_waiting).decode('ascii')  # Read data
                received_data_display.insert(END, received_data)  # Display in GUI
                received_data_display.yview(END)  # Auto-scroll
        except serial.SerialException as e:
            print(f"Serial error: {e}")
            ser.close()

    root.after(100, poll_serial)  # Continue polling  

def send_data():
    try:
        data = time_and_date.params.to_arrays()  # Dictionary to be sent
        json_data = json.dumps(data)  # Serialize to JSON
        ser.write((json_data + '\n').encode('utf-8'))  # Adding newline for delimiter
    except serial.SerialException as e:
        messagebox.showerror("Error", f"Failed to send data: {e}")

def update_time():
    current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    time_label.config(text=f"Current Time: {current_time}")
    root.after(1000, update_time)

root = Tk()
root.title("PC Software")
root.geometry("950x900")
root.configure(bg='darkgrey')
main_frame = Frame(root)
main_frame.grid(row=0, column=1, padx=5, pady=5)

# Create time label
time_label = Label(main_frame, text="", font=("Helvetica", 14))
time_label.pack(pady=2)

# Alarms GUI
top_frame = Frame(main_frame)
top_frame.pack(pady=10)

Label(top_frame, text="Alarms:", width=10).grid(row=0, column=0, padx=5, pady=5)

# Variables to hold the state of each alarm checkbox
alarm1_var = IntVar(value=0)
alarm2_var = IntVar(value=0)

# Frame to hold alarm checkboxes
alarm_frame = Frame(top_frame)
alarm_frame.grid(row=1, column=0, padx=5)

def toggle_alarm_options(alarm_var, display_cb, led_cb, buzzer_cb, message_entry, display_var):
    state = NORMAL if alarm_var.get() == 1 else DISABLED
    display_cb.config(state=state)
    led_cb.config(state=state)
    buzzer_cb.config(state=state)
    toggle_display_message(display_var, message_entry)

def toggle_display_message(display_var, message_entry):
    if display_var.get() == 1:
        message_entry.config(state=NORMAL)
    else:
        message_entry.delete(0, END)
        message_entry.config(state=DISABLED)

def create_alarm_checkboxes(frame, alarm_label, alarm_var, display_var, led_var, buzzer_var, row_index):
    alarm_cb = Checkbutton(frame, text=alarm_label, variable=alarm_var, command=lambda: toggle_alarm_options(alarm_var, display_cb, led_cb, buzzer_cb, message_entry, display_var))
    alarm_cb.grid(row=row_index, column=0, padx=5, pady=5)
    
    display_cb = Checkbutton(frame, text="Display Message", variable=display_var, state=DISABLED, command=lambda: toggle_display_message(display_var, message_entry))
    display_cb.grid(row=row_index, column=1, padx=5, pady=5)

    led_cb = Checkbutton(frame, text="LED On", variable=led_var, state=DISABLED)
    led_cb.grid(row=row_index, column=2, padx=5, pady=5)

    buzzer_cb = Checkbutton(frame, text="Buzzer On", variable=buzzer_var, state=DISABLED)
    buzzer_cb.grid(row=row_index, column=3, padx=5, pady=5)

    message_entry = Entry(frame, state=DISABLED)
    message_entry.grid(row=row_index, column=4, padx=5, pady=5)

create_alarm_checkboxes(alarm_frame, "Alarm 1", alarm1_var, IntVar(), IntVar(), IntVar(), 0)
create_alarm_checkboxes(alarm_frame, "Alarm 2", alarm2_var, IntVar(), IntVar(), IntVar(), 1)

# Serial port GUI
serial_frame = Frame(main_frame)
serial_frame.pack(pady=10)

Label(serial_frame, text="Select Serial Port:").grid(row=0, column=0, padx=5, pady=5)

port_combobox = ttk.Combobox(serial_frame)
port_combobox.grid(row=0, column=1, padx=5, pady=5)

# Call show_ports initially to populate the combobox
show_ports()

Button(serial_frame, text="Connect", command=connect_port).grid(row=0, column=2, padx=5, pady=5)

received_data_display = Text(main_frame, width=80, height=10)
received_data_display.pack(pady=10)

# Start refreshing the available ports
refresh_ports()

configure_settings_frame(main_frame)

update_time()
root.mainloop()
