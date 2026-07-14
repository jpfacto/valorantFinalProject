#ifndef MATCH_H
#define MATCH_H

#include "player.h" 
#include "barrier.h" // [Member 4 - Simon] swapped raw <barrier> for our shared Barrier class
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

    void playerMatchEvent(Player& player, Barrier& matchSync, Team* enemyTeam, // [Member 4 - Simon] barrier<>& -> Barrier&
                          bool& spikePlanted, bool& spikeDefused, vector<string>& deadPlayers);

public:
    Match(Team* tA, Team* tB);

    void startMatch();
    void simulateMatch();
    void score();
    Team* winner();
};

#endif