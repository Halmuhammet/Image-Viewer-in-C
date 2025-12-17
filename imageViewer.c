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

char* consumeNextHeaderLine(FILE *file);
void windowEventPoll(SDL_Window *pWindow);
void drawPixel(SDL_Surface *window_surface, FILE *file, int width, int height);

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
    
    char *storageFormat = consumeNextHeaderLine(file);
    free(storageFormat);
    
    // Get comment (ignore)
    
    consumeNextHeaderLine(file);
    
    // Get the width and height and store them in dimension to parse them using sscanf

    int width = 0, height = 0;
    char *dimension = consumeNextHeaderLine(file);
    // Parse the width and height of image accordingly
    sscanf(dimension, "%d %d", &width, &height);
    free(dimension);
    
    // Also ignore the max color value since most images use the value of 255
    
    char *maxColorValue = consumeNextHeaderLine(file);
    free(maxColorValue);

    // Initialize the SDL
       
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
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

    // Draw pixel by callig the responsible function
    drawPixel(window_surface, file, width, height);
    
    // Add some logic to keep the window open until quit event is triggered
   
    windowEventPoll(pWindow);
    
    SDL_DestroyWindow(pWindow);
    return 0;
}

void drawPixel(SDL_Surface *window_surface, FILE *file, int width, int height)
{
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
            // Get RGB values (8 bits per color value)
            r = getc(file);
            g = getc(file);
            b = getc(file);
            Uint32 color = SDL_MapRGB(pixelFormat, NULL, r, g, b);
            // Update next pixel's xy - coordinates
            pixel.x = x;
            pixel.y = y;
            // Render a pixel on the screen
            SDL_FillSurfaceRect(window_surface, &pixel, color );
        }
    }
}

/*
* Function to keep the window open until the user quits
*/

void windowEventPoll(SDL_Window *pWindow)
{
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
}

/*
* Helper function to allocate memory for string character pointer to consume the headers of .ppm file
*/

char* consumeNextHeaderLine(FILE *file)
{
    char *headerLine = (char*)calloc(1000 ,sizeof(char));
    fgets(headerLine, 1000, file);
    printf("%s", headerLine);
    return headerLine;
}

