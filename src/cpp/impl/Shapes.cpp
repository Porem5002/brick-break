#include <limits>

#include "../include/Shapes.hpp"

float SDL_FRect_swept(SDL_FRect m, SDL_FRect o, Vector2 v, Vector2& normal)
{   
    float xInvEntry, yInvEntry; 
    float xInvExit, yInvExit; 

    // find the distance between the objects on the near and far sides for both x and y 
    if (v.x > 0.0f) 
    { 
        xInvEntry = o.x - (m.x + m.w);
        xInvExit = (o.x + o.w) - m.x;
    }
    else 
    { 
        xInvEntry = (o.x + o.w) - m.x;  
        xInvExit = o.x - (m.x + m.w);  
    } 

    if (v.y > 0.0f) 
    { 
        yInvEntry = o.y - (m.y + m.h);  
        yInvExit = (o.y + o.h) - m.y;  
    }
    else 
    { 
        yInvEntry = (o.y + o.h) - m.y;  
        yInvExit = o.y - (m.y + m.h);  
    }

    // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero) 
    float xEntry, yEntry; 
    float xExit, yExit; 

    if (v.x == 0.0f) 
    {
        xEntry = -std::numeric_limits<float>::infinity(); 
        xExit = std::numeric_limits<float>::infinity(); 
    } 
    else 
    { 
        xEntry = xInvEntry / v.x; 
        xExit = xInvExit / v.x; 
    } 

    if (v.y == 0.0f) 
    {
        yEntry = -std::numeric_limits<float>::infinity(); 
        yExit = std::numeric_limits<float>::infinity(); 
    } 
    else 
    { 
        yEntry = yInvEntry / v.y; 
        yExit = yInvExit / v.y; 
    }

    if(yEntry > 1.0f) yEntry = -std::numeric_limits<float>::max();
    if(xEntry > 1.0f) xEntry = -std::numeric_limits<float>::max();

    float entryTime = std::max(xEntry, yEntry); 
    float exitTime = std::min(xExit, yExit);

    normal = Vector2(0.0f, 0.0f);
    
    if (entryTime > exitTime) return 1.0f;
    if (xEntry < 0.0f && yEntry < 0.0f) return 1.0f;
    if (xEntry < 0.0f) {
        // Check that the bounding box started overlapped or not.
        if(m.x + m.w < o.x || m.x > o.x + o.w) return 1.0f;
    }
    if (yEntry < 0.0f) {
        // Check that the bounding box started overlapped or not.
        if(m.y + m.h < o.y || m.y > o.y + o.h) return 1.0f;
    }

    // calculate normal of collided surface
    if (xEntry > yEntry) 
    { 
        if (xInvEntry < 0.0f) 
            normal = Vector2(1.0f, 0.0f);
        else 
            normal = Vector2(-1.0f, 0.0f);
    } 
    else 
    { 
        if (yInvEntry < 0.0f) 
            normal = Vector2(0.0f, 1.0f);
        else 
            normal = Vector2(0.0f, -1.0f);
    }
    
    // return the time of collision
    return entryTime; 
}