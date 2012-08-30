/**
 * @file
 *  Implementation of the Obj_Player class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 */
#include "World/Objects/Player.h"

using obj::Obj_Player;
using asset::g_TextureAssets;

/**
 * The constructor for the class.
 *  The main sprite image is loaded here, which resides in "Data/Images"
 *  for the time being. Since there are multiple sprites in the sheet,
 *  they are each cut out, and the GL_Entity is resized to 64x64 to
 *  exclusively accompany the first sprite. Then, in the Update() method,
 *  only half of the whole texture is rendered, properly displaying the
 *  sprite in use.
 */
Obj_Player::Obj_Player(){}

bool Obj_Player::Init()
{
    Obj_Tank::Init();

    // Load tank base images
    SDL_Surface* pSprite_Sheet = gfx::load_image("Data/Images/Player.png");
    mp_Tank = g_TextureAssets.GetEntityByID(
        g_TextureAssets.LoadEntityFromSurface(pSprite_Sheet));
    mp_Tank->Resize(64, 64);
    mp_Tank->SetRenderDimensions(math::ML_Rectf(0, 0, 0.5f, 1.0f));
    mp_Tank->Move(0, 0);
    SDL_FreeSurface(pSprite_Sheet);

    // Load tank tower image
    /// @todo Fix animations.
    mp_Tower = g_TextureAssets.GetEntityByID(
        g_TextureAssets.LoadEntityFromFile<asset::GL_Entity>(
        "Data/Images/Player_Barrel1.png"));
    mp_Tower->Move(0, 0);

    return true;
}

/**
 * Render the tank and the tower on-screen.
 *
 * @todo Animate tank tower on firing.
 */
int Obj_Player::Update()
{
    return Obj_Tank::Update();
}

void Obj_Player::SetSpawn(const math::ML_Vector2& Pos)
{
    mp_Tank->Move(Pos);
    mp_Tower->Move(Pos);
}
