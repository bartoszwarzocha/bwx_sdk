# BWX_SDK Library

## Table of Contents

- [Overview](#overview)
- [Features](#features)
  - [List of component libraries](#list-of-component-libraries)
- [Requirements](#requirements)
  - [Supported Compilers](#supported-compilers)
- [CI/CD – GitHub Actions](#cicd--github-actions)
- [Library Documentation](#library-documentation)
- [Installation & Build Instructions](#installation--build-instructions)
  - [Option 1: Automated (Recommended)](#option-1-automated-recommended)
  - [Option 2: Manual Setup](#option-2-manual-setup)
    - [1. Clone the Repository](#1-clone-the-repository)
    - [2. Install Dependencies](#2-install-dependencies)
    - [3. Build with CMake](#3-build-with-cmake)
- [Directory Structure](#directory-structure)
- [Usage Example](#usage-example)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

---

## Overview
**BWX_SDK** is a C++ library designed to extend and enhance the functionality of [wxWidgets](https://www.wxwidgets.org/). It provides a set of additional classes and utilities that simplify the usage of wxWidgets, making development more efficient and user-friendly.

The library is cross-platform, supporting **Windows**, **Linux**, and **macOS**, and offers both static and shared builds. BWX_SDK is distributed under the **wxWidgets license**.

[![LINUX](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-linux.yml/badge.svg)](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-linux.yml)
[![WINDOWS](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-windows.yml/badge.svg)](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-windows.yml)
[![MACOS](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-macos.yml/badge.svg)](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-macos.yml)
[![License](https://img.shields.io/badge/License-wxWidgets-orange)](https://www.wxwidgets.org/about/licence/)
[![Version](https://img.shields.io/badge/Version-Under_construction-blue)](https://github.com/bartoszwarzocha/bwx_sdk)

### From author...
_The library is a collection of solutions I have developed in over 25 years of programming work with the [wxWidgets](https://www.wxwidgets.org/) library. It addresses a variety of programming problems for various domains, often going beyond the implementation of the GUI itself, but with a focus on maintaining consistency with [wxWidgets](https://www.wxwidgets.org/)._

### Current version/release
The current GitHub version is still under construction (it's a long way to first release) and comes from my personal collection and is constantly being added to, while the code is updated and rewritten for modern C++. **I hope you find this stuff useful and are inspired to discover more uses for the fantastic [wxWidgets](https://www.wxwidgets.org/) library.**

---

## Features
- Extended classes for core wxWidgets components.
- Utilities for simplifying common tasks with wxWidgets:
    - comfortable command line parser
    - lightweight JSON parser and serializer (merge, diff, transform etc.)
    - easy to use internationalization stuff
    - and others...
- Cross-platform compatibility (Windows, Linux, macOS).
- Support for both static and shared (Windows) library builds.
- Examples demonstrating library usage.

### List of component libraries
Library|Description|Comments|Status
-|-|-|-
_bwx_adv_|Advanced stuff like **Python** plugins system, etc.||In preparation
_bwx_core_|Basic classes and functions for general functions (application configuration, cmd command parser, etc.), operations (such as data conversion or mathematical operations) and types (string, datetime, json, etc.)||**Moving in progress**
_bwx_gl_|A collection of classes and functions to facilitate working with **OpenGL** in wxWidgets applications||In preparation
_bwx_gui_|A collection of various custom GUI parts (sizer wrappers, dialog windows, custom GUI controls, etc.)|Requires _bwx_core_|**Moving in progress**
_bwx_utils_|A collection of different classes and functions that provide ‘non-primary’ solutions, such as operations on colours|Requires _bwx_core_|**Moving in progress**

---

## Requirements
- **C++20** compliant compiler.
- **CMake >= 3.21**
- **vcpkg** for dependency management.
- **wxWidgets >= 3.2.6** (handled via `vcpkg`).

### Supported Compilers
- **Windows:** MSVC (Visual Studio 2022, 2019), Ninja
- **Linux:** GCC, Clang, Ninja
- **macOS:** AppleClang, Xcode, Ninja

**(Tested for: Windows 11, Ubuntu Linux 22.04, macOS 14)**

---

## **CI/CD – GitHub Actions (_.github/workflows/..._)**  
### **Purpose:**  
Automatically build and optionally test the project on **Windows**, **Linux**, and **macOS**.  

### **What Does the Pipeline Do?**  
- Automatically builds the project after each source code **push** and **pull request** to master branch.  
- Installs system dependencies and dependencies from `vcpkg.json`.  
- Compiles the project in **Release** and **Debig** mode.  
- (Optional) Runs tests if defined.
- In release publish mode packs files to release pack.

_Results can be found in the **Actions** tab on GitHub._

---

## Library documentation
The library is supplied with a doxygen configuration file with which you can generate HTML and man documentation.

#### Usage:
```bash
cd docs/doxygen/
doxygen ./doxyfile
```

If you need documentation in the form of a CHM file, please enable the GENERATE_HTMLHELP parameter and disable the HTML_DYNAMIC_MENUS parameter in the doxygen file and generate the documentation again. You will get a set of files ready to build a CHM help file.

---

## Installation & Build Instructions

### Option 1: Automated (Recommended)
The repository includes a Python script (`init_lib_project.py`) that automates both the dependency installation and the build process.

#### Usage:
```bash
python init_lib_project.py
```

This script allows you to:
- Install all required dependencies (including `wxWidgets` via `vcpkg`).
- Select the build configuration: **Release**, **Debug**, or **Both**.
- Choose the desired IDE or build system.
- Decide between static or shared library builds (Windows only).

> Wrapper scripts are also available for convenience:
> - **Windows:** `init_lib_project.bat`
> - **Linux/macOS:** `init_lib_project.sh`

### Option 2: Manual Setup

#### 1. Clone the Repository
```bash
git clone https://github.com/your_username/bwx_sdk.git
cd bwx_sdk
```

#### 2. Install Dependencies
Manually install dependencies using `vcpkg`:
```bash
vcpkg install wxwidgets:x64-windows   # For Windows
vcpkg install wxwidgets:x64-linux     # For Linux
vcpkg install wxwidgets:x64-osx       # For macOS
```
> Ensure that the environment variable `VCPKG_ROOT` is set and `vcpkg` is accessible from the command line.

#### 3. Build with CMake
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build . --config Release
```

---

## Directory Structure
```
[bwx_sdk]
    ├── .github/              # GitHub workflows (CI/CD)
    ├── docs/                 # Library documentation
    │   └── doxygen/          # Doxygen configuration
    ├── examples/             # Example applications demonstrating usage
    ├── include/              # Public header files
    │   └── bwx_sdk/
    │       ├── bwx_globals.h # Common BWX SDK globals
    │       └── [specific libraries folders]/
    ├── locale/               # Multilanguage library files
    ├── scripts/              # Helper scripts
    ├── src/                  # Source files
    ├── vcpkg.json            # Dependency manifest
    ├── CMakeLists.txt        # Project-wide CMake configuration
    ├── init_lib_project.py   # Python script for automated setup and build
    ├── init_lib_project.bat  # Windows wrapper script
    ├── init_lib_project.sh   # Unix-like systems wrapper script
    ├── LICENSE.md            # License information
    └── README.md             # Project documentation (this file)
```

---

## Usage Example
All available example applications can be found in the _**examples**_ folder.

Below is a minimal sample application demonstrating how to use **BWX_SDK**:

```cpp
#include <wx/wx.h>
#include <bwx_sdk/bwx_core/bwx_core.h>

class MyApp : public wxApp {
public:
    bool OnInit() override {
        auto* frame = new wxFrame(nullptr, wxID_ANY, "Example BWX_SDK Application", wxDefaultPosition, wxSize(400, 300));

        auto* panel = new wxPanel(frame);
        auto* sizer = new wxBoxSizer(wxVERTICAL);

        auto* btn = new wxButton(panel, wxID_ANY, "Test BWX_SDK function");
        btn->Bind(wxEVT_BUTTON, [](wxCommandEvent&) { bwx_sdk::bwxStdPathsInfo(); });

        sizer->Add(btn, 0, wxALL | wxEXPAND, 10);
        panel->SetSizer(sizer);

        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
```

---

## Contributing
Contributions are welcome! Feel free to open issues or submit pull requests.  
For significant changes, please open an issue to discuss the proposed modifications beforehand.

---

## License
This project is licensed under the [wxWidgets License](https://www.wxwidgets.org/about/licence/).

Information about third-party components can be found directly in the source code of the library or, if required by the licence, in the relevant project files.

---

## Contact
Developed and maintained by **Bartosz Warzocha**.

For any ideas and suggestions, inquiries or support, please open an issue on the [GitHub repository](https://github.com/bartoszwarzocha/bwx_sdk/issues).

---
© 2025 Bartosz Warzocha (bartosz.warzocha@gmail.com)
