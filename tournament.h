#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "player.h"
#include <vector>

class Tournament {
private:
    vector<Team> teams;
    vector<Team> finalists;
    Team champion;

public:
    // Class Tournament constructor defining tournament information
    Tournament(const vector<Team>& initialTeams);
    
    // Required Functions from Project Guide
    void runTournament();
    void displayRoundBracket(const vector<Team>& currentRound, int round);
};

#endif