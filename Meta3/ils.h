#ifndef ILS_H
#define ILS_H

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "localsearch.h"

using namespace std;

class Ils
{
public:
    Ils(const vector<vector<int> > &distances, const vector<vector<int> > &flow, int seed = 123456);

    void search();
    int getCost();
    vector<int> getSolution();

private:
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;
    vector<int> solution_;
    int cost_, seed_;

    vector<int> generateRandomSolution();
};

#endif // ILS_H
