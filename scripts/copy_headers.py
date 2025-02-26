"""
BWX_SDK Library
Tool for header files synchronization (src -> include)
Copyright 2025 by Bartosz Warzocha (bartosz.warzocha@gmail.com)
wxWidgets licence

How to use:
    python copy_headers.py

Note: Source code sections between BEGIN_COPY_IGNORING and END_COPY_IGNORING
      are ignored and not copied to the include directory.
"""

import os
import shutil
import subprocess

IGNORE_START = "#BEGIN_COPY_IGNORING"
IGNORE_END = "#END_COPY_IGNORING"

def filter_and_copy_file(src_file, dest_file):
    os.makedirs(os.path.dirname(dest_file), exist_ok=True)

    with open(src_file, 'r', encoding='utf-8') as infile, open(dest_file, 'w', encoding='utf-8') as outfile:
        ignoring = False
        for line in infile:
            stripped = line.strip()
            if stripped.startswith(IGNORE_START):
                ignoring = True
                continue
            elif stripped.startswith(IGNORE_END):
                ignoring = False
                continue
            if not ignoring:
                outfile.write(line)

    print(f"Filtered and copied: {src_file} -> {dest_file}")

def copy_headers_with_filter(src_dir, dest_dir):
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)

    for root, _, files in os.walk(src_dir):
        for file in files:
            if file.endswith(('.h', '.hpp')):
                relative_path = os.path.relpath(root, src_dir)
                dest_path = os.path.join(dest_dir, relative_path)
                src_file = os.path.join(root, file)
                dest_file = os.path.join(dest_path, file)
                filter_and_copy_file(src_file, dest_file)

def remove_doxygen_comments(target_dir):
    script_path = os.path.join(os.path.dirname(__file__), 'remove_doxygen_comments.py')
    try:
        subprocess.run(['python', script_path, target_dir], check=True)
        print(f"Deoxygen comments removed in: {target_dir}")
    except subprocess.CalledProcessError as e:
        print(f"Error during doxygen comment removing: {e}")

if __name__ == "__main__":
    SRC_DIR = os.path.join(os.path.dirname(__file__), "..", "src")
    DEST_DIR = os.path.join(os.path.dirname(__file__), "..", "include", "bwx_sdk")

    copy_headers_with_filter(SRC_DIR, DEST_DIR)
    remove_doxygen_comments(DEST_DIR)
