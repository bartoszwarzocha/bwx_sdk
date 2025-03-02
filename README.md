# BWX\_SDK Library

## Table of Contents

- [Overview](#overview)
- [Features](#features)
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

## Overview

**BWX\_SDK** is a C++ library designed to extend and enhance the functionality of [wxWidgets](https://www.wxwidgets.org/). It provides a set of additional classes and utilities that simplify the usage of wxWidgets, making development more efficient and user-friendly.

The library is cross-platform, supporting **Windows**, **Linux**, and **macOS**, and offers both static and shared builds. BWX\_SDK is distributed under the **wxWidgets license**.

&#x20;  &#x20;

#### From author...

*The library is a collection of solutions I have developed in over 25 years of programming work with the *[*wxWidgets*](https://www.wxwidgets.org/)* library. It addresses a variety of programming problems for various domains, often going beyond the implementation of the GUI itself, but with a focus on maintaining consistency with *[*wxWidgets*](https://www.wxwidgets.org/)*. The current GitHub version is still under construction (it's a long way to first release) and comes from my personal collection and is constantly being added to, while the code is updated and rewritten for modern C++. ****I hope you find this stuff useful and are inspired to discover more uses for the fantastic ***[***wxWidgets***](https://www.wxwidgets.org/)*** library.***

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

## **CI/CD – GitHub Actions (*****.github/workflows/...*****)**

### **Purpose:**

Automatically build and optionally test the project on **Windows**, **Linux**, and **macOS**.

### **What Does the Pipeline Do?**

- Automatically builds the project after each source code **push** and **pull request** to master branch.
- Installs system dependencies and dependencies from `vcpkg.json`.
- Compiles the project in **Release** and **Debug** mode.
- (Optional) Runs tests if defined.
- In release publish mode packs files to release pack.

*Results can be found in the ****Actions**** tab on GitHub.*

---

## Library documentation

The library is supplied with a doxygen configuration file with which you can generate HTML and man documentation.

#### Usage:

```bash
cd docs/doxygen/
doxygen ./doxyfile
```

If you need documentation in the form of a CHM file, please enable the GENERATE\_HTMLHELP parameter and disable the HTML\_DYNAMIC\_MENUS parameter in the doxygen file and generate the documentation again. You will get a set of files ready to build a CHM help file.

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

---

## Contributing

Contributions are welcome!

---

## License

Licensed under the [wxWidgets License](https://www.wxwidgets.org/about/licence/).

---

## Contact

Developed by **Bartosz Warzocha**.

