#pragma once
#include "SDL.h"
#include "caster_math.h"

struct Player{
    
    
    Player();
    
    Vec2 position;
    Vec2 direction;
    float angle{0.0f};
    
    //Values for if the User is holding down one of the 4 arrow keys used for controlling the Player (these are updated by handleKeyEvent)
    bool fwd{false};
    bool bwd{false};
    bool lft{false};
    bool rgt{false};
    
    // Used for Debug-Drawing the Player position on the Map-Overlay
    SDL_Rect visual;
    
    void update(float fTime); //Update the player position (this uses the fwd,bwd,.. booleans)
    void handleKeyEvent(SDL_Keycode sym,uint32_t type); // This sets the fwd,bwd,.. booleans based on user input
    void draw(SDL_Renderer* ren); // Draws a quad at the player position (in the Debug Map-Overlay)
    
};
