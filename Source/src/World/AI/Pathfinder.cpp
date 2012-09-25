/**
 * @file
 *  Implementation of the CPathfinder class.
 *
 * @author George Kudrayvtsev
 * @version 1.1.2
 **/

#include "World/AI/Pathfinder.hpp"

using ai::CPathfinder;

/**
 * Finds the shortest path to a destination using A*.
 *
 * @param obj::CEntity* The tile to start from
 * @param obj::CEntity* The tile to end at
 *
 * @return TRUE if a path was found, FALSE if not.
 *  If a complete path is not found, what's done is still stored.
 *
 * @see http://www.policyalmanac.org/games/aStarTutorial.htm
 * @see game::CTerrainMap
 * @see game::CTerrainMap::FindTile(const math::CVector2&)
 *
 * @todo Remove corner-cutting when path-finding
 * @todo Make sure the tank can actually fit in the path
 * @todo Fix slight issues with not finding the shortest path
 **/
bool CPathfinder::FindPath(obj::CGameObject* pStart_Tile,
    obj::CGameObject* pEnd_Tile)
{
    // Clear previous path
    mp_Path.clear();

    // Set up lists
    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    // Add start node to open list
    Node* pStart = new Node;
    memset(pStart, 0, sizeof Node);
    pStart->pTile = pStart_Tile;
    openList.push_back(pStart);

    while(openList.size() > 0)
    {
        // Find node with lowest cost in open list
        int index = 0;
        int min_cost = openList[index]->cost;
        for(size_t i = 1; i < openList.size(); ++i)
        {
            if(min_cost > openList[i]->cost)
            {
                min_cost = openList[i]->cost;
                index = i;
            }
        }
        Node* pCurrent_Node = openList[index];

        // Remove node from open list and add to closed list.
        for(std::vector<Node*>::iterator i = openList.begin();
            i != openList.end(); /* no third **/)
        {
            if(pCurrent_Node == (*i))
            {
                i = openList.erase(i);
                break;
            }
            else
                ++i;
        }
        closedList.push_back(pCurrent_Node);

        // Is this the destination?
        if(pCurrent_Node->pTile == pEnd_Tile)
            break;

        // Iterate through the adjacent nodes
        for(int x = -1; x <= 1; x++)
        {
            for(int y = -1; y <= 1; y++)
            {
                // Find tile in current (x, y) position
                obj::CGameObject* pTile = NULL;
                pTile = mp_Level->GetTerrainMap().FindTile(
                    pCurrent_Node->pTile->GetX() + 1 + (32 * x),
                    pCurrent_Node->pTile->GetY() + 1 + (32 * y));

                // No tile?
                if(pTile == NULL)
                {
                    continue;
                }

                // Is it impassable?
                math::CRect Checker(pCurrent_Node->pTile->GetCollisionBox());
                Checker.Resize(34, 34);
                Checker.Move(Checker.x - 1, Checker.y - 1);
                if(mp_Level->GetCollisionMap().FindTile(Checker)
                    /*pCurrent_Node->pTile->GetX() + (64 * x),
                    pCurrent_Node->pTile->GetY() + (64 * y))**/ != NULL)
                {
                    continue;
                }

                // Is it impossible to get to without hitting a wall?
                // Create a rect that's midway between the parent and the 
                // adjacent node.
                /// @todo Make cutting corners illegal.
                /*
                math::Rect Midpt;
                Midpt.x = (pCurrent_Node->pTile->GetX() + pTile->GetX()) / 2;
                Midpt.y = (pCurrent_Node->pTile->GetY() + pTile->GetY()) / 2;
                Midpt.Resize(31, 31);   // Edge case, rect collision problem
                if(mp_Level->GetCollisionMap().FindTile(Midpt) != NULL)
                    continue;
                    **/

                // Is it in the closed list?
                bool is_closed = false;
                for(size_t i = 0; i < closedList.size(); ++i)
                {
                    if(pTile == closedList[i]->pTile)
                    {
                        is_closed = true;
                        break;
                    }
                }
                if(is_closed)
                    continue;

                // Is it in the open list?
                bool is_open = false;
                for(size_t i = 0; i < openList.size(); ++i)
                {
                    if(pTile == openList[i]->pTile)
                    {
                        // Is it better?
                        if(openList[i]->cost < (abs(pEnd_Tile->GetX() - pTile->GetX()) / 32) + 
                            (abs(pEnd_Tile->GetY() - pTile->GetY()) / 32) + openList[i]->pParent->move_count + 1)
                        {
                            openList[i]->pParent = pCurrent_Node;
                            openList[i]->move_count = openList[i]->pParent->move_count + 1;
                            openList[i]->heuristic  = (abs(pEnd_Tile->GetX() - pTile->GetX()) / 32) + 
                                (abs(pEnd_Tile->GetY() - pTile->GetY()) / 32);
                            openList[i]->cost = openList[i]->move_count + openList[i]->heuristic;
                        }
                        is_open = true;
                        break;
                    }
                }

                // Not in open list?
                if(!is_open)
                {
                    // Create a new node.
                    Node* pTmp = new Node;
                    memset(pTmp, 0, sizeof Node);

                    pTmp->pParent   = pCurrent_Node;
                    pTmp->pTile     = pTile;

                    pTmp->heuristic = (abs(pEnd_Tile->GetX() - pTile->GetX()) / 32) + 
                            (abs(pEnd_Tile->GetY() - pTile->GetY()) / 32);
                    pTmp->move_count= pTmp->pParent->move_count + 1;
                    pTmp->cost      = pTmp->heuristic + pTmp->move_count;

                    openList.push_back(pTmp);
                }
            }
        }
    }

    // Final path backwards from end to start following parents.
    for(Node* pNode = closedList.back(); pNode != NULL; pNode = pNode->pParent)
    {
        mp_Path.push_back(pNode->pTile);
    }

    // If we didn't find a path
    if(openList.empty())
    {
        // Clean up lists
        for(size_t i = 0; i < closedList.size(); ++i)
            delete closedList[i];
        return false;
    }
    else
    {
        // Clean up lists
        for(size_t i = 0; i < closedList.size(); ++i)
            delete closedList[i];
        for(size_t i = 0; i < openList.size(); ++i)
            delete openList[i];
    }

    m_current_node = 0;
    return true;
}

