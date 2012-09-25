/**
 * @file
 *	Declarations for the CWeapon class.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.1
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").\n
 *  You may not use this file except in compliance with the License.\n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0 \n
 *  Unless required by applicable law or agreed to in writing, software\n
 *  distributed under the License is distributed on an "AS IS" BASIS,\n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and\n
 *  limitations under the License.
 *
 * @addtogroup Objects
 **/
/// @{

#ifndef WORLD__OBJECTS__WEAPON_HPP
#define WORLD__OBJECTS__WEAPON_HPP

#include "Assets/Texture.hpp"
#include "Assets/Sound2D.hpp"
#include "Assets/AssetManager.hpp"
#include "World/Objects/GameObject.hpp"

namespace obj
{
    /**
     * Contains information about a weapon sound, damage, fire rate,
     * and other attributes.
     **/
    class CWeapon
    {
    public:
        CWeapon();
        virtual ~CWeapon();

        bool Init(const std::string& wp_data_filename);

        bool Fire();
        void Reload();
        void Update();

        // Modifiers
        void SetClipCount(const u_int count);

        // Accessors
        const std::string& GetName() const;
        const asset::CTexture* GetProjectileTexture() const;
        asset::asset_id GetFiringSound() const;
        u_int GetDamage()  const;        
        int GetClipCount() const;
        int GetAmmoCount() const;

        asset::CSound2D*    mp_ReloadSound;

    protected:
        CGameObject         m_OnTankSprite;
        CGameObject         m_InvSprite;
        asset::CSound2D*    mp_FireSound;
        

        std::string m_weapon_name;

        u_int m_mag_count,  m_DAMAGE,
            m_current_clip, m_CLIP_SIZE,
            m_fire_delay,   m_FIRING_DELAY,
            m_reload_delay, m_RELOAD_DELAY;

        asset::CTexture*    mp_projectile_texture;
        
        int m_ammo_count;       // Ammo is not unsigned b/c -1 means inf.
    };
}

#endif // WORLD__OBJECTS__WEAPON_HPP
/// @}