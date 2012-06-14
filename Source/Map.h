#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <string>
#include <vector>

#include "SDL_GL_Base.h"

#include "CGL_Display.h"
#include "CGL_Entity.h"
#include "CGL_Player.h"

namespace gk
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


    class CMap
    {
    public:
        CMap();
        ~CMap();

        /* Load a *.map file */
        bool Load(const char* filename);
        bool Load(const std::string& filename);

        /* Is a certain tile passable? */
        bool CanPass(const int x, const int y) const;
        bool CanPass(const gk_gl::GL_Rect& Boundary) const;
        bool CanPass(const SDL_Rect& Boundary) const;

        /* Find a tile at a certain location */
        Tile* FindTile(const int x, const int y) const;
        Tile* FindTile(const gk_gl::GL_Vertex2f& Position) const;

        /* Find tile index at certain location */
        unsigned int FindTile_Index(const int x, const int y) const;
        unsigned int FindTile_Index(const gk_gl::GL_Vertex2f& Position) const;

        /* Render the map on the screen.
         * This method will also analyze the 
         * player coordinates to determine if
         * panning is necessary.
         */
        void Pan(const gk_gl::CGL_Display& Display, 
            gk_gl::CGL_Player& Player);
        void Update(gk_gl::CGL_Player& Player);

        int GetWidth(){return this->w;}
        int GetHeight(){return this->h;}

    private:
        gk_gl::CGL_Entity MapEntity;
        std::vector<Tile*> Tiles;
        std::fstream map;

        gk_gl::GL_Vertex2f Render_Position;
        gk_gl::GL_Vertex2f Pan_Rate;

        int w, h;
    };
}

#endif // MAP_H