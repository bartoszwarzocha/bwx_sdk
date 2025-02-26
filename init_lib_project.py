import argparse
import json
import os
import platform
import shutil
import subprocess
import sys
from colorama import init, Fore, Style

# === INITIALIZATION ===
init(autoreset=True)
CYAN = '\033[96m'  # Bright cyan color
GREEN = '\033[92m'  # Bright green color for success messages
RESET = '\033[0m'   # Reset color

# === CONFIGURATION ===
VCPKG_ROOT = os.getenv("VCPKG_ROOT", "D:/vcpkg" if platform.system() == "Windows" else "~/tools/vcpkg")
DEFAULT_TRIPLETS = {
    "Windows": "x64-windows",
    "Linux": "x64-linux",
    "Darwin": "x64-osx"
}
BUILD_DIR = "build"

IDE_OPTIONS = {
    "Windows": [
        "Auto-detect Visual Studio",
        "Visual Studio 17 2022",
        "Visual Studio 16 2019",
        "Ninja",
        "None (No IDE project)"
    ],
    "Linux": [
        "None (No IDE project)",
        "Code::Blocks",
        "CLion",
        "VSCode",
        "QtCreator"
    ],
    "Darwin": [
        "Xcode",
        "CLion",
        "VSCode",
        "None (No IDE project)"
    ]
}

def run_command(command, cwd=None):
    """Run a shell command and handle errors."""
    print(f"Running: {' '.join(command)}")
    result = subprocess.run(command, cwd=cwd, shell=platform.system() == "Windows")
    if result.returncode != 0:
        print(f"{Fore.RED}Command failed: {' '.join(command)}{RESET}")
        sys.exit(result.returncode)

def get_user_input(prompt, options, default="1"):
    """Get user input with validation."""
    print(f"{CYAN}-" * 60)
    print(f"{CYAN}{prompt} (default: {options[int(default)-1]}){RESET}")
    for i, option in enumerate(options, 1):
        print(f"{CYAN}{i}. {option}{RESET}")

    while True:
        value = input(f"{CYAN}Select an option [{default}]: {RESET}").strip()
        if not value:
            value = default
        if value.isdigit() and 1 <= int(value) <= len(options):
            selected_option = options[int(value) - 1]
            print(f"Selected: {CYAN}{selected_option}{RESET}")
            return selected_option
        print(f"{Fore.RED}Invalid input. Please select a valid option.{RESET}")

def get_yes_no_input(prompt, default="No"):
    """Prompt for Yes/No input with validation."""
    options = ["Yes", "No"]
    default_value = default.capitalize()

    while True:
        value = input(f"{CYAN}{prompt} (Yes/No) [default: {default_value}]: {RESET}").strip().capitalize()
        if not value:
            value = default_value
        if value in options:
            print(f"Selected: {CYAN}{value}{RESET}")
            return value == "Yes"
        print(f"{Fore.RED}Invalid input. Please type 'Yes' or 'No'.{RESET}")

def detect_triplet():
    """Detect platform and select appropriate vcpkg triplet."""
    system = platform.system()
    triplet = DEFAULT_TRIPLETS.get(system, "x64-linux")
    print(f"{CYAN}Detected platform: {system} -> Using triplet: {triplet}{RESET}")
    return triplet, system

def detect_visual_studio():
    """Automatically detect installed Visual Studio versions."""
    vs_versions = {
        "17": "Visual Studio 17 2022",
        "16": "Visual Studio 16 2019",
    }
    print(f"{CYAN}Detecting installed Visual Studio versions...{RESET}")
    for version, name in vs_versions.items():
        vs_dir = os.path.join(os.environ.get("ProgramFiles(x86)", "C:/Program Files (x86)"), "Microsoft Visual Studio", version)
        if os.path.exists(vs_dir):
            print(f"{CYAN}Detected: {name}{RESET}")
            return name
    print(f"{Fore.YELLOW}No supported Visual Studio versions found. Defaulting to Visual Studio 17 2022.{RESET}")
    return "Visual Studio 17 2022"

def get_generator(system):
    """Determine the CMake generator based on the operating system and user choice."""
    ide_options = IDE_OPTIONS.get(system, ["None (No IDE project)"])
    ide_choice = get_user_input(f"Select target IDE for {system}:", ide_options, default="1")

    if system == "Windows":
        if ide_choice == "Auto-detect Visual Studio":
            return detect_visual_studio(), "-A x64"
        elif ide_choice.startswith("Visual Studio"):
            return ide_choice, "-A x64"
        elif ide_choice == "Ninja":
            return "Ninja", ""
        else:
            return "NMake Makefiles", ""

    elif system == "Linux":
        return ("Unix Makefiles" if ide_choice == "None (No IDE project)" else "Ninja"), ""

    elif system == "Darwin":
        return ("Xcode" if ide_choice == "Xcode" else "Ninja"), ""

    print(f"{Fore.RED}Unsupported platform: {system}{RESET}")
    sys.exit(1)

