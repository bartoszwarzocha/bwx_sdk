'''
BWX_SDK Library
Tool for removing of the doxygen comments from the source code
Copyright 2025 by Bartosz Warzocha (bartosz.warzocha@gmail.com)
wxWidgets licence

How to use:
    python remove_doxygen_comments.py <path>
Example:
    python remove_doxygen_comments.py ./
'''

import os
import re
import argparse

def remove_doxygen_comments_from_file(file_path):
    try:
        # Try to open the file with UTF-8 encoding
        with open(file_path, 'r', encoding='utf-8') as file:
            code = file.read()
    except UnicodeDecodeError:
        # If UTF-8 decoding fails, use 'latin1' encoding as a fallback
        with open(file_path, 'r', encoding='latin1') as file:
            code = file.read()

    # Regex patterns to remove only the targeted Doxygen comments:
    patterns = [
        r'/\*\*[\s\S]*?\*/',  # Block comments /** ... */
        r'/\*!([\s\S]*?)\*/', # Block comments /*! ... */
        r'///<.*$',           # Line comments ///<
        r'//!.*$'             # Line comments //! 
    ]

    for pattern in patterns:
        code = re.sub(pattern, '', code, flags=re.MULTILINE)

    # Remove consecutive empty lines
    code = re.sub(r'\n{3,}', '\n\n', code)

    # Write the cleaned code back to the file
    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(code)

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
