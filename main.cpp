#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "SDL.h"
#include "SDL2/SDL_ttf.h"

#include "raycaster.h"
#include "map.h"
#include "player.h"

#define WIDTH 640
#define HEIGHT 480


int main () {

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    SDL_Window *win = SDL_CreateWindow("SDL Raycaster", 100, 100 , WIDTH , HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_Renderer *ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    
    //Creating a Map and loading a Texture
    Map* map = loadMap("default.map");
    loadTexturesToMap(map,"textures.def");

    Player* p = new Player;
    Raycaster caster (WIDTH,HEIGHT,((float)60/(float)180)*3.14159f,map,p,ren);

    
    //FPS Font Rendering
    TTF_Font* fps_font = TTF_OpenFont("samanata.ttf", 24);
    if(fps_font == NULL){
        std::cout << TTF_GetError() << std::endl;
    }
    SDL_Color fps_color = {255, 0, 0};
    SDL_Rect fps_rect; 
    fps_rect.x = 0;
    fps_rect.y = 0;

    //FPS Measurement (every Second)
    uint32_t fps_lasttime = SDL_GetTicks();
    uint32_t fps_current = 0;
    uint32_t fps_frames = 0;
    //Frametime Measurement (each Frame)
    long int startTicks = SDL_GetTicks();
    
    bool running = true;
    SDL_Event e;
    while(running){
        
        //Frametime calculation
        int tickDifference = SDL_GetTicks() - startTicks;
        float frameTime = (float)tickDifference / 1000.0f;
        startTicks = SDL_GetTicks();
        
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    running = false;break;
                case SDL_KEYDOWN:
                    p->handleKeyEvent(e.key.keysym.sym,SDL_KEYDOWN);
                    break;
                case SDL_KEYUP:
                    p->handleKeyEvent(e.key.keysym.sym,SDL_KEYUP);
                    break;
                default: break;
            }
        }

        //Update the player (Movement and such)
        p->update(frameTime);
        
        //FPS measurement
        
        fps_frames++;
        if(fps_lasttime < SDL_GetTicks() - 1000.0f){
            fps_lasttime = SDL_GetTicks();
            fps_current = fps_frames;
            fps_frames = 0;
        }
        //FPS Rendering
        std::string fps = " " + std::to_string(fps_current);
        SDL_Surface* fps_surface = TTF_RenderText_Solid(fps_font, fps.c_str(), fps_color);
        SDL_Texture* fps_texture = SDL_CreateTextureFromSurface(ren, fps_surface);
        SDL_QueryTexture(fps_texture,NULL,NULL,&fps_rect.w,&fps_rect.h);
        
        
        SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(ren);
        //Raycasting and scene Drawing
        caster.drawScene();
        // Debug drawing (Player position,view direction)
        p->draw(ren);
        // Debug drawing (Map walls)
        SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
        for(int i = 0; i < map->numLines;++i){
            SDL_RenderDrawLine(ren,map->lines[i].x1,map->lines[i].y1,map->lines[i].x2,map->lines[i].y2);
        }
        // Drawing the FPS count
        SDL_RenderCopy(ren,fps_texture,NULL,&fps_rect);
        SDL_RenderPresent(ren);
        
        // Free the Surface and Texture that are used for Rendering the FPS Count because they are updated each Frame
        SDL_FreeSurface(fps_surface);
        SDL_DestroyTexture(fps_texture);
    }
     
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
    
}
