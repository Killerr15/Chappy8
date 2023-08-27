# Chippy8

This is a really basic CHIP-8 emulator made in C++. Chip-8 is an interpreted programming language that was developed by Joseph Weisbecker in the 1970s. It was designed to make it easier to program video games for 8-bit microcomputers, such as the COSMAC VIP and the Telmac 1800. Chip-8 programs run on a virtual machine that simulates the hardware of these computers.

Now working on both Windows and Linux!

## Building

CMake and SDL2 are required:

Windows:
* Install CMake and download SDL2.
* Add environment variable called CMAKE_PREFIX_PATH and point it to your SDL2 folder.
```
cmake -S . -B ./build
cmake --build ./build --config Release
```

Linux:
* Install CMake and SDL2:
```
$ sudo apt-get install cmake libsdl2-dev
```
* Build the project:
```
cmake -S . -B ./build
cd build
make
```

## Usage

Windows:
```
.\Chappy8.exe <path_to_rom_file>
```

Linux:
```
./Chappy8 <path_to_rom_file>
```

## Key Mapping

| CHIP-8 Keypad | Keyboard |
|:-------------:|:--------:|
|       1       |     1    |
|       2       |     2    |
|       3       |     3    |
|       4       |     Q    |
|       5       |     W    |
|       6       |     E    |
|       7       |     A    |
|       8       |     S    |
|       9       |     D    |
|       0       |     X    |
|       A       |     Z    |
|       B       |     C    |
|       C       |     4    |
|       D       |     R    |
|       E       |     F    |
|       F       |     V    |

## Credits

* Tobias V. Langhoff and Austin Morlan for their great guides on making CHIP-8 emulators.
* @sarbajitsaha for making his CHIP-8 emulator. It was a really good reference for me, I looked at his code whenever I couldn't figure out how to implement something.