# PSVR Player
This is a cross-platform 3D 360/180 video player for the Playstation VR headset. It uses libvlc to stream videos to OpenGL in order to display them on the headset. For rotational tracking, it reads the data from the HID device using hidapi. A GUI is provided using Qt 5. It has been tested on Windows and Linux so far.

This project has been made possible after emoRaivis found out how to get the tracking data from PSVR and developed the app MacMorpheus, which is essentially the same as this project, but as an exclusive Mac application. It is available here: https://github.com/emoRaivis/MacMorpheus. A big thanks to him!

### Warning
This program uses only a roughly estimated symmetrical field of view and does not apply any lens correction distortion. This means, the projection in the headset is not perfectly correct, which can lead to motion sickness.

## Screenshot
![Screenshot](screenshot.png)

## Download
Windows binaries are provided here: https://github.com/metallicent/psvr/releases

For Linux, you will have to compile the program yourself for now.

## Setup
emoRaivis has made a nice wiring diagram with instructions on how to set up PSVR for the PC: https://github.com/emoRaivis/MacMorpheus

## Supported Formats
Since libvlc is used, a huge variety of file formats and codecs is supported.

An arbitary projection angle can be used in combination with one of these modes:
* Monoscopic
* 3D Over/Under
* 3D Side by Side

## TODO / Known Issues
* Find out / measure accurate values for the asymmetrical FOV of the HMD and apply correct barrel distortion.
* libvlc outputs the frame data as BGR instead of RGB on different systems. A checkbox has been added as a temporary workaround to switch between both modes.
* Audio seems to stop working when stopping the video on Windows 10.