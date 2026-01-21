#teakes a .bmp file and turns it into a vector stored in a .txt

import sys
import os
from PIL import Image
import numpy as np

#set vector size
length = 128

def process_image_to_vector(image_path):
    try:
        with Image.open(image_path) as img:
            if img.format != 'BMP':
                #convert 
                print(f"Error: Input file is {img.format}, not BMP")
                return
            # 16x8 = 128
            # LANCZOS is used to preserve detail during downscaling.
            img_resized = img.resize(
                (int(length/8), int(length/16)), 
                resample=Image.Resampling.LANCZOS
            )

            # Convert to float
            img_array = np.array(img_resized, dtype=np.float32)
            
            # Normalize pixel intensity from 255 to 1
            normalized_array = img_array / 255.0

            vector = normalized_array.flatten()

            if len(vector) > length:
                vector = np.resize(vector, length)
            
            return vector

    except FileNotFoundError:
        print(f"Error: The file '{image_path}' was not found.")
        sys.exit(1)
    except Exception as error:
        print(f"Error: {error}")
        sys.exit(1)

def save_vector_to_txt(vector):
    #input.bmp -> input.txt
    output_path = "input_vector.txt"

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
        print(f"\nSuccessfully saved vector to: {output_path}")

    except IOError as error:
        print(f"Error writing to output file: {error}")
        sys.exit(1)

def main():
    # function should call a .bmp file to be converted
    if len(sys.argv) < 2:
        print("Usage: python img2vec.py <input_image.bmp>")
        sys.exit(1)

    image_path = sys.argv[1]
    vector = process_image_to_vector(image_path)

    print("Vector Created\n")
    formatted_console_output = [f"{v:.6f}" for v in vector]
    print(formatted_console_output)
    print(f"Vector Length: {len(vector)}")
    save_vector_to_txt(vector)
