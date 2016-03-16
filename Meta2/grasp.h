#ifndef GRASP_H
#define GRASP_H

#include <vector>
#include <stdlib.h>
#include "localsearch.h"

using namespace std;

struct Assignment{
    int u,l,cost;
};

class Grasp
{
public:
    Grasp(const vector<vector<int> > &distances, const vector<vector<int> > &flow, int seed = 123456);

    void search();
    int getCost();
    vector<int> getSolution();

private:
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;
    vector<int> solution_;
    int cost_, seed_;

    vector<int> randomGreedy();
    vector<Assignment> generateCandidateList(vector<bool> lcu, vector<bool> lcl, vector<int> &partial_solution);
};

#endif // GRASP_H
