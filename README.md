stixplayer
==========

MP3 player implemented on a gumstix verdex

Building Instructions
=====================

To build the C Application:
```bash
cd player
make
```

Then load the application onto the gumstix.

To build the Android Application:
The first option is to load the android project into a Java IDE
capable of deploying to android devices

```bash
cd android/playerControl
ant release
adb install bin/playerControl-release-unsigned.apk
```

Running the Application
=======================

To run the C application once on the gumstix

```bash
./player <path to mp3 folder>
```

To run the Android application, simply start it on your device
