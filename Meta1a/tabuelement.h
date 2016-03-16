#ifndef TABUELEMENT_H
#define TABUELEMENT_H

class TabuElement
{
public:
    TabuElement();
    void setElement(int i, int j, int posi, int posj);
    bool checkElement(int i, int j, int posi, int posj);
private:
    int i_, j_, posi_, posj_;
};

#endif // TABULIST_H
