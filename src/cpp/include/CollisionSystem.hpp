#pragma once

#include <vector>
#include <optional>
#include <cassert>

#include "Shapes.hpp"

enum class CollisionSide
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct CollisionInfo
{
    CollisionSide collision_side;
    Vector2 actor_position;

    CollisionInfo(CollisionSide collision_side, Vector2 actor_position)
        : collision_side(collision_side), actor_position(actor_position) { }
};

class CollisionSystem
{
public:
    static std::optional<CollisionInfo> get_collision_info(Rectangle actor, Vector2 move_dir, Rectangle obstacle)
    {
        if(!actor.colliding_with(obstacle)) return std::nullopt;

        std::optional<CollisionSide> side = find_collision_side(actor, move_dir, obstacle);
        
        assert(side.has_value());
        
        Vector2 col_pos = calc_collision_position(actor, move_dir, obstacle);
        
        return CollisionInfo(side.value(), col_pos);
    }

    static std::optional<CollisionSide> find_collision_side(Rectangle actor, Vector2 move_dir, Rectangle obstacle)
    {
        if(!actor.colliding_with(obstacle)) return std::nullopt;

        Vector2 diff = (actor.position - obstacle.position).normalized();

        //FIXME: This angle only works for detecting which side of the obstacle we hit
        // because we are running with good FPS, if the delta time was considerably greater then
        // the ball could move inside the obstacle, so the angle will not represent what we are after.
        // >>>> Use Swept AABB Collision Detection
        float angle = diff.angle();
        float obs_angle = std::atan2(obstacle.height/2, obstacle.width/2);

        if(angle <= M_PI - obs_angle && angle >= obs_angle)
            return CollisionSide::UP;
        
        if(angle <= -obs_angle && angle >= -M_PI + obs_angle)
            return CollisionSide::DOWN;
        
        if((angle <= obs_angle && angle >= 0) || (angle < 0 && angle >= -obs_angle))
            return CollisionSide::RIGHT;
        
        return CollisionSide::LEFT;
    }

    static Vector2 calc_collision_position(Rectangle actor, Vector2 move_dir, Rectangle obstacle)
    {
        std::optional<CollisionSide> o_side = find_collision_side(actor, move_dir, obstacle);
        if(!o_side.has_value()) return actor.position;

        CollisionSide side = o_side.value();
        Vector2 position = actor.position;        
        Vector2 inv_dir = Vector2(-move_dir.x, -move_dir.y);

        switch (side)
        {
            case CollisionSide::UP:
            case CollisionSide::DOWN:
            {
                float multiplier = side == CollisionSide::UP ? 1 : -1;
                float new_y = obstacle.position.y + multiplier * (obstacle.height/2 + actor.height/2);
                float dir_multiplier = inv_dir.y == 0.0 ? 0.0 : (new_y - position.y) / inv_dir.y;

                // Fixing the position since after the collision the ball could overlap with part of the obstacle
                // An equivalent thing is done in the other ifs
                position.y = new_y;
                position.x += inv_dir.x * dir_multiplier;
                return position;
            }
            case CollisionSide::RIGHT:
            case CollisionSide::LEFT:
            {
                float multiplier = side == CollisionSide::RIGHT ? 1 : -1;
                float new_x = obstacle.position.x + multiplier * (obstacle.width/2 + actor.width/2);
                float dir_multiplier = inv_dir.x == 0.0 ? 0.0 : (new_x - position.x) / inv_dir.x;

                position.x = new_x;
                position.y += inv_dir.y * dir_multiplier;
                return position;
            }
            default: return actor.position;
        }
    }
};

