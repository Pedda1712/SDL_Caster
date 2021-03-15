#include "player.h"
#include <iostream>
#include "math.h"

void Player::update(float fTime){
    direction = {cos(angle),sin(angle)};

    if (fwd) {
        position = position + direction * fTime * 50;
    }
    if (bwd) {
        position = position - direction * fTime * 50;
    }
    if (lft) {
        angle += fTime * 5;
    }
    if (rgt) {
        angle -= fTime * 5;
    }
}

void Player::handleKeyEvent(SDL_Keycode sym,uint32_t type){
    
    bool down = (type == SDL_KEYDOWN);
    
    switch(sym){
        
        case SDLK_UP:
            fwd = down ;
            break;
        case SDLK_DOWN:
            bwd = down ;
            break;
        case SDLK_RIGHT:
            lft = down ;
            break;
        case SDLK_LEFT:
            rgt = down ;
            break;
        
        default:break;
    }
}

void Player::draw(SDL_Renderer* ren){
    
    visual.x = position.x;
    visual.y = position.y;
    
    SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(ren,&visual);
    
    Vec2 start = position + Vec2{5,5};
    Vec2 target = start + (direction * 500);
    SDL_RenderDrawLine(ren,start.x,start.y,target.x,target.y);
    
}

Player::Player(){
    visual.w = 10;visual.h = 10;
    position = {50,50};
}
