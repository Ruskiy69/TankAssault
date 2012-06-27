#include "Graphics/GL_Menu.h"

using Game::GL_Menu;

/* GL_Menu public methods */
GL_Menu::GL_Menu() : Background(NULL), Title(NULL),
    On_Hover(NULL), playing_music(false), offset(0) {}

GL_Menu::~GL_Menu()
{
    for(size_t i = 0; i < this->Buttons.size(); i++)
    {
        delete this->Buttons[i]->High;
        delete this->Buttons[i]->Normal;
        delete this->Buttons[i];
    }

    this->Buttons.clear();

    if(this->Title)
        delete this->Title;

    if(this->On_Hover)
        delete this->On_Hover;
}

void GL_Menu::SetBackground(const char* filename)
{
    if(this->Background != NULL)    // Gotta delete the old one
        delete this->Background;

    this->Background = new GFX::GL_Entity();
    this->Background->LoadEntity(filename);
}

void GL_Menu::SetBackground(GFX::GL_Entity& Background)
{
    if(this->Background != NULL)
        delete this->Background;

    this->Background = &Background;
}

void GL_Menu::SetHoverSound(const char* filename)
{
    if(this->On_Hover != NULL)
        delete this->On_Hover;

    this->On_Hover = new MediaManager::SoundEffect;
    this->On_Hover->Load(filename);
    this->On_Hover->SetVolume(69);
}

void GL_Menu::SetHoverSound(MediaManager::SoundEffect& On_Hover)
{
    if(this->On_Hover != NULL)
        delete this->On_Hover;

    this->On_Hover = &On_Hover;
}

void GL_Menu::SetTitle(const char* font_name, const int size,
    const SDL_Color& Color, const MathHelper::Vector2& Position,
            const char* title)
{
    MediaManager::Font TextFont;
    TextFont.Load(font_name, size);

    if(this->Title != NULL)
        delete this->Title;

    this->Title = TextFont.RenderText(std::string(title), Color);
    this->Title->Move(Position);
}

void GL_Menu::SetTitle(GFX::GL_Entity& Title)
{
    this->Title = &Title;
}

void GL_Menu::SetTitle(const MediaManager::Font& TitleFont, const GFX::Color& Color,
            const MathHelper::Vector2& Position, const char* title)
{
    if(this->Title != NULL)
        delete this->Title;

    this->Title = TitleFont.RenderText(title, Color);
    this->Title->Move(Position);
}

void GL_Menu::Shift(const int pixels)
{
    this->offset += pixels;

    for(size_t i = 0; i < this->Buttons.size(); i++)
    {
        this->Buttons[i]->Normal->Move_Rate(pixels, 0);
        this->Buttons[i]->High->Move_Rate(pixels, 0);
    }

    this->Title->Move_Rate(pixels, 0);
}

int GL_Menu::AddMenuItem(const MathHelper::Vector2& Position, const char* normal, const char* highlighted)
{
    GL_MenuItem* Tmp        = new GL_MenuItem();
    GFX::GL_Entity* Norm    = new GFX::GL_Entity();
    GFX::GL_Entity* High    = new GFX::GL_Entity();

    Norm->LoadEntity(normal);
    Norm->Move(Position);

    High->LoadEntity(highlighted);//SetEntity(GFX::load_image_alpha(highlighted));
    High->Move(Position);

    Tmp->touch  = false;
    Tmp->Normal = Norm;
    Tmp->High   = High;
    Tmp->Active = Norm;
    Tmp->id     = 69 + this->Buttons.size();

    this->Buttons.push_back(Tmp);    
    return Tmp->id;
}

int GL_Menu::AddMenuItem(GFX::GL_Entity& Normal, GFX::GL_Entity& Highlighted)
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

int GL_Menu::Update()
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

/* GL_Menu private methods */
int GL_Menu::CheckMouseover()
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
                this->On_Hover->Play();
            }

            this->Buttons[i]->Active = this->Buttons[i]->High;
            if(Game::IsPressed(SDL_BUTTON_LEFT))
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