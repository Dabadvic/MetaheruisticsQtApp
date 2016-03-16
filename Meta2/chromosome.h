#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include "vector"

using namespace std;

class Chromosome
{
public:
    Chromosome();
    vector<int> solution;
    int cost;

    Chromosome& operator=(const Chromosome &orig);
};

#endif // CHROMOSOME_H
