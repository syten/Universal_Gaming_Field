//
// Created by dronl on 18.04.2023.
//

#include "game_objects.h"

GameObject::GameObject() {
    size = 1;
    cellsFromCenter = { { 0ll, 0ll } };
    movesPossible = 0;
    tags = {};
    interactableObjects = {};
    ignorableObjects = {};
    attachedField = nullptr;
}

GameObject::GameObject(Field& field, const Field::Position& gObjPos): GameObject() {
    attachedField = &field;
    attachToField(field, gObjPos);
}

void GameObject::attachToField(Field& field, const Field::Position& gObjPos) {
    if (attachedField != nullptr)
        attachedField->deleteObject(this);
    field.setObject(this, gObjPos);
    attachedField = &field;
}

const std::unordered_set<std::string>& GameObject::getTags() const {
    return tags;
}

void GameObject::addTag(std::string tag) {
    tags.insert(tag);
}

void GameObject::deleteTag(std::string tag) {
    tags.erase(tag);
}

bool GameObject::hasTag(std::string tag) const {
    return false;
}

_offsets_set GameObject::getCellsOfInteraction() {
    return _offsets_set();
}