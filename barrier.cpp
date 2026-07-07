#include "barrier.h"
#include <iostream>

using namespace std;

Barrier::Barrier(int threadCount, const string& completionMessage)
    : sync(threadCount > 0 ? threadCount : 1, [completionMessage]() {
          cout << "\n" << completionMessage << "\n" << endl;
      }) {
    if (threadCount <= 0)
        cout << "[ERROR] Barrier thread count must be greater than 0. Defaulting to 1.\n";
}

void Barrier::arriveAndWait() {
    sync.arrive_and_wait();
}

Barrier Barrier::connectionBarrier(int playerCount) {
    return Barrier(playerCount, "All players have reached the first barrier!\nAll players are connected!");
}

Barrier Barrier::selectionBarrier(int playerCount) {
    return Barrier(playerCount, "All players have reached the second barrier!\nAll players are ready!");
}

Barrier Barrier::matchBarrier(int threadCount) {
    return Barrier(threadCount, "All match events have been completed!");
}
