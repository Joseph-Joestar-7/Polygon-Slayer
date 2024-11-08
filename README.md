# Polygon Slayer

Polygon Slayer is a 2D game built in C++ using the SFML (Simple and Fast Multimedia Library). The game involves slaying enemy polygons and is designed with performance and gameplay in mind.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Build Instructions](#build-instructions)
  - [Windows](#windows)
  - [Mac/Linux](#maclinux)
- [Gameplay](#gameplay)
- [Contributing](#contributing)
- [License](#license)

## Features
- 2D gameplay using SFML
- Dynamic movement and combat
- Basic enemy AI and physics
- Configurable game settings

## Installation

### Prerequisites
- **SFML** (Simple and Fast Multimedia Library): Make sure SFML is set up in your development environment.
  - [Download SFML](https://www.sfml-dev.org/download.php)
- **Visual Studio** (for Windows users): Download Visual Studio with C++ desktop development support.

## Build Instructions

1. **Clone the Repository**:
   Open a terminal (Command Prompt or Git Bash) and run:
   ```bash
   git clone https://github.com/Joseph-Joestar-7/Polygon-Slayer.git
   cd Polygon-Slayer
2. **Open the Project**: Open **Visual Studio** and load the solution file `Polygon Slayer.sln` from the root of the repository.

3. **Configure SFML**: The project already contains `include/SFML` and `lib` folders. Ensure that the project is correctly configured to use these directories:
    - Right-click the project in Solution Explorer and select **Properties**.
    - Under **VC++ Directories**, set the **Include Directories** to `include/SFML`.
    - Set the **Library Directories** to `lib`.

4. **Link SFML Libraries**: In the **Linker** settings, under **Input**, ensure the following SFML libraries are linked:
    - `sfml-graphics.lib`
    - `sfml-window.lib`
    - `sfml-system.lib`

5. **Build the Project**: Click **Build** > **Build Solution** (or press `Ctrl+Shift+B`) to compile the project.

6. **Run the Project**: After building successfully, you can run the project by clicking **Debug** > **Start Debugging** (or pressing `F5`).

## Gameplay

- **Controls**: W,A,S,D to move around. Left Mouse Button for primary fire and Right Mouse for alt fire
- **Objective**: Slay all enemy polygons and survive as long as possible.
- **Features**: *TODO*

## Contributing

Contributions are welcome! Feel free to fork this repository, make improvements, and open a pull request. Please follow the guidelines for contributing and ensure that your code is clean and properly documented.
