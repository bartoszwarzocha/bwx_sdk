# BWX_SDK Library - Scripts

A collection of tools for managing header files, generating CMake configuration files, and installing dependencies for the **BWX_SDK Library**.

---

## Table of Contents
- [Overview](#overview)
- [Scripts](#scripts)
  - [copy_headers.py](#copy_headerspy)
  - [generate_lib_cmake_file.py](#generate_lib_cmake_filepy)
  - [remove_doxygen_comments.py](#remove_doxygen_commentspy)
  - [install_dependencies_linux.sh](#install_dependencies_linuxsh)
  - [install_dependencies_macos.sh](#install_dependencies_macossh)
- [License](#license)

---

## Overview
This set of scripts helps streamline the development process of the **BWX_SDK Library** by automating the following tasks:
- Synchronizing header files from the `src` directory to the `include` directory while filtering out specific code sections.
- Generating `CMakeLists.txt` files for individual sublibraries.
- Removing Doxygen comments from source files.
- Installing development dependencies on Linux and macOS.

All tools are designed to work seamlessly with the `BWX_SDK` build environment.

---

## Scripts

### `copy_headers.py`
**Purpose:** Synchronize header files from the `src` directory to the `include` directory, filtering out code sections between `#BEGIN_COPY_IGNORING` and `#END_COPY_IGNORING`.

#### Usage:
```bash
python copy_headers.py
```

#### Description:
- Copies all `.h` and `.hpp` files from `../src` to `../include/bwx_sdk`.
- Code sections between `#BEGIN_COPY_IGNORING` and `#END_COPY_IGNORING` are ignored during the copy.
- Automatically creates necessary destination directories.
- Invokes `remove_doxygen_comments.py` to clean up copied header files.

---

### `generate_lib_cmake_file.py`
**Purpose:** Generate a `CMakeLists.txt` file for a specified sublibrary.

#### Usage:
```bash
python generate_lib_cmake_file.py <LIBRARY_NAME>
```

#### Example:
```bash
python generate_lib_cmake_file.py CORE
```

#### Description:
- Generates a `CMakeLists.txt` in the current directory.
- Handles both source and header files.
- Supports shared and static library builds.
- Configures include directories and links to `wxWidgets` libraries.

---

### `remove_doxygen_comments.py`
**Purpose:** Remove Doxygen comments from C++ source and header files.

#### Usage:
```bash
python remove_doxygen_comments.py <DIRECTORY>
```

#### Example:
```bash
python remove_doxygen_comments.py ../include/bwx_sdk
```

#### Description:
- Processes files with extensions: `.cpp`, `.c`, `.h`, `.hpp`, `.cxx`, `.cc`.
- Removes block comments (`/** ... */`, `/*! ... */`) and inline comments (`///<`, `//!`).
- Ensures clean, comment-free code sections while preserving single empty lines between code blocks.

---

### `install_dependencies_linux.sh`
**Purpose:** Install development dependencies on Linux systems.

#### Usage:
```bash
./install_dependencies_linux.sh
```

#### Description:
- Installs required packages using `apt`.
- Sets up `vcpkg` if not already present.
- Installs libraries for:
  - Development tools (CMake, Ninja, compilers)
  - Graphics and font rendering
  - X11 libraries
  - Audio and input support
  - OpenGL and EGL support
- Prompts the user before installing each component.

---

### `install_dependencies_macos.sh`
**Purpose:** Install development dependencies on macOS systems.

#### Usage:
```bash
./install_dependencies_macos.sh
```

#### Description:
- Uses `brew` to install required packages.
- Sets up `vcpkg` if not already present.
- Installs libraries for:
  - Development tools (CMake, Ninja, compilers)
  - Graphics and font rendering
  - X11 libraries
  - Audio and input support
  - OpenGL and EGL support
- Prompts the user before installing each component.

---

## License
**BWX_SDK Library** - Scripts are provided under the wxWidgets license.

---
© 2025 Bartosz Warzocha (bartosz.warzocha@gmail.com)

