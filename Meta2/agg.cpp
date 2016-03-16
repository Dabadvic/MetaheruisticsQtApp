#include "agg.h"

Agg::Agg(const vector<vector<int> > &distances, const vector<vector<int> > &flow, int type, int seed, int evaluations, int population, float crossProbability, float mutationProbability)
{
    distances_ = distances;
    flow_ = flow;
    srand(seed);
    seed_ = seed;
    population_ = population;
    crossProbability_ = crossProbability;
    mutationProbability_ = mutationProbability;
    type_ = type;
    evaluations_ = evaluations;
}

int Agg::getCost()
{
    int min = 999999999;

    for (int i=0;i<(int)generation_.size();i++)
        if (generation_[i].cost < min)
            min = generation_[i].cost;

    return min;
}

vector<int> Agg::getSolution()
{
    vector<int> best;
    int min = 999999999, posMin = 0;

    for (int i=0;i<(int)generation_.size();i++)
        if (generation_[i].cost < min)
        {
            min = generation_[i].cost;
            posMin = i;
        }
    best = generation_[posMin].solution;

    return best;
}

void Agg::search()
{
    vector< Chromosome > fathers;
    pair< vector<int>,vector<int> > sons;
    vector<int> best;
    vector< Chromosome > nextGeneration;
    int evaluations = 0, father1, father2, toCross, toMutate, k, n;
    int max, min, maxPos = 0, minPos = 0;

    //Se calculan la cantidad de padres que se van a cruzar
    toCross = crossProbability_ * (population_/2);

    //Se calculan la cantidad de genes que van a mutar
    toMutate = mutationProbability_ * (population_ * flow_.size());

    //Generación 0, inicializarla aleatoriamente
    Chromosome chromosome;
    for (int i=0;i<population_;i++){
        chromosome.solution = generateRandomSolution();
        chromosome.cost = calculateCost(chromosome.solution);
        evaluations++;
        generation_.push_back(chromosome);
    }

    while (evaluations < evaluations_)
    {
        //Siguiente generación: Seleccionar, recombinar, mutar, reemplazar

        //Seleccionar mediante torneo binario los padres
        fathers.clear();
        for (int i=0;i<population_;i++)
        {
            father1 = rand() % population_;
            father2 = rand() % population_;
            fathers.push_back(binaryTournament(generation_[father1], generation_[father2]));
        }

        //Se cruzan la cantidad esperada de padres, dos a dos, generando dos hijos
        k = 0;
        chromosome.cost = 0;
        nextGeneration.clear();
        for (int i=0;i<toCross;i++)
        {
            if (type_ == 0)
            {
                chromosome.solution = crossParentsPosition(fathers[k].solution, fathers[k+1].solution);
                nextGeneration.push_back(chromosome);
                chromosome.solution = crossParentsPosition(fathers[k].solution, fathers[k+1].solution);
                nextGeneration.push_back(chromosome);
            }
            else
            {
                chromosome.solution = crossParentsOX(fathers[k].solution, fathers[k+1].solution);
                nextGeneration.push_back(chromosome);
                chromosome.solution = crossParentsOX(fathers[k+1].solution, fathers[k].solution);
                nextGeneration.push_back(chromosome);
            }
            k += 2;
        }

        //Se mutan la cantidad esperada de mutaciones
        for (int i=0;i<toMutate;i++)
        {
            n = rand() % nextGeneration.size();
            randomBinarySwap(nextGeneration[n].solution);
        }

        //Se evalua la nueva generación
        for (int i=0;i<(int)nextGeneration.size();i++)
        {
            nextGeneration[i].cost = calculateCost(nextGeneration[i].solution);
            evaluations++;
        }

        min = 999999999;
        //Se busca al mejor de la generación anterior
        for (int i=0;i<population_;i++)
        {
            if (generation_[i].cost < min)
            {
                min = generation_[i].cost;
                minPos = i;
            }
        }
        best = fathers[minPos].solution;

        //Los hijos sustituyen a los padres y se busca al peor
        max = 0;
        for (int i=0;i<(int)nextGeneration.size();i++)
        {
            fathers[i].solution = nextGeneration[i].solution;
            fathers[i].cost = nextGeneration[i].cost;

            if (fathers[i].cost > max)
            {
                max = fathers[i].cost;
                maxPos = i;
            }
        }

        //Se asegura que el mejor de la población anterior haya sobrevivido
        fathers[minPos].solution = best;
        fathers[minPos].cost = min;

        //Se guarda la nueva generación como la actual
        generation_ = fathers;
    }
}

/**
 * @brief Agg::generateRandomSolution
 * @return Solución generada
 *
 * Genera una solución aleatoria válida.
 */
vector<int> Agg::generateRandomSolution()
{
    int size = flow_.size(), location;
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
 * @brief Agg::calculateCost
 * @param permutation
 * @return Coste de la permutación
 *
 * Calcula el coste de una permutacion.
 */
int Agg::calculateCost(vector<int> permutation)
{
    int cost = 0;
    int size = permutation.size();
    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
        {
            cost += distances_[permutation[i]][permutation[j]] * flow_[i][j];
        }
    }

    return cost;
}

Chromosome Agg::binaryTournament(Chromosome parent1, Chromosome parent2)
{
    if (parent1.cost < parent2.cost)
        return parent1;
    else
        return parent2;
}

