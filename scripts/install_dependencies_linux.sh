#!/bin/bash

set -e

VCPKG_DIR="vcpkg"
VCPKG_TRIPLET="x64-linux"

# Function to prompt the user for confirmation
confirm() {
  while true; do
    read -rp "$1 [y/n]: " yn
    case $yn in
      [Yy]*) return 0 ;;
      [Nn]*) return 1 ;;
      *) echo "Please answer y (yes) or n (no)." ;;
    esac
  done
}

# Function to install system dependencies
install_system_dependencies() {
  echo "Installing system dependencies..."
  
  export CFLAGS="-D_strnicmp=strncasecmp -I/usr/include/audio -I/opt/vc/include $CFLAGS"
  export CXXFLAGS="-D_strnicmp=strncasecmp -I/usr/include/audio -I/opt/vc/include $CXXFLAGS"

  # Check if libraspberrypi-dev is available
  if apt-cache show libraspberrypi-dev > /dev/null 2>&1; then
    LIBRPI_PACKAGE="libraspberrypi-dev"
  else
    echo "Package libraspberrypi-dev not available, skipping."
    LIBRPI_PACKAGE=""
  fi

  sudo apt-get update
  sudo apt-get install -y \
    build-essential cmake ninja-build g++ autoconf automake libtool pkg-config libtool-bin m4 gettext bison flex gfortran python3-pip \
    nasm meson libfontconfig1-dev libfreetype6-dev libglib2.0-dev libgtk-3-dev \
    libx11-dev libxext-dev libxrender-dev libpng-dev zlib1g-dev \
    libdbus-1-dev libatk1.0-dev libxi-dev libxtst-dev \
    gobject-introspection python3-gi \
    libcairo2-dev libpango1.0-dev libgdk-pixbuf2.0-dev \
    libxinerama-dev libxrandr-dev libxcomposite-dev libxdamage-dev \
    libxcursor-dev libxfixes-dev libxss-dev libepoxy-dev \
    libwayland-dev libinput-dev \
    libasound2-dev libpulse-dev libxft-dev libxkbcommon-dev \
    libegl1-mesa-dev libibus-1.0-dev mesa-common-dev libgl1-mesa-dev \
    libxxf86vm-dev libaudio-dev $LIBRPI_PACKAGE
}

# Function to set up vcpkg
setup_vcpkg() {
  echo "Setting up vcpkg..."
  
  if [ ! -d "$VCPKG_DIR" ]; then
    git clone https://github.com/microsoft/vcpkg.git "$VCPKG_DIR"
    cd "$VCPKG_DIR"
    ./bootstrap-vcpkg.sh
    cd ..
  else
    echo "vcpkg directory already exists, skipping clone."
  fi
}

# Function to install vcpkg dependencies
install_vcpkg_dependencies() {
  echo "Installing vcpkg dependencies..."
  
  for attempt in {1..3}; do
    echo "Attempt $attempt to install vcpkg dependencies..."
    "./$VCPKG_DIR/vcpkg" install --triplet "$VCPKG_TRIPLET" && break || {
      echo "vcpkg install failed. Cleaning buildtrees and retrying in 10 seconds..."
      rm -rf "./$VCPKG_DIR/buildtrees"
      sleep 10
    }
  done
}

# Prompt and run installations
if confirm "Do you want to install system dependencies?"; then
  install_system_dependencies
else
  echo "Skipping system dependencies installation."
fi

if confirm "Do you want to set up vcpkg?"; then
  setup_vcpkg
else
  echo "Skipping vcpkg setup."
fi

if confirm "Do you want to install vcpkg dependencies?"; then
  install_vcpkg_dependencies
else
  echo "Skipping vcpkg dependencies installation."
fi

echo "Script execution completed."
