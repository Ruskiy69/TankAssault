/* MediaManager.h
 *
 * The following classes provide wrappers for the SDL_ttf,
 * SDL_image, SDL_mixer libraries.
 *
 * The dependencies are (obviously) SDL, SDL_ttf, SDL_image, 
 * and SDL_mixer. Link to them in your project settings or
 * as a pragma, they are NOT linked OR initialized here.
 */
 
#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

/* Vector2 */
#include "Math/MathHelper.h"

#include "Errors.h"
#include "Graphics/GL_Entity.h"
#include "Graphics/GL_Display.h"

/* All the elements of the MediaManager */
#include "Media/Font.h"
#include "Media/Music.h"
#include "Media/SoundEffect.h"
#include "Media/MusicPlayer.h"

#endif // MEDIA_MANAGER_H