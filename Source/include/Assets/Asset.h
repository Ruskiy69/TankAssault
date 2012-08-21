/**
 * @file
 *	Declaration of the IAsset interface.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Assets
 *  Manipulation and representation of graphical and audio elements.
 *  Provides functions that allow for conversion between SDL_Surface's
 *  and OpenGL compatible textures. Also provides handy window-creation
 *  and a sprite base class to allow objects to be drawn on the screen
 *  easily. Includes a TTF_Font* wrapper and an OpenAL audio mixing class
 *  that supports both uncompressed WAV sound effects and OGG music files.
 */
/// @{

#ifndef ASSET_H
#define ASSET_H

#include <string>

/**
 * Contains all asset related classes.
 *  Assets include fonts, textures, sound effects, etc. Asset managers
 *  are created to maintain and track these assets.
 */
namespace asset
{
    typedef unsigned int asset_id;

    /// Asset id tracker.
    static asset_id CURRENT_ASSET_ID = 1;

    /**
     * The interface for any game asset.
     */
    class IAsset
    {
    public:
        IAsset();
        virtual ~IAsset();

        virtual bool LoadFromFile(const char* p_filename) = 0;

        virtual void IncrementReferenceCount();
        virtual const std::string& GetFilename() const;
        virtual unsigned int GetReferenceCount() const;
        virtual asset_id GetID() const;

    protected:
        std::string m_filename;
        asset_id m_id;
        unsigned int m_refcount;
        bool m_loaded;
    };
}

#endif // ASSET_H

/// @}