/**
 * @brief Agg::crossParentsPosition
 * @param parent1
 * @param parent2
 * @return
 *
 * Función que cruza dos cromosomas. Para ello lo que hace es recorrer los padres y comprobar los elementos que son similares
 * metiéndolos en el vector hijo, mientras que los elementos que no son iguales se van guardando en otro vector, que se
 * va a utilizar a modo de "bolsa" de la que ir sacando los elemento restantes para colocarlos en el hijo.
 */
vector<int> Agg::crossParentsPosition(vector<int> parent1, vector<int> parent2)
{
    vector<int> son, no_assigned;
    int size = parent1.size();
    int pos = 0;
    int gen;

    son.assign(size, -1);

    for (int i=0;i<size;i++)
    {
        if (parent1[i] == parent2[i])
        {
            son[i] = parent1[i];
        }
        else
        {
            no_assigned.push_back(parent1[i]);
        }
    }
    while (no_assigned.size() > 0)
    {
        if (son[pos] == -1)
        {
            gen = rand() % no_assigned.size();
            son[pos] = no_assigned[gen];
            no_assigned.erase(no_assigned.begin() + gen);
        }
        pos++;
    }

    return son;
}

/**
 * @brief Agg::crossParentsPMX
 * @param parent1
 * @param parent2
 * @return
 *
 * Divide a los padres en tres partes y coge la subcadena central
 */
pair< vector<int>, vector<int> > Agg::crossParentsPMX(vector<int> parent1, vector<int> parent2)
{
    pair< vector<int>, vector<int> > son;
    vector< pair<int,int> > relation;
    pair<int,int> element;
    int begin, end, gen1 = -1, gen2 = -1;
    int size = parent1.size();
    bool found1, found2;

    begin = size * 1/3;
    end = size * 2/3;
    son.first.assign(size, 0);
    son.second.assign(size, 0);

    //Se calcula la correspondencia y los elementos centrales de los hijos
    for (int i=begin;i<end;i++)
    {
        son.first[i] = parent2[i];
        son.second[i] = parent1[i];

        element.first = parent1[i];
        element.second = parent2[i];

        relation.push_back(element);
    }

    //Elementos pre-centrales
    for (int i=0;i<begin;i++)
    {
        found1 = false; found2 = false;
        for (int j=0;j<(int)relation.size();j++)
        {
            if (relation[j].first == parent1[i])
            {
                gen1 = relation[j].second;
                found1 = true;
            }
            if (relation[j].second == parent1[i])
            {
                gen1 = relation[j].first;
                found1 = true;
            }

            if (relation[j].first == parent2[i])
            {
                found2 = true;
                gen2 = relation[j].second;
            }
            if (relation[j].second == parent2[i])
            {
                found2 = true;
                gen2 = relation[j].first;
            }
        }

        if (found1)
            son.first[i] = gen1;
        else
            son.first[i] = parent1[i];

        if (found2)
            son.second[i] = gen2;
        else
            son.second[i] = parent2[i];
    }

    //Elementos post-centrales
    for (int i=end;i<size;i++)
    {
        found1 = false; found2 = false;
        for (int j=0;j<(int)relation.size();j++)
        {
            if (relation[j].first == parent1[i])
            {
                gen1 = relation[j].second;
                found1 = true;
            }
            if (relation[j].second == parent1[i])
            {
                gen1 = relation[j].first;
                found1 = true;
            }

            if (relation[j].first == parent2[i])
            {
                found2 = true;
                gen2 = relation[j].second;
            }
            if (relation[j].second == parent2[i])
            {
                found2 = true;
                gen2 = relation[j].first;
            }
        }

        if (found1)
            son.first[i] = gen1;
        else
            son.first[i] = parent1[i];

        if (found2)
            son.second[i] = gen2;
        else
            son.second[i] = parent2[i];
    }

    return son;
}

/**
 * @brief Agg::crossParentsOX
 * @param parent1
 * @param parent2
 * @return
 */
vector<int> Agg::crossParentsOX(vector<int> parent1, vector<int> parent2)
{
    vector<int> son, elements;
    int size = parent1.size(), begin, end;
    bool is;
    son.assign(size, -1);

    begin = rand() % size/2;
    end = (rand() % size/2) + size/2;

    for (int i=begin;i<end;i++)
    {
        son[i] = parent1[i];
    }
    for (int i=0;i<size;i++)
    {
        is = false;

        for (int j=begin;j<end;j++)
            if (parent2[i] == son[j])
                is = true;

        if(!is)
            elements.push_back(parent2[i]);
    }
    for (int i=0;i<size;i++)
    {
        if (son[i] == -1)
        {
            son[i] = elements[0];
            elements.erase(elements.begin());
        }
    }

    return son;
}

/**
 * @brief Agg::randomBinarySwap
 * @param vector
 *
 * Intercambia aleatoriamente dos elementos de un vector.
 */
void Agg::randomBinarySwap(vector<int> &vector)
{
    int pos1, pos2;

    pos1 = rand() % vector.size();
    pos2 = rand() % vector.size();

    while (pos1 == pos2)
        pos2 = rand() % vector.size();

    iter_swap(vector.begin()+pos1, vector.begin()+pos2);
}
