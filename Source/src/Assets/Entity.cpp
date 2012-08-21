/**
 * @file
 *  GL_Entity implementation.
 *
 * @author George Kudrayvtsev
 * @version 1.5.0
 */

#include "Assets/Entity.h"

using asset::GL_Entity;

/**
 * Sets up everything.
 */
GL_Entity::GL_Entity() : m_texture(0),
    m_rotation(0),
    m_RenderDimensions(0, 0, 1, 1) {}

/**
 * Cleans all loaded textures.
 */
GL_Entity::~GL_Entity()
{
    if(m_texture > 0 && m_refcount <= 1)
        glDeleteTextures(1, &m_texture);
}

/**
 * Creates a texture from a filename.
 *
 * @param char* Filename
 */
bool GL_Entity::LoadFromFile(const char* p_filename)
{
    // Do nothing if the filenames are equivalent (meaning same texture).
    if(m_filename == p_filename)
        return true;

    // Unload the previously existing entity.
    this->UnloadEntity();

    // Load the texture from the filename.
    m_texture = gfx::load_texture_alpha(p_filename);
    if(m_texture == 0)
        return false;

    // Set parameters for the texture
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Determine texture dimensions.
    GLint w, h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_CollisionBox.Resize(w, h);

    m_filename = p_filename;
    return true;
}

bool GL_Entity::LoadFromExistingTexture(const u_int texture,
    const math::ML_Rect& Dimensions)
{
    // Invalid texture?
    if(texture < 0)
        return false;

    // Unload old texture.
    this->UnloadEntity();

    // Set up the new one.
    m_texture = texture;
    this->Resize(Dimensions);
    return true;
}

bool GL_Entity::LoadFromSDLSurface(SDL_Surface* p_Surface)
{
    // Unload previous texture.
    this->UnloadEntity();

    // Convert SDL_Surface to an OpenGL texture and resize it.
    m_texture = gfx::SDL_Surface_to_texture(p_Surface);
    m_CollisionBox.Resize(p_Surface->w, p_Surface->h);

    return true;
}

/**
 * Creates a copy of another entity.
 * 
 * @param GL_Entity* Entity to copy.
 *
 * @return TRUE if successful load, FALSE if not.
 */
bool GL_Entity::LoadFromEntity(GL_Entity* const p_Other)
{
    // Check for validity.
    if(p_Other == NULL)
        return false;

    // Unload old.
    this->UnloadEntity();

    p_Other->IncrementReferenceCount();

    // Copy new.
    m_refcount      = p_Other->GetReferenceCount();
    m_filename      = p_Other->GetFilename();
    m_texture       = p_Other->GetTexture();
    m_CollisionBox  = p_Other->GetCollisionBox();
    return true;
}

/**
 * Unloads the currently loaded texture.
 */
void GL_Entity::UnloadEntity()
{
    // We have a texture already, and we are the only owner of it.
    if(m_texture > 0 && m_refcount == 1)
    {
        glDeleteTextures(1, &m_texture);
        m_filename.clear();
    }
}

/**
 * Move the entity to a certain location.
 *
 * @param float X-coordinate to move to
 * @param float Y-coordinate to move to
 */
void GL_Entity::Move(const float x, const float y)
{
    m_Position.Move(x, y);
    m_CollisionBox.Move(m_Position);
}

/**
 * Move the entity to a certain location.
 *
 * @param math::ML_Vector2 Location to move to
 *
 * @see math::ML_Vector2
 * @see GL_Entity::Move_Rate()
 */
void GL_Entity::Move(const math::ML_Vector2& Position)
{
    m_Position.Move(Position.x, Position.y);
    m_CollisionBox.Move(m_Position);
}

/**
 * Move the entity at a certain rate.
 *
 * @param float Rate to move in the x direction
 * @param float Rate to move in the y direction
 */
void GL_Entity::Move_Rate(const float dx, const float dy)
{
    m_Position.Move(m_Position.x + dx,
        m_Position.y + dy);

    m_CollisionBox.Move(m_Position);
}

/**
 * @overload asset::GL_Entity(const float, const float)
 *
 * @param math::ML_Vector2 Rate to move at
 *
 * @see math::ML_Vector2
 */
void GL_Entity::Move_Rate(const math::ML_Vector2& Rate)
{
    m_Position = m_Position + Rate;
    m_CollisionBox.Move(m_Position);
}

/**
 * Resize the texture.
 *
 * Resizing the texture will change the amount rendered on-screen.
 *
 * @param int New width
 * @param int New height
 */
void GL_Entity::Resize(const u_int w, const u_int h)
{
    m_CollisionBox.Resize(w, h);
}

