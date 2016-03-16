#include "ils.h"

Ils::Ils(const vector<vector<int> > &distances, const vector<vector<int> > &flow, int seed)
{
    distances_ = distances;
    flow_ = flow;
    seed_ = seed;
    srand(seed);
}

/**
 * @brief Ils::generateRandomSolution
 * @return Vector con la solución aleatoria
 *
 * Crea una solución aleatoria.
 */
vector<int> Ils::generateRandomSolution()
{
    int size = distances_.size(), location;
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
 * @brief Ils::search
 */
void Ils::search()
{
    solution_ = generateRandomSolution();
    LocalSearch local_search(distances_, flow_, solution_, seed_, true);
    cost_ = local_search.getCost();

    local_search.search();
    solution_ = local_search.getSolution();
    cost_ = local_search.getCost();

    int mutation_size = solution_.size() / 4, first_mutation, cost;
    vector<int> solution;
    solution = solution_;

    for (int i=1;i<25;i++)
    {
        //Aplicar mutación
        first_mutation = rand() % (solution_.size() - mutation_size);
        random_shuffle(solution.begin() + first_mutation, solution.begin() + first_mutation + mutation_size);

        //Aplicar la búsqueda local
        local_search.setSolution(solution);
        local_search.search();
        solution = local_search.getSolution();
        cost = local_search.getCost();

        //Comprobar el criterio de aceptación
        if (cost < cost_)
        {
            cost_ = cost;
            solution_ = solution;
        }
        else
        {
            cost = cost_;
            solution = solution_;
        }
    }
}

int Ils::getCost()
{
    return cost_;
}

vector<int> Ils::getSolution()
{
    return solution_;
}
