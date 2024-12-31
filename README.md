# GHDcode

This is a featureless but very fast text editor written in C++ using raylib.
I wanted to understand how text editors work under the hood.

![demo image](images/demo_charbased.png)

## Features

- Display basic text
- Line numbers
- Zoom in and out
- Change vertical line spacing

## How to use

- Use `Ctrl + Q` to zoom out and `Ctrl + E` to zoom in
- Use `Shift + Q` to decrease the vertical spacing and `Shift + E` to increase the vertical spacing
- Use `Ctrl + G` to toggle the debug grid

## Demos

- Right Now!
    ![demo image](images/demo_charbased.png)
- Added Shortcuts for changing line spacing
    ![demo image](images/demo_charbased_grid.png)
- Added Line numbers and Text + padding
    ![demo image](images/demo_linespacing_and_text.png)
- Add vertical line spacing 
    ![demo image](images/demo_linespacing.png)
- Main character grid 
    ![demo image](images/demo_grid_2.png)

## Build

Build using CMake.

```sh
cmake -B build -S .
cmake --build build --config Debug
```
