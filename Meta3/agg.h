#ifndef AGG_H
#define AGG_H

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "chromosome.h"
#include "localsearch.h"

using namespace std;


class Agg
{
public:
    Agg(const vector<vector<int> > &distances, const vector<vector<int> > &flow, int type, int am_type = 0, int seed = 123456, int evaluations = 20000, int population = 50, float crossProbability = 0.7, float mutationProbability = 0.01);
    int getCost();
    vector<int> getSolution();
    void search();

private:
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;
    vector< Chromosome > generation_;
    int seed_, population_, type_, evaluations_, am_type_;
    float crossProbability_, mutationProbability_;

    vector<int> generateRandomSolution();
    int calculateCost(vector<int> permutation);
    Chromosome binaryTournament(Chromosome parent1, Chromosome parent2);
    vector<int> crossParentsPosition(vector<int> parent1, vector<int> parent2);
    void randomBinarySwap(vector<int> &vector);
    pair<vector<int>, vector<int> > crossParentsPMX(vector<int> parent1, vector<int> parent2);
    vector<int> crossParentsOX(vector<int> parent1, vector<int> parent2);
};

#endif // AGG_H