void CPathfinder::ShowPath()
{
    SDL_Surface* pRender = gfx::create_surface_alpha(32, 32, gfx::RED);
    
    for(int i = mp_Path.size() - 1; i >= 0; --i)
    {
        if(i == 0)
            gfx::fill_rect(pRender, NULL, gfx::PURPLE);
        else if(i < m_current_node)
            gfx::fill_rect(pRender, NULL, gfx::GREEN);
        else
            gfx::fill_rect(pRender, NULL, gfx::RED);

        obj::CEntity* pNew = new obj::CEntity;
        pNew->LoadFromSurface(pRender);
        pNew->Move(mp_Path[i]->GetPosition());
        pNew->Update();
        /// @todo Remove tile
        //delete pNew;
    }
    
    gfx::fill_rect(pRender, NULL, gfx::BLUE);

    if(m_current_node < mp_Path.size())
    {
        obj::CEntity* pNew = new obj::CEntity;
        pNew->LoadFromSurface(pRender);
        pNew->Move(mp_Path[m_current_node]->GetPosition());
        pNew->Update();
        delete pNew;
    }

    SDL_FreeSurface(pRender);
}

obj::CGameObject* CPathfinder::NextTile()
{
    if(m_current_node >= mp_Path.size())
        return NULL;
    else
    {
        obj::CGameObject* pFinal = mp_Path[m_current_node];
        ++m_current_node;
        return pFinal;
    }
}

const math::CVector2& CPathfinder::GetNextDestination() const
{
    if(m_current_node + 1 >= mp_Path.size())
        return this->GetCurrentDestination();
    else
        return mp_Path[m_current_node + 1]->GetPosition();
}

const math::CVector2& CPathfinder::GetPrevDestination() const
{
    if(m_current_node - 1 < 0)
        return this->GetCurrentDestination();
    else
        return mp_Path[m_current_node - 1]->GetPosition();
}

const math::CVector2& CPathfinder::GetCurrentDestination() const
{
    return mp_Path[m_current_node]->GetPosition();
}

void CPathfinder::ReversePath()
{
    std::vector<obj::CGameObject*> p_reversedPath;

    for(size_t i = mp_Path.size() - 1; i > 0; --i)
    {
        p_reversedPath.push_back(mp_Path[i]);
    }

    mp_Path = p_reversedPath;
    m_current_node = 0;
}
