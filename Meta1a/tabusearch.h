#ifndef TABUSEARCH_H
#define TABUSEARCH_H

#include "tabulist.h"
#include <vector>

using namespace std;

class TabuSearch
{
public:
    TabuSearch(const vector< vector<int> > &distances, const vector< vector<int> > &flow, const vector<int> &solution, int seed = 123456, bool useGreedy = false, int solutionsToCheck = 10000);
    int getCost();
    void search();
private:
    vector< vector<int> > distances_, flow_;
    vector<int> solution_, best_;
    int cost_, best_cost_, solutionsToCheck_;
    bool useGreedy_;
    void calculateCost();
    int calculateCost(int r, int s);
    vector<int> swap(vector<int> old, int pos1, int pos2);
    vector<int> generateRandomSolution();
    vector<int> generateTabuSolution(int **&frec);
};

#endif // TABUSEARCH_H
