/**
 * @file
 *	Declarations for the FL_AssetManager interface.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Assets
 */
/// @{

#ifndef FONT_ASSET_MANAGER_H
#define FONT_ASSET_MANAGER_H

#include <map>
#include <string>

#include "CollapseDef.h"

#include "Assets/Asset.h"
#include "Assets/Font.h"

namespace asset
{
    class FL_AssetManager
    {
    public:
        ~FL_AssetManager();

        static FL_AssetManager& GetInstance();

        asset_id LoadFontFromFile(const char* p_filename, const u_int size);

        FL_Font*    GetFontByFilename(const char* p_filename);
        FL_Font*    GetFontByID(const asset_id id);
        u_int       GetFontCount() const;

        void Update();

    private:
        FL_AssetManager() : m_assetcount(0) {};
        FL_AssetManager(const FL_AssetManager&);

        std::map<asset_id, FL_Font*> mp_fontAssets;
        u_int m_assetcount;
    };

    typedef std::map<asset_id, FL_Font*>::iterator FL_asset_iterator;

    static FL_AssetManager& g_FontAssets = FL_AssetManager::GetInstance();
}

#endif // FONT_ASSET_MANAGER_H

/// @}