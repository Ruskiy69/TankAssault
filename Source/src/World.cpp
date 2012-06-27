#include "World.h"

using Game::Game_World;

Game_World::~Game_World()
{
    for(size_t i = 0; i < this->Bullets.size(); i++)
        delete this->Bullets[i];

    this->Bullets.clear();
}

bool Game_World::LoadMap(const char* filename)
{
    return this->Map.Load(filename);
}

bool Game_World::MovePlayer(const float speed)
{
    this->Player.Move(speed);
    if(!this->Map.CanPass(&this->Player))
    {
        this->Player.Move(-speed);
        return false;
    }

    return true;
}

bool Game_World::RotatePlayer(const float angle)
{
    this->Player.RotateBody(angle);
    if(!this->Map.CanPass(&this->Player))
    {
        this->Player.RotateBody(-angle);
        return false;
    }

    return true;
}

void Game_World::Shoot()
{
    this->Player.Shoot(this->Bullets);
}

void Game_World::Update()
{
    this->Map.Pan(this->Player);
    this->Map.Update(this->Player);
    this->Player.Update();

    for(Game::GL_Bullets::iterator i = this->Bullets.begin(); 
        i != this->Bullets.end(); /* no third */)
    {
        if((*i)->IsOffscreen(this->Window))
            i = this->Bullets.erase(i);
        else
        {
            (*i)->Update();
            i++;
        }
    }
}

const Game::GL_Player& Game_World::GetPlayer() const
{
    return this->Player;
}