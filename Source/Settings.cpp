#include "Settings.h"

using std::ifstream;
using std::stringstream;
using std::string;
using std::vector;

using gk::split;

using gk::CSettings;

CSettings::CSettings(const string& filename)
{
    this->settings.open(filename, std::ios::in | std::ios::out);

    if(!this->settings.is_open() || this->settings.bad())
    {
        gk::HandleError("Unable to load Settings.ini!");
    }
}

CSettings::~CSettings()
{
    this->settings.close();
}

string CSettings::GetValueAt(const string& id)
{
    string line;

    settings.clear();
    settings.seekg(0, std::ios::beg);

    while(!this->settings.eof() && this->settings.good())
    {
        std::getline(settings, line);

        if(line.empty() || line[0] == ';' || line[0] == '[')
            continue;

        else
        {
            if(line.find(id) == 0)  // We found the ID at the beginning of the line
            {
                vector<string> tmp;
                tmp = split(line, '=');
                if(tmp.size() == 1) // No value after '='
                    return string("");
                else
                    return tmp[1];
            }
        }
    }

    return string("");
}

std::string CSettings::ChooseValueAt(const string& id)
{
    string line;

    settings.clear();
    settings.seekg(0, std::ios::beg);

    while(!this->settings.eof() && this->settings.good())
    {
        std::getline(settings, line);

        if(line.empty() || line[0] == ';' || line[0] == '[')
            continue;

        else
        {
            if(line.find(id) == 0)  // We found the ID at the beginning of the line
            {
                vector<string> tmp;
                tmp = split(line, '=');
                if(tmp.size() == 1) // No value after '='
                    return string("");
                else if(tmp.size() > 1 && tmp[1].find(';') != std::string::npos)
                {
                    vector<string> tmp2 = split(tmp[1], ';');
                    int index = rand() % (tmp2.size() - 1);
                    return tmp2[index];
                }
                else
                    return tmp[1];
            }
        }
    }

    return string("");
}

void CSettings::CreateValue(const string& id, const string& value)
{
    /* Add a value to the Settings file. 
     * TODO: Check if value exists and if it
     * does, modify it rather than creating
     * a new one.
     */
    stringstream line_stream;
    //string line;

    line_stream << id << "=" << value << std::endl;

    settings.seekg(0, std::ios::end);
    settings.write(line_stream.str().c_str(), line_stream.str().size());
    settings.flush();
}
