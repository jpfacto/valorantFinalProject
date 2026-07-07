#ifndef VALIDATION_H
#define VALIDATION_H

#include <vector>
#include "player.h"

bool checkTeams(const std::vector<Team>& teams);

bool checkAgents(const std::vector<Agent>& agents);

bool checkIfMatchIsReady(const std::vector<Team>& teams);

bool validateSystem(const std::vector<Team>& teams, const std::vector<Agent>& agents);

void systemStatus();

#endif