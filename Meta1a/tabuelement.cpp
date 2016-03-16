#include "tabuelement.h"

TabuElement::TabuElement()
{
}

/**
 * @brief TabuElement::setElement
 * @param i
 * @param j
 * @param posi
 * @param posj
 *
 * Cambia el valor del elemento.
 */
void TabuElement::setElement(int i, int j, int posi, int posj)
{
    i_ = i;
    j_ = j;
    posi_ = posi;
    posj_ = posj;
}

/**
 * @brief TabuElement::checkElement
 * @param i
 * @param j
 * @param posi
 * @param posj
 * @return  Devuelve true en caso positivo, false en caso negativo
 *
 * Comprueba si el valor del elemento es igual a los valores.
 */
bool TabuElement::checkElement(int i, int j, int posi, int posj)
{
    if (i == i_ || j == j_ || posi == posi_ || posj == posj_)
        return true;
    if (i == j_ || j == i_ || posi == posj_ || posj == posi_)
        return true;
    return false;
}
