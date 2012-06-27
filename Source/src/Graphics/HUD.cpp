#include "Graphics/HUD.h"

using Game::Game_HUD;
using Game::Game_HUDElement;

Game_HUDElement::Game_HUDElement(Game::Element_Type Type, MediaManager::Font& Font,
    const int initial_value) : MainFont(Font),
    initial_value(initial_value), 
    value(initial_value), HoverEntity(NULL), Type(Type)
{
    if(this->initial_value >= 100)
        this->initial_value = 100;
    else if(this->initial_value <= 0)
        this->initial_value = 0;
    else
        this->initial_value = initial_value;

    this->value = this->initial_value;
}

Game_HUDElement::~Game_HUDElement()
{
    if(this->HoverEntity)
        delete this->HoverEntity;


}

void Game_HUDElement::Move(const MathHelper::Vector2& Pos)
{
    this->ElementEntity.Move(Pos);
}

void Game_HUDElement::OnHover()
{
    if(!this->HoverEntity)
        return;

    static int x = 0, y = 0;
    Game::GetMousePosition(x, y);

    this->HoverEntity->Move(x+20.0f, 1.0f*y);
    this->HoverEntity->Update();
}

void Game_HUDElement::SetName(const std::string& name)
{
    this->prefix = name;
}

void Game_HUDElement::SetBackground(SDL_Surface* Background)
{
    /* Background is purely used for menus. */
    if(this->Type == Game::Menu)
        this->CreateMenu(Background);
}

void Game_HUDElement::SetHoverEntity(GFX::GL_Entity* OnHover)
{
    if(OnHover == NULL && this->Type == Game::Bar)
    {
        this->MainFont.Resize(12);
        this->HoverEntity = this->MainFont.RenderText(this->prefix, this->ElementColor);
    }
    else
        this->HoverEntity = OnHover;
}

void Game_HUDElement::SetColor(const GFX::Color& Color)
{
    this->ElementColor = Color;
}

void Game_HUDElement::UpdateValue(const int new_value)
{
    this->value = new_value;

    if(this->Type == Game::Bar)
        this->CreateBar();
    else if(this->Type == Game::Score)
        this->ChangeScore();
}

void Game_HUDElement::Update()
{
    this->ElementEntity.Update();
}

bool Game_HUDElement::CheckCollision(const int x, const int y)
{
    return this->ElementEntity.CheckCollision(x, y);
}

void Game_HUDElement::CreateBar()
{
    SDL_Surface* Bar_Image = GFX::load_image_alpha("Data/Images/HUD_Bar.png");
    SDL_Surface* Filler    = NULL;

    SDL_Rect Filler_Rect;

    float w = 0.0f;

    /* We cannot make a bar with a -1 length */
    if(this->value < 0)
        this->value = 0;

    w = Bar_Image->w * (this->value / 100.0f);
    if(w <= 0.1f) w = 1.0f;
    
    Filler = GFX::create_surface_alpha(Bar_Image->w, Bar_Image->h);
    SDL_SetAlpha(Bar_Image, 0, 0);
    
    Filler_Rect.x = Filler_Rect.y = 0;
    Filler_Rect.w = (int)w;
    Filler_Rect.h = Bar_Image->h;

    GFX::fill_rect(Filler, Filler_Rect, this->ElementColor);
    SDL_gfxBlitRGBA(Bar_Image, NULL, Filler, &Filler_Rect);

    this->ElementEntity.SetEntity(Filler);

    SDL_FreeSurface(Filler);
    SDL_FreeSurface(Bar_Image);
}

void Game_HUDElement::ChangeScore()
{
    static std::stringstream score_ss;
    
    score_ss << this->prefix << this->value;

    SDL_Surface* Tmp = this->MainFont.RenderText_SDL(score_ss.str(), this->ElementColor);
    this->ElementEntity.SetEntity(Tmp);
    SDL_FreeSurface(Tmp);
}

void Game_HUDElement::CreateMenu(SDL_Surface* BG)
{
    SDL_Surface* Text = this->MainFont.RenderText_SDL(this->prefix, this->ElementColor);

    if(BG != NULL)
    {
        SDL_Rect Spot = {0, 0, 0, 0};
        SDL_gfxBlitRGBA(Text, NULL, BG, &Spot);
    }
    else
    {
        this->ElementEntity.SetEntity(Text);
    }
    
    SDL_FreeSurface(Text);
}

/*********************************************
 * Game_HUD class methods
 *********************************************/

void Game_HUD::Init(MediaManager::Font& Font, MediaManager::Font::f_size hover_font_size)
{
    this->HUDFont = &Font;
    this->hud_font_size = this->HUDFont->GetSize();
    this->hover_font_size = hover_font_size;
}

Game_HUD::~Game_HUD()
{
    SDL_FreeSurface(this->MenuBG);
}

void Game_HUD::SetMenuBackground(SDL_Surface* BG)
{
    this->MenuBG = BG;

    for(size_t i = 0; i < this->Elements.size(); i++)
        delete this->Elements[i];

    this->Elements.clear();
}

Game::element_id Game_HUD::AddHUDElement(const Game::Element_Type Type,
    const MathHelper::Vector2& Position,
    const std::string& name, const GFX::Color& Color, 
    const int initial_value)
{
    Game_HUDElement* Element = new Game_HUDElement(
        Type, *this->HUDFont, initial_value);

    Element->Move(Position);
    Element->SetName(name);
    Element->SetColor(Color);
    Element->SetBackground(this->MenuBG);
    Element->UpdateValue(initial_value);
    Element->SetHoverEntity(NULL);
    
    this->Elements.push_back(Element);

    this->HUDFont->Resize(this->hud_font_size);

    return this->Elements.size() - 1;
}

Game::element_id Game_HUD::FindHUDElement(const Game::Element_Type type, 
    const std::string& name)
{
    for(size_t i = 0; i < this->Elements.size(); i++)
    {
        if(this->Elements[i]->GetName() == name && this->Elements[i]->GetType() == type)
            return i;
    }

    return -1;
}

void Game_HUD::UpdateElement(const Game::element_id id, const int new_value)
{
    /* Check if id is in bounds */
    if(id >= this->Elements.size())
        return;

    this->Elements[id]->UpdateValue(new_value);
}

int Game_HUD::Update()
{
    for(size_t i = 0; i < this->Elements.size(); i++)
    {
        this->Elements[i]->Update();
    }

    return this->CheckMouseClick();
}

int Game_HUD::CheckMouseClick()
{
    static int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    for(size_t i = 0; i < this->Elements.size(); i++)
    {
        if(this->Elements[i]->CheckCollision(mouse_x, mouse_y))
        {
            this->Elements[i]->OnHover();

            if(Game::IsPressed(SDL_BUTTON_LEFT))
            {
                printf("Held down\n");
                return i;
            }
        }
    }

    return -1;
}