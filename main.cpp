#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
using namespace std;

class Muitnieks
{
public:
    int id;
    int serveTime;
    int currentTime;
    char type;

    Muitnieks(int id, int serveTime, int currentTime, char type)
    {
        this->id = id;
        this->serveTime = serveTime;
        this->currentTime = currentTime;
        this->type = type;
    };
};

class Iebraucejs
{
public:
    int id;
    char type;

    Iebraucejs(int id, char type)
    {
        this->id = id;
        this->type = type;
    };
};

struct IebraucejiAfter
{
    int id;
    int currentTime;
    char type;
    int muitnieksID;
};

vector<IebraucejiAfter> iebraucejiAfter;
vector<Muitnieks> pilsonuRinda;
vector<Muitnieks> nepilsonuRinda;
vector<Iebraucejs> iebrauceji;

void addMuitnieks(int id, int serveTime, int currentTime, char type)
{
    Muitnieks muitnieks(id, serveTime, currentTime, type);
    if (type == 'P')
    {
        pilsonuRinda.push_back(muitnieks);
    }
    else
    {
        nepilsonuRinda.push_back(muitnieks);
    }
}

void addIebraucejs(int id, char type)
{
    Iebraucejs iebraucejs(id, type);
    iebrauceji.push_back(iebraucejs);
}

void addIebraucejsAfter(int id, int currentTime, char type)
{
    IebraucejiAfter iebraucejsAfter;
    iebraucejsAfter.id = id;
    iebraucejsAfter.currentTime = currentTime;
    iebraucejsAfter.type = type;
    iebraucejiAfter.push_back(iebraucejsAfter);
}

void initializeMuitnieki(int numMuitnieki, int serveTime, char type)
{
    for (int i = 0; i < numMuitnieki; i++)
    {
        addMuitnieks(i + 1, serveTime, 0, type);
    }
}

void setServeTime(vector<Muitnieks> &muitnieki, int Muitnieks, int Laiks)
{
    for (int i = 0; i < muitnieki.size(); i++)
    {
        if (muitnieki[i].id == Muitnieks)
        {
            muitnieki[i].serveTime = Laiks;
            break;
        }
    }
}

int main()
{
    // Open files
    ifstream file("customs.in");
    ofstream output("customs.out");

    int P_Muitnieki, N_Muitnieki, P_Laiks, N_Laiks;
    string firstLine;
    getline(file, firstLine);
    // Fill the int variables from firstLine
    sscanf(firstLine.c_str(), "%d %d %d %d", &P_Muitnieki, &N_Muitnieki, &P_Laiks, &N_Laiks);

    // Initialize the muitnieki
    initializeMuitnieki(P_Muitnieki, P_Laiks, 'P');
    initializeMuitnieki(N_Muitnieki, N_Laiks, 'N');

    // Read the input file line by line
    string line;
    while (getline(file, line))
    {
        // Means that the input file has ended
        if (line == "X")
        {
            if (iebrauceji.empty())
            {
                output << "nothing" << endl;
            }
            break;
        }
        // Means that the line is a command to set the serve time for a muitnieks
        else if (line[0] == 'T')
        {
            char Tips[1];
            int Muitnieks, Laiks;
            sscanf(line.c_str(), "T %s %d %d", Tips, &Muitnieks, &Laiks);

            if (Tips[0] == 'P')
            {
                setServeTime(pilsonuRinda, Muitnieks, Laiks);
            }
            else
            {
                setServeTime(nepilsonuRinda, Muitnieks, Laiks);
            }
        }
        // Means that the line is a command to add an iebraucejs
        else if (line[0] == 'P' || line[0] == 'N')
        {
            char Tips[1];
            int Laiks;
            sscanf(line.c_str(), "%s %d", Tips, &Laiks);
            addIebraucejs(Laiks, Tips[0]);
        }
    }

    // Process the iebrauceji trough the muitnieki
    for (const auto &iebraucejs : iebrauceji)
    {
        bool foundMuitnieks = false;

        // If the iebraucejs is a pilsonis
        if (iebraucejs.type == 'P')
        {
            // No waiting time in line
            for (int i = 0; i < pilsonuRinda.size(); i++)
            {
                Muitnieks &muitnieks = pilsonuRinda[i];
                if (muitnieks.currentTime <= iebraucejs.id)
                {
                    muitnieks.currentTime = 0;
                    muitnieks.currentTime = iebraucejs.id + muitnieks.serveTime;
                    iebraucejiAfter.push_back({iebraucejs.id, muitnieks.currentTime, iebraucejs.type, muitnieks.id});
                    foundMuitnieks = true;
                    break;
                }
            }

            // If the iebraucejs has to wait in line
            if (!foundMuitnieks)
            {
                // Find muitnieks with the smallest waiting time in pilsonuRinda
                auto smallestPilsonuMuitnieks = min_element(pilsonuRinda.begin(), pilsonuRinda.end(),
                                                            [](const Muitnieks &a, const Muitnieks &b)
                                                            {
                                                                return a.currentTime < b.currentTime;
                                                            });

                smallestPilsonuMuitnieks->currentTime += smallestPilsonuMuitnieks->serveTime;
                iebraucejiAfter.push_back({iebraucejs.id, smallestPilsonuMuitnieks->currentTime, iebraucejs.type, smallestPilsonuMuitnieks->id});
            }
        }
        // If the iebraucejs is a nepilsonis
        else
        {
            // No waiting time in line
            for (int i = 0; i < nepilsonuRinda.size(); i++)
            {
                Muitnieks &muitnieks = nepilsonuRinda[i];
                if (muitnieks.currentTime <= iebraucejs.id)
                {
                    muitnieks.currentTime = 0;
                    muitnieks.currentTime = iebraucejs.id + muitnieks.serveTime;
                    iebraucejiAfter.push_back({iebraucejs.id, muitnieks.currentTime, iebraucejs.type, muitnieks.id});
                    foundMuitnieks = true;
                    break;
                }
            }
            // If the iebraucejs has to wait in line
            if (!foundMuitnieks)
            {
                // Find muitnieks with the smallest waiting time in nepilsonuRinda
                auto smallestNepilsonuMuitnieks = min_element(nepilsonuRinda.begin(), nepilsonuRinda.end(),
                                                              [](const Muitnieks &a, const Muitnieks &b)
                                                              {
                                                                  return a.currentTime < b.currentTime;
                                                              });

                smallestNepilsonuMuitnieks->currentTime += smallestNepilsonuMuitnieks->serveTime;
                iebraucejiAfter.push_back({iebraucejs.id, smallestNepilsonuMuitnieks->currentTime, iebraucejs.type, smallestNepilsonuMuitnieks->id});
            }
        }
    }

    // Sort iebraucejiAfter by their end time and type if needed
    sort(iebraucejiAfter.begin(), iebraucejiAfter.end(), [](const IebraucejiAfter &a, const IebraucejiAfter &b)
         {
        if (a.currentTime == b.currentTime)
        {
            if (a.type == b.type)
            {
                if (a.muitnieksID == b.muitnieksID) {
                    return false;
                }
                return a.muitnieksID < b.muitnieksID;
            }
            return a.type == 'P' && b.type == 'N';
        }
        return a.currentTime < b.currentTime; });

    // Print out the sorted iebraucejiAfter
    for (const auto &iebraucejs : iebraucejiAfter)
    {
        output << iebraucejs.id << " " << iebraucejs.currentTime << endl;
    }

    file.close();
    output.close();
    return 0;
}