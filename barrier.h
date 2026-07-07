#ifndef BARRIER_H
#define BARRIER_H

#include <barrier>
#include <functional>
#include <string>

using namespace std;

class Barrier {
private:
    barrier<function<void()>> sync;

    Barrier(int threadCount, const string& completionMessage);

public:
    void arriveAndWait();

    static Barrier connectionBarrier(int playerCount);

    static Barrier selectionBarrier(int playerCount);

    static Barrier matchBarrier(int threadCount);
};

#endif
