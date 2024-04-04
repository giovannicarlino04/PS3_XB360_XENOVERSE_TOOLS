import os

def contains_printable_chars(data):
    return any(32 <= byte <= 126 for byte in data)

def swap_bytes(input_path, output_path, word_size):
    with open(input_path, 'rb') as input_file, open(output_path, 'wb') as output_file:
        magic = input_file.read(4)  # Read the first 4 bytes (magic)
        output_file.write(magic)  # Write the magic bytes to the output file
        
        while True:
            word = input_file.read(word_size)
            if not word:
                break
            
            if contains_printable_chars(word):
                output_file.write(word)  # Write unchanged if contains printable characters
            else:
                output_file.write(word[::-1])  # Swap bytes if not printable

def process_folder(input_folder, output_folder, word_size):
    for root, dirs, files in os.walk(input_folder):
        for file in files:
            input_path = os.path.join(root, file)
            relative_path = os.path.relpath(input_path, input_folder)
            output_path = os.path.join(output_folder, relative_path)
            os.makedirs(os.path.dirname(output_path), exist_ok=True)
            swap_bytes(input_path, output_path, word_size)

# Example usage:
input_folder = input("Enter input folder: ")
output_folder = input_folder + "_converted"
word_size = 4

process_folder(input_folder, output_folder, word_size)
