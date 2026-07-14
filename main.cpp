    //FILE NAME: main.cpp

    #include <iostream>   //Used for displaying menus and reading user input
    #include <vector>     //Stores the list of teams, players, and agents
    #include <string>     //Used for team names, player names, and agent names
    #include <limits>     //Clears invalid input when the user enters the wrong data
    #include <windows.h>  //Used for Windows console settings (Tournament Bracket)

    //MODULE ASSIGNMENTS
    #include "player.h"      //Janelle
    #include "match.h"       //John Rhenan
    #include "tournament.h"  //Sean
    #include "barrier.h"     //Simon
    #include "validation.h"  //Aleia

    using namespace std;

    //PURPOSE: Global vectors shared across the system to store teams and available agents
    vector<Team> masterTeams; // DEFAULT: 4 Teams A-D
    vector<Agent> masterAgents; // DEFAULT: 29 Agents

    //SYSTEM FUNCTIONS: Function declarations
    void initializeSystem();    //Initializes and loads the default teams and agents
    void runSystem();           //Displays the main menu and handles user actions
    void addTeam();             //Adds a new team to the system
    void deleteTeam();          //Removes an existing team
    void addAgent();            //Adds a new agent
    void deleteAgent();         //Removes an existing agent
    void viewData();            //Displays the current teams and agents
    void simulateTournament();  //Starts the tournament simulation

    //MAIN PROGRAM
    int main() {

        //PURPOSE: For displaying the UTF-8 characters
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);      

        //PURPOSE: [Phase 1] Loads the default teams and agents before the system starts
        initializeSystem(); 
        
        //PURPOSE: [Phase 2] Starts the CLI menu system
        runSystem();        
        
        return 0;
    }

    void initializeSystem() {
        /*FORMAT: vectorName.push_back(ClassName("Name"));
        push_back() adds a new object to the end of the vector
        PURPOSE: Stores the default teams in the master team list*/
        masterTeams.push_back(Team("Team A"));
        masterTeams.push_back(Team("Team B"));
        masterTeams.push_back(Team("Team C"));
        masterTeams.push_back(Team("Team D"));

        //PURPOSE: Stores the agent names in a fixed-size array
        string agentNames[29] = {
            "Jett", "Phoenix", "Sage", "Sova", "Brimstone", "Cypher", "Reyna", "Killjoy", "Viper", "Omen", "Breach", "Raze", "Skye", "Yoru", 
            "Astra", "KAY/O", "Chamber", "Neon", "Fade", "Harbor", "Gekko", "Deadlock", "Iso", "Clove", "Vyse", "Tejo", "Waylay", "Miks", "Veto",
        };

        //PURPOSE: For loop for adding each agent from the array into the master agent list
        for (int i = 0; i < 29; i++) {
            masterAgents.push_back(Agent(agentNames[i]));
        }
    }

    // MAIN MENU
    void runSystem() {
        int choice = 0; //PURPOSE: Store's the user's input
        while (true) {
            cout << "\n=====================================================================\n";
            cout << "\n██╗   ██╗ █████╗ ██╗      ██████╗ ██████╗  █████╗ ███╗   ██╗████████╗\n";
            cout << "\n██║   ██║██╔══██╗██║     ██╔═══██╗██╔══██╗██╔══██╗████╗  ██║╚══██╔══╝\n";
            cout << "\n██║   ██║███████║██║     ██║   ██║██████╔╝███████║██╔██╗ ██║   ██║\n";
            cout << "\n╚██╗ ██╔╝██╔══██║██║     ██║   ██║██╔══██╗██╔══██║██║╚██╗██║   ██║\n";
            cout << "\n ╚████╔╝ ██║  ██║███████╗╚██████╔╝██║  ██║██║  ██║██║ ╚████║   ██║\n";
            cout << "\n  ╚═══╝  ╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝\n";
            cout << "\n.                      TOURNAMENT SIMULATION                        .\n";
            cout << "\n=====================================================================\n";
            cout << "[1] Simulate Tournament\n";
            cout << "[2] Add Team\n";
            cout << "[3] Delete Team\n";
            cout << "[4] Add Agent\n";
            cout << "[5] Delete Agent\n";
            cout << "[6] View Data\n";
            cout << "[7] Exit\n";
            cout << "Please Enter Your Choice: ";

            //PURPOSE: If loop for invalid user choices
            if (!(cin >> choice)) {
                cout << "\n[Error] Invalid input. Integer required.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            //PURPOSE: Switch case which calls the function depending on the user's choice
            switch (choice) {
                case 1: simulateTournament(); break;
                case 2: addTeam(); break;
                case 3: deleteTeam(); break;
                case 4: addAgent(); break;
                case 5: deleteAgent(); break;
                case 6: viewData(); break;
                case 7: 
                    cout << "\nThank you! Exiting the system... See you in the next match!\n";
                    return;
                default:
                    cout << "\n[Warning] Choice must be between 1 and 7.\n";
            }
        }
    }

    //PURPOSE: Adds a new team to the system
    void addTeam() {
        string tName; //PURPOSE: Stores the team name entered by the user

        //PURPOSE: Gets the team name from the user
        cout << "Enter New Team Name: ";
        cin >> ws; 
        getline(cin, tName);
        
        //PURPOSE: Creates a Team object using the name stored in tName, then adds it to the end of masterTeams
        masterTeams.push_back(Team(tName));
        cout << "[Success] Team '" << tName << "' successfully registered.\n";
    }

    //PURPOSE: Removes an existing team
    void deleteTeam() {
        //PURPOSE: If loop for checking if there are any teams available before deleting one
        if (masterTeams.empty()) {
            cout << "No teams available.\n";
            return;
        }

        //PURPOSE: Shows the current teams in the system and prompts the user to enter the name of the team to delete
        viewData();

        //PURPOSE: Prompts the user to enter the name of the team to delete
        cout << "Enter Team Name to delete: ";
        string target;
        cin >> ws;
        getline(cin, target);

        //PURPOSE: Search the team list for the specified team
        for (auto it = masterTeams.begin(); it != masterTeams.end(); ++it) {
            if (it->name == target) { //PURPOSE: Checks if the current team's name matches the user's input
                masterTeams.erase(it); //PURPOSE: Removes the matching team from the system.
                cout << "[Success] Team successfully removed from bracket pool.\n";
                return;
            }
        }
        cout << "[Error] Team not found.\n";
    }

    //PURPOSE: Adds a new agent to the system
    void addAgent() {
        string aName; //PURPOSE: Stores the agent name entered by the user

        //PURPOSE: Prompts the user to enter the name of the agent to add
        cout << "Enter New Agent Name: ";
        cin >> ws;
        getline(cin, aName);
        
        //PURPOSE: Creates an Agent object using the name stored in aName, then adds it to the end of masterAgents
        masterAgents.push_back(Agent(aName));
        cout << "[Success] Agent '" << aName << "' added to available pool.\n";
    }

    //PURPOSE: Removes an existing agent from the system
    void deleteAgent() {
        //PURPOSE: If loop for checking if there are any agents available before deleting one
        if (masterAgents.empty()) {
            cout << "No agents are available.\n";
            return;
        }

        //PURPOSE: Prompts the user to enter the name of the agent to delete
        cout << "Enter Agent Name to remove: ";
        string target;
        cin >> ws;
        getline(cin, target);

        //PURPOSE: Search the agent list for the specified agent
        for (auto it = masterAgents.begin(); it != masterAgents.end(); ++it) {
            if (it->name == target) { //PURPOSE: Checks if the current agent's name matches the user's input
                masterAgents.erase(it); //PURPOSE: Removes the matching agent from the system.
                cout << "[Success] Agent decommissioned.\n"; 
                return;
            }
        }
        cout << "[Error] Agent not found.\n";
    }

    //PURPOSE: Displays the current teams and agents
    void viewData() {
        cout << "\n[ELIGIBLE TEAMS (" << masterTeams.size() << ")]\n";
        for (size_t i = 0; i < masterTeams.size(); i++) {
            cout << "  [" << i + 1 << "] " << masterTeams[i].name << "\n";
        }

        cout << "\n[AGENT ROSTER (" << masterAgents.size() << ")]\n";
        for (size_t i = 0; i < masterAgents.size(); i++) {
            cout << (i == 0 ? "" : ", ") << masterAgents[i].name;
        }
        cout << "\n";
    }

    //PURPOSE: Starts the tournament simulation by connecting all the modules
    void simulateTournament() {
        cout << "\n[System Validation Phase Initiated]\n";

        //1. GATEKEEPER (Aleia)
        //PURPOSE: Runs pre-flight validation check on data vectors
        if (!validateSystem(masterTeams, masterAgents)) {
            cout << "[Error] System validation returned errors. Check rosters.\n";
            return;
        }
        systemStatus(); //PURPOSE: Prints "System Status: Ready"
        cout << "\n[Spawning Dynamic Player Base]...\n";

        playerCode();
        return;
    }