#include "../include/Collisions.hpp"

std::vector<Collider> Collisions::colliders;

bool tag_contains(ColliderTag main_tag, ColliderTag other_tag)
{
    return (uint32_t)main_tag & (uint32_t)other_tag;
}