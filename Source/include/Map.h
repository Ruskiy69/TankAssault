#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <string>
#include <vector>

#include "Math/MathHelper.h"
#include "Graphics/GFX_Base.h"
#include "Graphics/GL_Display.h"
#include "Graphics/GL_Entity.h"
#include "Graphics/GL_Player.h"

namespace Game
{
    /* Tile struct, used by the 
     * CMap class to keep track
     * of map information.
     */
    struct Tile
    {
        int x, y;
        enum TileType
        {
            Wall,
            Floor,
            Hole
        } type;
    };


    class Game_Map
    {
    public:
        Game_Map();
        ~Game_Map();

        /* Load a *.map file */
        bool Load(const char* filename);
        bool Load(const std::string& filename);

        /* Is a certain tile passable? */
        bool CanPass(const int x, const int y) const;
        bool CanPass(const MathHelper::Vector2& Position) const;
        bool CanPass(const MathHelper::Rect& Boundary) const;
        bool CanPass(const GFX::GL_Entity* Entity) const;

        /* Find a tile at a certain location */
        Tile* FindTile(const int x, const int y) const;
        Tile* FindTile(const MathHelper::Vector2& Position) const;

        /* Find tile index at certain location */
        unsigned int FindTile_Index(const int x, const int y) const;
        unsigned int FindTile_Index(const MathHelper::Vector2& Position) const;

        /* Render the map on the screen.
         * This method will also analyze the 
         * player coordinates to determine if
         * panning is necessary.
         */
        void Pan(const Game::GL_Player& Player);
        void Update(Game::GL_Player& Player);

        int GetWidth() const{return this->w;}
        int GetHeight() const{return this->h;}

        const MathHelper::Vector2 GetPanRate() const{return this->Pan_Rate;}

        const std::vector<Tile*>& GetTiles() const{return this->Tiles;}

    private:
        GFX::GL_Entity MapEntity;
        std::vector<Tile*> Tiles;
        std::fstream map;

        MathHelper::Vector2 Render_Position;
        MathHelper::Vector2 Pan_Rate;

        int w, h;
    };
}

#endif // MAP_H