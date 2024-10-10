import struct
from tkinter import *
from tkinter.ttk import *
import tkinter as tk
from tkinter import ttk, messagebox
import requests
from tkinter import scrolledtext
import serial
from serial import Serial, SerialException
import serial.tools.list_ports
import json
from tkinter import PhotoImage
import time_and_date as time_and_date #time and date GUI file

from PIL import Image, ImageTk
from pathlib import Path #needs approval
from datetime import datetime
import time

# Serial port setup
baudRate = 9600

  

def configure_settings_frame(root):
    settings_frame = tk.Frame(root, padx=10, pady=10)
    settings_frame.pack()

    usb_optlink = tk.StringVar(settings_frame, "none")  # linking variable
    usb = tk.Radiobutton(settings_frame, text="USB", variable=usb_optlink, value="usb")
    usb.grid(column=1, row=1)

    optic_link = tk.Radiobutton(settings_frame, text="Optic Link", variable=usb_optlink, value="optic link")
    optic_link.grid(column=2, row=1)

    twelve_24 = tk.StringVar(settings_frame, value="1")  # linking variable
    twelve = tk.Radiobutton(settings_frame, text="12hr", variable=twelve_24, value="12") 
    twelve.grid(column=1, row=2)

    twenty4 = tk.Radiobutton(settings_frame, text="24hr", variable=twelve_24, value="24")
    twenty4.grid(column=2, row=2)

    def save_values():
        time_and_date.params.value_usb_optic = usb_optlink.get()
        time_and_date.params.value12_24 = twelve_24.get()
        # Trigger the optical link display if 'Optic Link' is selected
        # if value_usb_optic == "optic link":
        #     transmit_time_date()

    submit_preferences = tk.Button(settings_frame, text="Submit preferences", command=save_values)
    submit_preferences.grid(column=3, row=2)

def trigger_transmit():
    # Call the transmit function and pass the root window
    time_and_date.transmit_time_date(optical_frame)

# Function to fetch weather data from the server
def fetch_weather_data():
    url = "https://tp-weather.uqcloud.net/weather.json"
    try:
        response = requests.get(url)
        response.raise_for_status()
        data = response.json()
        return data['weather']
    except requests.exceptions.RequestException as e:
        messagebox.showerror("Error", f"Failed to fetch weather data: {e}")
        return None

#Ensure weather starts from today
def normalise_date(date_str):
    # Ensure the date is in the format YYYY-MM-DD
    parts = date_str.split('-')
    if len(parts) == 3:
        year, month, day = parts
        month = month.zfill(2)  # Add leading zero if needed
        day = day.zfill(2)      # Add leading zero if needed
        return f"{year}-{month}-{day}"
    return date_str

def show_weather():
    # Create a new window for displaying the weather
    newWindow = tk.Toplevel(main_frame)
    newWindow.title("Weather Forecast")
    newWindow.geometry("800x600")

    weatherframe = tk.Frame(newWindow)
    weatherframe.pack(pady=20)

    # List of weather data saved in the Parameter class
    saved_weather = [
        time_and_date.params.weather1,
        time_and_date.params.weather2,
        time_and_date.params.weather3,
        time_and_date.params.weather4,
        time_and_date.params.weather5,
        time_and_date.params.weather6,
        time_and_date.params.weather7
    ]

    # Initialise counter for displaying the first 7 days
    i = 0
    
    # Iterate through saved weather data
    for weather_data in saved_weather:
        if i >= 7:  # Limit to 7 entries
            break
        
        # Extract date, temperature, humidity, and forecast
        date, temperature, humidity, forecast = weather_data

        # Display date
        date_label = tk.Label(weatherframe, text=date, font=("Comic Sans", 14, "bold"))
        date_label.grid(row=i, column=0, padx=10, pady=5, sticky="w")

        # Convert temperature to Fahrenheit if necessary
        if time_and_date.params.celsius_fahrenheit == "F":
            temperature = (temperature * (9/5)) + 32

        # Display temperature
        temp_label = tk.Label(weatherframe, text=f"Temperature: {temperature} °{time_and_date.params.celsius_fahrenheit}", font=("Comic Sans", 14))
        temp_label.grid(row=i, column=1, padx=10, pady=5, sticky="w")

        # Display humidity
        humidity_label = tk.Label(weatherframe, text=f"Humidity: {humidity} %", font=("Comic Sans", 14))
        humidity_label.grid(row=i, column=2, padx=10, pady=5, sticky="w")

        # Display forecast
        forecast_label = tk.Label(weatherframe, text=f"Forecast: {forecast}", font=("Comic Sans", 14))
        forecast_label.grid(row=i, column=3, padx=10, pady=5, sticky="w")

        i += 1


