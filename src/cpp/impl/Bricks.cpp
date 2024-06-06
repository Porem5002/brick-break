#include "../include/Bricks.hpp"

Brick::Brick(Rectangle rect, uint32_t hit_count, const BrickOnHitEvent& on_hit)
        : hits_left(hit_count), rectangle(rect), on_hit(on_hit) { }

bool Brick::is_broken() const
{
    return hits_left == 0;
}

Rectangle Brick::get_rectangle() const
{
    return rectangle;
}

void Brick::draw(SDL_Renderer* renderer) const
{
    SDL_FRect rect = (SDL_FRect)rectangle;
    SDL_SetRenderDrawColor(renderer, FILL_COLOR.r, FILL_COLOR.g, FILL_COLOR.b, FILL_COLOR.a);
    SDL_RenderFillRectF(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, BORDER_COLOR.r, BORDER_COLOR.g, BORDER_COLOR.b, BORDER_COLOR.a);
    SDL_RenderDrawRectF(renderer, &rect);
}

void Brick::hit()
{
    if(!is_broken())
    {
        hits_left--;
        on_hit(hits_left);
    }
}

uint32_t BrickGroupLayout::get_brick_count() const
{
    return brick_count_horizontal * brick_count_vertical;
}

void BrickGroupLayout::generate_bricks_into(Rectangle container, std::vector<Brick>& bricks, const BrickOnHitEvent& on_hit) const
{
    float width = (container.width - padding_horizontal*2) / brick_count_horizontal;
    float height = (container.height - padding_vertical*2) / brick_count_vertical;
    Vector2 center = Vector2(0, padding_vertical + height/2);

    for(uint32_t y = 0; y < brick_count_vertical; y++)
    {
        center.x = padding_horizontal + width/2;

        for(uint32_t x = 0; x < brick_count_horizontal; x++)
        {
            Rectangle rect = Rectangle(center, width, height);
            bricks.emplace_back(rect, hits_to_break, on_hit);
            center.x += width;
        }

        center.y += height;
    }
}