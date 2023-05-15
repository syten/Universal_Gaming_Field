
#include <iostream>

#include "field.h"

int main() {
    auto myObj1 = new MyObject;
    auto myObj2 = new MyObject2;


    Field field(3, 4);


    field.setObjectAtPos(myObj2, Field::Position(0, 0));
    field.setObjectAtPos(myObj1, Field::Position(1, 0));


    std::unordered_set<GameObject*> objs = field.getObjectsToInteract(myObj1);

    InteractionsCaller::getRightInteraction(INTERACTION, myObj1, *objs.begin(), true);

    delete myObj1;
    delete myObj2;

    return 0;
}