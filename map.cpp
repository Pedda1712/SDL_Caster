#include "map.h"
#include "math.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

Map* generateRandomMap (uint16_t numLines,uint16_t width,uint16_t height){
    
    Map* m = new Map;
    m->numLines = numLines;
    
    m->lines = new Line[m->numLines];
    
    for(int i = 0; i < numLines;++i){
        Line temp = {rand()%width,rand()%height,rand()%width,rand()%height,rand()%4};
        float distance = sqrt((temp.x2-temp.x1)*(temp.x2-temp.x1) + (temp.y2-temp.y1)*(temp.y2-temp.y1));
        temp.uv_x2 = (distance) / 10.0f;
        m->lines[i] = temp;
    }
    
    return m;
}

void saveMap (Map* m, const char* filename){
    FILE* out_file = fopen(filename,"wb");
    fwrite(&m->numLines,sizeof(uint16_t),1,out_file); //First write the number of lines
    fwrite(m->lines,sizeof(Line),m->numLines,out_file); //Then write the actual lines
    fclose(out_file);
}


Map* loadMap (const char* filename){
    
    Map* m = new Map; // Map to load the File-Information into
    FILE* in_file = fopen(filename,"rb");
    
    fread(&m->numLines,sizeof(uint16_t),1,in_file); //First read information about how many lines are contained in the file
    
    m->lines = new Line[m->numLines]; // Allocate space for those lines
    fread(m->lines,sizeof(Line),m->numLines,in_file); //Write the lines to the allocated space in memory
    fclose(in_file);
    return m;
    
}

void loadTexturesToMap(Map* m,const char* filename){
    
    std::fstream texFile;
    texFile.open(filename);

    std::vector<std::string> texturePaths;
    if(texFile.is_open()){ // Read all the Texture Paths to a vector
        std::string currentLine;
        while(getline(texFile,currentLine)){
            texturePaths.push_back(currentLine);
        }
    }
    texFile.close();
    m->numTextures = texturePaths.size();
    m->textures = new SDL_Surface*[texturePaths.size()]; // Allocate Space for all the Textures
    for(int i = 0; i < texturePaths.size();i++){ // Load & Convert each texture
        m->textures[i] = SDL_LoadBMP(texturePaths[i].c_str());
        m->textures[i] = SDL_ConvertSurfaceFormat(m->textures[i],SDL_PIXELFORMAT_ABGR8888,0);
    }
    
}

Map::~Map(){
    delete[] lines;
    for(int i = 0; i <numTextures;i++){
        SDL_FreeSurface(textures[i]);
    }
    delete[] textures;
}
