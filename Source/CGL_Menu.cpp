#include "CGL_Menu.h"

using namespace gk_gl;

/* CGL_Menu public methods */
CGL_Menu::CGL_Menu() : Background(NULL), Title(NULL),
    Menu_Music(NULL), On_Hover(NULL), playing_music(false)
{
}

CGL_Menu::~CGL_Menu()
{
    for(size_t i = 0; i < this->Buttons.size(); i++)
    {
        delete this->Buttons[i];
    }

    this->Buttons.clear();
}

void CGL_Menu::SetBackground(const char* filename)
{
    if(this->Background != NULL)    // Gotta delete the old one
        delete this->Background;
        
    this->Background = new CGL_Entity();
    this->Background->LoadEntity(filename);
}

void CGL_Menu::SetBackground(CGL_Entity& Background)
{
    if(this->Background != NULL)
        delete this->Background;
        
    this->Background = &Background;
}

void CGL_Menu::SetMusic(const char* filename)
{
    if(this->Menu_Music != NULL)
        Mix_FreeMusic(this->Menu_Music);

    this->Menu_Music = Mix_LoadMUS(filename);
}

void CGL_Menu::SetMusic(Mix_Music* Music)
{
    if(this->Menu_Music != NULL)
        Mix_FreeMusic(this->Menu_Music);
        
    this->Menu_Music = Music;
}

void CGL_Menu::SetHoverSound(const char* filename)
{
    if(this->On_Hover != NULL)
        Mix_FreeChunk(this->On_Hover);

    this->On_Hover = Mix_LoadWAV(filename);
}

void CGL_Menu::SetHoverSound(Mix_Chunk* Chunk)
{
    if(this->On_Hover != NULL)
        Mix_FreeChunk(this->On_Hover);

    this->On_Hover = Chunk;
}

void CGL_Menu::SetTitle(const char* font_name, const int size,
            const SDL_Color& Color, const GL_Vertex2f& Position,
            const char* title)
{
    TTF_Font* Font = TTF_OpenFont(font_name, size);
    SDL_Surface* Text = gk_sdl::render_text(Font, (std::string)title, 
        NULL, Color, CREATE_SURFACE | TRANSPARENT_BG);

    if(this->Title != NULL)
        delete this->Title;
        
    this->Title = new CGL_Entity();
    this->Title->SetEntity(Text);
    this->Title->Move(Position);
    
    SDL_FreeSurface(Text);
    TTF_CloseFont(Font);
}

void CGL_Menu::SetTitle(CGL_Entity& Title)
{
    this->Title = &Title;
}

void CGL_Menu::SetTitle(TTF_Font* Font, const SDL_Color& Color,
            const GL_Vertex2f& Position, const char* title)
{
    SDL_Surface* Text = gk_sdl::render_text(Font, (std::string)title, 
        NULL, Color, CREATE_SURFACE | TRANSPARENT_BG);
        
    if(this->Title != NULL)
        delete this->Title;
        
    this->Title = new CGL_Entity();
    this->Title->SetEntity(Text);
    this->Title->Move(Position);
    
    SDL_FreeSurface(Text);
}

int CGL_Menu::AddMenuItem(const GL_Vertex2f& Position, const char* normal, const char* highlighted)
{
    GL_MenuItem* Tmp    = new GL_MenuItem();
    CGL_Entity* Norm    = new CGL_Entity();
    CGL_Entity* High    = new CGL_Entity();
    
    Norm->SetEntity(gk_sdl::LoadImage_Alpha(normal));
    Norm->Move(Position);
    
    High->SetEntity(gk_sdl::LoadImage_Alpha(highlighted));
    High->Move(Position);
    
    Tmp->touch  = false;
    Tmp->Normal = Norm;
    Tmp->High   = High;
    Tmp->Active = Norm;
    Tmp->id     = 69 + this->Buttons.size();
    
    this->Buttons.push_back(Tmp);    
    return Tmp->id;
}

int CGL_Menu::AddMenuItem(CGL_Entity& Normal, CGL_Entity& Highlighted)
{
    GL_MenuItem* Tmp = new GL_MenuItem();
    
    Tmp->touch  = false;
    Tmp->Normal = &Normal;
    Tmp->High   = &Highlighted;
    Tmp->Active = &Normal;
    Tmp->id     = 69 + this->Buttons.size();
    
    this->Buttons.push_back(Tmp); 
    return Tmp->id;
}

void CGL_Menu::PlayMusic()
{
    if(!this->playing_music)
    {
        this->playing_music = true;
        Mix_PlayMusic(this->Menu_Music, 0);
    }
}

void CGL_Menu::PauseMusic()
{
    if(this->playing_music)
    {
        this->playing_music = false;
        Mix_PauseMusic();
    }
}

int CGL_Menu::Update()
{
    if(this->Background != NULL)
        this->Background->Update();

    if(this->Title != NULL)
        this->Title->Update();
        
    for(size_t i = 0; i < this->Buttons.size(); i++)
    {
        this->Buttons[i]->Active->Update();
    }
    
    return this->CheckMouseover();
}

/* CGL_Menu private methods */
int CGL_Menu::CheckMouseover()
{
    static int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    for(size_t i = 0; i < this->Buttons.size(); i++)
    {
        if(this->Buttons[i]->Active->CheckCollision(mouse_x, mouse_y))
        {
            if(!this->Buttons[i]->touch)
            {
                this->Buttons[i]->touch = true;
                Mix_PlayChannel(-1, this->On_Hover, 0);
            }

            this->Buttons[i]->Active = this->Buttons[i]->High;
            if(IsPressed(SDL_BUTTON_LEFT))
                return this->Buttons[i]->id;
        }
        else
        {
            this->Buttons[i]->touch = false;
            this->Buttons[i]->Active = this->Buttons[i]->Normal;
        }
    }

    return -1;
}