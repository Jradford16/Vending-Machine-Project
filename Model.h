#ifndef MODEL_H
#define MODEL_H

#include "Enums.h"
#include "VendingDfa1.h"
#include "VendingDfa2.h"


class Model {

public:

    Model();

    // Abstracts underlying DFA's to return an integer representing
    // the id of the item to be dispensed. Can return A1, A2, B1, B2, Start, Steady, Reset.
    Dispense AcceptToken(Alphabet Token);

    // TODO: Refactor setting prices as a parent automata operation rather than a subsystem
    void SetPrice(int price, int item);

    int prices[4];

    void Reset(); 

private:

    VendingDfa1 Dfa1;
    VendingDfa2 Dfa2;

};

#endif // MODEL_H
