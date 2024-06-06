#pragma once

#include <vector>
#include <optional>
#include <iostream>
#include <functional>

#include "Shapes.hpp"
#include "Closures.hpp"

using ColliderOnHitEvent = Closure<void()>;

using ColliderId = size_t;

enum class ColliderTag
{
    PADDLE,
    OBSTACLE,
};

struct Collider
{
    bool is_active;
    ColliderTag tag;
    const Rectangle* hitbox;
    ColliderOnHitEvent on_hit;

    Collider(ColliderTag tag, const Rectangle* hitbox, const ColliderOnHitEvent& on_hit)
        : is_active(true), tag(tag), hitbox(hitbox), on_hit(on_hit) { }
};

struct CollisionInfo
{
    ColliderId id;
    Vector2 normal;
    float t;

    CollisionInfo(ColliderId id, Vector2 normal, float t)
        : id(id), normal(normal), t(t) { }
};

class Collisions
{
    static std::vector<Collider> colliders;

public:
    static ColliderId add_collider(ColliderTag tag, const Rectangle* hitbox, const ColliderOnHitEvent& on_hit)
    {
        ColliderId id = colliders.size();
        colliders.emplace_back(tag, hitbox, on_hit);
        return id;
    }

    static void set_collider_active(ColliderId id, bool active)
    {
        colliders[id].is_active = active;
    }

    static const Collider& get_collider(ColliderId id)
    {
        return colliders[id];
    }

    static std::optional<CollisionInfo> find_collision(const Rectangle& actor, Vector2 movement)
    {
        float t = 1.0f;
        Vector2 normal;
        ColliderId id = 0, curr_id = 0;

        for(const auto& c : colliders)
        {
            if(c.is_active)
            {
                Vector2 n;
                float curr_t = SDL_FRect_swept((SDL_FRect)actor, (SDL_FRect)*c.hitbox, movement, n);

                if(curr_t < t)
                {
                    id = curr_id;
                    normal = n;
                    t = curr_t;
                }
            }

            curr_id++;
        }

        if(abs(normal.x) > 0 || abs(normal.y) > 0)
        {
            colliders[id].on_hit();
            return CollisionInfo(id, normal, t);
        }

        return std::nullopt;
    }

    static void clear()
    {
        colliders.clear();
    }
};