def download_weather():
    weather = fetch_weather_data()

    if weather:
        # Get today's date and normalize it
        today_date = datetime.now().strftime('%Y-%m-%d')

        # List of weather parameters to save into (weather1, weather2, etc.)
        weather_params = [
            time_and_date.params.weather1,
            time_and_date.params.weather2,
            time_and_date.params.weather3,
            time_and_date.params.weather4,
            time_and_date.params.weather5,
            time_and_date.params.weather6,
            time_and_date.params.weather7
        ]

        # Initialise flag to start saving from today's dates
        display_date = False
        i = 0
        
        for date, info in weather.items():
            normalised_date = normalise_date(date)

            # Start saving weather from today's date
            if not display_date:
                if normalised_date >= today_date:
                    display_date = True
            
            if display_date:
                if i >= len(weather_params):  # Limit to the size of weather_params list
                    break
                
                # Save data to the corresponding weather parameters
                weather_params[i][0] = normalised_date
                weather_params[i][1] = info['temperature']
                weather_params[i][2] = info['humidity'] 
                weather_params[i][3] = info['forecast'] 

                i += 1
    else:
        messagebox.showerror("Error", "Weather data could not be retrieved.")




def show_ports():
    ports = list(serial.tools.list_ports.comports())
    port_options = [p.device for p in ports]
    port_combobox['values'] = port_options
    if port_options:
        port_combobox.current(0)
    else:
        port_combobox.set('No ports available')

def refresh_ports():
    show_ports()
    root.after(5000, refresh_ports)  # Refresh every 5 seconds


def connect_port():
    selected_port = port_combobox.get()
    if selected_port and selected_port != 'No ports available':
        try:
            global ser
            ser = Serial(selected_port, baudRate, timeout=1)

            if ser.is_open:
                messagebox.showinfo("Success", f"Connected to {selected_port}")
                start_serial_polling()
            else:
             messagebox.showerror("Error", f"Failed to open {selected_port}")
        except SerialException as e:
            messagebox.showerror("Error", f"Failed to connect to {selected_port}: {e}")
    else:
        messagebox.showwarning("Warning", "Please select a valid port from the dropdown.")

def start_serial_polling():
  root.after(100, poll_serial)  # Start polling every 100 milliseconds

def poll_serial():

  if ser and ser.is_open:
    try:
        
        if ser.in_waiting > 0: # Data is available to read
            received_data = ser.read(ser.in_waiting).decode('ascii')  # Read data
            print(f"Received: {received_data}")  # Debug statement to confirm reception

            received_data_display.insert(tk.END, received_data)  # Display in GUI
            received_data_display.yview(tk.END)  # Auto-scroll

            # If microcontroller checks connection
            if received_data == "CHECK": # "CHECK" received from microcontroller
                ser.write(b"ACK\n")  # Send "PONG" in response
                print("Connection active")

    except SerialException as e:
        print(f"Serial error: {e}")
        ser.close()
  
  root.after(100, poll_serial)  # Continue polling  




