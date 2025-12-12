/*
* Title: Image Viewer (.ppm format)
* Description: This program opens and reads a .ppm file line by line
*              to construct the image pixel by pixel using RGB values.
* Requirement: This program requires GCC/G++ and SDL3 to run.
* Author: Halmuhammet Muhamedorazov
* Date: 12.12.2025
*/

#include<SDL3/SDL.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    // Open the image to be viewed in .ppm format in binary read mode
    FILE *file = fopen("grandCanyon.ppm", "rb");
    
    /*
    * The first 4 lines of .ppm file contains headers
    * Line 1: P6 or P3, where P3 uses ASCII characters and P6 uses binary bytes
    * Line 2: comment
    * Line 3: width and height of the image
    * Line 4: maximum color value (e.g 255 mostly)
    * Lines after: RGB values in bytes (3 bytes per pixel)
    * Only line that is important for us here is line 3 to get the image dimension
    * Also make sure that file uses P6 storage format for this program to function as expected.
    */   
    
    // Get the type of storage format: P6 or P3 (ignore since we assume it is P6)
    
    char *fileType = (char*)calloc(1000 ,sizeof(char));
    fgets(fileType, 1000, file);
    printf("%s", fileType);
    free(fileType);
    
    // Get comment (ignore)
    
    char *comment = (char*)calloc(1000, sizeof(char));
    fgets(comment, 1000, file);
    printf("%s",comment);
    free(comment);
    
    // Get the width and height and store them in dimension to parse them using sscanf
    
    char *dimension = (char*)calloc(1000, sizeof(char));
    fgets(dimension, 1000, file);
    
    int width = 0;
    int height = 0;
    
    // Parse the width and height of image accordingly
    
    sscanf(dimension, "%d %d", &width, &height);
    printf("%d %d\n", width, height);
    free(dimension);
    
    // Also ignore the max color value since most images use the value of 255
    
    char *maxColorVal  = (char*)calloc(1000, sizeof(char));
    fgets(maxColorVal, 1000, file);
    printf("%s\n", maxColorVal);
    free(maxColorVal);
         
    // Initialize the SDL
       
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL3 library\n");
        return -1;
    }
    
    // We need a window, so let's craete one which will have image width and height
    
    SDL_Window *pWindow = SDL_CreateWindow("Image Viewer", width, height, 0);
    if(!pWindow)
    {
        printf("Failed to create window\n");
        return -1;
    }
    
    // Initialize the window surface with our window object
    
    SDL_Surface *window_surface = SDL_GetWindowSurface(pWindow);
    SDL_ShowWindow(pWindow);
    
    // We will draw the image pixel by pixel, each getting its RGB values from the .ppm file
    
    SDL_Rect pixel = (SDL_Rect){0, 0, 1 , 1}; // first two values specify the position
                                              // last two values specify width and height of each pixel
    
    // Declare the r, g, b variables 
    
    Uint8 r, g, b;
    
    // We also need the format of pixel which is needed to map the rgb values
    
    const SDL_PixelFormatDetails *pixelFormat = SDL_GetPixelFormatDetails(window_surface->format);
    
    // Iterate over the width and height of image to construct it pixel by pixel from top left to bottom right
    
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            r = getc(file);
            g = getc(file);
            b = getc(file);
            Uint32 color = SDL_MapRGB(pixelFormat, NULL, r, g, b);
            pixel.x = x;
            pixel.y = y;
            SDL_FillSurfaceRect(window_surface, &pixel, color );
        }
    }
    
    // Add some logic to keep the window open until quit event is triggered
     
    SDL_Event event;
    bool isRunning = true;
        
    while(isRunning)
    {
        while(SDL_PollEvent(&event)) 
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                isRunning = false;                    
            }
            
        }
        
        SDL_UpdateWindowSurface(pWindow);
    }
    SDL_DestroyWindow(pWindow);
    return 0;
}
