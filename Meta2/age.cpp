#include "age.h"

Age::Age(const vector<vector<int> > &distances, const vector<vector<int> > &flow, int type, int seed, int evaluations, int population, float mutationProbability)
{
    distances_ = distances;
    flow_ = flow;
    srand(seed);
    seed_ = seed;
    population_ = population;
    mutationProbability_ = mutationProbability;
    type_ = type;
    evaluations_ = evaluations;
}

void Age::search()
{
    Chromosome son1, son2;
    pair< vector<int>, vector<int> > parents;
    pair< Chromosome,Chromosome > nextGeneration;
    int evaluations = 0, parent1, parent2, toMutate, n;
    int max1, max2, maxPos1 = 0, maxPos2 = 0;

    //Se calculan la cantidad de genes que van a mutar
    toMutate = mutationProbability_ * (2 * flow_.size());

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
        parent1 = rand() % population_;
        parent2 = rand() % population_;
        parents.first = binaryTournament(generation_[parent1], generation_[parent2]);

        parent1 = rand() % population_;
        parent2 = rand() % population_;
        parents.second = binaryTournament(generation_[parent1], generation_[parent2]);

        //Se cruzan los padres, generando siempre dos hijos
        if (type_ == 0)
        {
            son1.solution = crossParentsPosition(parents.first, parents.second);
            son2.solution = crossParentsPosition(parents.first, parents.second);

        }
        else
        {
            son1.solution = crossParentsOX(parents.first, parents.second);
            son2.solution = crossParentsOX(parents.second, parents.first);
        }

        //Se mutan la cantidad esperada de mutaciones
        for (int i=0;i<toMutate;i++)
        {
            n = rand() % 100;
            if (n < 50)
                randomBinarySwap(son1.solution);
            else
                randomBinarySwap(son2.solution);
        }

        //Se evalua la nueva generación
        son1.cost = calculateCost(son1.solution);
        son2.cost = calculateCost(son2.solution);
        evaluations += 2;

        //Los hijos sustituyen a los dos peores, si son mejores que ellos
        max1 = 0;
        max2 = 0;
        for (int i=0;i<population_;i++)
        {
            if (generation_[i].cost > max1)
            {
                max1 = generation_[i].cost;
                maxPos1 = i;
            }

            if (generation_[i].cost > max2 && i != maxPos1)
            {
                max2 = generation_[i].cost;
                maxPos2 = i;
            }
        }
        if (son1.cost < generation_[maxPos1].cost)
        {
            generation_[maxPos1] = son1;

            if (son2.cost < generation_[maxPos2].cost)
                generation_[maxPos2] = son2;
        }
        else if (son1.cost < generation_[maxPos2].cost)
        {
            generation_[maxPos2] = son1;

            if (son2.cost < generation_[maxPos1].cost)
                generation_[maxPos1] = son2;
        }
        else
        {
            if (son2.cost < generation_[maxPos2].cost)
                generation_[maxPos2] = son2;
            else if (son2.cost < generation_[maxPos1].cost)
                generation_[maxPos1] = son2;
        }
    }
}

int Age::getCost()
{
    int min = 999999999;

    for (int i=0;i<(int)generation_.size();i++)
        if (generation_[i].cost < min)
            min = generation_[i].cost;

    return min;
}

vector<int> Age::getSolution()
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

/**
 * @brief Age::generateRandomSolution
 * @return Solución generada
 *
 * Genera una solución aleatoria válida.
 */
vector<int> Age::generateRandomSolution()
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
 * @brief Age::calculateCost
 * @param permutation
 * @return Coste de la permutación
 *
 * Calcula el coste de una permutacion.
 */
int Age::calculateCost(vector<int> permutation)
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

vector<int> Age::binaryTournament(Chromosome parent1, Chromosome parent2)
{
    if (parent1.cost < parent2.cost)
        return parent1.solution;
    else
        return parent2.solution;
}

/**
 * @brief Age::crossParentsPosition
 * @param parent1
 * @param parent2
 * @return
 *
 * Función que cruza dos cromosomas. Para ello lo que hace es recorrer los padres y comprobar los elementos que son similares
 * metiéndolos en el vector hijo, mientras que los elementos que no son iguales se van guardando en otro vector, que se
 * va a utilizar a modo de "bolsa" de la que ir sacando los elemento restantes para colocarlos en el hijo.
 */
vector<int> Age::crossParentsPosition(vector<int> parent1, vector<int> parent2)
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
 * @brief Age::crossParentsOX
 * @param parent1
 * @param parent2
 * @return
 */
vector<int> Age::crossParentsOX(vector<int> parent1, vector<int> parent2)
{
    vector<int> son, elements;
    int size = parent1.size(), begin, end;
    bool is;
    son.assign(size, -1);

    begin = rand() % size/2;
    end = begin + size/2;

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

        if (!is)
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
 * @brief Age::crossParentsPMX
 * @param parent1
 * @param parent2
 * @return
 *
 * Divide a los padres en tres partes para hacer dos hijos que cogen la parte central de uno de los padres y el resto se
 * asigna mediante correspondencia o si no existe tal, se deja el elemento.
 */
pair< vector<int>, vector<int> > Age::crossParentsPMX(vector<int> parent1, vector<int> parent2)
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

        element.first = parent2[i];
        element.second = parent1[i];

        relation.push_back(element);
    }

    //Elementos pre-centrales
    for (int i=0;i<begin;i++)
    {
        found1 = false; found2 = false;
        for (int j=0;j<(int)relation.size();j++)
        {
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
 * @brief Age::randomBinarySwap
 * @param vector
 *
 * Intercambia aleatoriamente dos elementos de un vector.
 */
void Age::randomBinarySwap(vector<int> &vector)
{
    int pos1, pos2;

    pos1 = rand() % vector.size();
    pos2 = rand() % vector.size();

    while (pos1 == pos2)
        pos2 = rand() % vector.size();

    iter_swap(vector.begin()+pos1, vector.begin()+pos2);
}
