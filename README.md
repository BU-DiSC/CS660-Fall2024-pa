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

# Deploying the Code
## Cloning the repository
If you are using an IDE, you can clone the repository directly using the IDE. When prompted for the link to the remote repo, **use this repository's link from github**.
- [Instructions for VS Code](https://code.visualstudio.com/docs/sourcecontrol/intro-to-git#:~:text=To%20clone%20a%20repository%2C%20run,to%20clone%20to%20your%20machine.)
- [Instructions for CLion](https://www.jetbrains.com/help/clion/set-up-a-git-repository.html#clone-repo)

- If you are using the terminal, you can clone the repo (in any directory of your choice) using: 
```bash
git clone <repo_url>
```

## Building the project

### VSCode
Click on the CMake tools extension on the extension pane (by default on the right side of the IDE) and click on the build icon. More instructions and tutorials can be found [here](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools).

### CLion
Right click on the project tree and choose "Load CMake Project". More instructions can be found [here](https://www.jetbrains.com/help/clion/reloading-project.html).

### Terminal

#### Navigate to your repo 
Make sure you are in the directory where you cloned the repo in the previous step:
```sh
cd CS660-Fall2024-pa
```

#### Build 
Use the following commands: 
```sh 
mkdir build 
cd build 
cmake ..
make
```
The following assumes you are inside the project folder. We are essentially creating a build/ folder and then building the project using cmake. 
From the next time you want to build the project, you don't need to create the build folder:
```sh 
cd build
cmake ..
make
```

#### Compile the code
Compile the code using the `make` command on the terminal. 

## Running Tests
- [Instructions for VSCode](https://learn.microsoft.com/en-us/visualstudio/test/how-to-use-ctest-for-cpp?view=vs-2022)

- [Instructions for CLion](https://www.jetbrains.com/help/clion/ctest-support.html#run-ctest)

- In the terminal, you can run the tests using the `ctest` command (when you are in the build/ folder).
