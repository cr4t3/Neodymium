![Static Badge](https://img.shields.io/badge/17-1?style=for-the-badge&label=C%2B%2B)


# Neodymium Virtual Machine 

An open-source 8-bit virtual machine.

## Description

Neodymium VM is a 8-bit virtual machine, which simulates a CPU, RAM (up to 64KB) and in-RAM stack. It's uses range from virtualization to simple program making.

## Getting Started

### Dependencies

* C++17 or higher.
* CMake 3.31.6 or higher.
* GLFW 3.4 or higher.
* Any compiler that supports C++17 standard.
* A UNIX-based OS. *(Windows is currently unsupported for use and compile)*

### Installing

1. Clone the git repo and move to the folder
```bash
git clone https://www.github.com/cr4t3/Neodymium.git 
cd Neodymium
```

2. Create a build folder and move there
```bash
mkdir build
cd build
```

3. Create the cache and make the binary
```bash
cmake ..
cmake --build .
```

1. Verify if Neodymium is installed in it's newest version.
```bash
neodymium --version
```

### Executing program

1. Using a compiled bin file, you can execute
```bash
neodymium file.bin
```

## Roadmap
* [x] ~~Add a virtual screen~~
* [ ] Make a C++ assembler
* [ ] Add unit testing
* [ ] Add example bins
* [ ] Add a virtual keyboard
* [ ] Add a virtual ROM (file-loadble)
* [ ] Add a virtual HDD (static file)
* [x] ~~Add more instructions~~

## Minimal Requirements

* 128 KB of RAM memory
* 1 CPU core
* 8 MB storage.

<!--## Known Issues

Nothin here yet

## Help

Neither here -->

## Authors

[@cr4t3](github.com/cr4t3) - Crate

## Version History

Read the version history at the [Releases](https://github.com/cr4t3/Neodymium/releases) tab.

## License

Copyright (C) 2026 Crate

This project is licensed under the GNU GPLv3 License - see the LICENSE file for details

## Acknowledgments

* [Quake 3 Arena](https://store.steampowered.com/app/2200/Quake_III_Arena/)
