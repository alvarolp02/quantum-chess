#!/bin/bash


packages=("cmake" "libsfml-dev" "libyaml-cpp-dev" "libeigen3-dev" "libomp-dev")

for pkg in "${packages[@]}"; do
    if ! dpkg -s "$pkg" &> /dev/null; then
        echo "$pkg is not installed. Installing..."

        sudo apt install "$pkg"

        # Confirm installation
        if dpkg -s "$pkg" &> /dev/null; then
            echo "$pkg successfully installed."
        else
            echo "Failed to install $pkg."
            exit 1
        fi
    fi
done



if [ ! -d "build" ]; then
  	mkdir build
fi
cd build
cmake ..
make

