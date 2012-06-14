#include "Map.h"

using namespace gk;

CMap::CMap(): Render_Position(0, 0), Pan_Rate(0, 0)
{
}

CMap::~CMap()
{
    for(size_t i = 0; i < this->Tiles.size(); i++)
    {
        if(this->Tiles[i])
            delete this->Tiles[i];
    }

    if(this->map.is_open())
        this->map.close();
}

bool CMap::Load(const std::string& filename)
{
    return this->Load(filename.c_str());
}

bool CMap::Load(const char* filename)
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
    SDL_Surface* Floor          = gk_sdl::LoadImage("Data/Images/Floor.png");
    SDL_Surface* Wall           = gk_sdl::LoadImage("Data/Images/Wall_Blue.png");

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

    while(!this->map.eof())
    {
        std::getline(this->map, line);
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
    Map_Surface = gk_sdl::create_surface(w, h);

    while(!this->map.eof())
    {
        std::getline(this->map, line);

        if(line.empty() || line[0] == ';')  // Empty line or comment?
            continue;

        for(int i = 0; i < line.length(); i++)
        {
            Tile* Tmp_Tile = new Tile();
            Tmp_Tile->x = x;
            Tmp_Tile->y = y;

            switch(toupper(line[i]))
            {
            case 'W':
                Tmp_Tile->type = Tile::Wall;
                gk_sdl::BlitSurface(Map_Surface, Wall, x, y);
                break;

            case 'F':
            default:
                Tmp_Tile->type = Tile::Floor;
                gk_sdl::BlitSurface(Map_Surface, Floor, x, y);
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


bool CMap::CanPass(const int x, const int y) const
{
    const gk_gl::GL_Rect Tmp = {gk_gl::GL_Vertex2f(x, y), 32, 32};
    return this->CanPass(Tmp);
}

bool CMap::CanPass(const gk_gl::GL_Rect& Boundary) const
{
    gk_gl::GL_Rect Tile_Rect = {gk_gl::GL_Vertex2f(0, 0), 32, 32};

    for(size_t i = 0; i < this->Tiles.size(); i++)
    {
        if(this->Tiles[i]->type == Tile::Floor)
            continue;

        Tile_Rect.Position.x = (int)this->Tiles[i]->x;
        Tile_Rect.Position.y = (int)this->Tiles[i]->y;

        if(gk_gl::detect_collision(Boundary, Tile_Rect))
            return false;
    }

    return true;
}

Tile* CMap::FindTile(const int x, const int y) const
{
    return this->FindTile(gk_gl::GL_Vertex2f(x, y));
}

Tile* CMap::FindTile(const gk_gl::GL_Vertex2f& Position) const
{
    gk_gl::GL_Rect Finder = {Position, 0, 0};
    gk_gl::GL_Rect Tile = {gk_gl::GL_Vertex2f(0, 0), 32, 32};

    for(size_t i = 0; i < this->Tiles.size(); i++)
    {
        Tile.Position.x = this->Tiles[i]->x;
        Tile.Position.y = this->Tiles[i]->y;

        if(gk_gl::detect_collision(Tile, Finder))
            return this->Tiles[i];
    }

    return NULL;
}

unsigned int CMap::FindTile_Index(const gk_gl::GL_Vertex2f& Pos) const
{
    gk_gl::GL_Rect Finder = {Pos, 0, 0};
    gk_gl::GL_Rect Tile = {gk_gl::GL_Vertex2f(0, 0), 32, 32};

    for(size_t i = 0; i < this->Tiles.size(); i++)
    {
        Tile.Position.x = this->Tiles[i]->x;
        Tile.Position.y = this->Tiles[i]->y;

        if(gk_gl::detect_collision(Tile, Finder))
            return i;
    }

    return -1;
}

unsigned int CMap::FindTile_Index(const int x, const int y) const
{
    return this->FindTile_Index(gk_gl::GL_Vertex2f(x, y));
}

void CMap::Pan(const gk_gl::CGL_Display& Display, gk_gl::CGL_Player& Player)
{
    /* CMap::Update() handles rendering of the map, 
     * and pans the map across the screen based
     * on player position.
     * The method returns 1 if any panning
     * was done in the x direction, 2 if 
     * in the y direction, and 0 if no
     * panning was done at all.
     */
    static float dx, dy;
    dx = dy = 0;

    if(Player.IsMoving())
    {
        /* If the player is within 200 pixels
         * of any side of the screen, shift the
         * map in the appropriate direction.
         */
        if(Player.GetX() > Display.GetWidth() - 200)
            dx = -3.0f;
        else if(Player.GetX() < 200)
            dx = 3.0f;
        if(Player.GetY() > Display.GetHeight() - 200)
            dy = -3.0f;
        else if(Player.GetY() < 200)
            dy = 3.0f;

        /*if(this->MapEntity.GetX() + Display.GetWidth() + dx < this->w && 
            this->MapEntity.GetX() + dx >= 0 && 
            this->MapEntity.GetY() + Display.GetHeight() + dy < this->h &&
            this->MapEntity.GetY() + dy >= 0)  // Is there even more map to render?
        */
            for(size_t i = 0; i < this->Tiles.size(); i++)
            {
                this->Tiles[i]->x += (int)dx;
                this->Tiles[i]->y += (int)dy;
            }

            this->Pan_Rate.x = dx;
            this->Pan_Rate.y = dy;        
    }
}

void CMap::Update(gk_gl::CGL_Player& Player)
{
    this->MapEntity.Move_Rate(this->Pan_Rate);

    Player.Move_Rate(this->Pan_Rate.x, this->Pan_Rate.y);
    this->MapEntity.Render();

    this->Pan_Rate.x = this->Pan_Rate.y = 0;
}
