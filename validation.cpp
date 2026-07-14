#include "validation.h"
#include <iostream>

using namespace std;

namespace {
    const vector<string> DEFAULT_TEAM_NAMES = {"Team A", "Team B", "Team C", "Team D"}; //default teams required for the sim to run
    const int MINIMUM_REQUIRED_AGENTS = 27; 
    /* matched what main.cpp initializes (right now 27 elements laman ng array)
       but, based on the project guide, 29 agents (may 2 placeholders) 
       this can be adjusted later if the agent pool is expanded to 29
    */
    bool systemReady = false;
}

bool checkTeams(const vector<Team>& teams) {
    for (const string& defaultName : DEFAULT_TEAM_NAMES) {
        bool found = false;
        for (const Team& t : teams) {
            if (t.name == defaultName) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "[ERROR] Missing default team: " << defaultName << "\n";
            return false;
        }
    }

    cout << "Default Teams (Teams A to D) Loaded\n";
    return true;
}

bool checkAgents(const vector<Agent>& agents) {
    if (agents.empty()) {
        cout << "[ERROR] Agent pool is empty.\n";
        return false;
    }

    if (static_cast<int>(agents.size()) < MINIMUM_REQUIRED_AGENTS) {
        cout << "[ERROR] Agent pool has only " << agents.size()
             << " agents; at least " << MINIMUM_REQUIRED_AGENTS << " required.\n";
        return false;
    }

    cout << "All Agents Available\n";
    return true;
}

bool checkIfMatchIsReady(const vector<Team>& teams) {
    int validTeamCount = 0;

    for (const Team& t : teams) {
        if (!t.name.empty()) {
            validTeamCount++;
        }
    }

    if (validTeamCount < 2) {
        cout << "[ERROR] Need at least 2 valid teams to start a match or tournament.\n";
        return false;
    }

    cout << "Players and Teams Available\n";
    return true;
}

bool validateSystem(const vector<Team>& teams, const vector<Agent>& agents) {
    cout << "\n[System Validation]\n\n";

    bool teamsOk = checkTeams(teams);
    bool agentsOk = checkAgents(agents);
    bool matchReadyOk = checkIfMatchIsReady(teams);

    systemReady = teamsOk && agentsOk && matchReadyOk;
    return systemReady;
}

void systemStatus() {
    cout << "\nSystem Status: " << (systemReady ? "Ready" : "Error") << "\n";
}