#pragma once
#include "SDL.h"

struct Line {
    
    // Coordinates of Point 1 & 2
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    
    //Texture Index (Textures are stored in the Map)
    uint32_t texIndex;
    //X-Coordinates of the Textures for Point 1 & 2 where 1.0f is the full length of a texture
    float uv_x1 {0.0f};
    float uv_x2 {25.0f};
    
};

struct Map{
    uint16_t numLines; // The number of lines stored in the map
    Line* lines; // Array of lines
    uint16_t numTextures;
    SDL_Surface** textures; // Array of Textures for the lines
    ~Map();
};

Map* generateRandomMap(uint16_t numLines,uint16_t width,uint16_t height); //Generates a random map with [numLines] lines in the Area of [width,height]

void saveMap (Map* m, const char* filename); //Saves map m to File filename
Map* loadMap (const char* filename); //Loads a map
void loadTexturesToMap(Map* m,const char* filename); // Adds Textures in form of SDL_Surface* to the Map

