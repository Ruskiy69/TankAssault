#include <fstream>

#include "Math/Matrix.h"
#include "Engine.h"

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")
#pragma comment(lib, "glew32.lib")

#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDL_mixer.lib")
#pragma comment(lib, "SDL_gfx.lib")

int main(int argc, char* argv[])
{
    std::ofstream log("Last_run.log", std::ios::out);

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    Game::Game_Engine Collapse;
    Collapse.Run();

    log << "OpenGL version: " << (char*)glGetString(GL_VERSION) << "\n";
    log << "OpenGL renderer: " << (char*)glGetString(GL_RENDERER) << "\n";
    log << "OpenGL vendor: " << (char*)glGetString(GL_VENDOR) << "\n";

    log << "Last error (if any) was: " << SDL_GetError() << "\n";
    log << "Last OpenGL error (if any) was: " << (char*)gluErrorString(glGetError()) << "\n";
    log.close();

    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    /* For some strange reason, after the 
     * application is done, there's a segfault.
     * There may be a memory leak somewhere in
     * this 3K line project, and I'm not looking
     * for it. Fuck that.
     */
    return 0;
}