/**
 * Resize the texture.
 *  Resizing the texture will change the amount rendered on-screen, and
 *  the size of the collision detection box. 
 *
 * @param math::ML_Rect& New texture dimensions.
 */
void GL_Entity::Resize(const math::ML_Rect& New_Size)
{
    m_CollisionBox.Resize(New_Size.w, New_Size.h);
}

/**
 * Rotate the entity counter-clockwise.
 *
 * @param float Degrees to set rotation to
 */
void GL_Entity::Rotate(const float theta)
{
    m_rotation = theta;
}

/**
 * Rotate the entity counter-clockwise at a rate.
 *
 * @param float Degrees to increment rotation angle at
 */
void GL_Entity::Rotate_Rate(const float dtheta)
{
    m_rotation += dtheta;
}

/**
 * Checks if the entity is off-screen in any way.
 *
 * @return TRUE if off-screen, FALSE otherwise.
 */
bool GL_Entity::IsOffscreen(const gfx::GL_Window& Window) const
{
    return (this->GetX() > Window.GetWidth()) ||
        (this->GetX() < 0) || (this->GetY() < 0) ||
        (this->GetY() > Window.GetHeight());
}

/**
 * @overload GL_Entity::IsOffscreen()
 *
 * @param int Window height
 * @param int Window width
 *
 * @return TRUE if off-screen, FALSE otherwise.
 */
bool GL_Entity::IsOffscreen(const int width, const int height) const
{
    return (this->GetX() > width) ||
        (this->GetX() < 0) || (this->GetY() < 0) ||
        (this->GetY() > height);
}

/**
 * Checks for collision with another GL_Entity.
 *
 * @param GL_Entity Other entity to check collision with.
 *
 * @return TRUE if they collide anywhere, FALSE otherwise.
 */
bool GL_Entity::CheckCollision(const GL_Entity& Obj) const
{
    return this->CheckCollision(Obj.GetCollisionBox());
}

/**
 * @overload GL_Entity::CheckCollision(const GL_Entity& Obj)
 *
 * @param math::ML_Rect ML_Rect to check collision with.
 *
 * @return TRUE if they collide anywhere, FALSE otherwise.
 *
 * @see math::ML_Rect
 */
bool GL_Entity::CheckCollision(const math::ML_Rect& Obj) const
{
    return Obj.CheckCollision(m_CollisionBox);
}

/**
 * @overload GL_Entity::CheckCollision(const GL_Entity& Obj)
 *
 * @param int x
 * @param int y
 *
 * @return TRUE if they collide anywhere, FALSE otherwise.
 */
bool GL_Entity::CheckCollision(const int x, const int y) const
{
    math::ML_Rect Tmp(x, y);
    return Tmp.CheckCollision(m_CollisionBox);
}

/**
 * @overload GL_Entity::CheckCollision(const GL_Entity& Obj)
 *
 * @param int x
 * @param int y
 *
 * @return TRUE if they collide anywhere, FALSE otherwise.
 */
bool GL_Entity::CheckCollision(const float x, const float y) const
{
    math::ML_Rect Tmp(x, y);
    return Tmp.CheckCollision(m_CollisionBox);
}

/**
 * Sets the entity filename. This goes by the honor system that the
 * entity was not created using LoadFromSDLSurface(), but rather loaded using
 * LoadFromFile().
 *
 * @param char* Filename
 */
void GL_Entity::SetFilename(const char* pfilename)
{
    m_filename = pfilename;
}

/**
 * Changes the dimensions to render the current texture at. Useful
 * for manipulating sprite sheets.
 *
 * @param math::ML_ML_Rectf Dimensions
 */
void GL_Entity::SetRenderDimensions(const math::ML_Rectf& Dim)
{
    m_RenderDimensions = Dim;
}

/**
 * Get entity height.
 *
 * @return Height
 */
int GL_Entity::GetH() const
{
    return m_CollisionBox.h;
}

/**
 * Get entity width.
 *
 * @return Width
 */
int GL_Entity::GetW() const
{
    return m_CollisionBox.w;
}

/**
 * Get entity x-coordinate
 *
 * @return X-coordinate
 */
float GL_Entity::GetX() const
{
    return m_Position.x;
}

/**
 * Get entity y-coordinate
 *
 * @return Y-coordinate
 */
float GL_Entity::GetY() const
{
    return m_Position.y;
}

/**
 * Retrieves the current rotation angle.
 *
 * @return Angle
 */
float GL_Entity::GetRotation() const
{
    return m_rotation;
}

/**
 * Retrieves the current texture id.
 *
 * @return int The texture.
 */
u_int GL_Entity::GetTexture() const
{
    return m_texture;
}

/**
 * Retrieves the filename, if the entity was loaded from a file.
 *
 * @return The filename.
 */
