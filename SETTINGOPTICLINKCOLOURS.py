import tkinter as tk
from tkinter import ttk  # for themed widgets
from datetime import datetime  # for set time automatically

# Global variables
hour = 0
minute = 0
am_pm = 0
day = 0
month = 0
year = 0
value_usb_optic = 0
value12_24 = 0
set_auto = 1  # 1 if time is set automatically, 0 if time is set manually
binary_data = ""  # store binary data

def int_to_bin(value, bits): 
    return format(value, f'0{bits}b')

def display_binary_colours(frame, colour, row=0): 
    color_label = tk.Label(frame, bg=colour, width=20, height=10)
    color_label.grid(row=row, column=0, padx=10, pady=10)

def show_optical_link_colours(binary_data):
    """Display binary data as colors in the GUI."""
    frame = tk.Frame(root)
    frame.pack(pady=20)

    colour_label = tk.Label(frame, bg='white', width=20, height=10)
    colour_label.grid(row=0, column=0, padx=10, pady=10)

    # Adding the red start signal to the data
    binary_values = ['start'] + list(binary_data) + ['stop']   

    def display_next_color(index):
        if index < len(binary_values):
            if binary_values[index] == 'start' or binary_values[index] == 'stop':
                colour = 'red'
            else:
                colour = 'white' if binary_values[index] == '1' else 'black'

            colour_label.config(bg=colour)
            # Schedule the next color change after 1 second (1000 ms)
            frame.after(1000, display_next_color, index + 1)

    # Start displaying the first color (start signal)
    display_next_color(0)

def transmit_time_date():
    """Convert time and date to binary and display/send over optical link."""
    print("Transmit Time and Date called")  # Debug line
    global hour, minute, day, month, year

    # Convert the time and date to binary strings
    binary_hour = int_to_bin(hour, 5)     # 5 bits for hour (0-23)
    binary_minute = int_to_bin(minute, 6) # 6 bits for minute (0-59)
    binary_day = int_to_bin(day, 5)       # 5 bits for day (1-31)
    binary_month = int_to_bin(month, 4)   # 4 bits for month (1-12)
    binary_year = int_to_bin(year - 2000, 7) # 7 bits for year (offset from 2000)
    
    # Combine binary data into a single string
    binary_data = binary_hour + binary_minute + binary_day + binary_month + binary_year

    print(f"Binary Data: {binary_data}")  # Debug line

    # Now show the binary data as colors in the GUI
    show_optical_link_colours(binary_data)

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
        global value_usb_optic
        global value12_24
        value_usb_optic = usb_optlink.get()
        value12_24 = twelve_24.get()
        # Trigger the optical link display if 'Optic Link' is selected
        if value_usb_optic == "optic link":
            transmit_time_date()

    submit_preferences = tk.Button(settings_frame, text="Submit preferences", command=save_values)
    submit_preferences.grid(column=3, row=2)

def automatic_time_frame(root):
    automatic_time_frame = tk.Frame(root, padx=10, pady=10)
    automatic_time_frame.pack()

    def get_current_time():
        global set_auto
        set_auto = 1
        now = datetime.now()
        global hour, minute, second, year, month, day
        hour = now.hour
        minute = now.minute
        second = now.second
        year = now.year
        month = now.month
        day = now.day
        if value_usb_optic == "optic link":
            transmit_time_date()

    automatic_time_button = tk.Button(automatic_time_frame, text="Set time automatically", command=get_current_time)
    automatic_time_button.pack()

def save_manual_time(hour_entry, minute_entry, am_pm_entry, date_entry, month_entry, year_entry):
    global set_auto
    set_auto = 0
    global hour, minute, am_pm, day, month, year
    hour = int(hour_entry.get())
    minute = int(minute_entry.get())
    am_pm = am_pm_entry.get()
    day = int(date_entry.get())
    month = int(month_entry.get())
    year = int(year_entry.get())
    if value_usb_optic == "optic link":
        transmit_time_date()

def set_manual_time_frame(root):
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

    set_time_manual_button = tk.Button(set_time_frame, text="Set time manually",
                                       command=lambda: save_manual_time(hour_entry, minute_entry, am_pm_entry, date_entry, month_entry, year_entry))
    set_time_manual_button.grid(column=6, row=2)

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
    set_manual_time_frame(root)
    configure_divider(root)

# Create the main window and configure it
root = tk.Tk()

# Button to show optical link colors
#optical_link_button = tk.Button(root, text="Show Optical Link", command=transmit_time_date, font=("Helvetica", 14))
#optical_link_button.pack(pady=10)

root.title("Time and Date Configuration")
root.geometry("600x500")  # Adjust as needed
configure_time_date(root)
root.mainloop()
