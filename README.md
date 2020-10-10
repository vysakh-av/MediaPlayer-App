# CPPND: Mediaplayer Application based on Gstreamer [Console Based]

This project is done as part of the CPP ND final project.
This is a mediaplayer application based on gstreamer. Its actually a console based application.

This application works in 2 modes:
1. AUTO mode
2. MANUAL mode

In AUTO mode, app automatically plays a number of streams available in playlist.
In MANUAL mode, user can control playback like start/stop/pause and exit.

In manual mode, player app supports the following options:
1. User can enter the playback URL in console
2. Type the following commands in console to controlle playback
-   play   (Start the playback)
-   pause  (Pause the playback)
-   stop   (Stop  the playback)
-   quit   (Exit from app)

**NOTE: This application is built and tested only in Linux based systems.**

## Dependencies for Running Locally

- cmake >= 3.5

    All OSes: [click here for installation instructions](https://cmake.org/install/)

- make >= 4.1 (Linux, Mac), 3.81 (Windows)

    Linux: make is installed by default on most Linux distros

- Gstreamer >= 1.0.0

    [Clicke here for installation instructions](https://gstreamer.freedesktop.org/documentation/installing/on-linux.html?gi-language=c)

    Also install
    ```
    sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
    ```

    In case if we get any plugins missing error, install required video plugins.
    In Ubuntu, the following command will install it.
    ```
    sudo apt-get install ubuntu-restricted-extras
    ```

- Glib >= 2.0
    ```
    sudo apt-get install libgtk2.0-dev libglib2.0-dev
    ```

- gcc/g++ >= 5.4

    - Linux: gcc / g++ is installed by default on most Linux distros
    - Mac: same deal as make - (install Xcode command line tools)[https://developer.apple.com/xcode/features/]
    - Windows: recommend using (MinGW)[http://www.mingw.org/]


## Basic Build Instructions
1. Clone this repo.
2. Make a build directory in the top level directory: mkdir build && cd build
3. Compile: cmake .. && make
4. Run it: ./play

## Project Requirement
### Loops, Functions, I/O
- Used different controle structures in the entire application. In main.cpp, we could see different control structures for example.

### Object Oriented Programming
- Application mainly used object oriented programmin technique.
- It follows inheritance. Player abstract class and HTTPPlayer & FilePlayer has inherited Player class and implemented virtual functions. MediaPlayer class has a Player instance to play the media. And on fron, a Controller class to controll the application.

### Memory Management
- The project makes use of references in places. For eg:In inc/HTTPPlayer.h (Line #20) takes reference to string. Also in MediaPlayer.h, FilePlayer.h, Controller.h(Line #33) used reference.
- In MediaPlayer.h have  default copy and move constructors defined.
- This application used smart pointers. In Controller.h (Line #36), main.cpp (Line #48) are examples of smart pointers.

### Concurrency
- This application is built as multi threaded application.
- Used threads, mutex and conditional varaible in the application.
- main.cpp (Line #83,103,114) are examples of thread creation. Used mutex and condtional variables in MessageQueue.h, Controller.h
- Promise-future used to communicate with threads. Eg: main.cpp (Line #83)

### Misc
- Reads data from a file.
- Also  application is read playback URL from user input.

## Application Structure

Application consists of the following classes:

- Player abstract class
	- Abstract class define the basic functionalities for player as
	  virtual functions.

- HTTPPlayer
	- Inherit Player class and implements the functionality for play back http:// based URLs

- FilePlayer
    - Inherit Player class and implements the functionality for play back file:// based URLs

- MediaPlayer
	- Manages the Player object creation based on the URL type and started the playback.

- Controller
    - Controllers the application by managing the user input, controll the playback operations, and start/stop application main loop etc.

## How to Use

- This Application works in 2 modes
1. Auto mode : In this mode, application reads the media URLs from default Playlists.txt files and play each media one by one till the last url listed in the file. Each media stream played fo 10 seconds and swapped to next one.
2. Manual mode: In this mode, user need to enter the playback url in console first. Once user enetered the URL, user needs to type the following strings in the console,
- `play` -> It will start the playback
- `pause` -> Pause the playback
- `stop` -> Stop the playback
- `quit` -> Exit from the applciation.

NOTE: Just type any of the above string in `console` to do the required operation.

## Notes
- This is a project done to pratcically apply all the knowledge received from CPPNd. So the applciations is not fully stable application and its not a well stablized player applicaion.
- Cosideration is trying to apply all the lessons learned in the project.

## Execution & Testing

**This application built and tested in only linux system. Tested only in Ubuntu based system (Ubuntu 16.04 & 14.04)**

**Example Streams tested**
```
https://www.radiantmediaplayer.com/media/big-buck-bunny-360p.mp4
https://bitdash-a.akamaihd.net/content/MI201109210084_1/m3u8s/f08e80da-bf1d-4e3d-8899-f0f6155f6efa.m3u8
https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm
https://bitdash-a.akamaihd.net/content/sintel/hls/playlist.m3u8
https://1eobz5yd.skychnl.net/1EOBZ5YD/6YDPSCHKRKWS/hd/hls/6YDPSCHKRKWS_clear.m3u8
https://bitmovin-a.akamaihd.net/content/playhouse-vr/mpds/105560.mpd
```
