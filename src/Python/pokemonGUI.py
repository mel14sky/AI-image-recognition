import tkinter as tk
import tkinter.messagebox
from tkinterdnd2 import DND_FILES, TkinterDnD
import pygame
import cv2
import numpy as np
import matplotlib.pyplot as plt


# Initialises Pygame mixer and loads startup sound
pygame.mixer.init()
startup_sound = pygame.mixer.Sound("whos-that-pokemon_.mp3")
startup_sound.play()

# Begins the Tkinter main window
root = TkinterDnD.Tk()
root.configure(bg = "red")

# Title for the window
greeting = tk.Label(root, text = "Welcome to the AI Pokemon Recognition System!", foreground = "yellow", bg = "red")
greeting.pack()

# Instructs the user to upload an image
label = tk.Label(root, text = "Upload an image of a Pokemon to identify it.", foreground = "black", bg = "red")
label.pack()

# Function to process the uploaded image
def process_image():
    image_path = tkinter_entry.get()
    image = cv2.imread(image_path)
    if image is None:
        error_box = tk.messagebox.showerror("Error", "Invalid image path. Please try again.")
        return
    else:
        image_resize = cv2.resize(image, None, fx = 0.01, fy = 0.01, interpolation = cv2.INTER_LINEAR)
        image_grey = cv2.cvtColor(image_resize, cv2.COLOR_BGR2GRAY)
        total_elements = image_grey.size
        np.savetxt("pokemon_image.txt", image_grey, fmt='%.6f', header = f"{total_elements}", comments = "")
      

# Uploads processed image when button is clicked
button = tk.Button(root, text = "Upload Image", command = process_image, fg = "black", bg = "red")
button.pack()
        
# Entry field for image path destination
tkinter_entry = tk.Entry(root, fg = "black", bg= "red", width = 50,)
tkinter_entry.pack()

# Listbox for drag and drop functionality
lb = tk.Listbox(root, fg = "red", bg = "white", width = 40, height = 4)
lb.insert(1, "Drag and Drop Pokemon image files here")
lb.drop_target_register(DND_FILES)
lb.dnd_bind('<<Drop>>', lambda e: tkinter_entry.insert(0, e.data))
lb.pack()

# Ends the Tkinter main window
root.mainloop()