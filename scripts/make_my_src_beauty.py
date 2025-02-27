'''
BWX_SDK Library
Tool for C++ code formatting
Copyright 2025 by Bartosz Warzocha (bartosz.warzocha@gmail.com)
wxWidgets licence
'''

import os
import sys
import re
import subprocess

# Script settings
DEFAULT_CLANG_FORMAT = "clang-format"
CLANG_FORMAT_CONFIG = """
BasedOnStyle: Google
IndentWidth: 4
ContinuationIndentWidth: 4
AccessModifierOffset: -4
IndentCaseLabels: true
UseTab: Never
ColumnLimit: 120
BreakBeforeBraces: Attach
KeepEmptyLinesAtTheStartOfBlocks: false
"""
HEADER_NOTICE = """
// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.
"""


def create_clang_format_config(file_path):
    """Ensures a .clang-format file exists in the directory of the file being formatted."""
    config_path = os.path.join(os.path.dirname(file_path), ".clang-format")
    with open(config_path, "w", encoding="utf-8") as f:
        f.write(CLANG_FORMAT_CONFIG)
    return config_path


def run_clang_format(file_path, clang_format=DEFAULT_CLANG_FORMAT):
    """Formats the file using clang-format with enforced style."""
    config_path = create_clang_format_config(file_path)
    try:
        result = subprocess.run(
            [clang_format, f"-style=file:{config_path}", file_path],
            capture_output=True, text=True
        )
        if result.returncode == 0:
            return result.stdout.splitlines()
        else:
            print("clang-format error:", result.stderr)
            return None
    except FileNotFoundError:
        print("clang-format not found! Make sure it is installed and available in PATH.")
        return None


def insert_header_notice(lines):
    """Inserts a header notice below the existing header comment if present."""
    if lines and lines[0].strip().startswith("//"):
        index = 0
        while index < len(lines) and lines[index].strip().startswith("//"):
            index += 1
        if not any(HEADER_NOTICE.strip() in line for line in lines):
            lines.insert(index, HEADER_NOTICE + "\n")
    return lines


def clean_empty_lines(lines, iterations=3):
    """Removes unnecessary empty lines between function and variable declarations."""
    for _ in range(iterations):
        cleaned = []
        prev_type = None
        
        def detect_line_type(line):
            if re.match(r'^(constexpr\s+|inline\s+|static\s+|virtual\s+|\w+\s+)?[\w\*&<>]+\s+[\w_]+\s*\(', line) and line.strip().endswith(";"):
                return "function"
            elif re.match(r'^[\w\s\*&<>]+\s+[\w_]+\s*[=;]', line):
                return "variable"
            return "other"
        
        for line in lines:
            stripped = line.strip()
            current_type = detect_line_type(stripped)
            
            if stripped == "":
                if prev_type in {"function", "variable"} and current_type == prev_type:
                    continue  # Remove empty lines between function or variable declarations
                if cleaned and cleaned[-1].strip() == "":
                    continue  # Skip multiple consecutive empty lines
            
            cleaned.append(line)
            prev_type = current_type if stripped else prev_type
        
        lines = cleaned  # Apply another pass
    
    return lines


def format_cpp_file(file_path):
    """Main function for formatting a C++ file."""
    with open(file_path, "r", encoding="utf-8-sig") as f:
        lines = f.readlines()
    
    lines = insert_header_notice(lines)
    cleaned_lines = clean_empty_lines(lines)
    
    with open(file_path, "w", encoding="utf-8") as f:
        f.writelines(cleaned_lines)
    
    formatted_lines = run_clang_format(file_path)
    if formatted_lines is None:
        return
    
    formatted_lines = clean_empty_lines(formatted_lines)
    
    with open(file_path, "w", encoding="utf-8") as f:
        f.writelines("\n".join(formatted_lines) + "\n")
    
    print(f"File {file_path} has been formatted.\nClang-Format configuration applied from: {os.path.dirname(file_path)}/.clang-format")


def format_cpp_files_in_directory(directory):
    """Recursively formats all .cpp, .c, .h, .hpp, .cxx, .cc files in the directory."""
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith((".cpp", ".c", ".h", ".hpp", ".cxx", ".cc")):
                file_path = os.path.join(root, file)
                format_cpp_file(file_path)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python cpp_formatter.py <file.cpp | directory>")
        sys.exit(1)
    
    input_path = sys.argv[1]
    
    if os.path.isdir(input_path):
        format_cpp_files_in_directory(input_path)
    elif os.path.isfile(input_path):
        format_cpp_file(input_path)
    else:
        print("The specified path does not exist or is invalid.")
