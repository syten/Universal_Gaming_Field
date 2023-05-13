//
// Created by dronl on 09.05.2023.
//

#ifndef UNIVERSAL_GAMING_FIELD_INTERACTIONS_CALLER_H
#define UNIVERSAL_GAMING_FIELD_INTERACTIONS_CALLER_H

class MovingObject;
class RotatingObject;
class FormChangingObject;
class CreatorObject;
class GameObject;
class MyObject;
class MyObject2;

#include "game_objects.h"
#include "user_defined_objects.h"

class InteractionsCaller {
public:
    static bool getRightInteraction(Mode mode, GameObject* _interactor, GameObject* gObj, bool needToInteract = true) {
        // add your classes here...

        auto myObj = dynamic_cast<MyObject*>(_interactor);
        if (myObj != nullptr) {
            if ((*myObj)(mode, dynamic_cast<MyObject2*>(gObj), needToInteract))
                return true;
            return false;
        }
    }
};


#endif //UNIVERSAL_GAMING_FIELD_INTERACTIONS_CALLER_H