def send_data(params):
    # Check if any alarms are set
    # alarm1_selected = params.alarm1_set == 1
    # alarm2_selected = params.alarm2_set == 1

    # # Set messages to 256 underscores if corresponding alarms are not selected
    # if not alarm1_selected:
    #     params.message1 = '_' * 256  # Set message1 to 256 underscores if alarm1 is not selected
    # if not alarm2_selected:
    #     params.message2 = '_' * 256  # Set message2 to 256 underscores if alarm2 is not selected


    
    # Create a formatted string from the parameters  NOT SENDING MSG AND 2 ATM
    data_to_send = f"""
{params.celsius_fahrenheit},
{params.hour},{params.minute},{params.day},{params.month},{params.year},
{params.weather1[0]},{params.weather2[0]},{params.weather3[0]},{params.weather4[0]},{params.weather5[0]},{params.weather6[0]},{params.weather7[0]},
{params.weather1[3]},{params.weather2[3]},{params.weather3[3]},{params.weather4[3]},{params.weather5[3]},{params.weather6[3]},{params.weather7[3]},
{params.weather1[1]},{params.weather1[2]},{params.weather2[1]},{params.weather2[2]},{params.weather3[1]},{params.weather3[2]},{params.weather4[1]},{params.weather4[2]},
{params.weather5[1]},{params.weather5[2]},{params.weather6[1]},{params.weather6[2]},{params.weather7[1]},{params.weather7[2]},
{params.alarm1_set},{params.display_message1},{params.notification_led1},{params.buzzer1},{params.hour1_entry},{params.minute1_entry},{params.am_pm1_entry},
{params.date1_entry},{params.month1_entry},{params.year1_entry},{params.set_auto},
{params.alarm2_set},{params.display_message2},{params.notification_led2},{params.buzzer2},{params.hour2_entry},{params.minute2_entry},{params.am_pm2_entry},
{params.date2_entry},{params.month2_entry},{params.year2_entry},
{params.value_usb_optic},{params.value12_24},{params.am_pm}
"""

    # Strip any unnecessary whitespace and newlines
    data_to_send = data_to_send.replace("\n", "").replace(" ", "")

    # Append a termination character (newline)
    data_to_send += "\n"  # Add a newline character to signal end of message

    # Send the string data over the serial connection
    ser.write(data_to_send.encode('utf-8'))
    print(len(data_to_send.encode('utf-8')))

    print("Data sent successfully as string")
 #   time_and_date.params.reset()
    

def update_time():
    current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    time_label.config(text=f"Current Time: {current_time}")
    root.after(1000, update_time)


root = tk.Tk()
root.title("PC Software")
root.geometry("950x800")
root.configure(bg='darkgrey')
main_frame = tk.Frame(root) #main control panel frame
main_frame.grid(row=0, column=1, padx=5, pady=5)

left_frame = tk.Frame(root)
left_frame.grid(row=0, column=0, padx=5, pady=(5, 0), sticky='n') 

optical_frame =tk.Frame(root)
optical_frame.place(x=150, rely=0.5, anchor='n')  # Place it at 50% of the available space

left_frame.grid_propagate(False)
optical_frame.grid_propagate(False)

root.grid_rowconfigure(0, weight=1)
root.grid_rowconfigure(1, weight=0)  # Set to 0 to prevent expansion
# Create time label
time_label = tk.Label(main_frame, text="", font=("Helvetica", 14))
time_label.pack(pady=2)

# ALARMS GUI
top_frame = tk.Frame(main_frame)
top_frame.pack(pady=10)

alarm_label = tk.Label(top_frame, text="Alarms:", width=10)
alarm_label.grid(row=0, column=0, padx=5, pady=5)
# Variables to hold the state of each alarm checkbox
alarm1_var = tk.IntVar(value=0)
alarm2_var = tk.IntVar(value=0)

# Variables to hold the state of each alarm's sub-checkboxes
alarm1_display_var = tk.IntVar(value=0)
alarm1_led_var = tk.IntVar(value=0)
alarm1_buzzer_var = tk.IntVar(value=0)

alarm2_display_var = tk.IntVar(value=0)
alarm2_led_var = tk.IntVar(value=0)
alarm2_buzzer_var = tk.IntVar(value=0)

# Frame to hold alarm checkboxes
alarm_frame = tk.Frame(top_frame)
alarm_frame.grid(row=1, column=0, padx=5)

# Function to enable or disable alarm sub-checkboxes and text entry based on the alarm's main checkbox state
def toggle_alarm_options(alarm_var, display_cb, led_cb, buzzer_cb, message_entry, display_var):
    state = tk.NORMAL if alarm_var.get() == 1 else tk.DISABLED
    display_cb.config(state=state)
    led_cb.config(state=state)
    buzzer_cb.config(state=state)
    toggle_display_message(display_var, message_entry)

# Function to enable/disable the text box for display message
def toggle_display_message(display_var, message_entry):
    if display_var.get() == 1:
        message_entry.config(state=tk.NORMAL)
    else:
        message_entry.delete(0, tk.END)
        message_entry.config(state=tk.DISABLED)

