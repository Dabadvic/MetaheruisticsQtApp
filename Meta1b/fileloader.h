#ifndef FILELOADER_H
#define FILELOADER_H

#include <vector>
#include <string>
#include <QString>

using namespace std;

class FileLoader
{
public:
    FileLoader();

    int loadFile(QString path);
    vector< vector<int> > getDistancesMatrix();
    vector< vector<int> > getFlowMatrix();
private:
    vector< vector<int> > distances;
    vector< vector<int> > flow;
};

#endif // FILELOADER_H
