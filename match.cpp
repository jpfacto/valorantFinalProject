    //FILE NAME: match.cpp

    #include "match.h"

    #include <iostream> //Used for displaying menus and reading user input
    #include <chrono> //For creating delays
    #include <random> //For generating random numbers
    #include <thread> //For running threads concurrently
    #include <mutex> //Prevents multiple threads from printing at the same time
    #include <algorithm> //For using find() to search for eliminated players

    using namespace std;

    extern mutex console; //Mutex in the player.cpp file

    //PURPOSE: Initializes a match with the two participating teams
    Match::Match(Team* tA, Team* tB) 
        : teamA(tA), teamB(tB), teamAScore(0), teamBScore(0), matchWinner(nullptr), currentRound(1){}

    //PURPOSE: Starts the match with a countdown before gameplay begins
    void Match::startMatch(){
        cout << "\nMatch between " << teamA->name << " and " << teamB->name << " is starting in...\n";
        for (int i = 5; i > 0; --i){
            cout << i << "\n";
            this_thread::sleep_for(chrono::milliseconds(800)); 
        }
        cout << "Match Started!\n\n";
    }

    //Player Actions
    void Match::playerMatchEvent(Player& player, Barrier& matchSync, Team* enemyTeam, // [Member 4 - Simon] barrier<>& -> Barrier&
                                bool& spikePlanted, bool& spikeDefused, vector<string>& deadPlayers){
        
        static thread_local mt19937 generator(random_device{}());
        uniform_int_distribution<int> sleepDist(1000, 2000);
        uniform_int_distribution<int> choiceDist(0, 2);

        this_thread::sleep_for(chrono::milliseconds(sleepDist(generator)));
        {
            lock_guard<mutex> lock(console);
            this_thread::sleep_for(chrono::milliseconds(1500)); //For each action

            auto isDead = [&](const string& name){
                return find(deadPlayers.begin(), deadPlayers.end(), name) != deadPlayers.end();
            };  

            //DEATH CHECK: if the player was killed earlier in the round
            if (!isDead(player.name)){
                int choice = choiceDist(generator);

                if (choice == 1){
                    if (!spikePlanted){
                        spikePlanted = true;
                        cout << player.name << " (" << player.agent << ") has PLANTED the spike!\n";
                    } else if (spikePlanted){
                        spikeDefused = true;
                        cout << player.name << " (" << player.agent << ") has DEFUSED the spike!\n";
                    } else{
                        cout << player.name << " (" << player.agent << ") picked up a dropped weapon!\n";
                    }
                } 
                else if (choice == 2){
                    cout << player.name << " (" << player.agent << ") picked up a dropped weapon!\n";
                } 
                else {
                    vector<Player*> aliveEnemies; 

                    for (Player* enemy : enemyTeam->players) {
                        if (!isDead(enemy->name)) {
                            aliveEnemies.push_back(enemy);
                        }
                    }

                    Player* victim = nullptr;

                    if (!aliveEnemies.empty()){
                        uniform_int_distribution<int> pick(0, aliveEnemies.size() - 1);
                        victim = aliveEnemies[pick(generator)];
                    }

                    if (victim != nullptr){
                        deadPlayers.push_back(victim->name);
                        cout << player.name << " (" << player.agent << ") eliminated " 
                                << victim->name << " (" << victim->agent << ")!\n";
                    } else {
                        cout << player.name << " (" << player.agent << ") picked up a dropped weapon!\n";
                    }
                }
            }
        }

        // [Member 4 - Simon] arrive_and_wait() -> arriveAndWait()
        matchSync.arriveAndWait();
    }

    void Match::simulateMatch(){
        cout << "[MATCH PHASE]\n";
        
        random_device rd;
        mt19937 mainGen(rd());
        uniform_int_distribution<int> coinFlip(0, 1);

        //First to 2 (Best of 3)
        while (teamAScore < 2 && teamBScore < 2){
            cout << "\n--- Round " << currentRound << " Started ---\n";

            bool spikePlanted = false;
            bool spikeDefused = false;
            vector<string> deadPlayers;

            Barrier matchSync = Barrier::matchBarrier(10); // [Member 4 - Simon] now uses shared Barrier class
            vector<thread> threads;

            for (Player* p : teamA->players){
                threads.emplace_back(&Match::playerMatchEvent, this, ref(*p), ref(matchSync), teamB, ref(spikePlanted), ref(spikeDefused), ref(deadPlayers));
            }
            for (Player* p : teamB->players){
                threads.emplace_back(&Match::playerMatchEvent, this,ref(*p), ref(matchSync), teamA, ref(spikePlanted), ref(spikeDefused), ref(deadPlayers));
            }

            for (thread& t : threads){
                t.join();
            }

            if (spikeDefused){
                cout << "Spike Defused: Defenders win the round!\n";
                teamBScore++;
            } 
            else if (spikePlanted){
                cout << "Spike Detonated: Attackers win the round via explosion.\n";
                teamAScore++;
            } 
            else {
                //No spike event happened
                if (coinFlip(mainGen) == 0){
                    cout << teamA->name << " (Attackers) eliminated all opponents!\n";
                    teamAScore++;
                } else {
                    cout << teamB->name << " (Defenders) eliminated all opponents!\n";
                    teamBScore++;
                }
            }

            score();
            currentRound++;
        }

        matchWinner = (teamAScore > teamBScore) ? teamA : teamB;

    }

    void Match::score(){
        cout << "\nUpdating Scores...\n";
        cout << teamA->name << " Score: " << teamAScore << "\n";
        cout << teamB->name << " Score: " << teamBScore << "\n";
    }

    Team* Match::winner(){
        cout << "Winner: " << matchWinner->name << "\n";
        return matchWinner;
    }