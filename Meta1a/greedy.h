#ifndef GREEDY_H
#define GREEDY_H

#include <vector>

using namespace std;

class greedy
{
public:
    greedy(const vector<vector<int> > &dis, const vector<vector<int> > &fl);
    void calculateSolution();
    vector<int> getSolution();
private:
    vector< vector<int> > distances;
    vector< vector<int> > flow;
    vector<int> _solution;
};

#endif // GREEDY_H
