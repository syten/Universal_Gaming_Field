//
// Created by dronl on 13.05.2023.
//

#ifndef UNIVERSAL_GAMING_FIELD_USER_DEFINED_OBJECTS_H
#define UNIVERSAL_GAMING_FIELD_USER_DEFINED_OBJECTS_H

#

class MyObject2: public GameObject {
public:
    MyObject2() {
        health = 1;
        interactionsPossible = 0;
        currentForm = { std::make_pair(0, 0) };
    }
};

class MyObject: public MovingObject {
public:
    MyObject() {
        health = 1;
        movesPossible = 1;
        interactionsPossible = 1;
        cellsOfInteraction = { std::make_pair(-1, 0) };
        cellToMove = { std::make_pair(-1, 0) };
        currentForm = { std::make_pair(0, 0) };
    }

    bool operator() (Mode mode, MyObject2* gObj, bool needToInteract) {
        if (!isFieldAttached() || gObj->getAttachedField() != getAttachedField())
            return false;
        if (!needToInteract)
            return true;
        if (mode == INTERACTION) {
            gObj->changeHealth(-1);
            interactionsPossible--;
            return true;
        }
        if (mode == INTERSECTION) {
            return true;
        }
    }
};


#endif //UNIVERSAL_GAMING_FIELD_USER_DEFINED_OBJECTS_H
