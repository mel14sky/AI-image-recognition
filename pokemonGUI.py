import tkinter as tk
from tkinterdnd2 import DND_FILES, TkinterDnD
import pygame

# Initialises Pygame mixer and loads startup sound
pygame.mixer.init()
startup_sound = pygame.mixer.Sound("whos-that-pokemon_.mp3")
startup_sound.play()

# Begins the Tkinter main window
root = TkinterDnD.Tk()

# Title for the window
greeting = tk.Label(text="Welcome to the AI Pokemon Recognition System!")
greeting.pack()

# Instructs the user to upload an image
label = tk.Label(text="Upload an image of a Pokemon to identify it.",
                 foreground="black")
label.pack()

# Uploads image to the AI 
button = tk.Button(text="Upload Image")
button.pack()

# Entry field for image path destination
tkinter_entry = tk.Entry(fg="black", bg="lightgrey", width=50)
tkinter_entry.pack()

# Listbox for drag and drop functionality
lb = tk.Listbox(root)
lb.insert(1, "Drag and Drop Pokemon image files here")
lb.drop_target_register(DND_FILES)
lb.dnd_bind('<<Drop>>', lambda e: lb.insert(tk.END, e.data))
lb.pack()

# Ends the Tkinter main window
root.mainloop()