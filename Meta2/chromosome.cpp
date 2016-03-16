#include "chromosome.h"

Chromosome::Chromosome()
{
}

Chromosome &Chromosome::operator=(const Chromosome &orig)
{
    solution = orig.solution;
    cost = orig.cost;
    return *this;
}
