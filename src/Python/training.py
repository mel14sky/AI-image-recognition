import os
from tkinter import messagebox
from PIL import Image
import img2vec

path_name = "archivegen1/dataset"

# Function to convert images to BMP format
def convert_bmp(filename):
    try:
        with Image.open(filename) as img:
            file_root, file_ext = os.path.splitext(filename)
            bmp_filename = f"{file_root}.bmp"
            img.save(bmp_filename, "BMP")
        # Remove the original file if it is not already a BMP
            if file_ext.lower() != ".bmp":
                os.remove(filename)
            return bmp_filename
        
    except Exception as e:
        print("Error", f"Failed to process image: {e}")

# Function to get find an image folder path
def get_folder_path(target_folder_name):
    full_path = os.path.join(path_name, target_folder_name)
    if os.path.isdir(full_path):
        return full_path

def main():
# reads order.txt and checks for images in each folder and prints out the file names
    if os.path.exists("order.txt"):
        with open("order.txt", "r") as f:
            order = f.readlines()   
    # checks the elements in order.txt and strips any whitespace
        for line in order:
            pokemon_name = line.strip()
            if not pokemon_name:
                continue
            print(f"Checking folder: {pokemon_name}")
    # finds the folder path and lists files inside
            folder_path = get_folder_path(pokemon_name)
            if folder_path:
                files_inside = os.listdir(folder_path)
                if len(files_inside) == 0:
                    print(f"The folder '{pokemon_name}' is empty.")
                else:
            # Converts images to BMP format
                    print(f"Found {len(files_inside)} files inside:")
                    for filename in files_inside:
                        path = path_name + "/" + pokemon_name + "/" + filename
                        convert_bmp(path)
                        img2vec.main(path)
                    print("Complete")
            else:
                print(f"FAILURE: Folder '{pokemon_name}' not found.")
    else:
        print("order.txt not found.")

main()






