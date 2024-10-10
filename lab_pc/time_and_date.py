
import tkinter as tk
from tkinter import ttk  # for themed widgets
from datetime import datetime # for set time automatically

MESSAGE_SIZE = 256
WEATHER_DATA_SIZE = 10
FORECAST_SIZE = 7  

class Parameters:
    def __init__(self):
        # Alarm 1 settings
        self.alarm1_set = 0
        self.display_message1 = 0
        self.notification_led1 = 0
        self.buzzer1 = 0
        self.message1 = "" * MESSAGE_SIZE

        self.hour1_entry = 0
        self.minute1_entry = 0
        self.am_pm1_entry = 0 #0 if am, 1 if pm
        self.date1_entry = 0
        self.month1_entry = 0
        self.year1_entry = 0
        
        # Alarm 2 settings
        self.alarm2_set = 0
        self.display_message2 = 0
        self.notification_led2 = 0
        self.buzzer2 = 0
        self.message2 = "" * MESSAGE_SIZE

        self.hour2_entry = 0
        self.minute2_entry = 0
        self.am_pm2_entry = 0 #0 if am, 1 if pm
        self.date2_entry = 0
        self.month2_entry = 0
        self.year2_entry = 0
        
        # Time settings
        # -1 means the time has not been set/reset
        # on microcontroller side: only change time variables if the time variables != -1
        self.hour = -1
        self.minute = -1
        self.am_pm = 0 #0 if am, 1 if pm
        self.day = -1
        self.month = -1
        self.year = -1
        
        # Other settings
        self.value_usb_optic = 0 #0 if usb, 1 if optic
        self.value12_24 = 0 #0 if 12hr, 1 if 24hr
        self.set_auto = 1
        
        # Celsius or Fahrenheit
        self.celsius_fahrenheit = "C"  # C for Celsius, F for Fahrenheit
        
        # Weather data for 7 days: [date, temperature, humidity, forecast]
        self.weather1 = ["", 0, 0, ""]
        self.weather2 = ["", 0, 0, ""]
        self.weather3 = ["", 0, 0, ""]
        self.weather4 = ["", 0, 0, ""]
        self.weather5 = ["", 0, 0, ""]
        self.weather6 = ["", 0, 0, ""]
        self.weather7 = ["", 0, 0, ""]

    def reset(self):
        self.alarm1_set = 0
        self.display_message1 = 0
        self.notification_led1 = 0
        self.buzzer1 = 0
        self.message1 = "_" * MESSAGE_SIZE

        self.hour1_entry = 0
        self.minute1_entry = 0
        self.am_pm1_entry = 0 #0 if am, 1 if pm
        self.date1_entry = 0
        self.month1_entry = 0
        self.year1_entry = 0
        
        # Alarm 2 settings
        self.alarm2_set = 0
        self.display_message2 = 0
        self.notification_led2 = 0
        self.buzzer2 = 0
        self.message2 = "_" * MESSAGE_SIZE

        self.hour2_entry = 0
        self.minute2_entry = 0
        self.am_pm2_entry = 0 #0 if am, 1 if pm
        self.date2_entry = 0
        self.month2_entry = 0
        self.year2_entry = 0
        
        # Time settings
        # -1 means the time has not been set/reset
        # on microcontroller side: only change time variables if the time variables != -1
        self.hour = -1
        self.minute = -1
        self.am_pm = 0 #0 if am, 1 if pm
        self.day = -1
        self.month = -1
        self.year = -1
        
        # Other settings
        self.value_usb_optic = 0 #0 if usb, 1 if optic
        self.value12_24 = 0 #0 if 12hr, 1 if 24hr
        self.set_auto = 1
        
        # Celsius or Fahrenheit
        self.celsius_fahrenheit = "C"  # C for Celsius, F for Fahrenheit
        
        # Weather data for 7 days: [date, temperature, humidity, forecast]
        self.weather1 = ["", 0, 0, ""]
        self.weather2 = ["", 0, 0, ""]
        self.weather3 = ["", 0, 0, ""]
        self.weather4 = ["", 0, 0, ""]
        self.weather5 = ["", 0, 0, ""]
        self.weather6 = ["", 0, 0, ""]
        self.weather7 = ["", 0, 0, ""]

    def seperate_weather_arrays(self):
        #seperate out the array values
        self.weather1_date = self.weather1[0]
        self.weather1_temperature = self.weather1[1]
        self.weather1_humidity = self.weather1[2]
        self.weather1_forecast = self.weather1[3]

        self.weather2_date = self.weather2[0]
        self.weather2_temperature = self.weather2[1]
        self.weather2_humidity = self.weather2[2]
        self.weather2_forecast = self.weather2[3]

        self.weather3_date = self.weather3[0]
        self.weather3_temperature = self.weather3[1]
        self.weather3_humidity = self.weather3[2]
        self.weather3_forecast = self.weather3[3]

        self.weather4_date = self.weather4[0]
        self.weather4_temperature = self.weather4[1]
        self.weather4_humidity = self.weather4[2]
        self.weather4_forecast = self.weather4[3]

        self.weather5_date = self.weather5[0]
        self.weather5_temperature = self.weather5[1]
        self.weather5_humidity = self.weather5[2]
        self.weather5_forecast = self.weather5[3]

        self.weather6_date = self.weather6[0]
        self.weather6_temperature = self.weather6[1]
        self.weather6_humidity = self.weather6[2]
        self.weather6_forecast = self.weather6[3]

        self.weather7_date = self.weather7[0]
        self.weather7_temperature = self.weather7[1]
        self.weather7_humidity = self.weather7[2]
        self.weather7_forecast = self.weather7[3]

    # Returns the parameters as an array (for transmitting to microcontroller)
    def to_arrays(self):
        self.seperate_weather_arrays()
        
        string_parameters = [
            self.message1,
            self.message2,
            self.celsius_fahrenheit,
            self.weather1_date,
            self.weather2_date,
            self.weather3_date,
            self.weather4_date,
            self.weather5_date,
            self.weather6_date,
            self.weather7_date,
            self.weather1_forecast,
            self.weather2_forecast,
            self.weather3_forecast,
            self.weather4_forecast,
            self.weather5_forecast,
            self.weather6_forecast,
            self.weather7_forecast
        ]

        float_parameters = [
            self.weather1_temperature,
            self.weather1_humidity,
            self.weather2_temperature,
            self.weather2_humidity,
            self.weather3_temperature,
            self.weather3_humidity,
            self.weather4_temperature,
            self.weather4_humidity,
            self.weather5_temperature,
            self.weather5_humidity,
            self.weather6_temperature,
            self.weather6_humidity,
            self.weather7_temperature,
            self.weather7_humidity,
        ]

        integer_parameters = [
            self.alarm1_set,
            self.display_message1,
            self.notification_led1,
            self.buzzer1,
            self.hour1_entry,
            self.minute1_entry,
            self.am_pm1_entry,
            self.date1_entry,
            self.month1_entry,
            self.year1_entry,
            self.set_auto,
            self.alarm2_set,
            self.display_message2,
            self.notification_led2,
            self.buzzer2,
            self.hour2_entry,
            self.minute2_entry,
            self.am_pm2_entry,
            self.date2_entry,
            self.month2_entry,
            self.year2_entry,
            self.value_usb_optic,
            self.value12_24,
            self.am_pm,
            self.hour,
            self.minute,
            self.day,
            self.month,
            self.year,
        ]


        if (self.value_usb_optic == "usb"):
            parameters = [string_parameters, float_parameters, integer_parameters]
        elif (self.value_usb_optic == "optic link"):
            parameters = [string_parameters, float_parameters, integer_parameters[:-5]] #does not send the date parameters

        return parameters

    # Returns the parameters as a dictionary (for printing to terminal)
    def to_dict(self):
        self.seperate_weather_arrays()
        #don't send time parameters if setting via optic link
        if (self.value_usb_optic == "usb"):
            dictionary = {
                "alarm1_set": self.alarm1_set,
                "display_message1": self.display_message1,
                "notification_led1": self.notification_led1,
                "buzzer1": self.buzzer1,
                "message1": self.message1,
                "hour1_entry": self.hour1_entry,
                "minute1_entry": self.minute1_entry,
                "am_pm1_entry": self.am_pm1_entry,
                "date1_entry": self.date1_entry,
                "month1_entry": self.month1_entry,
                "year1_entry": self.year1_entry,
                "alarm2_set": self.alarm2_set,
                "display_message2": self.display_message2,
                "notification_led2": self.notification_led2,
                "buzzer2": self.buzzer2,
                "message2": self.message2,
                "hour2_entry": self.hour2_entry,
                "minute2_entry": self.minute2_entry,
                "am_pm2_entry": self.am_pm2_entry,
                "date2_entry": self.date2_entry,
                "month2_entry": self.month2_entry,
                "year2_entry": self.year2_entry,
                "celsius_fahrenheit": self.celsius_fahrenheit,
                "weather1": self.weather1,
                "weather2": self.weather2,
                "weather3": self.weather3,
                "weather4": self.weather4,
                "weather5": self.weather5,
                "weather6": self.weather6,
                "weather7": self.weather7,
                "hour": self.hour,
                "minute": self.minute,
                "am_pm": self.am_pm,
                "day": self.day,
                "month": self.month,
                "year": self.year,
            }
        elif (self.value_usb_optic == "optic link"):
            dictionary = {
                "alarm1_set": self.alarm1_set,
                "display_message1": self.display_message1,
                "notification_led1": self.notification_led1,
                "buzzer1": self.buzzer1,
                "message1": self.message1,
                "hour1_entry": self.hour1_entry,
                "minute1_entry": self.minute1_entry,
                "am_pm1_entry": self.am_pm1_entry,
                "date1_entry": self.date1_entry,
                "month1_entry": self.month1_entry,
                "year1_entry": self.year1_entry,
                "alarm2_set": self.alarm2_set,
                "display_message2": self.display_message2,
                "notification_led2": self.notification_led2,
                "buzzer2": self.buzzer2,
                "message2": self.message2,
                "hour2_entry": self.hour1_entry,
                "minute2_entry": self.minute1_entry,
                "am_pm2_entry": self.am_pm1_entry,
                "date2_entry": self.date1_entry,
                "month2_entry": self.month1_entry,
                "year2_entry": self.year1_entry,
                "celsius_fahrenheit": self.celsius_fahrenheit,
                "weather1": self.weather1,
                "weather2": self.weather2,
                "weather3": self.weather3,
                "weather4": self.weather4,
                "weather5": self.weather5,
                "weather6": self.weather6,
                "weather7": self.weather7,
            }

        else:
            dictionary = {}

        return dictionary

