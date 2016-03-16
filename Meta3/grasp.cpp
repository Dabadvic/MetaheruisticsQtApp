#include "grasp.h"

Grasp::Grasp(const vector<vector<int> > &distances, const vector<vector<int> > &flow, int seed)
{
    distances_ = distances;
    flow_ = flow;
    srand(seed);
    seed_ = seed;
}

/**
 * @brief Grasp::randomGreedy
 * @return  Solución generada
 *
 * Crea una solución Greedy aleatoria.
 */
vector<int> Grasp::randomGreedy()
{
    //Generacion lista de candidatos
    int size = distances_.size();
    vector<int> u_costs, l_costs, solution;
    vector<bool> lc_u, lc_l;
    u_costs.assign(size, 0);
    l_costs.assign(size, 0);
    solution.assign(size, -1);

    //Estas listas se usan para comprobar si un elemento se puede usar (true) o no (false)
    lc_u.assign(size, true);
    lc_l.assign(size, true);

    int max = 0, max_u, max_l, min = 9999999, min_u, min_l;
    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
            u_costs[i] += flow_[i][j] + flow_[j][i];

        if (u_costs[i] > max)
        {
            max = u_costs[i];
            max_u = i;
        }
        if (u_costs[i] < min)
        {
            min = u_costs[i];
            min_u = i;
        }
    }

    max = 0; min = 99999999;
    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
            l_costs[i] += distances_[i][j] + distances_[j][i];

        if (l_costs[i] > max)
        {
            max = l_costs[i];
            max_l = i;
        }
        if (l_costs[i] < min)
        {
            min = l_costs[i];
            min_l = i;
        }
    }

    //Generar listas restringidas
    float alfa = 0.3;
    float umbral_u = u_costs[max_u] - alfa * (u_costs[max_u] - u_costs[min_u]);
    float umbral_l = l_costs[min_l] + alfa * (l_costs[max_l] - l_costs[min_l]);
    vector<int> lrc_u,lrc_l;

    for (int i=0;i<size;i++)
    {
        if (u_costs[i] >= umbral_u)
            lrc_u.push_back(i);
        if (l_costs[i] <= umbral_l)
            lrc_l.push_back(i);
    }

    //Se asignan los dos primeros elementos aleatorios de las listas restringidas, que se añaden y borran
    int ele1 = rand() % lrc_u.size();
    int ele2 = rand() % lrc_l.size();

    solution[lrc_u[ele1]] = lrc_l[ele2];

    lc_u[lrc_u[ele1]] = false;
    lc_l[lrc_l[ele2]] = false;

    //Usar la lista de candidatos y eliminar los elementos escogidos

    while (lc_u[lrc_u[ele1]] == false)
        ele1 = rand() % lrc_u.size();
    while (lc_l[lrc_l[ele2]] == false)
        ele2 = rand() % lrc_l.size();

    solution[lrc_u[ele1]] = lrc_l[ele2];

    lc_u[lrc_u[ele1]] = false;
    lc_l[lrc_l[ele2]] = false;

    //Comienza la etapa 2

    //Creacion LC
    vector<Assignment> candidateList, restricted_list;
    candidateList = generateCandidateList(lc_u, lc_l, solution);
    //for (int k=2;k<(int)solution.size();k++)
    while (candidateList.size() > 1)
    {
        //Calculo del umbral
        int cmax = 0, cmin = 99999999;
        for (int i=0;i<(int)candidateList.size();i++)
        {
            if (candidateList[i].cost > cmax)
                cmax = candidateList[i].cost;

            if (candidateList[i].cost < cmin)
                cmin = candidateList[i].cost;
        }

        float umbral = cmin + alfa * (cmax - cmin);

        //Creacion LRC
        restricted_list.clear();
        for (int i=0;i<(int)candidateList.size();i++)
            if (candidateList[i].cost <= umbral)
                restricted_list.push_back(candidateList[i]);

        //Se escoge y añade aleatoriamente una asignación
        int random = rand() % restricted_list.size();

        solution[restricted_list[random].u] = restricted_list[random].l;
        lc_u[restricted_list[random].u] = false;
        lc_l[restricted_list[random].l] = false;

        candidateList = generateCandidateList(lc_u, lc_l, solution);
    }

    solution[candidateList[0].u] = candidateList[0].l;

    return solution;
}

/**
 * @brief Grasp::generateCandidateList
 * @param lcu   Lista de candidatos de unidades
 * @param lcl   Lista de candidatos de localidades
 * @return      Lista de candidatos
 *
 * Genera una lista de candidatos, no restringida, para la segunda etapa de la creación de una solución Greedy aleatoria.
 *
 * Tiene en cuenta que los elementos no se hayan usado y estén disponibles.
 */
vector<Assignment> Grasp::generateCandidateList(vector<bool> lcu, vector<bool> lcl, vector<int> &partial_solution)
{
    Assignment assign;
    vector<Assignment> candidates;
    int size = lcu.size();

    for (int i=0;i<size;i++)
    {
        if (lcu[i])
        {
            assign.u = i;
            for (int j=0;j<size;j++)
            {
                if(lcl[j])
                {
                    assign.l = j;
                    assign.cost = 0;

                    for (int k=0;k<size;k++)
                        if (partial_solution[k] != -1)
                            assign.cost += flow_[i][k] * distances_[j][partial_solution[k]];

                    candidates.push_back(assign);
                }
            }
        }
    }

    return candidates;
}

/**
 * @brief Grasp::search
 */
void Grasp::search()
{
    vector<int> solution;
    int cost;
    //Repetir 25 veces
    solution.clear();
    solution = randomGreedy();

    //Aplicar Búsqueda local
    LocalSearch local_search(distances_, flow_, solution, seed_, true);
    cost_ = local_search.getCost();
    solution_ = local_search.getSolution();
    for (int i=1;i<25;i++)
    {
        //Generar solución inicial
        solution.clear();
        solution = randomGreedy();

        //Aplicar Búsqueda local
        local_search.setSolution(solution);
        local_search.search();
        cost = local_search.getCost();

        //Si la solución encontrada es mejor que la que tenemos, guardamos esa
        if (cost < cost_)
        {
            solution_ = local_search.getSolution();
            cost_ = cost;
        }
    }
}

int Grasp::getCost()
{
    return cost_;
}

vector<int> Grasp::getSolution()
{
    return solution_;
}
