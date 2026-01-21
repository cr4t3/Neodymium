![Static Badge](https://img.shields.io/badge/17-1?style=for-the-badge&label=C%2B%2B)


# Neodymium Virtual Machine 

An open-source virtualizer with custom and modular architectures.

## Description

Neodymium VM is a virtualizer, It's uses range from virtualization to simple program making.

## Notice

This release is considered to be an alpha release, expect changes and report issues on [GitHub Issue](https://github.com/cr4t3/Neodymium/issues).

## Getting Started

### Dependencies

* C++17 or higher.
* CMake 3.31.6 or higher.
* Any compiler that supports C++17 standard.
* A UNIX-based OS. *(Windows is currently unsupported for use and compile)*

#### Neo8 dependencies
* GLFW 3.4-3 or higher

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

3. Create the cache and make the binary with the respective architecture
```bash
cmake .. -DARCH=[arch]
cmake --build .
```

4. Verify if Neodymium is installed in it's newest version.
```bash
neodymium-{arch} --version
```

### Executing program

1. Using a compiled bin file, you can execute
```bash
neodymium-[arch] file.bin
```

## Minimal Requirements

* 1 CPU core
* 1 MB storage.

## Known Issues

None yet

## Authors

[@cr4t3](github.com/cr4t3) - Crate

## Version History

Read the version history at the [Releases](https://github.com/cr4t3/Neodymium/releases) tab.

## License

Copyright (C) 2026 Crate

This project is licensed under the GNU GPLv3 License - see the LICENSE file for details

## Acknowledgments

* [Quake 3 Arena](https://store.steampowered.com/app/2200/Quake_III_Arena/)