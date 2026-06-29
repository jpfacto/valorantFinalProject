#ifndef MATCH_H
#define MATCH_H

#include "player.h" 
#include <barrier>
#include <vector>
#include <string>

using namespace std;

class Match {
private:
    Team* teamA;
    Team* teamB;
    int teamAScore;
    int teamBScore;
    Team* matchWinner;
    int currentRound;

    void playerMatchEvent(Player& player, barrier<>& matchSync, Team* enemyTeam, 
                          bool& spikePlanted, bool& spikeDefused, vector<string>& deadPlayers);

public:
    Match(Team* tA, Team* tB);

    void startMatch();
    void simulateMatch();
    void score();
    Team* winner();
};

#endif