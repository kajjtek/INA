import os
import re
from PIL import Image, ImageDraw, ImageFont

def combine_scenario_images(output_filename, title, 
                              insert_max_files, insert_avg_files, 
                              delete_max_files, delete_avg_files):
    """
    Combines images for a scenario into a 2x2 grid layout.
    - Top row: Insert metrics (Max vs Avg)
    - Bottom row: Delete metrics (Max vs Avg)

    Args:
        output_filename (str): The name of the file to save the combined image to.
        title (str): The main title for the scenario.
        insert_max_files (list): File paths for max insert metrics.
        insert_avg_files (list): File paths for avg insert metrics.
        delete_max_files (list): File paths for max delete metrics.
        delete_avg_files (list): File paths for avg delete metrics.
    """
    all_files = insert_max_files + insert_avg_files + delete_max_files + delete_avg_files
    
    try:
        # Open all images to get their dimensions.
        opened_images = [Image.open(p) for p in all_files]
    except FileNotFoundError as e:
        print(f"Error: Could not find image file {e.filename}. Skipping creation of '{output_filename}'.")
        return

    # Assume all images are the same size.
    img_width, img_height = opened_images[0].size

    # --- Layout Configuration ---
    padding = 50
    title_height = 70
    section_title_height = 50
    num_rows_per_section = 3

    # Calculate total dimensions for the new canvas.
    total_width = (img_width * 2) + (padding * 3)
    total_height = (img_height * num_rows_per_section * 2) + (padding * 4) + title_height + (section_title_height * 2)

    # Create a new blank canvas with a white background.
    new_image = Image.new('RGB', (total_width, total_height), 'white')
    draw = ImageDraw.Draw(new_image)

    # --- Font Setup ---
    try:
        title_font = ImageFont.truetype("DejaVuSans-Bold.ttf", 32)
        section_font = ImageFont.truetype("DejaVuSans-Bold.ttf", 24)
        column_font = ImageFont.truetype("DejaVuSans.ttf", 20)
    except IOError:
        print("Default font 'DejaVuSans' not found. Using Pillow's standard font.")
        title_font = ImageFont.load_default()
        section_font = ImageFont.load_default()
        column_font = ImageFont.load_default()

    # --- Draw Titles ---
    # Main Title
    formatted_title = title.replace('_', ' ').replace('&', '&')
    title_bbox = draw.textbbox((0, 0), formatted_title, font=title_font)
    title_width = title_bbox[2] - title_bbox[0]
    draw.text(((total_width - title_width) / 2, padding / 2), formatted_title, font=title_font, fill='black')

    # Define column positions
    left_col_x = padding
    right_col_x = padding * 2 + img_width

    # --- Section 1: Insert Metrics ---
    section1_y = title_height
    draw.text((padding, section1_y), "Insert Metrics", font=section_font, fill='black')
    
    # Column headers for Insert
    col_title_y1 = section1_y + section_title_height
    draw.text((left_col_x, col_title_y1), "Maximum", font=column_font, fill='dimgray')
    draw.text((right_col_x, col_title_y1), "Average", font=column_font, fill='dimgray')
    
    # Paste Insert images
    y_offset = col_title_y1 + 30
    for img_path in insert_max_files:
        img = Image.open(img_path)
        new_image.paste(img, (left_col_x, y_offset))
        y_offset += img_height + padding

    y_offset = col_title_y1 + 30
    for img_path in insert_avg_files:
        img = Image.open(img_path)
        new_image.paste(img, (right_col_x, y_offset))
        y_offset += img_height + padding

    # --- Section 2: Delete Metrics ---
    section2_y = title_height + section_title_height + (img_height + padding) * num_rows_per_section + padding
    draw.text((padding, section2_y), "Delete Metrics", font=section_font, fill='black')

    # Column headers for Delete
    col_title_y2 = section2_y + section_title_height
    draw.text((left_col_x, col_title_y2), "Maximum", font=column_font, fill='dimgray')
    draw.text((right_col_x, col_title_y2), "Average", font=column_font, fill='dimgray')

    # Paste Delete images
    y_offset = col_title_y2 + 30
    for img_path in delete_max_files:
        img = Image.open(img_path)
        new_image.paste(img, (left_col_x, y_offset))
        y_offset += img_height + padding

    y_offset = col_title_y2 + 30
    for img_path in delete_avg_files:
        img = Image.open(img_path)
        new_image.paste(img, (right_col_x, y_offset))
        y_offset += img_height + padding

    # Save the final image
    new_image.save(output_filename)
    print(f"Successfully created '{output_filename}'")


def main():
    """
    Finds and groups PNG files based on the complex scenario naming convention
    and calls the combination function for each group.
    """
    try:
        all_files = [f for f in os.listdir('.') if f.lower().endswith('.png')]
    except FileNotFoundError:
        print("Error: Cannot access the current directory.")
        return

    grouped_files = {}
    # Use regex to robustly find the scenario key before "_Avg_" or "_Max_"
    for filename in all_files:
        match = re.match(r'(.+?)_([AM][av][gx])_', filename)
        if match:
            group_key = match.group(1)
            if group_key not in grouped_files:
                grouped_files[group_key] = []
            grouped_files[group_key].append(filename)

    # Process each scenario group
    for group_name, filenames in grouped_files.items():
        # Filter files into the four sub-categories for the layout
        max_insert = sorted([f for f in filenames if '_Max_Insert_' in f])
        avg_insert = sorted([f for f in filenames if '_Avg_Insert_' in f])
        max_delete = sorted([f for f in filenames if '_Max_Delete_' in f])
        avg_delete = sorted([f for f in filenames if '_Avg_Delete_' in f])

        # Ensure we have files to process
        if all((max_insert, avg_insert, max_delete, avg_delete)):
            output_file = f"combined_{group_name}.png"
            combine_scenario_images(output_file, group_name,
                                      max_insert, avg_insert,
                                      max_delete, avg_delete)
        else:
            print(f"Skipping group '{group_name}': Missing one or more sub-categories of images.")

if __name__ == '__main__':
    main()
