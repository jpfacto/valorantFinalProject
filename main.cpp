#include "player.h"
#include "match.h"
#include "tournament.h"
#include <iostream>

using namespace std;

int main() {
    int choice = 0;
    vector<Team> teams = defaultTeams(); 

    while (choice != 7) {
        cout << "\n=========================================\n";
        cout << "      VALORANT TOURNAMENT SIMULATION     \n";
        cout << "=========================================\n";
        cout << "[1] Simulate Tournament\n";
        cout << "[2] Add Team\n";
        cout << "[3] Delete Team\n";
        cout << "[4] Add Agent\n";
        cout << "[5] Delete Agent\n";
        cout << "[6] View Data\n";
        cout << "[7] Exit\n";
        cout << "Please Enter Your Choice: ";
        cin >> choice;

        if (choice == 1) {
            Tournament tourney(teams);
            
            // 1. Generate and showcase initial clear bracket setup
            tourney.createBracket(teams);
            tourney.displayBracket();
            
            // 2. Run your groupmate's simulation phase
            cout << "\n--- Commencing Semifinal Match Simulations ---\n";
            playerCode();
            
            // 3. Update Tournament Standings based on the rules
            cout << "\n--- Updating Tournament Standings ---\n";
            tourney.advanceTeam(teams[2]);  // Advances Team C
            tourney.advanceTeam(teams[3]);  // Advances Team D
            
            // 4. Run the final match showcase using the finalists
            tourney.finalMatch(teams[2], teams[3]);

            // 5. NEW: Show the final completed bracket with all results filled out!
            cout << "\n--- Final Tournament Standings Breakdown ---";
            tourney.displayBracket();
        } 
        else if (choice == 7) {
            cout << "Exiting system. Simulation terminated.\n";
        } 
        else {
            cout << "Feature processing... (Maintained by other modules)\n";
        }
    }
    return 0;
}
