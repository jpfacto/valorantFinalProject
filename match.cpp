#include "match.h"

#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <algorithm>

extern std::mutex console; 

Match::Match(Team* tA, Team* tB) 
    : teamA(tA), teamB(tB), teamAScore(0), teamBScore(0), matchWinner(nullptr), currentRound(1){}

void Match::startMatch(){
    std::cout << "\nMatch between " << teamA->name << " and " << teamB->name << " is starting in...\n";
    for (int i = 5; i > 0; --i){
        std::cout << i << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(800)); 
    }
    std::cout << "Match Started!\n\n";
}

//Player Actions
void Match::playerMatchEvent(Player& player, std::barrier<>& matchSync, Team* enemyTeam, 
                             bool& spikePlanted, bool& spikeDefused, std::vector<std::string>& deadPlayers){
    
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> sleepDist(1000, 2000);
    std::uniform_int_distribution<int> choiceDist(0, 2);

    std::this_thread::sleep_for(std::chrono::milliseconds(sleepDist(generator)));
    {
        std::lock_guard<std::mutex> lock(console);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); //For each action

        auto isDead = [&](const std::string& name){
            return std::find(deadPlayers.begin(), deadPlayers.end(), name) != deadPlayers.end();
        };

        //DEATH CHECK: if the player was killed earlier in the round
        if (!isDead(player.name) && !spikeDefused){
            int choice = choiceDist(generator);

            if (choice == 1){
                if (!spikePlanted){
                    spikePlanted = true;
                    std::cout << player.name << " (" << player.agent << ") has PLANTED the spike!\n";
                } else if (spikePlanted){
                    spikeDefused = true;
                    std::cout << player.name << " (" << player.agent << ") has DEFUSED the spike!\n";
                } else{
                    std::cout << player.name << " (" << player.agent << ") picked up a dropped weapon!\n";
                }
            } 
            else if (choice == 2){
                std::cout << player.name << " (" << player.agent << ") picked up a dropped weapon!\n";
            } 
            else {
                std::vector<Player*> aliveEnemies; 

                for (Player* enemy : enemyTeam->players) {
                    if (!isDead(enemy->name)) {
                        aliveEnemies.push_back(enemy);
                    }
                }

                Player* victim = nullptr;

                if (!aliveEnemies.empty()){
                    std::uniform_int_distribution<int> pick(0, aliveEnemies.size() - 1);
                    victim = aliveEnemies[pick(generator)];
                }

                if (victim != nullptr){
                    deadPlayers.push_back(victim->name);
                    std::cout << player.name << " (" << player.agent << ") eliminated " 
                              << victim->name << " (" << victim->agent << ")!\n";
                } else {
                    std::cout << player.name << " (" << player.agent << ") picked up a dropped weapon!\n";
                }
            }
        }
    }

    // Member 4 - Barrier
    matchSync.arrive_and_wait();
}

void Match::simulateMatch(){
    std::cout << "[MATCH PHASE]\n";
    
    std::random_device rd;
    std::mt19937 mainGen(rd());
    std::uniform_int_distribution<int> coinFlip(0, 1);

    //First to 2 (Best of 3)
    while (teamAScore < 2 && teamBScore < 2){
        std::cout << "\n--- Round " << currentRound << " Started ---\n";

        bool spikePlanted = false;
        bool spikeDefused = false;
        std::vector<std::string> deadPlayers;

        std::barrier<> matchSync(10); 
        std::vector<std::thread> threads;

        for (Player* p : teamA->players){
            threads.emplace_back(&Match::playerMatchEvent, this, std::ref(*p), std::ref(matchSync), teamB, std::ref(spikePlanted), std::ref(spikeDefused), std::ref(deadPlayers));
        }
        for (Player* p : teamB->players){
            threads.emplace_back(&Match::playerMatchEvent, this, std::ref(*p), std::ref(matchSync), teamA, std::ref(spikePlanted), std::ref(spikeDefused), std::ref(deadPlayers));
        }

        for (std::thread& t : threads){
            t.join();
        }

        if (spikeDefused){
            std::cout << "Spike Defused: Defenders win the round!\n";
            teamBScore++;
        } 
        else if (spikePlanted){
            std::cout << "Spike Detonated: Attackers win the round via explosion.\n";
            teamAScore++;
        } 
        else {
            //No spike event happened
            if (coinFlip(mainGen) == 0){
                std::cout << teamA->name << " (Attackers) eliminated all opponents!\n";
                teamAScore++;
            } else {
                std::cout << teamB->name << " (Defenders) eliminated all opponents!\n";
                teamBScore++;
            }
        }

        score();
        currentRound++;
    }

    matchWinner = (teamAScore > teamBScore) ? teamA : teamB;
}

void Match::score(){
    std::cout << "\nUpdating Scores...\n";
    std::cout << teamA->name << " Score: " << teamAScore << "\n";
    std::cout << teamB->name << " Score: " << teamBScore << "\n";
}

Team* Match::winner(){
    std::cout << "Winner: " << matchWinner->name << "\n";
    return matchWinner;
}