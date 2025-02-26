# BWX_SDK Library

## Overview
**BWX_SDK** is a C++ library designed to extend and enhance the functionality of [wxWidgets](https://www.wxwidgets.org/). It provides a set of additional classes and utilities that simplify the usage of wxWidgets, making development more efficient and user-friendly.

The library is cross-platform, supporting **Windows**, **Linux**, and **macOS**, and offers both static and shared builds. BWX_SDK is distributed under the **wxWidgets license**.

[![LINUX](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-linux.yml/badge.svg)](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-linux.yml)
[![WINDOWS](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-windows.yml/badge.svg)](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-windows.yml)
[![MACOS](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-macos.yml/badge.svg)](https://github.com/bartoszwarzocha/bwx_sdk/actions/workflows/ci-macos.yml)

---

## Features
- Extended classes for core wxWidgets components.
- Utilities for simplifying common tasks with wxWidgets.
- Cross-platform compatibility (Windows, Linux, macOS).
- Support for both static and shared library builds.
- Examples demonstrating library usage.

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

---

## Library documentation
The library is supplied with a doxygen configuration file with which you can generate HTML and man documentation.
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

---

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
├── .github/            # GitHub workflows (CI/CD)
├── docs/               # Library documentation
│   └── doxygen/        # Doxygen configuration
├── examples/           # Example applications demonstrating usage
│   └── example_app/
│       ├── CMakeLists.txt
│       └── main.cpp    # Example application source code
├── include/            # Public header files
│   └── bwx_sdk/
│       ├── bwx_globals.h
│       ├── [specific libraries folders]/
├── scripts/            # Helper scripts
├── src/                # Source files
├── vcpkg.json          # Dependency manifest
├── CMakeLists.txt      # Project-wide CMake configuration
├── init_lib_project.py # Python script for automated setup and build
├── init_lib_project.bat# Windows wrapper script
├── init_lib_project.sh # Unix-like systems wrapper script
├── LICENSE.md          # License information
└── README.md           # Project documentation (this file)
```

---

## Usage Example
Below is a sample application demonstrating how to use **BWX_SDK**:

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

---

## Contact
Developed and maintained by **Bartosz Warzocha**.

For any inquiries or support, please open an issue on the [GitHub repository](https://github.com/bartoszwarzocha/bwx_sdk/issues).

