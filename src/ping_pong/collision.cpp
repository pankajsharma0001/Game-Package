#include "./pingpong/collision.h"
#include <raylib.h>
#pragma once

bool Collision::collision_Detection(Vector2 circlePos, float radius, Rectangle rec)
{
    //find closest point on the rectangle to the centre of the circle
    float closestX = (circlePos.x < rec.x) ? rec.x : (circlePos.x > rec.x + rec.width) ? rec.x + rec.width : circlePos.x;
    float closestY = (circlePos.y < rec.y) ? rec.y : (circlePos.y > rec.y + rec.height) ? rec.y + rec.height : circlePos.y;
    
    //calculate the distance between the circle's center and this closest point
    float distanceX = circlePos.x - closestX;
    float distanceY = circlePos.y - closestY;
    
    // if the distance is less than the circle's radius, there's a collision
    return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);
}
