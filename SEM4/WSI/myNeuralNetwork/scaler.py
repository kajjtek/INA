from PIL import Image
import os

# Directory containing the images
input_dir = "./imagesbeforescaling"
output_dir = "./imagesscaled"

# Ensure the output directory exists
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# List all image files in the directory
image_files = [f for f in os.listdir(input_dir) if f.endswith(('jpg', 'jpeg', 'png'))]

# Resize all images
for image_file in image_files:
    # Open the image
    img_path = os.path.join(input_dir, image_file)
    img = Image.open(img_path)
    
    # Resize the image to 28x28
    img_resized = img.resize((28, 28)).convert("L")
    
    
    # Save the resized image
    output_path = os.path.join(output_dir, image_file)
    img_resized.save(output_path)

print("Images resized and saved successfully.")