/**
 * @file
 *	Declarations for the GL_Entity class and several functions for
 *	easily rendering some stuff.
 *
 * @author  George Kudrayvtsev
 * @version 1.5.0
 *
 * @addtogroup Assets
 */
/// @{

#ifndef ENTITY_H
#define ENTITY_H

#include "SystemEvents.h"

#include "Math/Math.h"

#include "Assets/Asset.h"

#include "Shader.h"
#include "Graphics.h"
#include "Window.h"

namespace asset
{
    /// Meet them now to make friends later.
    class GL_AssetManager;

    class GL_Entity : public IAsset
    {
    public:
        ~GL_Entity();

        bool LoadFromSDLSurface(SDL_Surface* p_Surface);
        bool LoadFromExistingTexture(const u_int texture,
            const math::ML_Rect& Dimensions);

        void UnloadEntity();

        void Move(const float x, const float y);
        void Move(const math::ML_Vector2& Position);
        void Move_Rate(const float dx, const float dy);
        void Move_Rate(const math::ML_Vector2& Rate);

        void Resize(const u_int w, const u_int h);
        void Resize(const math::ML_Rect& New_Size);
    
        void Rotate(const float degrees);
        void Rotate_Rate(const float ddegrees);

        bool IsOffscreen(const gfx::GL_Window& Window) const;
        bool IsOffscreen(const int screen_w, const int screen_h) const;

        bool CheckCollision(const GL_Entity& Other) const;
        bool CheckCollision(const math::ML_Rect& Box) const;
        bool CheckCollision(const float x, const float y) const;
        bool CheckCollision(const int x, const int y) const;

        void SetFilename(const char* pfilename);
        void SetRenderDimensions(const math::ML_Rectf& Dimensions);

        float GetX() const;
        float GetY() const;
        int GetW() const;
        int GetH() const;

        float GetRotation() const;
        u_int GetTexture() const;
        const std::string& GetFilename() const;

        const math::ML_Vector2& GetPosition() const;
        const math::ML_Rect& GetCollisionBox() const;

        virtual void Update();
        virtual void Render() const;

        friend class GL_AssetManager;
        friend class FL_Font;

    protected:
        GL_Entity(gfx::GL_Shader* p_VShader = NULL,
            gfx::GL_Shader* p_FShader = NULL);

        bool LoadFromFile(const char* p_filename);
        bool LoadFromEntity(GL_Entity* const p_Other);

        math::ML_Vector2 m_Position;
        math::ML_Rect m_CollisionBox;
        math::ML_Rectf m_RenderDimensions;

        u_int m_texture;
        float m_rotation;

        gfx::GL_Shader* mp_VShader;
        gfx::GL_Shader* mp_PShader;
    };
}

namespace gfx
{
    bool FadeOut(const std::vector<asset::GL_Entity*> pentitiesToFade,
        const float fade_rate = 0.01f);
    bool FadeIn(const std::vector<asset::GL_Entity*> pentitiesToFade,
        const float fade_rate = 0.01f);

    bool FadeOut(asset::GL_Entity* p_ToFade, const float fade_rate = 0.01f);
    bool FadeIn(asset::GL_Entity* p_ToFade, const float fade_rate = 0.01f);
}

#endif // ENTITY_H

/// @}