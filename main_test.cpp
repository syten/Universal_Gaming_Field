
#include <iostream>
#include "field.h"

int main() {
    auto gObj = new GameObject;
    Field field(3, 4);
    field.setObjectAtPos(gObj, Field::Position(0, 0));
    GameObject* gObj2 = *field.getObjectsAtPos(Field::Position(0, 0)).begin();
    if (gObj == gObj2 && field.getPosOfObject(gObj2) == Field::Position(0, 0))
        std::cout << "Success!\n";
    else
        std::cout << "Failed. :(\n";
    delete gObj;

    return 0;
}