#ifndef MATCH_H
#define MATCH_H

#include "player.h" 
#include <barrier>
#include <vector>
#include <string>

class Match {
private:
    Team* teamA;
    Team* teamB;
    int teamAScore;
    int teamBScore;
    Team* matchWinner;
    int currentRound;

    void playerMatchEvent(Player& player, std::barrier<>& matchSync, Team* enemyTeam, 
                          bool& spikePlanted, bool& spikeDefused, std::vector<std::string>& deadPlayers);

public:
    Match(Team* tA, Team* tB);

    void startMatch();
    void simulateMatch();
    void score();
    Team* winner();
};

#endif