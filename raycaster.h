#pragma once
#include "SDL.h"
#include "map.h"
#include "player.h"



struct Raycaster {
    
private:

    uint32_t m_width,m_height; // Width and Height of m_screen
    SDL_Texture* m_screen; // Output texture 
    SDL_Rect m_screen_rect; // Output Texture Rect
    SDL_Renderer* m_ren; //Pointer to the SDL_Renderer
    
    float m_fov; // Field of View of the camera

    Map* m_map; //Pointer to a map object
    Player* m_player; // Pointer to a player object
    
public:
    
    Raycaster (uint32_t width,uint32_t height,float fov,Map* m,Player* p,SDL_Renderer* ren);
    
    void drawScene(); // Draws the m_map from the m_players perspective to the m_sceen using Raycasting ans subsequently copies m_screen to the Window using m_ren
    
    ~Raycaster();
};
