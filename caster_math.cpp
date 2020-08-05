#include "caster_math.h"


    Vec2 Vec2::operator+ (const Vec2 &v){
        Vec2 ret;
        ret.x = this->x + v.x;
        ret.y = this->y + v.y;
        return ret;
    }
    
    Vec2 Vec2::operator- (const Vec2 &v){
        Vec2 ret;
        ret.x = this->x - v.x;
        ret.y = this->y - v.y;
        return ret;
    }
    
    Vec2 Vec2::operator* (const Vec2 &v){
        Vec2 ret;
        ret.x = this->x * v.x;
        ret.y = this->y * v.y;
        return ret;
    }
    
    Vec2 Vec2::operator/ (const Vec2 &v){
        Vec2 ret;
        ret.x = this->x / v.x;
        ret.y = this->y / v.y;
        return ret;
    }
    
    Vec2 Vec2::operator+ (const float &v){
        Vec2 ret;
        ret.x = this->x + v;
        ret.y = this->y + v;
        return ret;
    }
    
    Vec2 Vec2::operator- (const float &v){
        Vec2 ret;
        ret.x = this->x - v;
        ret.y = this->y - v;
        return ret;
    }
    
    Vec2 Vec2::operator* (const float &v){
        Vec2 ret;
        ret.x = this->x * v;
        ret.y = this->y * v;
        return ret;
    }
    
    Vec2 Vec2::operator/ (const float &v){
        Vec2 ret;
        ret.x = this->x / v;
        ret.y = this->y / v;
        return ret;
    }
    
