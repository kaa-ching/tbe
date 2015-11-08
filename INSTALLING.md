The Butterfly Effect - INSTALLING
=================================

If you downloaded an installer and executed it, being able to read
this file already suggests you're done - the installer did its work.

This file explains step-by-step how to build TBE from the source archive.
For now, this only discusses how to build for Linux platforms.
Building for other platforms should be documented here:
https://github.com/kaa-ching/tbe/wiki/HowToMakeARelease

1)
Make sure that you have all tools & libraries installed that you need.
These include:
  * gcc, glibc-devel
  * binutils
  * make
  * cmake
  * patch
Depending on whether you have these:
  * libqt4        (which must be QT version 4.6 or up !!!)
  * libqt4-devel
if you do not, you'll need to build the static version, which in turn
requires building QT first - you'll now also need:
  * a lot of X11 development libraries
  * wget
However, doing a static build is beyond the scope of this document, 
please refer to the TBE wiki:
https://github.com/kaa-ching/tbe/wiki/HowToMakeARelease

2)
As a *NORMAL* user, build the main project. 
If you have cmake installed, this should be as easy as just run make in this
directory:
    make
That make will in turn call cmake and build everything for you.
It will also immediately install the game in the local directory and install
a link to the binary `tbe` in the project root.

3)
There is no action three :-)

You can now start the game by typing:  ./tbe

Enjoy!

