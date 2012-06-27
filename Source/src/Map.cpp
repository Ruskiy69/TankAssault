#include "Map.h"

using Game::Game_Map;

Game_Map::Game_Map(): Render_Position(0, 0), Pan_Rate(0, 0)
{
}

Game_Map::~Game_Map()
{
    for(size_t i = 0; i < this->Tiles.size(); i++)
    {
        if(this->Tiles[i])
            delete this->Tiles[i];
    }

    if(this->map.is_open())
        this->map.close();
}

bool Game_Map::Load(const std::string& filename)
{
    return this->Load(filename.c_str());
}

bool Game_Map::Load(const char* filename)
{
    /* First, we have to load the map file
     * and determine how wide / tall the 
     * map is by counting the amount of tiles
     * per row (each tile is 32px by 32px)
     * and the amount of rows total.
     * TODO: Modify level files in order to
     * contain W and H data as a part of it.
     */
    SDL_Surface* Map_Surface    = NULL;
    SDL_Surface* Floor          = GFX::load_image_alpha("Data/Images/Floor.png");
    SDL_Surface* Wall           = GFX::load_image_alpha("Data/Images/Wall_Blue.png");

    std::string line;
    int w = 0, h = 0, x = 0, y = 0;

    if(this->map.is_open())
        this->map.close();

    this->map.open(filename, std::ios::in);

    if(this->map.fail())
        return false;

    /* Determine the length of the first line */
    std::getline(this->map, line);
    w = line.length() * 32;

    this->map.seekg(0); // Go back to the beginning

    while(std::getline(this->map, line))
    {
        if(line.empty() || line[0] == ';')  // Empty line or comment?
            continue;   // Do nothing
        else
            h += 32;
    }

    this->w = w; this->h = h;

    /* We have determined the total size of the map, 
     * now it's time to actually load it.
     */
    this->map.clear();
    this->map.seekg(0); // Back to the beginning again
    Map_Surface = GFX::create_surface_alpha(w, h);
    SDL_SetAlpha(Floor, 0, 0);
    SDL_SetAlpha(Wall, 0, 0);

    while(!this->map.eof())
    {
        std::getline(this->map, line);

        if(line.empty() || line[0] == ';')  // Empty line or comment?
            continue;

        for(size_t i = 0; i < line.length(); i++)
        {
            Tile* Tmp_Tile = new Tile();
            Tmp_Tile->x = x;
            Tmp_Tile->y = y;

            switch(toupper(line[i]))
            {
            case 'W':
                Tmp_Tile->type = Tile::Wall;
                GFX::blit_surface(Map_Surface, Wall, x, y);
                break;

            case 'F':
            default:
                Tmp_Tile->type = Tile::Floor;
                GFX::blit_surface(Map_Surface, Floor, x, y);
                break;
            }

            this->Tiles.push_back(Tmp_Tile);
            x += 32;    // Next tile location
        }

        /* TODO: Add support for various line lengths */

        /* It's a new line, so we move on to 
         * the next row of tiles.
         */
        x = 0;
        y += 32;
    }

    this->MapEntity.SetEntity(Map_Surface);
    this->MapEntity.Move(this->Render_Position);

    SDL_FreeSurface(Map_Surface);
    SDL_FreeSurface(Floor);
    SDL_FreeSurface(Wall);

    this->map.close();
    return true;
}

bool Game_Map::CanPass(const int x, const int y) const
{
    const MathHelper::Rect Tmp(x, y);
    return this->CanPass(Tmp);
}

bool Game_Map::CanPass(const MathHelper::Vector2& Pos) const
{
    const MathHelper::Rect Tmp(Pos.GetX(), Pos.GetY(), 32, 32);
    return this->CanPass(Tmp);
}

bool Game_Map::CanPass(const MathHelper::Rect& Boundary) const
{
    MathHelper::Rect Tile_Rect(0, 0, 32, 32);

    for(size_t i = 0; i < this->Tiles.size(); i++)
    {
        if(this->Tiles[i]->type == Tile::Floor)
            continue;

        Tile_Rect.Move(this->Tiles[i]->x, this->Tiles[i]->y);

        if(Tile_Rect.CheckCollision(Boundary))
            return false;
    }

    return true;
}

bool Game_Map::CanPass(const GFX::GL_Entity* Entity) const
{
    return this->CanPass(Entity->GetCollisionBox());
}

Game::Tile* Game_Map::FindTile(const int x, const int y) const
{
    return this->FindTile(MathHelper::Vector2(x, y));
}

Game::Tile* Game_Map::FindTile(const MathHelper::Vector2& Position) const
{
    MathHelper::Rect Finder((int)Position.GetX(), (int)Position.GetY(), 0, 0);
    MathHelper::Rect Tile(0, 0, 32, 32);

    for(size_t i = 0; i < this->Tiles.size(); i++)
    {
        Tile.x = this->Tiles[i]->x;
        Tile.y = this->Tiles[i]->y;

        if(Tile.CheckCollision(Finder))
            return this->Tiles[i];
    }

    return NULL;
}

unsigned int Game_Map::FindTile_Index(const MathHelper::Vector2& Pos) const
{
    MathHelper::Rect Finder((int)Pos.GetX(), (int)Pos.GetY(), 0, 0);
    MathHelper::Rect Tile(0, 0, 32, 32);

    for(size_t i = 0; i < this->Tiles.size(); i++)
    {
        Tile.x = this->Tiles[i]->x;
        Tile.y = this->Tiles[i]->y;

        if(Tile.CheckCollision(Finder))
            return i;
    }

    return -1;
}

unsigned int Game_Map::FindTile_Index(const int x, const int y) const
{
    return this->FindTile_Index(MathHelper::Vector2(x, y));
}

void Game_Map::Pan(const Game::GL_Player& Player)
{
    /* Game_Map::Update() handles rendering of the map, 
     * and pans the map across the screen based
     * on player position.
     * The method returns 1 if any panning
     * was done in the x direction, 2 if 
     * in the y direction, and 0 if no
     * panning was done at all.
     */
    static float dx, dy;
    dx = dy = 0;
    this->Pan_Rate.Move(0, 0);

    if(Player.IsMoving())
    {
        /* If the player is within 200 pixels
         * of any side of the screen, shift the
         * map in the appropriate direction.
         */
        if((int)Player.GetX() > 800 - 200)
            dx = -3.0f;
        else if((int)Player.GetX() < 200)
            dx = 3.0f;
        if((int)Player.GetY() > 600 - 200)
            dy = -3.0f;
        else if((int)Player.GetY() < 200)
            dy = 3.0f;

        for(size_t i = 0; i < this->Tiles.size(); i++)
        {
            this->Tiles[i]->x += (int)dx;
            this->Tiles[i]->y += (int)dy;
        }

        this->Pan_Rate.Move(this->Pan_Rate.GetX() + dx,
            this->Pan_Rate.GetY() + dy);
    }
}

void Game_Map::Update(Game::GL_Player& Player)
{
    this->MapEntity.Move_Rate(this->Pan_Rate);

    Player.Move_Rate(this->Pan_Rate);
    this->MapEntity.Update();
}