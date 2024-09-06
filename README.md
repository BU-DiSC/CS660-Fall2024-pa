# Assignments

- [Introduction](docs/README.md)
- [Assignment 0](docs/pa0.md)

# Development Environment Setup

We are grading your solutions on Linux, so we suggest that you implement the assignments on Linux.
That being said you should be able to use any OS of your choice if you follow these instructions.

## CLion Instructions

We highly recommend using an IDE such as [CLion](https://www.jetbrains.com/clion/) as it comes with the tools you need
to complete your assignment. You will need a **free** license for CLion, which you can get by following the
instructions [here](https://www.jetbrains.com/shop/eform/students).

### Windows

Windows are supported through [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).

### Linux

No need for any steps further.

### MacOS

No need for any steps further.

## VS Code Instructions

As an alternative, you can use a source code editor such as [VS Code](https://code.visualstudio.com/). This tool needs a
few more steps to configure it properly.

Install the following extensions:

- [C/C++ for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

### Windows

Windows are supported through [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).
After installing WSL you can follow the [Linux](#linux-1) instructions.

### Linux

Run the following commands:

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y gcc g++ build-essential gdb make cmake valgrind
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
