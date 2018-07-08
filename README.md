# py-libseek-thermal-master

##Description

``NOTE``: Work in progress.

This project provides a python interface wrapper to the [libseek-thermal](https://travis-ci.org/maartenvds/libseek-thermal)
c++ user space driver with a python interface.

Many different options exist for doing this, many poorly documented and/or have complicated custom dependencies/build systems that make them less palatable:

1) Extending python manually:
    * Advantages: Simple
    * Disadvantages: It gets tricky when trying to export/deal with more complex datastructs, and technically I was rather confused
     with the documentation on what the true goal of this all was (they talk about rebuilding Python, and technically I don't want to rebuild an extension of python -- I
     want to wrapper a c++ library with a python module.
2) SIP:
    * Advantages: Supposedly seemless integration/wrappering of c++.  A part of the Qt package interface to provide the PyQt wrappers.
    * Disadvantages: Ugly .sip interface files, confusing syntax for more complex situations, and failed to seemlessly build module when I attempted to include
    the libseek headers (couldn't find uint16_t, despite the fact that I gave the correct include reference in my .sip file), and I didn't want to try to figure out
    why it was failing to build.
3) ctypes
    * Advantages: Very simple, native interface in python to ctypes.
    * Disadvantages: Technically made as a c-interface to python, not c++, so I'd have to wrapper C around C++ to wrap python.  Moreover, dealing with complex opencv Mat class types was
    something I didn't want to spend a lot of time trying to reverse engineer to feed into python.
4) boost.python
    * Advantages: One of the simpler syntax/easier wrapper frameworks for c++ to python.  Supposedly handles c++ classes, reference counts, etc. seemlessly.
    * Disadvantages: Uses the bjam build system, a boost-specific build system that didn't work out of the box for their examples, and their documentation was
    convoluted enough that I didn't want to spend hours trying to debug & solve it.
5) CLIF - Wrapper generator c++ for several languages using LLVM
    * [Link](https://github.com/google/clif)
    * Advantages: Once you learn, supposedly scales to many different languages.
    * Disadvantages: Uses a compiler I'm not familiar with, I don't want to have to learn about for this simple task,  and not sure of support on Macs.  It also has a large number of 
    annoying dependencies.

In the end, this implementation uses libseedoost.python, as it seems to be one of the better wrapper frameworks for python -> C++.  Given that the bjam build framework was avoided, this
interface worked great.

##Setup
This python SeekThermal wrapper is currently built and tested on a Raspberry Pi Raspian OS.  It currently only supports the [SeekThermal Compact camera](https://www.thermal.com/compact-series.html).

###Dependencies

*libboost
*[libseek-thermal-master](https://travis-ci.org/maartenvds/libseek-thermal)
*python (>=2.7)
*[libopencv](https://opencv.org/)
*[boost.python](https://www.boost.org/doc/libs/1_66_0/libs/python/doc/html/index.html)


###Building and Installing

####Macports presetup
* I had some linker issues that may be tied to the python version that MacPorts boost was built under (+python27), given that I'm using python 3.6.  I uninstalled and reinstalled as follows:
```
>sudo port install boost +python36
```
* Rudimentary compilation attempt:
```
sudo clang++ -v -shared -L/opt/local/lib -L/opt/local/Library/Frameworks/Python.framework/Versions/3.6/lib -lpython3.6m -
I/opt/local/include -I "/opt/local/Library/Frameworks/Python.framework/Versions/3.6/include/python3.6m" hello.cpp -lboost_python3-mt -fpic -o libhello.so
```
This appears to have generated libhello.so without a hitch, but now I may want to expand to cmake (as non-bjam people seem to use this build framework) to build what I need (I have the library now,
but I obviously need to generate other files to correctly have the python script import.

After running cmake, I realized that I don't need to use cmake to do this (but I will anyways, as libseek-thermal already uses cmake for it's build system).  In fact,
the output \*.so file is what is imported by the 'import' command in python.

####CMake build and Install
```
#mkdir build
#cd build
#cmake ../
#make
```

Copy the pylibseek_ext.so to the appropriate location and import as in python.

###TODO

*Fix CMakeLists.txt to be more universal.
*Allow to be installed as system-wide python package for import (currently only a local package).
*Include example. 
