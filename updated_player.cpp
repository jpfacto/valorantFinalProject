#include "player.h"
#include "match.h"

#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

mutex console;

// Constructors
Agent::Agent(string n) {
    name = n;
}

Player::Player(string n) {
    name = n;
    connected = false;
    selected = false;
}

Team::Team(string n) {
    name = n;
}

// Teams

vector<Team> defaultTeams() {
    vector<Team> teams;

    teams.push_back(Team("Team A"));
    teams.push_back(Team("Team B"));
    teams.push_back(Team("Team C"));
    teams.push_back(Team("Team D"));
 
    cout << "[INITIALIZATION PHASE]" << endl;
    cout << "Successfully created Teams A to D.\n";
    return teams;
}

void createTeams(vector<Team>& teams, int additionalTeams) {
    char letter = 'E';

    for (int i = 0; i < additionalTeams; i++)
        teams.push_back(Team("Team " + string(1, letter++)));

    if (additionalTeams > 0)
        cout << "Successfully created new teams.\n";
}

// Players

vector<Player> createPlayers(int totalPlayers) {
    vector<Player> players;

    if (totalPlayers <= 0) {
        cout << "Error: Invalid player count.\n";
        return players;
    }

    for (int i = 1; i <= totalPlayers; i++)
        players.push_back(Player("Player " + to_string(i)));

    cout << "Successfully created " << totalPlayers << " players.\n";

    return players;
}

void teamAssignment(vector<Player>& players, vector<Team>& teams) {
    int requiredPlayers = teams.size() * 5;

    if (players.size() != requiredPlayers) {
        cout << "[ERROR] Need exactly " << requiredPlayers << " players.\n";
        return;
    }

    for (Team& t : teams)
        t.players.clear();

    int index = 0;

    for (Team& team : teams) {
        for (int i = 0; i < 5; i++) {
            players[index].team = team.name;
            team.players.push_back(&players[index]);
            index++;
        }
    }

    cout << "Successfully distributed players to their teams.\n";
}

// Agents

vector<Agent> createAgents() {
    string names[] = {
        "Astra","Breach","Brimstone","Chamber","Clove", "Cypher","Deadlock","Fade","Gekko","Harbor", "Iso","Jett","KAY/O","Killjoy","Neon",
        "Omen","Phoenix","Raze","Reyna","Sage", "Skye","Sova","Tejo","Viper","Vyse", "Waylay","Yoru","Ballas","Null"
    };

    vector<Agent> agents;

    for (string name : names)
        agents.push_back(Agent(name));

    return agents;
}

// Connecting Players

void playerConnection(Player& player, Barrier& sync) { // [Member 4 - Simon] barrier<>& -> Barrier&

    {
        lock_guard<mutex> lock(console);
        cout << player.name << " from " << player.team << " is connecting...\n";
    }

    this_thread::sleep_for(chrono::milliseconds(500 + rand() % 1500));

    player.connected = true;

    {
        lock_guard<mutex> lock(console);
        cout << player.name << " from " << player.team << " has connected!\n";
    }

    sync.arriveAndWait(); // [Member 4 - Simon] arrive_and_wait() -> arriveAndWait()
}

// Agent Selection

void agentSelection(Player& player, vector<Agent>& availableAgents, Barrier& sync) { // [Member 4 - Simon] barrier<>& -> Barrier&
    
    {
        lock_guard<mutex> lock(console);
        cout << player.name << " from " << player.team << " is selecting an agent...\n";
    }

    this_thread::sleep_for(chrono::milliseconds(300 + rand() % 1000));

    player.selected = true;

    {
        lock_guard<mutex> lock(console);
        cout << player.name << " from " << player.team << " selected " << player.agent << "!\n";
    }

    sync.arriveAndWait(); // [Member 4 - Simon] arrive_and_wait() -> arriveAndWait()
}

// Member 1 Main Code

void playerCode()
{
    srand(time(nullptr));

    vector<Team> teams = defaultTeams();
    vector<Player> players = createPlayers(20);

    if(players.empty())
        return;

    teamAssignment(players, teams);

    vector<Match> matches;

    matches.emplace_back(&teams[0], &teams[2]);
    matches.emplace_back(&teams[1], &teams[3]);

    vector<Agent> agents = createAgents();

    cout << "\n[CONNECTION PHASE]\n\n";

    for(Team& team : teams)
    {
        Barrier sync = Barrier::connectionBarrier(team.players.size()); // [Member 4 - Simon] now uses shared Barrier class
        vector<thread> threads;

        for(Player* player : team.players)
            threads.emplace_back(playerConnection, ref(*player), ref(sync));

        for(thread& t : threads)
            t.join();

        cout << "\nAll players from " << team.name << " have connected!\n" << endl;
    }

    cout << "\n[AGENT SELECTION PHASE]\n\n";

    random_device rd;
    mt19937 g(rd());

    vector<Agent> match1Agents = agents;
    shuffle(match1Agents.begin(), match1Agents.end(), g);

    int index = 0;

    for(Player* player : teams[0].players)
        player->agent = match1Agents[index++].name;

    for(Player* player : teams[2].players)
        player->agent = match1Agents[index++].name;

    vector<Agent> match2Agents = agents;
    shuffle(match2Agents.begin(), match2Agents.end(), g);

    index = 0;

    for(Player* player : teams[1].players)
        player->agent = match2Agents[index++].name;

    for(Player* player : teams[3].players)
        player->agent = match2Agents[index++].name;

    {
        Barrier sync = Barrier::selectionBarrier(teams[0].players.size()); // [Member 4 - Simon] now uses shared Barrier class
        vector<thread> threads;

        for(Player* player : teams[0].players)
            threads.emplace_back(agentSelection, ref(*player), ref(match1Agents), ref(sync));

        for(thread& t : threads)
            t.join();

        cout << "\nAll players from Team A have selected their agents!\n\n";
    }

    {
        Barrier sync = Barrier::selectionBarrier(teams[2].players.size()); // [Member 4 - Simon] now uses shared Barrier class
        vector<thread> threads;

        for(Player* player : teams[2].players)
            threads.emplace_back(agentSelection, ref(*player), ref(match1Agents), ref(sync));

        for(thread& t : threads)
            t.join();

        cout << "\nAll players from Team C have selected their agents!\n\n";
    }

    {
        Barrier sync = Barrier::selectionBarrier(teams[1].players.size()); // [Member 4 - Simon] now uses shared Barrier class
        vector<thread> threads;

        for(Player* player : teams[1].players)
            threads.emplace_back(agentSelection, ref(*player), ref(match2Agents), ref(sync));

        for(thread& t : threads)
            t.join();

        cout << "\nAll players from Team B have selected their agents!\n\n";
    }

    {
        Barrier sync = Barrier::selectionBarrier(teams[3].players.size()); // [Member 4 - Simon] now uses shared Barrier class
        vector<thread> threads;

        for(Player* player : teams[3].players)
            threads.emplace_back(agentSelection, ref(*player), ref(match2Agents), ref(sync));

        for(thread& t : threads)
            t.join();

        cout << "\nAll players from Team D have selected their agents!\n\n";
    }

    cout << "[MATCH PHASE]\n";

    for (Match& match : matches) {
        match.startMatch();
        match.simulateMatch();
        match.winner();
    }
}
