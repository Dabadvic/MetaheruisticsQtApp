#include "tabusearch.h"
#include <random>
#include <time.h>

#include<array>

TabuSearch::TabuSearch(const vector<vector<int> > &distances, const vector<vector<int> > &flow, const vector<int> &solution, int seed, bool useGreedy, int solutionsToCheck)
{
    distances_ = distances;
    flow_ = flow;
    solution_ = best_ = solution;//generateRandomSolution();
    calculateCost();
    best_cost_ = cost_;
    useGreedy_ = useGreedy;
    solutionsToCheck_ = solutionsToCheck;
    srand(seed);
}

int TabuSearch::getCost()
{
    return best_cost_;
}

/**
 * @brief TabuSearch::calculateCost
 *
 * Calcula el coste de la actual solución.
 */
void TabuSearch::calculateCost()
{
    cost_ = 0;
    int size = solution_.size();
    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
        {
            cost_ += distances_[solution_[i]][solution_[j]] * flow_[i][j];
        }
    }
}

/**
 * @brief TabuSearch::calculateCost
 * @param pos1
 * @param pos2
 * @return  Coste de la solución
 *
 * Calcula el coste de una posible solución para el problema que está manejando,
 * tras realizar el intercambio de los dos elementos. Quita el coste de los dos
 * elementos de la solución vieja y añade el coste de la nueva.
 */
int TabuSearch::calculateCost(int r, int s)
{
    int cost = cost_;
    for (int k=0;k<(int)solution_.size();k++)
    {
        if (k != r && k != s)
        {
            //Se añade el coste de los nuevos elementos
            cost += flow_[r][k] * (distances_[solution_[s]][solution_[k]] - distances_[solution_[r]][solution_[k]]);
            cost += flow_[s][k] * (distances_[solution_[r]][solution_[k]] - distances_[solution_[s]][solution_[k]]);

            cost += flow_[k][r] * (distances_[solution_[k]][solution_[s]] - distances_[solution_[k]][solution_[r]]);
            cost += flow_[k][s] * (distances_[solution_[k]][solution_[r]] - distances_[solution_[k]][solution_[s]]);
        }
    }

    return cost;
}

/**
 * @brief TabuSearch::swap
 * @param old
 * @param pos1
 * @param pos2
 * @return  Vector con los dos elementos intercambiados
 *
 * Intercambia dos elementos en el vector
 */
vector<int> TabuSearch::swap(vector<int> old, int pos1, int pos2)
{
    vector<int> new_(old);
    iter_swap(new_.begin()+pos1, new_.begin()+pos2);
    return new_;
}

/**
 * @brief TabuSearch::generateRandomSolution
 * @return Solución generada
 *
 * Genera una solución aleatoria.
 */
vector<int> TabuSearch::generateRandomSolution()
{
    int size = solution_.size(), location;
    vector<int> new_;
    new_.assign(size, -1);
    bool found = false;

    for (int i=0;i<size;i++)
    {
        //Crear una localización aleatoria que no haya sido asignada ya
        found = false;
        while (!found)
        {
            location = rand() % size;
            found = true;
            //Recorrer el vector con la solución, comprobando si se encuentra ya
            for (int j=0;j<size;j++)
                if (location == new_[j])
                    found = false;
        }
        new_[i] = location;
    }
    return new_;
}

/**
 * @brief TabuSearch::generateTabuSolution
 * @param frec
 * @return Vector con la solución generada
 *
 * Genera una nueva solución a partir de una lista de frecuencias.
 *
 * Para la generación, se tiene en cuenta un vector de booleanos para las distancias, que se
 * encontrará a true cuando la distancia haya sido asignada y false en caso contrario.
 */
vector<int> TabuSearch::generateTabuSolution(int** &frec)
{
    int size = solution_.size(), min, min_pos;
    vector<int> new_;
    vector<bool> assigned;
    new_.assign(size, -1);
    assigned.assign(size, false);

    //Recorre los flujos
    for (int i=0;i<size;i++)
    {
        //Recorre las distancias
        min = 999999;
        for (int j=0;j<size;j++)
        {
            if (frec[i][j]+frec[j][i] < min && !assigned[j])
            {
                min = frec[i][j]+frec[j][i];
                min_pos = j;
            }
        }
        new_[i] = min_pos;
        assigned[min_pos] = true;
    }

    return new_;
}

/**
 * @brief TabuSearch::search
 *
 * Realiza la búsqueda Tabu.
 */
void TabuSearch::search()
{
    //Genera solución inicial aleatoria o usa Greedy (ya en la variable)
    if (!useGreedy_)
    {
        solution_ = generateRandomSolution();
        calculateCost();
    }

    int cost, iterations = 0;
    bool reinitialize = true;
    int size = solution_.size();
    TabuList tabu_list(size/2);
//    vector< vector<int> > long_memory_frec;
//    vector<int> initialize;
//    initialize.assign(size, 0);
//    long_memory_frec.assign(size, initialize);
    int** long_memory_frec;
    long_memory_frec = new int*[size];
    for (int i=0;i<size;i++)
        long_memory_frec[i] = new int[size];

    int solutions_checked = 0, r, s;
    while (solutions_checked < solutionsToCheck_)
    {
        for (int i=0;i<30;i++)
        {
            r = rand() % size;
            s = rand() % size;

            //Comprobar si estos elementos están ya en la lista tabú
            if (!tabu_list.checkElement(solution_.at(r), solution_.at(s), r, s) && r != s)
            {
                solutions_checked++;
                cost = calculateCost(r, s);
                if (cost < cost_)
                {
                    solution_ = swap(solution_, r, s);
                    cost_ = cost;
                    if (cost < best_cost_)
                        reinitialize = false;
                    long_memory_frec[r][solution_.at(r)]++;
                    long_memory_frec[s][solution_.at(s)]++;
                }
                else
                {
                    tabu_list.addElement(solution_.at(r), solution_.at(s), r, s);
                }
            }
        }

        if (cost_ < best_cost_)
        {
            best_ = solution_;
            best_cost_ = cost_;
        }

        iterations++;
        //Comprobar si hay que reiniciar
        if (iterations == 10)
        {
            if (reinitialize)
            {
                int chance = rand() % 100 +1;
                if (chance < 25)
                {
                    //Solucion inicial aleatoria
                    solution_ = generateRandomSolution();
                    calculateCost();
                }
                else if (chance < 50)
                {
                    //Usar la mejor solucion obtenida
                    solution_ = best_;
                    cost_ = best_cost_;
                }
                else
                {
                    //Usar memoria largo plazo para generar solucion
                    solution_ = generateTabuSolution(long_memory_frec);
                    calculateCost();
                }
                chance = rand() % 100 +1;
                if (chance < 50)
                {
                    tabu_list.deleteOneSpace();
                }
                else
                {
                    tabu_list.addOneSpace();
                }
                tabu_list.reinitializeList();
            }
            iterations = 0;
            reinitialize = true;
        }
    }
    for (int i=0;i<size;i++)
        delete[] long_memory_frec[i];
    delete[] long_memory_frec;
}
