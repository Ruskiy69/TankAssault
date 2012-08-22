/**
 * @file
 *  Declarations for the CollisionMap class.
 *
 * @author George Kudrayvtsev
 * @version 1.2.1
 *
 * @addtogroup Game
 */
/// @{

#ifndef COLLISION_MAP_H
#define COLLISION_MAP_H

#include <vector>
#include <string>

#include "Errors.h"
#include "Helpers.h"

#include "World/Levels/Map.h"

namespace game
{
    /// Extension for terrain maps (@a Collapse Collision Map)
    static const char COLLISION_MAP_EXT[] = {".ccm"};

    /**
     * The collision tile map.
     */
    class CL_CollisionMap : public CL_Map
    {
    public:
        CL_CollisionMap(bool edit_mode = false);
        ~CL_CollisionMap();

        bool Load(const char* pfilename);
        bool Save(const char* pfilename);

        void PlaceTile(int x, int y);
        void Update(bool show_active);
        
    private:
        SDL_Surface* mp_Overlay;
    };
}

#endif // COLLISION_MAP_H
/// @}