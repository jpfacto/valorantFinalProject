#include <iostream>
#include <vector>
#include <string>
#include <limits>

// GROUP MODULES 
#include "player.h"      // Janelle (Provides Team, Agent, Player classes)
#include "match.h"       // John Rhenan
#include "tournament.h"  // Sean
#include "barrier.h"     // Simon
#include "validation.h"  // Aleia

using namespace std;

// Master System Dynamic Storage
vector<Team> masterTeams;
vector<Agent> masterAgents;

// Core Function Prototypes
void initializeSystem();
void runSystem();
void addTeam();
void deleteTeam();
void addAgent();
void deleteAgent();
void viewData();
void simulateTournament();

int main() {
    // Phase 1: Bootstrapping default environment
    initializeSystem(); 
    
    // Phase 2: Handing control to CLI Menu Loop
    runSystem();        
    
    return 0;
}

// Loads Default Teams A-D and the 27 Valorant Agents
void initializeSystem() {
    // Instantiate Default Teams A through D
    masterTeams.push_back(Team("Team A"));
    masterTeams.push_back(Team("Team B"));
    masterTeams.push_back(Team("Team C"));
    masterTeams.push_back(Team("Team D"));

    // 27 Agents 
    string Agents[27] = {
        "Jett", "Phoenix", "Sage", "Sova", "Brimstone", "Cypher", "Reyna", "Killjoy",
        "Viper", "Omen", "Breach", "Raze", "Skye", "Yoru", "Astra", "KAY/O",
        "Chamber", "Neon", "Fade", "Harbor", "Gekko", "Deadlock", "Iso", "Clove",
        "Vyse", "Tejo", "Waylay",
    };

    for (int i = 0; i < 27; i++) {
        masterAgents.push_back(Agent(Agents[i]));
    }
}

// Master CLI Loop
void runSystem() {
    int choice = 0;
    while (true) {
        cout << "\n========================================\n";
        cout << "    VALORANT TOURNAMENT SIMULATION      \n";
        cout << "========================================\n";
        cout << "[Valorant Tournament Simulation Menu]\n";
        cout << "[1] Simulate Tournament\n";
        cout << "[2] Add Team\n";
        cout << "[3] Delete Team\n";
        cout << "[4] Add Agent\n";
        cout << "[5] Delete Agent\n";
        cout << "[6] View Data\n";
        cout << "[7] Exit\n";
        cout << "Please Enter Your Choice: ";

        if (!(cin >> choice)) {
            cout << "\n[System Error] Invalid input. Integer required.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: simulateTournament(); break;
            case 2: addTeam(); break;
            case 3: deleteTeam(); break;
            case 4: addAgent(); break;
            case 5: deleteAgent(); break;
            case 6: viewData(); break;
            case 7: 
                cout << "\nTerminating Simulation Session. Good luck presenting!\n";
                return;
            default:
                cout << "\n[Warning] Choice must be between 1 and 7.\n";
        }
    }
}

// Runtime Mutability: Add Team
void addTeam() {
    string tName;
    cout << "Enter New Team Name: ";
    cin >> ws; 
    getline(cin, tName);
    
    masterTeams.push_back(Team(tName));
    cout << "[Success] Team '" << tName << "' successfully registered.\n";
}

// Runtime Mutability: Delete Team
void deleteTeam() {
    if (masterTeams.empty()) {
        cout << "[Denied] Team roster is already empty.\n";
        return;
    }
    viewData();
    cout << "Enter exact Team Name to delete: ";
    string target;
    cin >> ws;
    getline(cin, target);

    for (auto it = masterTeams.begin(); it != masterTeams.end(); ++it) {
        if (it->name == target) { // Assumes class Team has 'string name'
            masterTeams.erase(it);
            cout << "[Success] Team successfully removed from bracket pool.\n";
            return;
        }
    }
    cout << "[Error] Target team not found in memory.\n";
}

// Runtime Mutability: Add Agent
void addAgent() {
    string aName;
    cout << "Enter New Agent Name: ";
    cin >> ws;
    getline(cin, aName);
    
    masterAgents.push_back(Agent(aName));
    cout << "[Success] Agent '" << aName << "' added to available pool.\n";
}

// Runtime Mutability: Delete Agent
void deleteAgent() {
    if (masterAgents.empty()) {
        cout << "[Denied] Agent pool is already empty.\n";
        return;
    }
    cout << "Enter exact Agent Name to remove: ";
    string target;
    cin >> ws;
    getline(cin, target);

    for (auto it = masterAgents.begin(); it != masterAgents.end(); ++it) {
        if (it->name == target) { // Assumes class Agent has 'string name'
            masterAgents.erase(it);
            cout << "[Success] Agent decommissioned.\n";
            return;
        }
    }
    cout << "[Error] Agent not found in active pool.\n";
}

// Displays Master Roster of Teams and Agents
void viewData() {
    cout << "\n--- ELIGIBLE TEAMS (" << masterTeams.size() << ") ---\n";
    for (size_t i = 0; i < masterTeams.size(); i++) {
        cout << "  [" << i + 1 << "] " << masterTeams[i].name << "\n";
    }

    cout << "\n--- AGENT ROSTER (" << masterAgents.size() << ") ---\n";
    for (size_t i = 0; i < masterAgents.size(); i++) {
        cout << (i == 0 ? "" : ", ") << masterAgents[i].name;
    }
    cout << "\n";
}

// Flow: Player -> Barrier -> Match -> Tournament
void simulateTournament() {
    cout << "\n[System Validation Phase Initiated]...\n";

    // 1. GATEKEEPER (Aleia)
    // Runs pre-flight validation check on data vectors
    if (!validateSystem(masterTeams, masterAgents)) {
        cout << "[Simulation Aborted] System validation returned errors. Check rosters.\n";
        return;
    }
    systemStatus(); // Prints "System Status: Ready"

    cout << "\n[Spawning Dynamic Player Base]...\n";
    
    // 2. PLAYER & CONNECTION SYSTEM (Janelle + Simon)
    int requiredPlayers = masterTeams.size() * 5; // 5 players per team
    vector<Player> activePlayers = createPlayers(requiredPlayers);
    teamAssignment(activePlayers, masterTeams);

    cout << "\n[Initiating Threaded Connection Phase]...\n";
    // Fires threads & triggers connectionBarrier.arrive_and_wait()
    startConnectionPhase(activePlayers); 

    cout << "\n[Initiating Concurrent Agent Selection Phase]...\n";
    // Fires threads & triggers Simon's selectionBarrier.arrive_and_wait()
    startAgentSelectionPhase(activePlayers, masterAgents);

    // 3. MATCH & BRACKET EXECUTION (Barquin + Sean)
    cout << "\n[Generating Dynamic Tournament Bracket]...\n";
    Tournament activeTournament;
    activeTournament.createBracket(masterTeams);
    activeTournament.displayBracket();

    cout << "\n[Executing Matches Under Synchronization Locks]...\n";
    activeTournament.runTournament(); // Resolves bracket & prints Champion
}
