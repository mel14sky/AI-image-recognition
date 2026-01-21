#teakes a .bmp file and returns a vector

import sys
from PIL import Image
import numpy as np

#set vector size
length = 128

def image2vector(image_path):
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
