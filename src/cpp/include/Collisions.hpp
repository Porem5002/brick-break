#pragma once

#include <vector>
#include <optional>
#include <variant>

#include "Shapes.hpp"
#include "Closures.hpp"

using ColliderOnHitEvent = Closure<void()>;

using ColliderId = size_t;

enum class ColliderTag : uint32_t
{
    PADDLE = 0x00000001,
    OBSTACLE = 0x00000002,
    ALL = PADDLE | OBSTACLE,
};

bool tag_contains(ColliderTag main_tag, ColliderTag other_tag);

struct Collider
{
    bool is_active;
    ColliderTag tag;
    std::variant<Rectangle, const Rectangle*> hitbox;
    ColliderOnHitEvent on_hit;

    Collider(ColliderTag tag, const Rectangle& hitbox, const ColliderOnHitEvent& on_hit)
        : is_active(true), tag(tag), hitbox(hitbox), on_hit(on_hit) { }

    Collider(ColliderTag tag, const Rectangle* hitbox, const ColliderOnHitEvent& on_hit)
        : is_active(true), tag(tag), hitbox(hitbox), on_hit(on_hit) { }

    Rectangle get_hitbox() const
    {
        if(std::holds_alternative<Rectangle>(hitbox))
            return std::get<Rectangle>(hitbox);
            
        return *std::get<const Rectangle*>(hitbox);
    }
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
    static ColliderId add_collider(ColliderTag tag, const Rectangle& hitbox, const ColliderOnHitEvent& on_hit)
    {
        ColliderId id = colliders.size();
        colliders.emplace_back(tag, hitbox, on_hit);
        return id;
    }

    static ColliderId add_collider_ref(ColliderTag tag, const Rectangle* hitbox, const ColliderOnHitEvent& on_hit)
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

    static std::optional<CollisionInfo> find_collision(const Rectangle& actor, Vector2 movement, ColliderTag tag = ColliderTag::ALL)
    {
        float t = 1.0f;
        Vector2 normal;
        ColliderId id = 0, curr_id = 0;

        for(const auto& c : colliders)
        {
            if(c.is_active && tag_contains(c.tag, tag))
            {
                Rectangle hitbox = c.get_hitbox();

                Vector2 n;
                float curr_t = SDL_FRect_swept((SDL_FRect)actor, (SDL_FRect)hitbox, movement, n);

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