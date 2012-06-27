#include "EventHandler.h"

using Game::Game_Events;

void Game_Events::HandleGameEvents()
{
    static int speed = 0, angle = 0;
    static int ammo = 100;

    while(SDL_PollEvent(&Evt))
    {
        switch(Evt.type)
        {
        case SDL_QUIT:
            this->State = Game_Events::Quit;
            break;

        case SDL_MOUSEBUTTONDOWN:
            printf("MB Down\n");
            if(Evt.button.button == SDL_BUTTON_LEFT)
            {
                if(this->State == Game_Events::Play)
                {
                    if(ammo > 0 && this->World.Shoot())
                    {
                        ammo -= 2;
                        static Game::element_id ammo_id = this->HUD.FindHUDElement(Game::Bar, "Ammo");
                        this->HUD.UpdateElement(ammo_id, ammo);
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if(this->State == Game_Events::Play)
            {
                switch(Evt.key.keysym.sym)
                {
                case SDLK_w:
                case SDLK_UP:
                    speed = this->World.GetPlayer().PLAYER_SPEED;
                    break;

                case SDLK_s:
                case SDLK_DOWN:
                    speed = -this->World.GetPlayer().PLAYER_SPEED;
                    break;

                case SDLK_d:
                case SDLK_RIGHT:
                    angle = -this->World.GetPlayer().PLAYER_SPEED;
                    break;

                case SDLK_a:
                case SDLK_LEFT:
                    angle = this->World.GetPlayer().PLAYER_SPEED;
                    break;
                }
            }

            break;

        case SDL_KEYUP:
            if(this->State == Game_Events::Play)
                {
                switch(Evt.key.keysym.sym)
                {
                case SDLK_w:
                case SDLK_s:
                case SDLK_UP:
                case SDLK_DOWN:
                    speed = 0;
                    break;

                case SDLK_d:
                case SDLK_a:
                case SDLK_RIGHT:
                case SDLK_LEFT:
                    angle = 0;
                    break;
                }
            }
            break;
        }
    }

    if(this->State == Game_Events::Play)
    {
        this->World.MovePlayer(speed*1.0f);
        this->World.RotatePlayer(angle*1.0f);
    }
}

Game::Game_Events::Game_State& Game_Events::GetState()
{
    return this->State;
}