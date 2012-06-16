#include "Engine.h"

#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDL_mixer.lib")

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    gk::CEngine TankAssault_GL;
    TankAssault_GL.Run();
    
    std::cerr << "Last error (if any) was: " << SDL_GetError() << std::endl;

    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}