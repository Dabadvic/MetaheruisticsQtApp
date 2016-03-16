#include "localsearch.h"

#include <array>

#include <iostream>

/**
 * @brief LocalSearch::LocalSearch
 * @param distances Matriz con las distancias entre lugares
 * @param flow      Matriz con el flujo entre unidades
 * @param solution  Permutación con la solución base desde la que buscar
 */
LocalSearch::LocalSearch(const vector<vector<int> > &distances,const vector<vector<int> > &flow,const vector<int> &solution, int seed, bool useGreedy, int solutionsToCheck)
{
    distances_ = distances;
    flow_ = flow;
    solution_ = solution;//generateRandomSolution();
    calculateCost();
    useGreedy_ = useGreedy;
    solutionsToCheck_ = solutionsToCheck;
    srand(seed);
}

/**
 * @brief LocalSearch::calculateCost
 *
 * Calcula el coste de la actual solución.
 */
void LocalSearch::calculateCost()
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
 * @brief LocalSearch::calculateCost
 * @param pos1
 * @param pos2
 * @return  Coste de la solución
 *
 * Calcula el coste de una posible solución para el problema que está manejando,
 * tras realizar el intercambio de los dos elementos. Quita el coste de los dos
 * elementos de la solución vieja y añade el coste de la nueva.
 */
int LocalSearch::calculateCost(int r, int s)
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
 * @brief LocalSearch::swap
 * @param old
 * @param pos1
 * @param pos2
 * @return Vector con los dos elementos intercambiados
 *
 * Intercambia dos elementos en el vector
 */
vector<int> LocalSearch::swap(vector<int> old, int pos1, int pos2)
{
    vector<int> new_(old);
    iter_swap(new_.begin()+pos1, new_.begin()+pos2);
    return new_;
}

/**
 * @brief LocalSearch::generateRandomSolution
 * @return Vector con la solución generada
 *
 * Genera una solución aleatoria.
 */
vector<int> LocalSearch::generateRandomSolution()
{
    int size = solution_.size(), location;
    vector<int> new_;
    new_.assign(size, -1);
    bool found;

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
 * @brief localsearch::search
 *
 * Realiza la búsqueda local.
 */
void LocalSearch::search()
{
    //Genera solución inicial aleatoria o usa Greedy (ya en la variable)
    if (!useGreedy_)
    {
        solution_ = generateRandomSolution();
        calculateCost();
    }

    int size = solution_.size();
    int cost, solutions_checked = 0, j;
    bool improve = true, found;

    int* dontLookBits;
    dontLookBits = new int[size];

    //Inicialización del array Don't look bits
    for (int i=0;i<size;i++)
        dontLookBits[i] = 0;

    int i = 0;
    while (improve && solutions_checked < solutionsToCheck_)
    {
        j = i+1;
        found = false;

        //Comprueba que la unidad está activada
        if (dontLookBits[i] != 0)
            found = true;

        while (j < size && !found)
        {
            solutions_checked++;
            cost = calculateCost(i, j);
            if (cost < cost_)
            {
                //La nueva solución es mejor
                dontLookBits[i] = 0;
                solution_ = swap(solution_, i, j);
                cost_ = cost;
                found = true;
                i = 0;
            }
            j++;
        }
        if (!found)
            dontLookBits[i] = 1;
        //Si llega al último elemento y no ha encontrado mejores soluciones, considerará que no hay mejora posible
        if (i == size-1)
            improve = false;

        i++;
    }

    delete[] dontLookBits;
}

int LocalSearch::getCost()
{
    return cost_;
}
