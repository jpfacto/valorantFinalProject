#include "tournament.h"
#include "match.h"

#include <iostream>

using namespace std;

Tournament::Tournament(const vector<Team>& initialTeams) {
    teams = initialTeams;
}

void Tournament::runTournament()
{
    if (teams.size() < 2)
    {
        cout << "Need at least 2 teams.\n";
        return;
    }

    vector<Team> currentRound = teams;

    int round = 1;

    while(currentRound.size() > 1)
    {
        displayRoundBracket(currentRound, round);

        vector<Team> winners;

        for(size_t i = 0; i < currentRound.size(); i += 2)
        {
            if(i + 1 >= currentRound.size())
            {
                cout << currentRound[i].name
                     << " receives a bye.\n";

                winners.push_back(currentRound[i]);

                continue;
            }

            cout << "\nMatch "
                 << (i/2)+1
                 << '\n';

            cout << currentRound[i].name
                 << " vs "
                 << currentRound[i+1].name
                 << "\n";

            Match match(&currentRound[i], &currentRound[i+1]);

            match.startMatch();

            match.simulateMatch();

            Team* winner = match.winner();

            winners.push_back(*winner);

            cout << winner->name
                 << " advances to the next round.\n";
        }

        currentRound = winners;

        cout << "\n========================================\n";
        cout << "      TEAMS ADVANCING TO NEXT ROUND\n";
        cout << "========================================\n";

        for (size_t i = 0; i < winners.size(); i++)
        {
            cout << "[" << i + 1 << "] "
                << winners[i].name
                << '\n';
        }

        round++;
    }

    champion = currentRound.front();

    cout << "\n========================================\n";
    cout << "[CHAMPION] "
         << champion.name
         << '\n';
    cout << "========================================\n";
}

void Tournament::displayRoundBracket(const vector<Team>& currentRound, int round)
{
    cout << "\n=====================================================\n";
    cout << "                 TOURNAMENT BRACKET\n";
    cout << "=====================================================\n";
    cout << "ROUND " << round << "\n\n";

    int matchNumber = 1;

    for (size_t i = 0; i < currentRound.size(); i += 2)
    {
        if (i + 1 < currentRound.size())
        {
            cout << "Match " << matchNumber++ << '\n';
            cout << currentRound[i].name << "\n";
            cout << "        VS\n";
            cout << currentRound[i + 1].name << "\n";
        }
        else
        {
            cout << currentRound[i].name << "\n";
            cout << "Receives a BYE\n";
        }
    }
}