# Function to create alarm checkboxes and text box
def create_alarm_checkboxes(frame, alarm_label, alarm_var, display_var, led_var, buzzer_var, row_index):
    alarm_cb = tk.Checkbutton(frame, text=alarm_label, variable=alarm_var, command=lambda: toggle_alarm_options(alarm_var, display_cb, led_cb, buzzer_cb, message_entry, display_var))
    alarm_cb.grid(row=row_index, column=0, padx=5, pady=5)
    
    display_cb = tk.Checkbutton(frame, text="Display Message", variable=display_var, state=tk.DISABLED, command=lambda: toggle_display_message(display_var, message_entry))
    display_cb.grid(row=row_index, column=1, padx=5, pady=5)

    led_cb = tk.Checkbutton(frame, text="Notification LED", variable=led_var, state=tk.DISABLED)
    led_cb.grid(row=row_index, column=2, padx=5, pady=5)

    buzzer_cb = tk.Checkbutton(frame, text="Buzzer", variable=buzzer_var, state=tk.DISABLED)
    buzzer_cb.grid(row=row_index, column=3, padx=5, pady=5)

    # Text box for Display Message (disabled by default)
    message_entry = tk.Entry(frame, state=tk.DISABLED, width=30)
    message_entry.grid(row=row_index, column=4, padx=5, pady=5)
    
    return alarm_cb, display_cb, led_cb, buzzer_cb, message_entry

# Add checkboxes for Alarm 1
_, _, _, _, alarm1_message_entry = create_alarm_checkboxes(alarm_frame, "Alarm 1", alarm1_var, alarm1_display_var, alarm1_led_var, alarm1_buzzer_var, row_index=1)
frame = tk.Frame(alarm_frame)
frame.grid(row=2, column=0)
# add time entry boxes for alarm 1
alarm1_entries = time_and_date.set_manual_time_frame(frame, False)

# Add checkboxes for Alarm 2
_, _, _, _, alarm2_message_entry = create_alarm_checkboxes(alarm_frame, "Alarm 2", alarm2_var, alarm2_display_var, alarm2_led_var, alarm2_buzzer_var, row_index=3)
frame2 = tk.Frame(alarm_frame)
frame2.grid(row=4, column=0)
# add time entry boxes for alarm 2
alarm2_entries = time_and_date.set_manual_time_frame(frame2, False)

# Function to get selected options for each alarm
#need to get entry box values
def get_alarm_parameters(alarm1_entries, alarm2_entries):

    #if alarm 1 has been selected
    if alarm1_var.get() == 1:
        time_and_date.params.alarm1_set = 1
        time_and_date.params.message1 = alarm1_display_var.get()
        time_and_date.params.notification_led1 = alarm1_led_var.get()
        time_and_date.params.buzzer1 = alarm1_buzzer_var.get()
        time_and_date.params.display_message1 = alarm1_display_var.get()
        #if display message has been selected
        if alarm1_display_var.get() == 1:
            time_and_date.params.message1 = alarm1_message_entry.get()[:256]
    
    #if alarm 2 has been selected
    if alarm2_var.get() == 1:
        time_and_date.params.alarm2_set = 1
        time_and_date.params.message2 = alarm2_display_var.get()
        time_and_date.params.notification_led2 = alarm2_led_var.get()
        time_and_date.params.buzzer2 = alarm2_buzzer_var.get()
        time_and_date.params.display_message2 = alarm2_display_var.get()
        #if display message has been selected
        if alarm2_display_var.get() == 1:
            time_and_date.params.message2 = alarm2_message_entry.get()[:256]

    #save alarm entry boxes
    time_and_date.params.hour1_entry = alarm1_entries[0].get()
    time_and_date.params.minute1_entry = alarm1_entries[1].get()
    time_and_date.params.am_pm1_entry = alarm1_entries[2].get()
    time_and_date.params.date1_entry = alarm1_entries[3].get()
    time_and_date.params.month1_entry = alarm1_entries[4].get()
    time_and_date.params.year1_entry = alarm1_entries[5].get()

    time_and_date.params.hour2_entry = alarm2_entries[0].get()
    time_and_date.params.minute2_entry = alarm2_entries[1].get()
    time_and_date.params.am_pm2_entry = alarm2_entries[2].get()
    time_and_date.params.date2_entry = alarm2_entries[3].get()
    time_and_date.params.month2_entry = alarm2_entries[4].get()
    time_and_date.params.year2_entry = alarm2_entries[5].get()

    

# Button to submit and pass selected options to another function
submit_button = tk.Button(alarm_frame, text="Submit", command=lambda: get_alarm_parameters(alarm1_entries, 
                                                                                   alarm2_entries))