params = Parameters()




def int_to_bin(value, bits): 
    return format(value, f'0{bits}b')

#  global flag to control the infinite loop
continue_display = True

def stop_display():
    """Set the flag to stop the display loop."""
    global continue_display
    continue_display = False

def show_optical_link_colours(binary_data, parent_frame):
    global continue_display
    continue_display = True  # Allow the loop to start

     # Clear the previous frame's contents by destroying all child widgets
    for widget in parent_frame.winfo_children():
        widget.destroy()

    # Add a new frame for optical link colors in the same window/frame
    frame = tk.Frame(parent_frame)
    frame.pack(pady=0)

    # Label to show colors
    colour_label = tk.Label(frame, bg='white', width=30, height=20)
    colour_label.grid(row=0, column=0, padx=10, pady=10)

    # Adding the red start signal to the data
    binary_values = ['start'] + list(binary_data) + ['stop']

    def display_next_color(index):
        if continue_display:  # Check if the loop should continue
            if index < len(binary_values):
                if binary_values[index] == 'start' or binary_values[index] == 'stop':
                    colour = 'red'
                else:
                    colour = 'white' if binary_values[index] == '1' else 'black'

                colour_label.config(bg=colour)
                # Schedule the next color change after 1 sec
                frame.after(300, display_next_color, index + 1)
            else:
                # Loop back to the start of the binary data
                display_next_color(0)

    # Start displaying the first color (start signal)
    display_next_color(0)



