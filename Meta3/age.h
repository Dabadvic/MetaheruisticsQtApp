#ifndef AGE_H
#define AGE_H

#include <vector>
#include <stdlib.h>
#include "chromosome.h"

using namespace std;

class Age
{
public:
    Age(const vector<vector<int> > &distances, const vector<vector<int> > &flow, int type, int seed = 123456, int evaluations = 20000, int population = 50, float mutationProbability = 0.01);
    void search();
    int getCost();
    vector<int> getSolution();

private:
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;
    vector< Chromosome > generation_;
    int seed_, population_, type_, evaluations_;
    float mutationProbability_;

    vector<int> generateRandomSolution();
    vector<int> binaryTournament(Chromosome parent1, Chromosome parent2);
    void randomBinarySwap(vector<int> &vector);
    pair<vector<int>, vector<int> > crossParentsPMX(vector<int> parent1, vector<int> parent2);
    vector<int> crossParentsPosition(vector<int> parent1, vector<int> parent2);
    int calculateCost(vector<int> permutation);
    vector<int> crossParentsOX(vector<int> parent1, vector<int> parent2);
};

#endif // AGE_H
