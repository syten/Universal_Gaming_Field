#ifndef UNIVERSAL_GAMING_FIELD_USER_DEFINED_OBJECTS_H
#define UNIVERSAL_GAMING_FIELD_USER_DEFINED_OBJECTS_H

class MyStaticObject: public GameObject {
public:
    MyStaticObject() {
        health = 1;
        interactionsPossible = 0;
        currentForm = { std::make_pair(0, 0) };
    }
};

class MyMovingObject: public MovingObject {
public:
    MyMovingObject() {
        health = 1;
        movesPossible = 10;
        interactionsPossible = 1;
        cellsOfInteraction = { std::make_pair(-1, 0),
                               std::make_pair(1, 0),
                               std::make_pair(0, 1),
                               std::make_pair(0, -1)
        };
        cellToMove = { std::make_pair(-1, 0),
                       std::make_pair(1, 0),
                       std::make_pair(0, 1),
                       std::make_pair(0, -1),
                       std::make_pair(1, 1),
                       std::make_pair(1, -1),
                       std::make_pair(-1, -1),
                       std::make_pair(1, 1)
        };
        currentForm = { std::make_pair(0, 0) };
    }

    bool operator() (Mode mode, GameObject* gObj, bool needToInteract);
};

class MyRotatingObject: public RotatingObject {
public:
    MyRotatingObject() {
        orientationsAvailable[UP] = true;
        orientationsAvailable[LEFT] = true;
    }
    MyRotatingObject(const Orientation& _orientation): MyRotatingObject() {
        if (!orientationsAvailable[_orientation])
            throw wrong_orientation_exception();
        health = 1;
        interactionsPossible = 0;
        currentForm = {
                std::make_pair(0, 1),
                std::make_pair(1, 1),
                std::make_pair(1, 0)
        };
        pivot = std::make_pair(0, 0);
        orientation = _orientation;
    }
};

class MyCreatorObject: public CreatorObject {
public:
    MyCreatorObject() {
        health = 1;
        interactionsPossible = 0;
        currentForm = { std::make_pair(0, 0) };
    }

    GameObject* createObject(const std::type_info& info) override {
        if (info == typeid(MyMovingObject)) {
            auto objToPlace = new MyMovingObject;
            objToPlace->changeMovesPossible(5);
            return objToPlace;
        }
    }
};


inline bool MyMovingObject::operator()(Mode mode, GameObject* gObj, bool needToInteract) {
    if (!isFieldAttached() || gObj->getAttachedField() != getAttachedField())
        return false;
    if (!needToInteract)
        return true;

    if (mode == INTERACTION) {
        gObj->changeHealth(-1.);
        getAttachedField();
        interactionsPossible--;
        return true;
    }

    if (mode == INTERSECTION) {
        return true;
    }
}

#endif //UNIVERSAL_GAMING_FIELD_USER_DEFINED_OBJECTS_H
