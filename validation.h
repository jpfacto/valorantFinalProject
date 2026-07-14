#ifndef VALIDATION_H
#define VALIDATION_H

#include <vector>
#include "player.h"

using namespace std;

bool checkTeams(const vector<Team>& teams);

bool checkAgents(const vector<Agent>& agents);

bool checkIfMatchIsReady(const vector<Team>& teams);

bool validateSystem(const vector<Team>& teams, const vector<Agent>& agents);

void systemStatus();

#endif