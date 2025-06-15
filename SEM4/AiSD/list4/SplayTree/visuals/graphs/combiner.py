import os
from PIL import Image, ImageDraw, ImageFont

def combine_images_grid(left_images, right_images, output_filename, title):
    """
    Combines two sets of images into a two-column grid with a title,
    saving the result as a new PNG file.

    Args:
        left_images (list): A list of file paths for the images in the left column.
        right_images (list): A list of file paths for the images in the right column.
        output_filename (str): The name of the file to save the combined image to.
        title (str): The main title to be displayed at the top of the image.
    """
    try:
        # Open all images to ensure they exist and to get their dimensions.
        opened_left = [Image.open(p) for p in left_images]
        opened_right = [Image.open(p) for p in right_images]
        all_opened_images = opened_left + opened_right
    except FileNotFoundError as e:
        print(f"Error: Could not find image file {e.filename}. Skipping creation of '{output_filename}'.")
        return

    # Assume all images are the same size, based on the first one.
    img_width, img_height = all_opened_images[0].size

    # --- Layout Configuration ---
    padding = 50  # Space around images and edges
    title_height = 80  # Space at the top for the main title
    column_title_height = 40 # Space for column headers
    
    # Calculate the total dimensions for the new canvas image.
    # Width = 2 columns + 3 padding sections (left, middle, right)
    total_width = (img_width * 2) + (padding * 3)
    # Height = 3 rows of images + 4 padding sections + title areas
    num_rows = max(len(opened_left), len(opened_right))
    total_height = (img_height * num_rows) + (padding * (num_rows + 1)) + title_height + column_title_height

    # Create a new blank canvas with a white background.
    new_image = Image.new('RGB', (total_width, total_height), 'white')
    draw = ImageDraw.Draw(new_image)

    # --- Font and Title Setup ---
    try:
        # Use a common, bold font for the main title.
        title_font = ImageFont.truetype("DejaVuSans-Bold.ttf", 32)
        # Use a regular font for column headers.
        column_font = ImageFont.truetype("DejaVuSans.ttf", 22)
    except IOError:
        print("Default font 'DejaVuSans' not found. Using Pillow's standard font.")
        title_font = ImageFont.load_default()
        column_font = ImageFont.load_default()

    # --- Draw Titles on the Canvas ---
    
    # Main Title (centered)
    formatted_title = title.replace('_', ' ').title()
    title_bbox = draw.textbbox((0, 0), formatted_title, font=title_font)
    title_width = title_bbox[2] - title_bbox[0]
    draw.text(((total_width - title_width) / 2, padding // 2), formatted_title, font=title_font, fill='black')
    
    # Column Titles
    left_col_x = padding
    right_col_x = padding * 2 + img_width
    titles_y = title_height
    
    draw.text((left_col_x, titles_y), "Maximum Metrics", font=column_font, fill='dimgray')
    draw.text((right_col_x, titles_y), "Average Metrics", font=column_font, fill='dimgray')
    
    # --- Paste Images onto the Canvas ---
    
    # Paste left column images
    y_offset = title_height + column_title_height + padding
    for img in opened_left:
        new_image.paste(img, (left_col_x, y_offset))
        y_offset += img_height + padding

    # Paste right column images
    y_offset = title_height + column_title_height + padding
    for img in opened_right:
        new_image.paste(img, (right_col_x, y_offset))
        y_offset += img_height + padding

    # Save the final combined image.
    new_image.save(output_filename)
    print(f"Successfully created '{output_filename}'")


def main():
    """
    Finds all PNG files in the current directory, groups them by operation,
    and calls the combination function for each group.
    """
    try:
        # Find all .png files in the current directory
        all_files = [f for f in os.listdir('.') if f.lower().endswith('.png')]
    except FileNotFoundError:
        print("Error: Cannot access the current directory.")
        return

    # Group files by their prefixes (e.g., 'asc_delete', 'rand_insert')
    grouped_files = {}
    for filename in all_files:
        # Expecting filenames like 'asc_delete_Avg_Comparisons.png'
        parts = filename.split('_')
        if len(parts) >= 2:
            group_key = f"{parts[0]}_{parts[1]}"
            if group_key not in grouped_files:
                grouped_files[group_key] = []
            grouped_files[group_key].append(filename)

    # Process each group of images
    for group_name, filenames in grouped_files.items():
        # Filter for 'Max' and 'Avg' files within the group
        max_files = sorted([f for f in filenames if 'Max' in f])
        avg_files = sorted([f for f in filenames if 'Avg' in f])
        
        # We need at least one of each to create a combined image
        if max_files and avg_files:
            output_file = f"combined_{group_name}.png"
            combine_images_grid(max_files, avg_files, output_file, group_name)
        else:
            print(f"Skipping group '{group_name}': Missing either Max or Avg image files.")

if __name__ == '__main__':
    main()