def transmit_time_date(main_frame):
    """Convert time and date to binary and display/send over optical link."""
    print("Transmit Time and Date called")  # Debug line

    # Convert the time and date to binary strings
    binary_hour = int_to_bin(params.hour, 5)     # 5 bits for hour (0-23)
    binary_minute = int_to_bin(params.minute, 6) # 6 bits for minute (0-59)
    binary_day = int_to_bin(params.day, 5)       # 5 bits for day (1-31)
    binary_month = int_to_bin(params.month, 4)   # 4 bits for month (1-12)
    binary_year = int_to_bin(params.year, 12) # 7 bits for year (offset from 2000)
    
    # Combine binary data into a single string
    binary_data = binary_hour + binary_minute + binary_day + binary_month + binary_year

    print(f"Binary Data: {binary_data}")  # Debug line

    # Now show the binary data as colors in the GUI
    show_optical_link_colours(binary_data, main_frame)

def configure_settings_frame(root):
    global settings_frame
    settings_frame = tk.Frame(root, padx=10, pady=10)
    settings_frame.pack()

    usb_optlink = tk.StringVar(settings_frame, "none")  # linking variable
    usb = tk.Radiobutton(settings_frame, text="USB", variable=usb_optlink, value="usb")
    usb.grid(column=1, row=1)

    optic_link = tk.Radiobutton(settings_frame, text="Optic Link", variable=usb_optlink, value="optic link")
    optic_link.grid(column=2, row=1)

    twelve_24 = tk.StringVar(settings_frame, value="12")  # linking variable
    twelve = tk.Radiobutton(settings_frame, text="12hr", variable=twelve_24, value="12") 
    twelve.grid(column=1, row=2)

    twenty4 = tk.Radiobutton(settings_frame, text="24hr", variable=twelve_24, value="24")
    twenty4.grid(column=2, row=2)

    def save_values():
        
        params.value_usb_optic = usb_optlink.get()
        params.value12_24 = twelve_24.get()

    submit_preferences = tk.Button(settings_frame, text="Submit preferences", command=save_values)
    submit_preferences.grid(column=3, row=2)

