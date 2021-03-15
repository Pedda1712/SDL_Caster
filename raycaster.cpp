#include "raycaster.h"
#include "caster_math.h"
#include <math.h>
#include <iostream>


Raycaster::Raycaster(uint32_t width,uint32_t height,float fov,Map* m,Player* p,SDL_Renderer* ren){
    m_width = width;m_height = height;m_fov = fov;m_map = m;m_player = p;m_ren = ren;
    
    m_screen_rect.x = 0;m_screen_rect.y = 0;
    m_screen = SDL_CreateTexture(m_ren,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_STREAMING,m_width,m_height);
    SDL_QueryTexture(m_screen,NULL,NULL,&m_screen_rect.w,&m_screen_rect.h);

    
}

Raycaster::~Raycaster(){
    SDL_DestroyTexture(m_screen);
    delete m_map;
    delete m_player;
}

void Raycaster::drawScene(){

    int pitch;
    uint8_t* pixelArray; //Pointer to the Pixel Data of the Texture
    
    SDL_LockTexture(m_screen,NULL,(void**)&pixelArray,&pitch); //Gain Access to the pixel data of the texture to modify it
    memset(pixelArray,0x00,m_width*m_height*4); //Clear the pixel data
    
    float startAngle = m_player->angle - m_fov/2; // Ray Angle at the left of the sceen
    float finishAngle = m_player->angle + m_fov/2;// Ray Angle at the right of the screen

    float angleStep = (finishAngle-startAngle) / m_width; //By how much does the angle change per Ray?
    
    for(int atRay = 0; atRay < m_width; atRay++){ //Loop over each vertical column in m_screen
        
        float currentAngle = startAngle + (angleStep * atRay); // Calculate the Angle of the Ray
        Vec2 rayDirection = {cos(currentAngle),sin(currentAngle)}; //Calculate the direction Vector of the Ray using the Ray Angle
        Vec2 rayTarget = m_player->position + (rayDirection * 100000); // Where the Ray points to 
        
        bool first = true;
        Vec2 closest = {0,0}; // This will store the closest found intersection
        Line closestLine; // This will sotre the Line on which the closest found intersection-point lies
        
        //Define a line that goes from the Players position to the Target of the Ray
        float x1 = m_player->position.x;float y1 = m_player->position.y;
        float x2 = rayTarget.x;float y2 = rayTarget.y;
        
        for(int i = 0; i < m_map->numLines; ++i){ //Loop over every line in the Map to find the closest intersection
            float t,u;
            float x3 = m_map->lines[i].x1;float y3 = m_map->lines[i].y1;
            float x4 = m_map->lines[i].x2;float y4 = m_map->lines[i].y2;
            
            float denominator = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
            if(denominator != 0.0){
                t = ((x1 - x3)*(y3-y4)-(y1-y3)*(x3-x4))/(denominator);
                u = -( (x1-x2)*(y1-y3)-(y1-y2)*(x1-x3))/(denominator);
                
                if((t >= 0.0 && t <= 1.0) && (u >= 0.0 && u <= 1.0)){ //Check that the Point actually lies on the line segments and not on the infinitely long extensions of them
                    
                    Vec2 intersection = {x1 + t*(x2 - x1),y1 + t*(y2 - y1)}; // calculate the intersection point (wikipedia is your friend :D )
                    
                    if(first){ // If this is the first line checked , it is the current closest candidate
                        closest = intersection;
                        first = false;
                        closestLine = m_map->lines[i];

                    }else{ // If this isnt the first line checked, compare the distance to the player to the distance of the player of the last closest intersection
                        
                        float current_closest_distance = ((closest.x - m_player->position.x)*(closest.x - m_player->position.x) + (closest.y - m_player->position.y)*(closest.y - m_player->position.y));
                        float newpoint_closest = ((intersection.x - m_player->position.x)*(intersection.x - m_player->position.x) + (intersection.y - m_player->position.y)*(intersection.y - m_player->position.y));
                        
                        if(newpoint_closest < current_closest_distance){ //If the new one is closer
                            closest = intersection;
                            closestLine = m_map->lines[i];
                        }
                        
                    }

                }
                
            }
        }
        
        float hit_uv_x = 0; // This will store the UV-Coordinate (X) of the closest intersection point
        uint32_t hitTexIndex = closestLine.texIndex;
        //Calculate the Texture coordinate using the closest Intersection and the Line with which the Ray intersected (closestLine)
        float x3 = closestLine.x1;float y3 = closestLine.y1;
        float x4 = closestLine.x2;float y4 = closestLine.y2;
        
        if(abs(y4-y3) > abs(x4-x3)){
            hit_uv_x = closestLine.uv_x1 + ((closest.y - y3) / (y4 - y3)) * (closestLine.uv_x2 - closestLine.uv_x1);
        }else{
            hit_uv_x = closestLine.uv_x1 + ((closest.x - x3) / (x4 - x3)) * (closestLine.uv_x2 - closestLine.uv_x1);
        }
        
        if(closest.x != 0.0f && closest.y != 0.0f){
            float relativeAngle = abs((currentAngle - startAngle) - m_fov/2); // The Angle relative to the "View Centre"

            float projected_distance = (closest.x - m_player->position.x) * cos(m_player->angle) + (closest.y - m_player->position.y) * sin(m_player->angle); //Wolfensteins Formula
            
            float height = ((400) / projected_distance) * m_height * 0.03f; //Calculate how tall the wall is in pixel space           
            
            int startingY = (m_height / 2) - (height/2); // Where the wall starts on the sceen
            int endY = startingY + height; //Where the Wall ends on the screen
            
            int texWidth = m_map->textures[hitTexIndex]->w; //The width of the Texture of the Line
            int texHeight = m_map->textures[hitTexIndex]->h; // The height "
            int actual_uv_x = (int)(hit_uv_x * texWidth) % texWidth; // The UV-Coordinate (X) of the texture in pixel space
            float tex_y_increase = texHeight / ((float)endY - (float)startingY); // By how much the UV-Y-Coordinate of the Texture increases for each pixel in the column
            uint8_t* texturePixels = (uint8_t*)m_map->textures[hitTexIndex]->pixels;//Pointer to the pixel data
            float currentY = 0; // The current UV-Y-Coordinate at the Texture (always starts from 0)
            
            float fadeMod = 60.0f/projected_distance;
            //float fadeMod = 1.0f;
            
            if(startingY <= 0.0f) { // If the line starts offscreen, start at 0 and end at the Maximum height of the Screen and adjust the current UV-Y-Coordinate accordingly
                currentY = abs(startingY) * tex_y_increase;
                startingY = 0.0f;endY = m_height;

            }
             
            for(int y = startingY; y < endY; y++){ //Loop over each pixel in the column
                currentY += tex_y_increase; // Increase the UV-Y-Coordinate
                
                if(currentY > texHeight-1){ //Clamp the UV-Y-Coordinate so we dont overflow into unused memory
                    currentY = texHeight-1;
                }
                
                if(y > 0.0f && y < m_height){ //If we are not offscreen copy the pixel data from the Texture to the screen
                    pixelArray[((atRay) + ((m_width) * (y)))*4 + (0)] = texturePixels[((actual_uv_x) + ((texWidth) * ((int)currentY)))*4 + (0)] * std::min(fadeMod,1.0f);
                    pixelArray[((atRay) + ((m_width) * (y)))*4 + (1)] = texturePixels[((actual_uv_x) + ((texWidth) * ((int)currentY)))*4 + (1)] * std::min(fadeMod,1.0f);
                    pixelArray[((atRay) + ((m_width) * (y)))*4 + (2)] = texturePixels[((actual_uv_x) + ((texWidth) * ((int)currentY)))*4 + (2)] * std::min(fadeMod,1.0f);
                }
            }
            
        }
        
    }
    
    SDL_UnlockTexture(m_screen); //Upload the pixelData to the graphics card
    
    SDL_RenderCopy(m_ren,m_screen,NULL,&m_screen_rect); // Draw it to the Window

}


