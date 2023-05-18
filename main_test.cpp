
#include <iostream>

#include "field.h"

bool areEqual(double a, double b) {
    return (std::fabs(a - b) < 0.000001);
}

bool interactDelete(Field& myField, MovingObject* movObj) {
    std::unordered_set<GameObject*> toInteract = myField.getObjectsToInteract(movObj);
    if (toInteract.empty()) {
        std::cout << "No objects to interact!\n" << std::flush;
        return false;
    }
    GameObject* gObj = *toInteract.begin();
    InteractionsCaller::getRightInteraction(INTERACTION, movObj, gObj, true);
    if (areEqual(gObj->getHealth(), 0.)) {
        myField.deleteObject(gObj);
        delete gObj;
    }
    return true;
}

int main() {
    Field myField(5, 4);
    auto myCreatorObject = new MyCreatorObject;
    auto myMovingObject = new MyMovingObject;
    auto myStaticObject = new MyStaticObject;
    auto myRotatingObject = new MyRotatingObject(RotatingObject::Orientation::UP);

    myField.setObjectAtPos(myCreatorObject, Field::Position(0, 0));
    myField.setObjectAtPos(myMovingObject, Field::Position(1, 1));
    myField.setObjectAtPos(myStaticObject, Field::Position(1, 2));
    myField.setObjectAtPos(myRotatingObject, Field::Position(2, 3));

    MyMovingObject* createdObject = dynamic_cast<MyMovingObject*>(myField.createObjectAtPos(myCreatorObject,
                                                                                            typeid(MyMovingObject),
                                                                                            Field::Position(1, 0)));
    if (createdObject == nullptr) {
        std::cout << "Can't create object.\n" << std::flush;
        return 0;
    }

    myField.moveObjectAtPos(myMovingObject, Field::Position(0, 1));

    interactDelete(myField, myMovingObject);
    myField.moveObjectAtPos(myMovingObject, Field::Position(0, 0));

    myField.moveObjectAtPos(createdObject, Field::Position(1, 1));

    // interacting with static object
    interactDelete(myField, createdObject);
    myField.moveObjectAtPos(createdObject, Field::Position(1, 2));

    interactDelete(myField, createdObject);
    myField.rotateObjectCounterclockwise(myRotatingObject, 1);
    interactDelete(myField, createdObject);

    std::cout << "Successful!\n";

    return 0;
}