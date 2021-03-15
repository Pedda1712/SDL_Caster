#pragma once

struct Vec2{
    double x;
    double y;
    
    Vec2 operator+ (const Vec2 &v);
    Vec2 operator- (const Vec2 &v);
    Vec2 operator* (const Vec2 &v);
    Vec2 operator/ (const Vec2 &v);
    
    Vec2 operator+ (const float &v);
    Vec2 operator- (const float &v);
    Vec2 operator* (const float &v);
    Vec2 operator/ (const float &v);
};