const std::string& GL_Entity::GetFilename() const
{
    return m_filename;
}

/**
 * Gets position
 *
 * @return ML_Vector2 position.
 *
 * @see math::ML_Vector2
 */
const math::ML_Vector2& GL_Entity::GetPosition() const
{
    return m_Position;
}

/**
 * Gets the collision box.
 *
 * @return ML_Rect collision box
 *
 * @see math::ML_Rect
 */
const math::ML_Rect& GL_Entity::GetCollisionBox() const
{
    return m_CollisionBox;
}

/**
 * Renders the entity on-screen.
 */
void GL_Entity::Render() const
{
    // Alpha channel is gucci
    GLboolean blend = glIsEnabled(GL_BLEND);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Render the primitive with the texture on top.
    // First, rotate around the origin.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(this->GetX() + this->GetW() / 2,
        this->GetY() + this->GetH() / 2, 0.0f);
    glRotatef(-m_rotation, 0.0f, 0.0f, 1.0f);
    glTranslatef(-this->GetX() - this->GetW() / 2,
        -this->GetY() - this->GetH() / 2, 0.0f);
    
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Draw the primitive
    glBegin(GL_QUADS);
        // Top left corner
        glTexCoord2f(m_RenderDimensions.x, m_RenderDimensions.y);
        glVertex2f(m_Position.x, m_Position.y);
        
        // Top right corner
        glTexCoord2f(m_RenderDimensions.x + m_RenderDimensions.w,
            m_RenderDimensions.y);
        glVertex2f(m_Position.x + m_CollisionBox.w, m_Position.y);
        
        // Bottom right corner
        glTexCoord2f(m_RenderDimensions.x + m_RenderDimensions.w,
            m_RenderDimensions.y + m_RenderDimensions.h);
        glVertex2f(m_Position.x + m_CollisionBox.w, m_Position.y + m_CollisionBox.h);
        
        // Bottom left corner
        glTexCoord2f(m_RenderDimensions.x, m_RenderDimensions.y + m_RenderDimensions.h);
        glVertex2f(m_Position.x, m_Position.y + m_CollisionBox.h);
    glEnd();

    // Disable blending if it was disabled originally
    if(!blend)
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * Updates the entity by animating and rendering it.
 */
void GL_Entity::Update()
{
    this->Render();
}

/**
 * Fades entities off of the screen.
 *
 * Should be called within a game loop, once per frame.
 * 
 * @param std::vector<GL_Entity*> Entities to fade out
 * @param float Rate to fade out at, default is 0.01 (optional)
 *
 * @return bool TRUE when done fading, FALSE otherwise.
 */
bool gfx::FadeOut(const std::vector<GL_Entity*> pentitiesToFade,
    const float rate)
{
    static float alpha = 1.0f;

    glColor4f(1, 1, 1, alpha);
    for(size_t i = 0; i < pentitiesToFade.size(); ++i)
    {
        pentitiesToFade[i]->Update();
    }
    glColor4f(1, 1, 1, 1);

    alpha -= rate;

    if(alpha < 0.0f)
    {
        alpha = 1.0f;
        return true;
    }
    else
        return false;
}

bool gfx::FadeOut(asset::GL_Entity* p_ToFade, const float rate)
{
    static float alpha = 1.0f;

    glColor4f(1, 1, 1, alpha);
    p_ToFade->Update();
    glColor4f(1, 1, 1, 1);

    alpha -= rate;

    if(alpha < 0.0f)
    {
        alpha = 1.0f;
        return true;
    }
    else
        return false;
}

/**
 * Fades entities into the screen.
 *
 * Should be called within a game loop, once per frame.
 * 
 * @param std::vector<GL_Entity*> Entities to fade in
 * @param float Rate to fade in at, default is 0.01 (optional)
 *
 * @return bool TRUE when done fading, FALSE otherwise.
 */
bool gfx::FadeIn(const std::vector<GL_Entity*> pentitiesToFade,
    const float rate)
{
    static float alpha = 0.0f;

    glColor4f(1, 1, 1, alpha);
    for(size_t i = 0; i < pentitiesToFade.size(); ++i)
    {
        pentitiesToFade[i]->Update();
    }
    glColor4f(1, 1, 1, 1);

    alpha += rate;

    if(alpha > 1.0f)
    {
        alpha = 0.0f;
        return true;
    }
    else
        return false;
}

bool gfx::FadeIn(asset::GL_Entity* p_ToFade, const float rate)
{
    static float alpha = 0.0f;

    glColor4f(1, 1, 1, alpha);
    p_ToFade->Update();
    glColor4f(1, 1, 1, 1);

    alpha += rate;

    if(alpha > 1.0f)
    {
        alpha = 0.0f;
        return true;
    }
    else
        return false;
}