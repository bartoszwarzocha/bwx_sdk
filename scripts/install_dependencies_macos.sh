#!/bin/bash

set -e

VCPKG_DIR="vcpkg"
VCPKG_TRIPLET="x64-osx"

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

# Function to install system dependencies using Homebrew
install_system_dependencies() {
  echo "Installing system dependencies with Homebrew..."

  brew update

  # Basic development tools
  brew install \
    cmake ninja autoconf automake libtool pkg-config gettext bison flex gfortran nasm meson

  # Font and graphics libraries
  brew install \
    fontconfig freetype glib gtk+3

  # X11 libraries
  brew install \
    xquartz libpng zlib dbus atk

  # GObject introspection and Python bindings
  brew install \
    gobject-introspection pygobject3

  # Graphics rendering libraries
  brew install \
    cairo pango gdk-pixbuf

  # X11 additional libraries
  brew install \
    libxinerama libxrandr libxcomposite libxdamage libxcursor libxfixes

  # Epoxy library
  brew install \
    libepoxy

  # Input support
  brew install \
    libint

  # Audio support libraries
  brew install \
    pulseaudio libxft libxkbcommon

  # OpenGL and EGL support
  brew install \
    mesa libusb
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
