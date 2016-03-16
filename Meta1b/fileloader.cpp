#include "fileloader.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>

#include <iostream>

FileLoader::FileLoader()
{

}

/**
 * @brief loadFile
 * @param path
 * @return Devuelve 0 en caso de error, 1 en caso de éxito en la lectura
 *
 *  Carga el contenido del archivo dentro de las variables de la clase.
 */
int FileLoader::loadFile (QString path)
{
//    fstream file;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return 0;

    QTextStream in(&file);
    int size, val, i=0, j;
    in >> size;
    vector<int> aux;
    while (!in.atEnd() && i<size)
    {
        j=0;
        aux.clear();
        while (j<size && !in.atEnd())
        {
            in >> val;
            aux.push_back(val);
            j++;
        }
        flow.push_back(aux);
        i++;
    }

    i=0;
    while (!in.atEnd() && i<size)
    {
        j=0;
        aux.clear();
        while (j<size && !in.atEnd())
        {
            in >> val;
            aux.push_back(val);
            j++;
        }
        distances.push_back(aux);
        i++;
    }

    file.close();
    //file.exceptions ( fstream::failbit | fstream::badbit );

//    try
//    {
//        int size;
//        //const char* name = path.toStdString().c_str();
//        file.open("D:/Dropbox/Mi Uni/Meta2014/Practica1a/Instancias y tablas QAP 14-15/chr20a.dat");
//        file >> size;//Tamaño

//        //Guarda flujos
//        vector<int> aux;
//        int i=0, j, val;
//        while (i<size && !file.eof())
//        {
//            aux.clear();
//            j=0;
//            while (j<size && !file.eof())
//            {
//                file >> val;
//                aux.push_back(val);
//                j++;
//            }
//            flow.push_back(aux);
//            i++;
//        }

//        //Guarda distancias
//        i=0;
//        while (i<size && !file.eof())
//        {
//            aux.clear();
//            j=0;
//            while (j<size && !file.eof())
//            {
//                file >> val;
//                aux.push_back(val);
//                j++;
//            }
//            distances.push_back(aux);
//            i++;
//        }
//    }
//    catch (fstream::failure e)
//    {
//        return 0;
//    }

//    file.close();
    return 1;
}

vector<vector<int> > FileLoader::getDistancesMatrix()
{
    return distances;
}

vector<vector<int> > FileLoader::getFlowMatrix()
{
    return flow;
}
