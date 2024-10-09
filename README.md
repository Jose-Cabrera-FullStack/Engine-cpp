# Project Installation Guide for Windows 11 with Visual Studio Code

Welcome to the project! This guide is designed to help you set up the project on a Windows 11 system using Visual Studio Code (VS Code) as smoothly as possible. Follow these steps to get started.

## Running the Project

To run the project, you need to compile the source code using a C++ compiler and the SDL2 library. You can compile the project using the following command:

**Build and Run**: Open the main.cpp file in the src folder, and press `Ctrl+Shift+B` to build the project. You can then run the compiled executable from the terminal.

```bash
mkdir build
cd build
cmake ..
make
./gameengine
```

## Prerequisites

Before you begin, ensure you have the following installed on your system:
- **Visual Studio Code**: Download and install from [https://code.visualstudio.com/](https://code.visualstudio.com/)
- **Git**: Download and install from [https://git-scm.com/download/win](https://git-scm.com/download/win)
- **SDL2**: Download the development libraries for SDL2 from [https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php). Choose the `SDL2-devel-2.x.x-VC.zip` (Visual C++ 32/64-bit) package.

## Setting Up SDL2

1. **Extract SDL2**: Unzip the SDL2 development library package to a known location on your system, e.g., `C:\libs\SDL2`.
2. **Configure Environment Variable**:
   - Right-click on `This PC` and select `Properties`.
   - Click on `Advanced system settings` and then `Environment Variables`.
   - Under `System Variables`, find `Path`, and click `Edit`.
   - Add a new entry pointing to the `lib\x64` directory inside your SDL2 folder, e.g., `C:\libs\SDL2\lib\x64`.
   - Click `OK` to close all dialogues.

### Install dependencies using

To install the dependencies, run the following command in the in MSYS2 terminal:

```bash
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-lua
```

Note: You may have to configure the IDE to use the correct compiler and linker flags (MSYS2).

## Setting Up the Project in VS Code

1. **Clone the Repository**: Open VS Code, launch the Terminal (`Ctrl+``), and clone the project repository by running:
   ```bash
   git clone <repository-url>
   ```
2. **Open the Project**: In VS Code, go to File > Open Folder and select the cloned project directory.

3. **Install C/C++ Extension**: Install the Microsoft C/C++ extension for VS Code from the marketplace if you haven't already.

4. **Configure the Project**:

    - Create a new file named c_cpp_properties.json inside the .vscode folder at the root of your project.
    - Add the following configuration, adjusting paths as necessary:
    ```json
    {
        "configurations": [
            {
                "name": "Win32",
                "includePath": [
                    "${workspaceFolder}/**",
                    "C:/libs/SDL2/include"
                ],
                "defines": [
                    "_DEBUG",
                    "UNICODE",
                    "_UNICODE"
                ],
                "compilerPath": "C:/MinGW/bin/g++.exe",
                "cStandard": "c17",
                "cppStandard": "c++17",
                "intelliSenseMode": "gcc-x64"
            }
        ],
        "version": 4
    }
    ```
    - Replace "compilerPath" with the path to your compiler executable, e.g., the path to cl.exe for MSVC.


## Troubleshooting

- **SDL2 Not Found**: If you encounter an error about SDL2 not being found, ensure that the SDL2 path is correctly added to the system PATH variable.
- **Compiler Errors**: If you encounter errors related to the compiler, ensure that the compiler path is correctly set in the c_cpp_properties.json file.
