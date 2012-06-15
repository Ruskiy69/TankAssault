#include "SDL_Base.h"

SDL_Surface* gk_sdl::LoadImage(const char* filename)
{
    /* An obvious check if there even is
     * a filename.
     */
    if(filename == NULL)
        gk::HandleError("No file to load!");

    /* We need to set up our surfaces for
     * the original image and the version
     * optimized for our display.
     */
    SDL_Surface* image     = NULL;
    SDL_Surface* optimized = NULL;

    /* Use the SDL_image library to load
     * the image. This allows us to load many
     * types of image files, like PNG and JPG,
     * rather than be limited to BMP's by
     * the SDL defaults.
     */
    image = IMG_Load(filename);

    /* If the image failed to load, show
     * the user an error message.
     */
    if(!image)
    {
        gk::HandleError(SDL_GetError());
    }

    /* Optimize the image */
    optimized = SDL_DisplayFormat(image);

    /* If we could not optimize it, simply
     * return the unoptimized version, otherwise
     * we can return the optimized image.
     */
    if(!optimized)
    {
        return image;
    }
    else
    {
        SDL_FreeSurface(image);
        return optimized;
    }
}

SDL_Surface* gk_sdl::LoadImage_Alpha(const char* filename)
{
    /* An obvious check if there even is
     * a filename.
     */
    if(filename == NULL)
        gk::HandleError("No file to load!");

    /* This function loads images in the same way
     * that LoadImage() does, except that it will
     * maintain alpha-transparency for images such 
     * as transparent PNG's.
     */
    SDL_Surface* image     = NULL;
    SDL_Surface* optimized = NULL;

    image = IMG_Load(filename);

    if(!image)
    {
        gk::HandleError(SDL_GetError());
    }

    optimized = SDL_DisplayFormatAlpha(image);

    if(!optimized)
        return image;

    else
    {
        SDL_FreeSurface(image);
        return optimized;
    }
}

void gk_sdl::blit_surface(SDL_Surface* Dst, SDL_Surface* Src, const int x, const int y)
{
    /* Blitting is basically placing a surface on another
     * surface at a certain location. 
     */
    SDL_Rect offset = {x, y, 0, 0};

    SDL_BlitSurface(Src, NULL, Dst, &offset);
}

void gk_sdl::blit_surface(SDL_Surface* Dst, SDL_Surface* Src, SDL_Rect& Pos)
{
    /* Blitting is basically placing a surface on another
     * surface at a certain location. 
     */
    SDL_BlitSurface(Src, NULL, Dst, &Pos);
}