# mouseclicker

[![Build Status](https://travis-ci.org/dhedegaard/mouseclicker.svg)](https://travis-ci.org/dhedegaard/mouseclicker)

A simple application for doing the autoclicking in the game Clicker Heroes.

Always nice to mess around with the win32 api.

Should be buildable on most modern C++ compilers, including:

- mingw gcc 4.8.1
- Visual C++ 14.0
- clang 3.5
- Using mingw through the docker image from
  [here](https://hub.docker.com/r/dhedegaard/mouseclicker/)
  or by building the image from the repo.
- Via vagrant, described below

Released binaries can be found [here](https://github.com/dhedegaard/mouseclicker/releases)

## Building via vagrant and mingw

Simply up'ing with vagrant should build a working binary, like so:

```
$ vagrant up
```

To build manually after the vagrant image has been started:

```
$ vagrant ssh
$ cd /vagrant_data
$ make CC=i686-w64-mingw32-g++-win32 WINDRES=i686-w64-mingw32-windres
```
