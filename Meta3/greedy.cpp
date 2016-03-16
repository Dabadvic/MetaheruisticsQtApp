#include "greedy.h"
#include <array>

greedy::greedy(const vector<vector<int> > &dis,const vector<vector<int> > &fl)
{
    distances = dis;
    flow = fl;
}

void greedy::calculateSolution()
{
    int size = distances.size();

    //Guardar las sumas de flujos y distancias
    vector<int> distances_sum (size, 0), flow_sum (size, 0);

    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
        {
            distances_sum[i] += distances.at(i).at(j);
            flow_sum[i] += flow.at(i).at(j);
        }
    }
    vector<int> solution (size, 0);

    int max, max_pos, min, min_pos;
    //Asignar sitios
    for (int i=0;i<size;i++)
    {
        //Se busca el mayor flujo y la menor distancia
        max = -2;
        min = 999999999;
        max_pos = 0;
        min_pos = 0;
        for (int j=0;j<size;j++)
        {
            if (flow_sum[j] > max)
            {
                max = flow_sum[j];
                max_pos = j;
            }
            if (distances_sum[j] < min && distances_sum[j] > 0)
            {
                min = distances_sum[j];
                min_pos = j;
            }
        }
        /*En la solución, se asigna a la unidad con mayor flujo
            el lugar con menor distancia.*/
        solution[max_pos] = min_pos;
        //Para que no vuelva a coger la distancia o flujo, se marcan como -1
        flow_sum[max_pos] = -1;
        distances_sum[min_pos] = -1;
    }

    //Se almacena la solución en la variable
    for (int i=0;i<size;i++)
    {
        _solution.push_back(solution[i]);
    }

}

vector<int> greedy::getSolution()
{
    return _solution;
}
