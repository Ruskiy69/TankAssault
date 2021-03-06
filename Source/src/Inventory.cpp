#include "Inventory.hpp"

using game::CInventory;
using game::g_Settings;

bool CInventory::Init()
{
    if(!(m_Background.LoadFromFile("Data/Textures/Inventory.png") &&
        m_LargePlayerIMG.LoadFromFile("Data/Textures/Sprites/PlayerTank.png")))
    {
        return false;
    }
    m_LargePlayerIMG.Move(100, 200);

    mp_Font = asset::CAssetManager::Create<asset::CFont>(
        "Data/Fonts/GUIFont.ttf");
    mp_Font->Resize(16);

    return true;
}

void CInventory::Update()
{
    static gfx::Color OffBlue = gfx::create_color(20, 135, 220);
    static std::stringstream ss;

    mp_Font->Resize(16);

    ss.str(std::string());
    ss << m_Player.GetPrimary().GetName() << "  (";
    ss << m_Player.GetPrimary().GetClipCount() << ", ";
    ss << m_Player.GetPrimary().GetAmmoCount() << ")";

    mp_Weapon1Stats = mp_Font->RenderText(ss.str().c_str(), OffBlue);
    mp_Weapon1Stats->Move(440, 65);

    ss.str(std::string());
    ss << m_Player.GetSecondary().GetName() << "  (";
    ss << m_Player.GetSecondary().GetClipCount() << ", ";
    ss << m_Player.GetSecondary().GetAmmoCount() << ")";

    mp_Weapon2Stats = mp_Font->RenderText(ss.str().c_str(), OffBlue);
    mp_Weapon2Stats->Move(440, 240);

    mp_Font->Resize(20);

    ss.str(std::string());
    ss << "Days Survived        : "   << m_Player.GetDaysSurvived() << "\n\n";
    ss << "Mechs Destroyed : " << m_Player.GetKills() << "\n\n";
    ss << "Survivors saved    : " << m_Player.GetSurvivorsSaved();
    
    mp_PlayerStats = mp_Font->RenderText(ss.str().c_str(), OffBlue);
    mp_PlayerStats->Move(435, 455);

    ss.str(std::string());
    ss << "Tower Health: \n";
    ss << "Tank Health: ";

    mp_PlayerHealth = mp_Font->RenderText(ss.str().c_str(), OffBlue);
    mp_PlayerHealth->Move(40, 100);

    ss.str(std::string());
    ss << m_Player.GetTankHealth();

    printf("TEXT: %d\n", mp_Font->GetTextWidth("Tower Health: "));

    mp_TankHealth = mp_Font->RenderText(ss.str().c_str(), 
        color_from_health(m_Player.GetTankHealth()));
    mp_TankHealth->Move(225, 100);

    ss.str(std::string());
    ss << "\n" << m_Player.GetTowerHealth();

    mp_TowerHealth = mp_Font->RenderText(ss.str().c_str(), 
        color_from_health(m_Player.GetTowerHealth()));
    mp_TowerHealth->Move(225, 100);

    m_Background.Update();
    m_LargePlayerIMG.Update();
    mp_Weapon2Stats->Update();
    mp_Weapon1Stats->Update();
    mp_PlayerStats->Update();
    mp_PlayerHealth->Update();
    mp_TowerHealth->Update();
    mp_TankHealth->Update();

    delete mp_Weapon1Stats;
    delete mp_Weapon2Stats;
    delete mp_PlayerStats;
    delete mp_PlayerHealth;
    delete mp_TankHealth;
    delete mp_TowerHealth;
}

gfx::Color game::color_from_health(const u_int health)
{
    gfx::Color Result;

    Result.r = min((100 - health) / 100.0f * 225.0f, 255);
    Result.g = min(health / 100.0f * 255.0f, 255);
    Result.b = 0;

    return Result;
}