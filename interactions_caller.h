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

#include "game_objects.h"

class InteractionsCaller {
public:
    static bool getRightInteraction(GameObject* _interactor, GameObject* gObj, bool needToInteract = true) {
        // add your classes here...

        if ((*_interactor)(dynamic_cast<MovingObject*>(gObj), needToInteract))
            return true;
        if ((*_interactor)(dynamic_cast<RotatingObject*>(gObj), needToInteract))
            return true;
        if ((*_interactor)(dynamic_cast<FormChangingObject*>(gObj), needToInteract))
            return true;
        if ((*_interactor)(dynamic_cast<CreatorObject*>(gObj), needToInteract))
            return true;
        if ((*_interactor)(dynamic_cast<GameObject*>(gObj), needToInteract))
            return true;
    }
};


#endif //UNIVERSAL_GAMING_FIELD_INTERACTIONS_CALLER_H
