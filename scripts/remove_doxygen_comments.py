'''
BWX_SDK Library
Tool for removing of the doxygen comments from C++ src files
Copyright 2025 by Bartosz Warzocha (bartosz.warzocha@gmail.com)
wxWidgets licence
'''

import os
import re
import argparse

def remove_doxygen_comments_from_file(file_path):
    try:
        # Try to open the file with UTF-8 encoding
        with open(file_path, 'r', encoding='utf-8') as file:
            lines = file.readlines()
    except UnicodeDecodeError:
        # If UTF-8 decoding fails, use 'latin1' encoding as a fallback
        with open(file_path, 'r', encoding='latin1') as file:
            lines = file.readlines()

    cleaned_lines = []
    inside_block_comment = False

    for line in lines:
        stripped_line = line.strip()

        # Handle block comments /** ... */ and /*! ... */
        if inside_block_comment:
            if re.search(r'\*/', line):
                inside_block_comment = False
            continue

        if re.match(r'\s*/\*\*', line) or re.match(r'\s*/\*!', line):
            if not re.search(r'\*/', line):  # Start of multi-line block comment
                inside_block_comment = True
            continue

        # Remove inline comments: ///< and //! at the end of a line with code
        line = re.sub(r'\s*///<.*$', '', line)
        line = re.sub(r'\s*//!.*$', '', line)

        # If the line is not empty after removals, keep it
        if stripped_line or (cleaned_lines and cleaned_lines[-1].strip()):
            cleaned_lines.append(line)
        else:
            # Preserve a single empty line between code sections
            if cleaned_lines and cleaned_lines[-1].strip() != '':
                cleaned_lines.append('\n')

    # Remove leading and trailing empty lines
    while cleaned_lines and not cleaned_lines[0].strip():
        cleaned_lines.pop(0)
    while cleaned_lines and not cleaned_lines[-1].strip():
        cleaned_lines.pop()

    # Write the cleaned code back to the file
    with open(file_path, 'w', encoding='utf-8') as file:
        file.writelines(cleaned_lines)

def process_directory(directory):
    extensions = ('.cpp', '.c', '.h', '.hpp', '.cxx', '.cc')

    for root, _, files in os.walk(directory):
        for filename in files:
            if filename.endswith(extensions):
                file_path = os.path.join(root, filename)
                print(f'Processing file: {file_path}')
                remove_doxygen_comments_from_file(file_path)

def main():
    parser = argparse.ArgumentParser(description='Remove Doxygen comments from C++ source files in the specified directory.')
    parser.add_argument('directory', help='Path to the directory containing C++ source files')

    args = parser.parse_args()
    process_directory(args.directory)

if __name__ == '__main__':
    main()
