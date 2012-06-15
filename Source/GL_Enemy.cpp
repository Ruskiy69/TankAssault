#include "GL_Enemy.h"

using gk_gl::CGL_Enemy;

CGL_Enemy::CGL_Enemy(const gk::CTimer& Timer,
    const gk_gl::CGL_Player& Player,
    const gk::CMap& Map) : Map(Map), Player(Player), Tiles(Map.GetTiles())
{
    this->Move(100, 100);

    SDL_Surface* Entity = gk_sdl::LoadImage_Alpha("Data/Images/Enemy_1.png");
    this->SetEntity(Entity);
    this->SetAnimationRate(Timer.GetFrameRate() / 10);
    this->Resize(64, 64);

    this->InitAI();
}

CGL_Enemy::~CGL_Enemy()
{}

void CGL_Enemy::Update()
{
    this->Move_Rate(this->Map.GetPanRate());
    this->AI();
    this->Render();
}

void CGL_Enemy::Render()
{
    /* Alpha channel is gucci */
    GLboolean blend = glIsEnabled(GL_BLEND);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Render the primitive with the texture on top.
     * First, rotate around the origin.
     */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(this->GetX() + this->GetW() / 2,
        this->GetY() + this->GetH() / 2, 0.0f);
    glRotatef(-this->rotation, 0.0f, 0.0f, 1.0f);
    glTranslatef(-this->GetX() - this->GetW() / 2,
        -this->GetY() - this->GetH() / 2, 0.0f);
    
    /* Then bind the texture */
    glBindTexture(GL_TEXTURE_2D, this->texture);

    /* Draw the primitive */
    glBegin(GL_QUADS);
        glTexCoord2f(0,     0); glVertex2f(this->Position.x, this->Position.y);
        glTexCoord2f(0.5f,  0); glVertex2f(this->Position.x + this->CollisionBox.w, this->Position.y);
        glTexCoord2f(0.5f,  1); glVertex2f(this->Position.x + this->CollisionBox.w, this->Position.y + this->CollisionBox.h);
        glTexCoord2f(0,     1); glVertex2f(this->Position.x, this->Position.y + this->CollisionBox.h);
    glEnd();

    /* Disable blending if it was disabled originally */
    if(!blend)
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }

    glPopMatrix();
}

void CGL_Enemy::InitAI()
{
    /* Basically set a destination for the 
     * pathfinding algorithm (A*) and generate
     * nodes based on map data.
     */
    this->RenewLists();
    this->Destination = this->Player.GetPosition();

    for(size_t i = 0; i < this->Tiles.size(); i++)
    {
        if(this->Tiles[i]->type != gk::Tile::Wall)
        {
            Node* Tmp_Node = new Node();
            Tmp_Node->Tile = this->Tiles[i];
            Tmp_Node->f = Tmp_Node->g = Tmp_Node->h = 0;
            this->Open_List.push_back(Tmp_Node);
        }
    }

    /* Remove the tile the enemy is currently
     * standing on from the open list, and
     * add it to the closed list.
     * I use the index 'current_tile' because
     * technically, after initial creation
     * of the Open_List, the index of the tiles
     * matches with the index of the Open_List Nodes.
     * An alternative, and more reliable, method would
     * be to use gk::CMap::FindTile() to find the Tile*
     * and then loop through Open_List to find the
     * matching Node::Tile* element. Currently the
     * gk::CMap::FindTile_Index() method causes no issues.
     */
    int current_tile = this->Map.FindTile_Index(this->Position);
    Node* Tmp_Node = new Node();
    Tmp_Node->Tile = this->Tiles[current_tile];
    Tmp_Node->f = Tmp_Node->g = Tmp_Node->h = 0;
    
    delete this->Open_List[current_tile];
    this->Open_List[current_tile] = 0;
}

void CGL_Enemy::RenewLists()
{
    for(size_t i = 0; i < this->Open_List.size(); i++)
        if(this->Open_List[i])
            delete this->Open_List[i];

    for(size_t i = 0; i < this->Closed_List.size(); i++)
        if(this->Closed_List[i])
            delete this->Closed_List[i];

    this->Open_List.clear();
    this->Closed_List.clear();
}

void CGL_Enemy::AI()
{
    do
    {

    }
    while(0);
}