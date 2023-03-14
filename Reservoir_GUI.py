import tkinter as tk
from tkinter import ttk
import serial
import time
import tkinter.font as font

ser = serial.Serial(port='COM3', baudrate=115200, timeout=.1)
#time.sleep(4)
'''
p = btle
p = btle.Peripheral("de:fc:54:87:b0:04")
services=p.getServices()
s = p.getServiceByUUID(list(services)[2].uuid)
serlock = s.getCharacteristics()[0]
##c.write(bytes("0001".encode())
'''
class bfitApp(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        container = tk.Frame(self)
        container.pack(side = "top", fill = "both", expand = True)

        container.grid_rowconfigure(0, weight = 1)
        container.grid_columnconfigure(0, weight = 1)

        self.frames = {}

        for F in (Startpage, Cooling, Heating, Compression, Settings):

            frame = F(container, self)

            self.frames[F] = frame

            frame.grid(row = 0, column = 0, sticky = "nsew")

        self.show_frame(Startpage)

    def show_frame(self, cont):
        frame = self.frames[cont]
        frame.tkraise()

#homepage window

class Startpage(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        
        self.grid_columnconfigure(1,weight=1)
        self.grid_columnconfigure(2,weight=1)
        self.grid_rowconfigure(1,weight=1)
        self.grid_rowconfigure(2,weight=1)
        fontsize = font.Font(size=20)

        button_cool = tk.Button(self, text=f"Cooling", height = 10, width = 30, bg="#64bcff", command = lambda : controller.show_frame(Cooling))
        button_cool['font'] = fontsize
        button_cool.grid(row=1, column=1, padx=5, pady=5, sticky="NSEW")

        button_heat = tk.Button(self, text=f"Heating", height = 10, width = 30, bg="#ff9090", command = lambda : controller.show_frame(Heating))
        button_heat['font'] = fontsize
        button_heat.grid(row=1, column=2, padx=5, pady=5, sticky="NSEW")

        button_compression = tk.Button(self, text=f"Compression", height = 10, width = 30, bg="#fdfdfd", command = lambda : controller.show_frame(Compression))
        button_compression['font'] = fontsize
        button_compression.grid(row=2, column=1, padx=5, pady=5, sticky="NSEW")

        button_settings = tk.Button(self, text=f"Lock", height = 10, width = 30, command = lambda : controller.show_frame(Settings))
        button_settings['font'] = fontsize
        button_settings.grid(row=2, column =2, padx=5, pady=5, sticky="NSEW")
     
class Cooling(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)

        self.grid_columnconfigure(1,weight=1)
        self.grid_columnconfigure(2,weight=1)
        self.grid_columnconfigure(3,weight=1)
        self.grid_rowconfigure(1,weight=1)
        self.grid_rowconfigure(2,weight=1)
        self.grid_rowconfigure(3,weight=1)
        self.grid_rowconfigure(4,weight=1)
        fontsize = font.Font(size=20)

        def decrease():
            value = int(label["text"])
            label["text"] = f"{value - 5}"
    
        def increase():
            value = int(label["text"])
            label["text"] = f"{value + 5}"

        def serial_start_cool():
            ser.write("<6969>".encode())

        def serial_active_cool():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<6868:" + value2 + ">"
            ser.write(coder.encode())

        def serial_pause_cool():
            ser.write("<7070>".encode())

        def serial_stop_cool():
            ser.write("<7171>".encode())

        button_home = tk.Button(self, text=f"Home", command = lambda : controller.show_frame(Startpage))
        button_home['font'] = fontsize
        button_home.grid(row=1, column=3, padx=5, pady=5, sticky="NSEW")

        button_start_cool = tk.Button(self, text=f"Start Cooling", command=serial_start_cool, height = 11, width = 20, bg="#64bcff")
        button_start_cool['font'] = fontsize
        button_start_cool.grid(rowspan=2, column=1, padx=5, pady=5, sticky="NSEW")

        button_start_pump = tk.Button(self, text=f"Start Pump", command=serial_active_cool, height = 5, width = 20, bg="#60ce80")
        button_start_pump['font'] = fontsize
        button_start_pump.grid(row=2, column=2, padx=5, pady=5, sticky="NSEW")

        button_pause = tk.Button(self, text=f"Pause Pump", command=serial_pause_cool, height = 5, width = 20, bg="#fff152")
        button_pause['font'] = fontsize
        button_pause.grid(row=3, column=2, padx=5, pady=5, sticky="NSEW")

        button_stop = tk.Button(self, text=f"Stop All", command=serial_stop_cool, height = 5, width = 20, bg="#ff6162")
        button_stop['font'] = fontsize
        button_stop.grid(row=4, column=2, padx=5, pady=5, sticky="NSEW")

        button_up = tk.Button(self, text=f"+", height=5, width=20, command=increase, bg="#f7efc8")
        button_up['font'] = fontsize
        button_up.grid(row=2, column=3, padx=5, pady=5, sticky="NSEW")

        button_down = tk.Button(self, text=f"-", height=5, width=20, command=decrease, bg="#f7efc8")
        button_down['font'] = fontsize
        button_down.grid(row=4, column=3, padx=5, pady=5, sticky="NSEW")

        label = tk.Label(self, text="0")
        label.grid(row=3, column=3, padx=5, pady=5, sticky="NSEW")

        cooling_time = tk.Label(self, text="0")
        cooling_time.grid(row=4, column=1, padx=5, pady=5, sticky="NSEW")


class Heating(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)

        self.grid_columnconfigure(1,weight=1)
        self.grid_columnconfigure(2,weight=1)
        self.grid_columnconfigure(3,weight=1)
        self.grid_rowconfigure(1,weight=1)
        self.grid_rowconfigure(2,weight=1)
        self.grid_rowconfigure(3,weight=1)
        self.grid_rowconfigure(4,weight=1)
        fontsize = font.Font(size=20)

        def decrease():
            value = int(label["text"])
            label["text"] = f"{value - 5}"
    
        def increase():
            value = int(label["text"])
            label["text"] = f"{value + 5}"

        def high_heating():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<2323:" + value2 + ">"
            ser.write(coder.encode())

        def medium_heating():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<2424:" + value2 + ">"
            ser.write(coder.encode())
        
        def low_heating():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<2525:" + value2 + ">"
            ser.write(coder.encode())

        def heat_start():
            ser.write("<7272>".encode())

        def heat_pause():
            ser.write("<7373>".encode())

        def heat_stop():
            ser.write("<7474>".encode())


        button_home = tk.Button(self, text=f"Home", command = lambda : controller.show_frame(Startpage))
        button_home['font'] = fontsize
        button_home.grid(row=1, column=3, padx=5, pady=5, sticky="NSEW")

        button_high = tk.Button(self, text=f"High", command=high_heating, height=5, width=20, bg="#ff0000")
        button_high['font'] = fontsize
        button_high.grid(row=2, column=1, padx=5, pady=5, sticky="NSEW")

        button_medium = tk.Button(self, text=f"Medium", command=medium_heating, height=5, width=20, bg="#ff5252")
        button_medium['font'] = fontsize
        button_medium.grid(row=3, column=1, padx=5, pady=5, sticky="NSEW")

        button_low = tk.Button(self, text=f"Low", command=low_heating, height=5, width=20, bg="#ff7b7b")
        button_low['font'] = fontsize
        button_low.grid(row=4, column=1, padx=5, pady=5, sticky="NSEW")

        button_start = tk.Button(self, text=f"Start", command=heat_start, height=5, width=20, bg="#60ce80")
        button_start['font'] = fontsize
        button_start.grid(row=2, column=2, padx=5, pady=5, sticky="NSEW")

        button_pause = tk.Button(self, text=f"Pause", command=heat_pause, height=5, width=20, bg="#fff152")
        button_pause['font'] = fontsize
        button_pause.grid(row=3, column=2, padx=5, pady=5, sticky="NSEW")

        button_stop = tk.Button(self, text=f"Stop", command=heat_stop, height=5, width=20, bg="#ff6162")
        button_stop['font'] = fontsize
        button_stop.grid(row=4, column=2, padx=5, pady=5, sticky="NSEW")

        button_up = tk.Button(self, text=f"+", height=5, width=20, command=increase, bg="#f7efc8")
        button_up['font'] = fontsize
        button_up.grid(row=2, column=3, padx=5, pady=5, sticky="NSEW")

        button_down = tk.Button(self, text=f"-", height=5, width=20, command=decrease, bg="#f7efc8")
        button_down['font'] = fontsize
        button_down.grid(row=4, column=3, padx=5, pady=5, sticky="NSEW")

        label = tk.Label(self, text="0")
        label.grid(row=3, column=3, padx=5, pady=5)



class Compression(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
 
        self.grid_columnconfigure(1,weight=1)
        self.grid_columnconfigure(2,weight=1)
        self.grid_columnconfigure(3,weight=1)
        self.grid_rowconfigure(1,weight=1)
        self.grid_rowconfigure(2,weight=1)
        self.grid_rowconfigure(3,weight=1)
        self.grid_rowconfigure(4,weight=1)
        fontsize = font.Font(size=20)

        def decrease():
            value = int(label["text"])
            label["text"] = f"{value - 5}"
    
        def increase():
            value = int(label["text"])
            label["text"] = f"{value + 5}"

        def high_comp():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<5454:" + value2 + ">"
            ser.write(coder.encode())

        def medium_comp():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<5555:" + value2 + ">"
            ser.write(coder.encode())
        
        def low_comp():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<5656:" + value2 + ">"
            ser.write(coder.encode())

        def comp_start():
            ser.write("<7575>".encode())

        def comp_pause():
            ser.write("<7676>".encode())

        def comp_stop():
            ser.write("<7878>".encode())

        button_home = tk.Button(self, text=f"Home", command = lambda : controller.show_frame(Startpage))
        button_home['font'] = fontsize
        button_home.grid(row=1, column=3, padx=5, pady=5, sticky="NSEW")

        button_high = tk.Button(self, text=f"High", command=high_comp, height=5, width=20, bg="#997e9f")
        button_high['font'] = fontsize
        button_high.grid(row=2, column=1, padx=5, pady=5, sticky="NSEW")

        button_medium = tk.Button(self, text=f"Medium", command=medium_comp, height=5, width=20, bg="#aa8db1")
        button_medium['font'] = fontsize
        button_medium.grid(row=3, column=1, padx=5, pady=5, sticky="NSEW")

        button_low = tk.Button(self, text=f"Low", command=low_comp, height=5, width=20, bg="#bba3c0")
        button_low['font'] = fontsize
        button_low.grid(row=4, column=1, padx=5, pady=5, sticky="NSEW")

        button_start = tk.Button(self, text=f"Start", command=comp_start, height=5, width=20, bg="#60ce80")
        button_start['font'] = fontsize
        button_start.grid(row=2, column=2, padx=5, pady=5, sticky="NSEW")

        button_pause = tk.Button(self, text=f"Pause", command=comp_pause, height=5, width=20, bg="#fff152")
        button_pause['font'] = fontsize
        button_pause.grid(row=3, column=2, padx=5, pady=5, sticky="NSEW")

        button_stop = tk.Button(self, text=f"Stop", command=comp_stop, height=5, width=20, bg="#ff6162")
        button_stop['font'] = fontsize
        button_stop.grid(row=4, column=2, padx=5, pady=5, sticky="NSEW")

        button_up = tk.Button(self, text=f"+", height=5, width=20, command=increase, bg="#f7efc8")
        button_up['font'] = fontsize
        button_up.grid(row=2, column=3, padx=5, pady=5, sticky="NSEW")

        button_down = tk.Button(self, text=f"-", height=5, width=20, command=decrease, bg="#f7efc8")
        button_down['font'] = fontsize
        button_down.grid(row=4, column=3, padx=5, pady=5, sticky="NSEW")

        label = tk.Label(self, text="0")
        label.grid(row=3, column=3, padx=5, pady=5)



class Settings(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)

        self.grid_columnconfigure(1,weight=1)
        self.grid_columnconfigure(2,weight=1)
        self.grid_rowconfigure(1,weight=1)
        self.grid_rowconfigure(2,weight=1)
        self.grid_rowconfigure(3,weight=1)
        self.grid_rowconfigure(4,weight=1)
        fontsize = font.Font(size=20)

        def decrease():
            value = int(label["text"])
            label["text"] = f"{value - 5}"
    
        def increase():
            value = int(label["text"])
            label["text"] = f"{value + 5}"
        '''
        def upper_lock():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<8001:" + value2 + ">"
            serlock.write(coder.encode())

        def lower_lock():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<8002:" + value2 + ">"
            serlock.write(coder.encode())
        
        def full_lock():
            value = int(label["text"])
            value2 = value * 1000
            value2 = str(value2)
            coder =  "<8003:" + value2 + ">"
            serlock.write(coder.encode()) '''
        
        button_home = tk.Button(self, text=f"Home", command = lambda : controller.show_frame(Startpage))
        button_home['font'] = fontsize
        button_home.grid(row=1, column=2, padx=5, pady=5, sticky="NSEW")

        button_upper = tk.Button(self, text=f"Upper Lock", height=5, width=20, bg="#60ce80")
        button_upper['font'] = fontsize
        button_upper.grid(row=2, column=1, padx=5, pady=5, sticky="NSEW")

        button_lower = tk.Button(self, text=f"Lower Lock", height=5, width=20, bg="#fff152")
        button_lower['font'] = fontsize
        button_lower.grid(row=3, column=1, padx=5, pady=5, sticky="NSEW")

        button_lock = tk.Button(self, text=f"Locked", height=5, width=20, bg="#ff6162")
        button_lock['font'] = fontsize
        button_lock.grid(row=4, column=1, padx=5, pady=5, sticky="NSEW")

        button_up = tk.Button(self, text=f"+", height=5, width=20, command=increase, bg="#f7efc8")
        button_up['font'] = fontsize
        button_up.grid(row=2, column=2, padx=5, pady=5, sticky="NSEW")

        button_down = tk.Button(self, text=f"-", height=5, width=20, command=decrease, bg="#f7efc8")
        button_down['font'] = fontsize
        button_down.grid(row=4, column=2, padx=5, pady=5, sticky="NSEW")

        label = tk.Label(self, text="0")
        label.grid(row=3, column=2, padx=5, pady=5)

    
app = bfitApp()
app.mainloop()


