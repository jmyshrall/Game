#include "Sprite.h"
#include "Game.h" 

namespace SpriteSystem
{
    void render(SDL_Renderer* renderer, ECS& ecs)
    {
        for (auto& [id, comps] : ecs.entities())
        {
            auto tit = comps.find(typeid(Transform));
            auto sit = comps.find(typeid(Sprite));
            if (tit == comps.end() || sit == comps.end()) continue;

            auto* xf  = static_cast<Transform*>(tit->second.get());
            auto* spr = static_cast<Sprite*>(sit->second.get());

            if (!spr->visible || !spr->texture) continue;

            SDL_Rect dst {
                static_cast<int>(xf->position.x) - spr->drawW / 2,
                static_cast<int>(xf->position.y) - spr->drawH / 2,
                spr->drawW,
                spr->drawH
            };

            SDL_SetTextureColorMod(spr->texture,
                spr->tint.r, spr->tint.g, spr->tint.b);
            SDL_SetTextureAlphaMod(spr->texture, spr->tint.a);

            SDL_Rect* src = (spr->srcRect.w > 0) ? &spr->srcRect : nullptr;

            SDL_RenderCopyEx(renderer,
                spr->texture,
                src,
                &dst,
                static_cast<double>(spr->angle),
                nullptr,
                spr->flip);
        }
    }
} // namespace SpriteSystem
