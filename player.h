#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <barrier>
#include <mutex>

using namespace std;

class Agent {
public:
    string name;

    Agent(string n = "");
};

class Player {
public:
    string name;
    string team;
    string agent;
    bool connected;
    bool selected;

    Player(string n = "");
};

class Team {
public:
    string name;
    vector<Player*> players;

    Team(string n = "");
};

vector<Team> defaultTeams();
void createTeams(vector<Team>& teams, int additionalTeams);
vector<Player> createPlayers(int totalPlayers);
void teamAssignment(vector<Player>& players, vector<Team>& teams);

vector<Agent> createAgents();

void playerConnection(Player& player, barrier<>& sync);
void agentSelection(Player& player, vector<Agent>& availableAgents, barrier<>& sync);

void playerCode();

#endif