submit_button.grid(row=4, column=1, columnspan=5, padx=5, pady=5)


#CELSIUS AND FARENHEIT
cf_frame = tk.Frame(main_frame, padx=10, pady=10)
cf_frame.pack()

c_f = tk.StringVar(cf_frame, "C") #linking variable

#celsius radio button
celsius = tk.Radiobutton(cf_frame, text="Celsius", variable=c_f, value="C")
celsius.grid(column=1, row=1)

#farenheit radio button
farenheit = tk.Radiobutton(cf_frame, text="Farenheit", variable=c_f, value="F")
farenheit.grid(column=2, row=1)

def save_degrees():
    #saves the value of the C/F radio button that was selected
    time_and_date.params.celsius_fahrenheit = c_f.get() 

#submit button
submit = tk.Button(cf_frame, text="Submit", command=save_degrees)
submit.grid(column=3, row=2)

weather_button = tk.Button(main_frame, text="Show Weather", command=show_weather, font=("Helvetica", 14))
weather_button.pack(pady=10)

ports_title = tk.Label(main_frame, text="Available Serial Ports:", font=("Helvetica", 14, "bold"))
ports_title.pack(pady=10)

port_combobox = ttk.Combobox(main_frame, width=50)
port_combobox.pack(pady=10)

connect_button = tk.Button(main_frame, text="Connect to Selected Port", command=connect_port, font=("Helvetica", 14))
connect_button.pack(pady=10)


#read parameters from device by sending a flag
def read_parameters():
       
    # Create a formatted string to request parameters
    data_to_send = f""" ! """
    data_to_send = data_to_send.replace("\n", "").replace(" ", "")
    data_to_send += "\n"  # Add a newline character to signal end of message

    # Send the string data over the serial connection
    ser.write(data_to_send.encode('utf-8'))
    print(len(data_to_send.encode('utf-8')))

    print("request sent (!)")
 #   time_and_date.params.reset()
    
     

#write parameters to device (currently prints to terminal instead)
def write_parameters():
    time_and_date.stop_display()
    #12/24 HOUR TIME
    if time_and_date.params.set_auto: #time has been set automatically
        if time_and_date.params.value12_24 == "12":
            # Convert hour to 12-hour format
            if time_and_date.params.hour == 0:
                time_and_date.params.hour = 12  # Midnight case
                time_and_date.params.am_pm = "AM"
            elif time_and_date.params.hour == 12:
                time_and_date.params.hour = 12  # Noon case
                time_and_date.params.am_pm = "PM"
            elif time_and_date.params.hour > 12:
                time_and_date.params.hour = time_and_date.params.hour - 12
                time_and_date.params.am_pm = "PM"
            else:
                time_and_date.params.hour = time_and_date.params.hour
                time_and_date.params.am_pm = "AM"

    #send parameters via serial connection
    print("Printing parameters to terminal...")
    print(time_and_date.params.to_dict())
    print(time_and_date.params.to_arrays())
    # Trigger the optical link display if 'Optic Link' is selected
    if time_and_date.params.value_usb_optic == "optic link":
        time_and_date.transmit_time_date(optical_frame)

    #send parameters via USB
    send_data(time_and_date.params)

received_data_display = scrolledtext.ScrolledText(main_frame, wrap=tk.WORD, height=10, width=80)
received_data_display.pack(pady=10)

#time and date GUI
time_date_frame = tk.Frame(left_frame)
time_date_frame.pack()
time_and_date.configure_time_date(time_date_frame)

#configures read and write parameters button
def read_write_buttons(frame):
	
    read_write_frame = tk.Frame(frame, padx=10, pady=0)	
    read_write_frame.pack()
		
    #read button
	
    read_parameters_button = tk.Button(read_write_frame, text="Download Weather",
                                       command=download_weather)
	
    read_parameters_button.grid(column=1, row=1)
	
    #write button
	
    write_parameters_button = tk.Button(read_write_frame, text="Write parameters",	
                                        command=lambda: write_parameters())
    
    write_parameters_button.grid(column=2, row=1)	

    actual_read_parameters_button = tk.Button(read_write_frame, text="Read parameters",
                                              command=lambda: read_parameters())
    
    actual_read_parameters_button.grid(column=3, row=1)	

	
read_write_buttons(left_frame)

refresh_ports()  # Initial call to populate ports
update_time()    # Start updating the time
#transmit_time_date();
root.mainloop()
