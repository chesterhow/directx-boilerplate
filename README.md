# DirectX Boilerplate
This is a starter boilerplate app for DirectX 2D games.

![Game Preview](http://i.imgur.com/u5rBTA5.gif)

## Pre-requisites
You'll need to have DirectX 9 and Visual Studio installed in order to run the demo. Also, as this is a Windows app, it'll only work on Windows.

## Running the app
1. Clone the repo - `git clone https://github.com/chesterhow/directx-boilerplate.git`
1. Open and run in Visual Studio

## Usage
I highly recommend creating a **new project** of your own instead of renaming the files in this repo.

Lets get started.

### Creating the Project

1. Create a new project in Visual Studio
1. Navigate to **Project** > **Properties**

#### "All Configuration" Settings
1. Switch **Configuration** to **All Configurations**

1. On the left, navigate to **Configuration Properties** > **VC++ Directories**
1. On the right, select **Include Directories**, open the dropdown and click **Edit**
1. Enter `$(DXSDK_DIR)\Include` and click **OK**
1. Still on the right, select **Library Directories**, open the dropdown and click **Edit**
1. Enter `$(DXSDK_DIR)\Lib\x86` and click **OK**
1. On the left, navigate to **Configuration Properties** > **Linker** > **Input**
1. On the right, select **Additional Dependencies**, open the dropdown and click **Edit**
1. Enter the following, each on a new line and click **OK**
```
d3d9.lib
d3dx9.lib
winmm.lib
xinput.lib
```
1. Click **Apply**

#### "Release" Settings
1. Switch **Configuration** to **Release**
1. On the left, navigate to **Configuration Properties** > **C/C++** > **Code Generation**
1. On the right, select **Runtime Library**, open the dropdown and select **Multi-threaded (/MT)**
1. Click **OK**

### Adding files to the Project
1. Copy the `sprites` and `src` directories from this repo into your project directory
1. Navigate to **Project** > **Add Existing Item**
1. Add all files from the `src` directory

### Modifying the Project
Make the following changes in the respective files

#### `constants.h`
1. Replace `CLASS_NAME` and `GAME_TITLE` with your desired names

#### `samplegame.h` & `samplegame.cpp`
1. Rename the file to the name of your game
1. Replace all occurances of **samplegame** with the name of your game

#### `winmain.cpp`
1. Replace all occurances of **samplegame** with the name of your game

### Ready to Go
And with that, you now have a working DirectX 2D app. The rest is on you :)

## Contributing
Feel free to submit an **issue** if you encounter any bugs, or create a **pull request** if you would like to contribute.

## License
See [LICENSE](https://github.com/chesterhow/directx-boilerplate/blob/master/LICENSE)
