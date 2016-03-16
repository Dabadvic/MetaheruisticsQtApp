#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include <vector>

using namespace std;

class LocalSearch
{
public:
    LocalSearch(const vector<vector<int> > &distances, const vector<vector<int> > &flow, const vector<int> &solution, int seed = 123456, bool useGreedy = false, int solutionsToCheck = 10000);
    void search();
    int getCost();

private:
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;
    vector<int> solution_;
    int cost_, solutionsToCheck_;
    bool useGreedy_;

    vector<int> swap(vector<int> old, int pos1, int pos2);
    void calculateCost();
    int calculateCost(int r, int s);
    vector<int> generateRandomSolution();
};

#endif // LOCALSEARCH_H
