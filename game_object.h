//
// Created by dronl on 18.04.2023.
//

#ifndef UNIVERSAL_GAMING_FIELD_GAME_OBJECT_H
#define UNIVERSAL_GAMING_FIELD_GAME_OBJECT_H

#include <unordered_set>
#include <string>
#include <set>
#include <field.h>

class GameObject {
public:
    GameObject() {};
    GameObject(const Field& field): attachedField(&field) {};
    void attachToField(const Field& field);

    const std::unordered_set<std::string>& getTags() const;
    const void addTag(std::string tag);
    const bool hasTag(std::string tag) const;

    //Interaction
    virtual std::unordered_set<std::pair<long long, long long>> getCellsOfInteraction() = 0;
    struct Interactions; // declare functions of interations between Objects in this class

protected:
    static GameObject* objectType;

    const Field* attachedField;

    int health;
    int size;
    int moves;

    std::unordered_set<std::string> tags;
    static std::unordered_set<std::string> possibleTags;
    static std::unordered_set<GameObject*> interactableObjects;
    static std::unordered_set<GameObject*> ignorableObjects; // objects with which current object can be in the same cell
};

#endif //UNIVERSAL_GAMING_FIELD_GAME_OBJECT_H
