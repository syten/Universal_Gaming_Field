
#include <iostream>

#include "field.h"

int main() {
    auto myObj1 = new MyObject;
    auto myObj2 = new MyObject2;


    Field field(6, 2);


    field.setObjectAtPos(myObj2, Field::Position(0, 3));
    field.setObjectAtPos(myObj1, Field::Position(1, 1));

    field.moveObjectAtPos(myObj1, Field::Position(1, 2));
    field.moveObjectAtPos(myObj1, Field::Position(1, 3));

    std::unordered_set<GameObject*> objs = field.getObjectsToInteract(myObj1);

    InteractionsCaller::getRightInteraction(INTERACTION, myObj1, *objs.begin(), true);

    if (myObj2->getHealth() == 0.) {
        std::cout << "Success\n";
    }

    delete myObj1;
    delete myObj2;

    return 0;
}