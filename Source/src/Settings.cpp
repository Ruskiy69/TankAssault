#include "Settings.hpp"

using game::CSettings;

CSettings::~CSettings()
{
    if(m_settingsfile.is_open())
        m_settingsfile.close();
}

/**
 * Loads a settings file.
 *  The settings file should be in the format of an .ini file,
 *  with various headings containing key-value pairs.
 *
 * @param const std::string & filename
 * @return TRUE on a successful load, FALSE otherwise.
 **/       
bool CSettings::Load(const std::string& filename)
{
    m_settingsfile.open(filename, std::ios::in);
    return (m_settingsfile.is_open());
}

/**
 * Chooses a random value out of a list of values in a key-value pair.
 *  If the settings file contains an item in the form 
 *  "key=value1,value2,value3" this function will return one
 *  of the values at random.
 * 
 * @param std::string& Key to search for
 * @return The chosen value, or an empty string if the key could not be found.
 * @pre The settings file has been loaded.
 * @see CSettings::Load(const std::string& filename)
 **/        
std::string CSettings::ChooseValueAt(const std::string& id)
{
    std::string line;

    m_settingsfile.clear();
    m_settingsfile.seekg(0, std::ios::beg);

    while(std::getline(m_settingsfile, line))
    {
        if(line.empty() || line[0] == ';' || line[0] == '[')
            continue;

        else
        {
            if(line.find(id) == 0)  // We found the ID at the beginning of the line
            {
                std::vector<std::string> tmp;
                tmp = gk::split(line, '=');
                if(tmp.size() == 1) // No value after '='
                    return std::string("");
                else if(tmp.size() > 1 && tmp[1].find(';') != std::string::npos)
                {
                    std::vector<std::string> tmp2 = gk::split(tmp[1], ';');
                    int index = rand() % (tmp2.size() - 1);
                    return tmp2[index];
                }
                else
                    return tmp[1];
            }
        }
    }

    return std::string("");
}

/**
 * Retrieves the value for a certain key.
 * 
 * @param std::string& Key to search for
 * @return The value in the key-value pair, or an empty
 * @pre The settings file has been loaded.
 * @see CSettings::Load(const std::string& filename)
 *  string if the value was not found.
 **/       
std::string CSettings::GetValueAt(const std::string& id)
{
    std::string line;

    m_settingsfile.clear();
    m_settingsfile.seekg(0, std::ios::beg);

    while(std::getline(m_settingsfile, line))
    {
        if(line.empty() || line[0] == ';' || line[0] == '[')
            continue;

        else
        {
            if(line.find(id) == 0)  // We found the ID at the beginning of the line
            {
                std::vector<std::string> tmp;
                tmp = gk::split(line, '=');
                if(tmp.size() == 1) // No value after '='
                    return std::string("");
                else
                    return tmp[1];
            }
        }
    }

    return std::string("");
}

CSettings& game::CSettings::GetInstance()
{
    static CSettings Settings;
    return Settings;
}
