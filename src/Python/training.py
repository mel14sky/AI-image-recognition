import os
from tkinter import messagebox
from PIL import Image
import img2vec

path_name = "archivegen1/dataset"

# Function to convert images to BMP format
def convert_bmp(filename):
    try:
        filename = os.path.abspath(filename)
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

def vector2txt(vector, name):
    if vector is None:
        print("No vector to save.")
        return
    name = os.path.abspath(name)
    pokemon_folder = os.path.dirname(name)
    txt_folder = os.path.join(pokemon_folder, "vectors")
    os.makedirs(txt_folder, exist_ok=True)
    image_name = os.path.splitext(os.path.basename(name))[0]
    output_path = os.path.join(txt_folder, f"{image_name}.txt")
    try:
        with open(output_path, 'w') as f:
            #write length of vector + 1 for the bias
            f.write(f"{len(vector)+1}\n")

            for value in vector:
                # force vector values to be 6dp
                formatted_value = f"{value:.6f}"
                f.write(f"{formatted_value}\n")
            # force last element to be 1.000000
            f.write("1.000000")

    except IOError as error:
        print(f"Error writing to output file: {error}")
        return

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
                files_inside = [f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f)) and f.lower() != "vectors"]
                if len(files_inside) == 0:
                    print(f"The folder '{pokemon_name}' is empty.")
                else:
                # Converts images to BMP format
                    for filename in files_inside:
                        # path = path_name + "/" + pokemon_name + "/" + filename
                        current_path = os.path.join(path_name, pokemon_name, filename)
                        bmp_path = convert_bmp(current_path)
                        if bmp_path:
                            vector = img2vec.image2vector(bmp_path)
                            vector2txt(vector, bmp_path)
                    print("Complete")
            else:
                print(f"FAILURE: Folder '{pokemon_name}' not found.")
    else:
        print("order.txt not found.")

main()






