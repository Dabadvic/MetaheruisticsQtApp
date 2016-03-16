#ifndef TABULIST_H
#define TABULIST_H

#include "tabuelement.h"
#include <vector>

using namespace std;

class TabuList
{
public:
    TabuList(int max_size);
    void addElement(int i, int j, int posi, int posj);
    bool checkElement(int i, int j, int posi, int posj);
    void deleteOneSpace();
    void addOneSpace();
    void reinitializeList();
private:
    vector<TabuElement> vector_;
    int pos_, size_;
};

#endif // TABULIST_H
