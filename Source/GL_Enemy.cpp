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

    this->SetupAI();
    this->FindPath();
}

CGL_Enemy::~CGL_Enemy()
{}

void CGL_Enemy::Update()
{
    this->Move_Rate(this->Map.GetPanRate());
    //this->UpdateAI();
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

void CGL_Enemy::SetupAI()
{
    this->ResetLists();

    this->Start = new CNode();
    this->Start->Tile = this->Map.FindTile(this->Position);
    this->Start->x = this->Start->Tile->x;
    this->Start->y = this->Start->Tile->y;

    this->Destination = new CNode();
    this->Destination->Tile = this->Map.FindTile(this->Player.GetPosition());
    this->Destination->x = this->Destination->Tile->x;
    this->Destination->y = this->Destination->Tile->y;

    this->Open_List.push_back(*this->Start);
    this->Closed_List.push_back(*this->Start);
    this->Path.push_back(*this->Start);
}

void CGL_Enemy::ResetLists()
{
    this->Open_List.clear();
    this->Closed_List.clear();
}

void CGL_Enemy::FindPath()
{
    while(!this->Open_List.empty())
    {
        for(size_t i = 0; i < this->Open_List.size(); i++)
        {
            if(this->Open_List[i].Tile->x == this->Destination->Tile->x &&
                this->Open_List[i].Tile->y == this->Destination->Tile->y)
            {
                return;
            }

            std::priority_queue<CNode> Node_Queue;

            for(int x = -32; x <= 32; x += 32)
            {
                for(int y = -32; y <= 32; y += 32)
                {
                    /* Check each adjacent node */
                    const int node_x = this->Open_List[i].x + x;
                    const int node_y = this->Open_List[i].y + y;

                    if(node_x == this->Destination->x && node_y == this->Destination->y)
                        return;

                    bool checked = false;
                    for(size_t j = 0; j < this->Closed_List.size(); j++)
                    {
                        if(node_x == this->Closed_List[i].x &&
                            node_y == this->Closed_List[i].y)
                        {
                            checked = true;
                            break;
                        }
                    }

                    if(checked)
                        continue;

                    gk::Tile* Wall = this->Map.FindTile(node_x+1, node_y+1);
                    if(Wall->type == gk::Tile::Wall)
                        continue;

                    const int x_diff = abs(node_x - this->Open_List[i].x);
                    const int y_diff = abs(node_y - this->Open_List[i].y);

                    const int dir = (x_diff >= 32 && y_diff >= 32) ? 45 : 32;

                    const int cost = (abs(node_x - this->Destination->x) + abs(node_y - this->Destination->y));

                    Node_Queue.push(CNode(node_x, node_y, cost, Wall));
                }
            }

            if(!Node_Queue.empty())
            {
                this->Open_List.push_back(Node_Queue.top());                
                this->Path.push_back(Node_Queue.top());

                while(!Node_Queue.empty())
                {
                    this->Closed_List.push_back(Node_Queue.top());
                    Node_Queue.pop();
                }
            }
        }
    }
}

void CGL_Enemy::UpdateAI()
{
    static int index = 0;
    if(index >= this->Path.size() - 1)
    {
        this->SetupAI();
        this->FindPath();
        index = 0;
    }

    CNode* Current_Node = &this->Path[index+1];
    
    if(Current_Node->x > this->GetX())
        this->Move_Rate(3.0f, 0.0f);
    else
        this->Move_Rate(-3.0f, 0.0f);

    if(Current_Node->y > this->GetY())
        this->Move_Rate(0.0f, 3.0f);
    else
        this->Move_Rate(0.0f, -3.0f);

    GL_Rect Next(this->Path[index+1].x, this->Path[index+1].y, 32, 32);

    if(gk_gl::detect_collision(Next, this->GetCollisionBox()))
        index++;
}