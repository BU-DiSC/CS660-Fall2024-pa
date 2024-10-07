# Assignments

- [Introduction](docs/README.md)
- [Assignment 0](docs/pa0.md)
- [Assignment 1](docs/pa1.md)
- [Assignment 2](docs/pa2.md)

# Development Environment Setup

We are grading your solutions on Linux, so we suggest that you implement the assignments on Linux.
That being said you should be able to use any OS of your choice if you follow these instructions.  
(Note: If you use Windows, you need to install [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).)

## Setting up the environment

### Linux or WSL

Run the following commands:

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y g++ build-essential gdb make cmake valgrind
```

### MacOS

Clang may already be installed on your Mac. To verify that it is, open a macOS Terminal window and enter the following
command:

```zsh
clang --version
```

If Clang isn't installed, enter the following command to install the command line developer tools:

```zsh
xcode-select --install
```

## Setting up an IDE

### CLion

We highly recommend using the [CLion](https://www.jetbrains.com/clion/) IDE as it comes with the tools you need
to complete your assignment. You will need a **free** license for CLion, which you can get by following the
instructions [here](https://www.jetbrains.com/shop/eform/students).

### VSCode IDE

[VS Code](https://code.visualstudio.com/) needs the following two extensions to be configured properly.

- [C/C++ for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

## Deploying the Code

### Cloning the repository

If you are using an IDE, you can clone the repository directly using the IDE. When prompted for the link to the remote
repo, **use this repository's link from github**.

- [Instructions for VS Code](https://code.visualstudio.com/docs/sourcecontrol/intro-to-git#:~:text=To%20clone%20a%20repository%2C%20run,to%20clone%20to%20your%20machine.)
- [Instructions for CLion](https://www.jetbrains.com/help/clion/set-up-a-git-repository.html#clone-repo)

- If you are using the terminal, you can clone the repo (in any directory of your choice) using:

```bash
git clone <repo_url>
```

## Building the project

### CLion

Right-click on the project tree and choose "Load CMake Project". More instructions can be
found [here](https://www.jetbrains.com/help/clion/reloading-project.html).

[Running tests with CLion](https://www.jetbrains.com/help/clion/ctest-support.html#run-ctest)  
(Note: Make sure the All CTest configuration is selected in the top right corner of the IDE)

### VSCode

Click on the CMake tools extension on the extension pane (by default on the right side of the IDE) and click on the
build icon. More instructions and tutorials can be
found [here](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools).

[Running tests with VSCode](https://learn.microsoft.com/en-us/visualstudio/test/how-to-use-ctest-for-cpp?view=vs-2022)

### Terminal

Navigate to the directory where you cloned the repo
```sh
cd CS660-Fall2024-pa
```

Create a build directory and navigate to it
```sh
mkdir build
cd build
```

Run cmake to generate the build files
```sh
cmake ..
```

Build the project
```sh
make
```

Run the tests
```sh
ctest
```
