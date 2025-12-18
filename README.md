# Image Viewer in C (using .ppm format)
I was inspired by Youtuber Daniel Hirsch to write an Image Viewer in C

## Description
This is a simple program to open and display `.ppm` images in **P6 (binary) format**.  
It reads the image file line by line and constructs the image pixel by pixel using RGB values.

## Requirements
- GCC or G++ compiler  
- SDL3 library  

## How It Works
1. Opens a `.ppm` file in binary mode (`P6` format).  
2. Reads the header to get image dimensions (width and height).  
3. Ignores comments and maximum color value (assumes 255).  
4. Initializes an SDL window of the same size as the image.  
5. Reads RGB values for each pixel and draws them on the window surface.  
6. Keeps the window open until the user closes it.

## Usage
1. Make sure to place the path to ...bin/SDL3 in your environment variables or just place SDL3 in the same folder as your executable file.
2. Place a `.ppm` image file in the same directory as the program (e.g., `grandCanyon.ppm`).  
3. Compile the program:

```bash
g++ <yourSourceCode.c> -o <giveAnyExecutableName> -I<include_path_here> -L<linker_path_here (e.g. .../lib)> -lSDL3