def validate_vcpkg_manifest():
    """Check if vcpkg.json exists and validate package name."""
    manifest_path = os.path.join(os.getcwd(), "vcpkg.json")
    if not os.path.isfile(manifest_path):
        print(f"{Fore.RED}Error: vcpkg.json not found in project directory.{RESET}")
        sys.exit(1)

    with open(manifest_path, 'r', encoding='utf-8') as f:
        data = json.load(f)
        package_name = data.get("name", "")
        if not package_name.islower() or not package_name.replace("-", "").isalnum():
            print(f"{Fore.RED}Invalid package name '{package_name}' in vcpkg.json.\n"
                  f"Package names must be lowercase, alphanumeric, and may include hyphens.{RESET}")
            sys.exit(1)
        print(f"{GREEN}vcpkg.json is valid. Package name: {package_name}{RESET}")

def configure_cmake(build_dir, config, triplet, use_global, skip_deps, generator, arch_flag, build_shared):
    """Configure the project with CMake."""
    print(f"Configuring CMake ({config}) with generator: {CYAN}{generator}{RESET}...")

    if os.path.exists(build_dir):
        print(f"Cleaning previous build directory: {build_dir}")
        shutil.rmtree(build_dir)
    os.makedirs(build_dir, exist_ok=True)

    vcpkg_installed_path = os.path.abspath(os.path.join("vcpkg_installed", triplet))

    cmake_command = [
        "cmake", "../..",
        f"-DCMAKE_TOOLCHAIN_FILE={os.path.join(VCPKG_ROOT, 'scripts', 'buildsystems', 'vcpkg.cmake')}",
        f"-DVCPKG_TARGET_TRIPLET={triplet}",
        f"-DCMAKE_BUILD_TYPE={config}",
        f"-DCMAKE_PREFIX_PATH={vcpkg_installed_path}",
        f"-DBUILD_SHARED_LIBS={'ON' if build_shared else 'OFF'}",
        "-DVCPKG_KEEP_BUILT_PACKAGES=ON",
        "-G", generator
    ]

    if arch_flag:
        cmake_command.extend(arch_flag.split())

    cmake_command.append("-DVCPKG_MANIFEST_MODE=OFF" if skip_deps else "-DVCPKG_MANIFEST_MODE=ON")

    if use_global:
        cmake_command.append(f"-DCMAKE_PREFIX_PATH={os.path.join(VCPKG_ROOT, 'installed', triplet)}")

    print(f"{CYAN}{' '.join(cmake_command)}{RESET}")
    run_command(cmake_command, cwd=build_dir)

def build_project(build_dir, config):
    """Build the project."""
    print(f"Building project ({config})...")
    result = subprocess.run(["cmake", "--build", ".", "--config", config], cwd=build_dir, shell=platform.system() == "Windows")

    if result.returncode != 0:
        print(f"{Fore.RED}Build failed. Please check the logs.{RESET}")
        sys.exit(result.returncode)

    print(f"{GREEN}Build successful! Executable and libraries are in: {build_dir}{RESET}")

def main():
    print("=" * 60)
    print(f"Starting BWX SDK project setup for {platform.system()}")
    print("=" * 60)

    validate_vcpkg_manifest()

    triplet, system = detect_triplet()
    config = get_user_input("Select build configuration:", ["Release", "Debug", "Both"], default="1")
    skip_deps = get_yes_no_input("Skip dependency installation?", default="No")
    use_global = get_yes_no_input("Use global vcpkg installation?", default="No")

    if system == "Windows":
        build_shared = get_yes_no_input("Build shared libraries?", default="No")
    else:
        build_shared = False
        print(f"{CYAN}Non-Windows platform detected. Defaulting to static libraries.{RESET}")

    generator, arch_flag = get_generator(system)
    configs = ["Debug", "Release"] if config == "Both" else [config]

    for build_config in configs:
        build_dir = os.path.join(BUILD_DIR, f"{triplet}-{build_config}{'-dll' if build_shared else ''}")
        configure_cmake(build_dir, build_config, triplet, use_global, skip_deps, generator, arch_flag, build_shared)
        build_project(build_dir, build_config)

if __name__ == "__main__":
    main()
    