def automatic_time_frame(root):
    automatic_time_frame = tk.Frame(root, padx=10, pady=10)
    automatic_time_frame.pack()

    def get_current_time():
        params.set_auto = 1
        now = datetime.now()
        params.hour = now.hour
        params.minute = now.minute
        params.second = now.second
        params.year = now.year
        params.month = now.month
        params.day = now.day
        

    automatic_time_button = tk.Button(automatic_time_frame, text="Set time automatically", command=get_current_time)
    automatic_time_button.pack()

def save_manual_time(hour_entry, minute_entry, am_pm_entry, date_entry, month_entry, year_entry):
    params.set_auto = 0
    params.hour = int(hour_entry.get())
    params.minute = int(minute_entry.get())
    params.am_pm = am_pm_entry.get()
    params.day = int(date_entry.get())
    params.month = int(month_entry.get())
    params.year = int(year_entry.get())

def set_manual_time_frame(root, add_button):
    set_time_frame = tk.Frame(root, padx=10, pady=10)
    set_time_frame.pack()

    hour = tk.StringVar(set_time_frame)
    hour_entry = tk.Entry(set_time_frame, width=2, textvariable=hour)
    hour_entry.insert(0, 'hh')
    hour_entry.grid(column=1, row=1)

    time_seperator = tk.Label(set_time_frame, text=":")
    time_seperator.grid(column=2, row=1)

    minute = tk.StringVar(set_time_frame)
    minute_entry = tk.Entry(set_time_frame, width=2, textvariable=minute)
    minute_entry.insert(0, 'mm')
    minute_entry.grid(column=3, row=1)

    am_pm = tk.StringVar(set_time_frame)
    am_pm_entry = tk.Entry(set_time_frame, width=5, textvariable=am_pm)
    am_pm_entry.insert(0, 'am/pm')
    am_pm_entry.grid(column=4, row=1)

    date = tk.StringVar(set_time_frame)
    date_entry = tk.Entry(set_time_frame, width=2, textvariable=date)
    date_entry.insert(0, 'dd')
    date_entry.grid(column=1, row=2)

    date1_seperator = tk.Label(set_time_frame, text="/")
    date1_seperator.grid(column=2, row=2)

    month = tk.StringVar(set_time_frame)
    month_entry = tk.Entry(set_time_frame, width=2, textvariable=month)
    month_entry.insert(0, 'mm')
    month_entry.grid(column=3, row=2)

    date2_seperator = tk.Label(set_time_frame, text="/")
    date2_seperator.grid(column=4, row=2)

    year = tk.StringVar(set_time_frame)
    year_entry = tk.Entry(set_time_frame, width=4, textvariable=year)
    year_entry.insert(0, 'yyyy')
    year_entry.grid(column=5, row=2)

    if add_button:
        set_time_manual_button = tk.Button(set_time_frame, text="Set time manually",
                                        command=lambda: save_manual_time(hour_entry, minute_entry, am_pm_entry, date_entry, month_entry, year_entry))
        set_time_manual_button.grid(column=6, row=2)

    return [hour, minute, am_pm, date, month, year]

def configure_divider(root):
    preferences_divider = tk.Label(root, text="------------------------------------------------------------")
    preferences_divider.pack()

def configure_time_date(root):
    title = ttk.Label(root, text="\nSET TIME AND DATE")
    title.config(font=("Helvetica", 12, "bold"))
    title.pack()

    configure_settings_frame(root)
    configure_divider(root)
    automatic_time_frame(root)
    configure_divider(root)
    set_manual_time_frame(root, True)
    configure_divider(root)

# Create the main window and configure it
# root = tk.Tk()
# root.title("Time and Date Configuration")
# root.geometry("600x500")  # Adjust as needed
# configure_time_date(root)
# root.mainloop()