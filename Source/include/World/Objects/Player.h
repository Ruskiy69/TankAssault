/**
 * @file
 *  Declarations for the CL_Player class.
 *
 * @author George Kudrayvtsev
 * @version 0.1
 *
 * @addtogroup Objects
 * @see asset::GL_Entity
 */
/// @{

#ifndef PLAYER_H
#define PLAYER_H

#include "World/Objects/BaseTank.h"

namespace obj
{
    /// The user-controlled entity.
    class Obj_Player : public Obj_Tank
    {
    public:
        Obj_Player();

        void SetSpawn(const math::ML_Vector2& Pos);
        int Update();
    };
}

#endif // PLAYER_H
/// @}