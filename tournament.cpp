#include "tournament.h"
#include <iostream>

using namespace std;

Tournament::Tournament(const vector<Team>& initialTeams) {
    teams = initialTeams;
}

void Tournament::createBracket(vector<Team>& teamsList) {
    cout << "\n[Tournament Bracket]\n\n";
    if (teamsList.size() >= 4) {
        cout << "Match 1:\n" << teamsList[0].name << " vs " << teamsList[2].name << "\n\n";
        cout << "Match 2:\n" << teamsList[1].name << " vs " << teamsList[3].name << "\n";
    } else {
        cout << "Error: Minimal 4 teams needed for bracket generation.\n";
    }
}

void Tournament::advanceTeam(Team winner) {
    finalists.push_back(winner);
    cout << "Winner " << winner.name << " advances!\n";
}

void Tournament::finalMatch(Team teamA, Team teamB) {
    cout << "\n[Final Match]\n";
    cout << teamA.name << " vs " << teamB.name << "\n\n";
    
    cout << "(From Member 4)\nAll match events have been completed!\n\n";
    
    champion = teamA; 
    cout << "Tournament Champion: " << champion.name << "\n";
}

void Tournament::displayBracket() {
    // 1. Determine dynamic names based on current tournament progress state
    string f1 = (finalists.size() > 0) ? finalists[0].name : "Finalist 1";
    string f2 = (finalists.size() > 1) ? finalists[1].name : "Finalist 2";
    string champName = (!champion.name.empty()) ? "🏆 " + champion.name : "CHAMPION";

    while (f1.length() < 10) f1 += " ";
    while (f2.length() < 10) f2 += " ";

    cout << "\n============================================\n";
    cout << "         CURRENT TOURNAMENT STRUCTURE       \n";
    cout << "============================================\n";
    if (teams.size() >= 4) {
        cout << " [" << teams[0].name << "] ──┐\n";
        cout << "              ├── [ " << f1 << " ] ──┐\n";
        cout << " [" << teams[2].name << "] ──┘                      │\n";
        cout << "                                       ├── " << champName << "\n";
        cout << " [" << teams[1].name << "] ──┐                      │\n";
        cout << "              ├── [ " << f2 << " ] ──┘\n";
        cout << " [" << teams[3].name << "] ──┘\n";
    }
    cout << "============================================\n";
}