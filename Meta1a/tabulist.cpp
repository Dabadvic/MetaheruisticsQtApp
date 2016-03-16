#include "tabulist.h"

TabuList::TabuList(int max_size)
{
    pos_ = 0;
    TabuElement t;
    t.setElement(0,0,0,0);
    vector_.assign(max_size, t);
    size_ = max_size;
}

/**
 * @brief TabuList::addElement
 * @param i
 * @param j
 * @param posi
 * @param posj
 *
 * Añade el elemento a la lista tabú, si ya se encuentra en la lista, lo añade a la posición
 * que corresponda, eliminando el anterior.
 */
void TabuList::addElement(int i, int j, int posi, int posj)
{
    pos_ = pos_ % size_;

    vector_[pos_].setElement(i, j, posi, posj);

    pos_++;
}

/**
 * @brief TabuList::checkElement
 * @param i
 * @param j
 * @param posi
 * @param posj
 * @return  True si se encuentra, false si no
 *
 * Comprueba si el cambio se encuentra en la lista tabu.
 */
bool TabuList::checkElement(int i, int j, int posi, int posj)
{
    for (int k=0;k<vector_.size();k++)
    {
        if (vector_[k].checkElement(i, j, posi, posj))
            return true;
    }
    return false;
}

/**
 * @brief TabuList::deleteLastElement
 *
 * Borra el último elemento del vector, reduciendo el tamaño en 1.
 */
void TabuList::deleteOneSpace()
{
    if (size_ > 1)
    {
        vector_.erase(vector_.end());
        size_--;
    }
}

/**
 * @brief TabuList::addOneSpace
 *
 * Incrementa en uno el tamaño del vector.
 */
void TabuList::addOneSpace()
{
    TabuElement t;
    vector_.push_back(t);
    size_++;
}

/**
 * @brief TabuList::reinitializeList
 *
 * Reinicia la lista tabú.
 */
void TabuList::reinitializeList()
{
    size_ = vector_.size();
    vector_.clear();
    TabuElement t;
    vector_.assign(size_, t);
}
