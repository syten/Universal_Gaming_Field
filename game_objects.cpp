#include "game_objects.h"

// GameObject

GameObject::GameObject(Field& field, const Field::Position& gObjPos): GameObject() {
    attachedField = &field;
    attachToField(field, gObjPos);
}

void GameObject::attachToField(Field& field, const Field::Position& gObjPos) {
    if (isFieldAttached())
        attachedField->deleteObject(this);
    field.setObjectAtPos(this, gObjPos);
    attachedField = &field;
}

Field* GameObject::getAttachedField() {
    return attachedField;
}

bool GameObject::isFieldAttached() {
    return attachedField != nullptr;
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
    return tags.find(tag) != tags.end();
}

std::size_t GameObject::getSize() const {
    return currentForm.size();
}

_offsets_set GameObject::getPositionsOfInteraction() const {
    return cellsOfInteraction;
}

void GameObject::changeCellsOfInteraction(const _offsets_set& cells) {
    cellsOfInteraction = cells;
}

void GameObject::changeCellsOfInteraction(const std::vector<std::pair<long long int, long long int>> &cells) {
    _offsets_set _cells(cells.begin(), cells.end());
    changeCellsOfInteraction(_cells);
}

void GameObject::changeCellsOfinteraction(const std::set<std::pair<long long int, long long int>> &cells) {
    _offsets_set _cells(cells.begin(), cells.end());
    changeCellsOfInteraction(_cells);
}

bool GameObject::canInteractWith(const GameObject *gObj) const {
    return gObj != nullptr;
}

bool GameObject::canIntersectWith(const GameObject *gObj) const {
    return gObj != nullptr;
}

// MovingObject

MovingObject::MovingObject(Field &field, const Field::Position &gObjPos) : MovingObject() {
    attachToField(field, gObjPos);
}

_offsets_set MovingObject::getMovesFromCenter() const {
    return _offsets_set();
}

// Rotating Object

RotatingObject::RotatingObject(): GameObject() {
    orientationsAvailable = {
            UP,
            RIGHT,
            DOWN,
            LEFT,
    };
    pivot = { 0ll, 0ll };
}

RotatingObject::RotatingObject(const RotatingObject::Orientation& _orientation): RotatingObject() {
    if (orientationsAvailable.find(orientation) == orientationsAvailable.end())
        throw wrong_orientation_exception();
    orientation = _orientation;
}

RotatingObject::RotatingObject(Field &field, const Field::Position &gObjPos, const RotatingObject::Orientation& _orientation):
        RotatingObject(_orientation) {
    attachToField(field, gObjPos);
}

bool RotatingObject::rotateClockwiseOffField(int rotates) {
    if (isFieldAttached())
        return false;
    rotates %= 4;
    if (!rotates)
        return true;

    auto newOrientation = Orientation((orientation + rotates) % 4);
    _offsets_set newCellsFromCenter;
    switch(rotates) {
        case 1:
            changePivot(std::make_pair(pivot.second, -pivot.first));
            for (_offset offset: currentForm)
                newCellsFromCenter.insert(std::make_pair(offset.second, -offset.first));
            break;
        case 2:
            changePivot(std::make_pair(-pivot.first, -pivot.second));
            for (_offset offset: currentForm)
                newCellsFromCenter.insert(std::make_pair(-offset.first, -offset.second));
            break;
        case 3:
            changePivot(pivot = std::make_pair(-pivot.second, pivot.first));
            for (_offset offset: currentForm)
                newCellsFromCenter.insert(std::make_pair(-offset.second, -offset.first));
            break;
        default: break;
    }
    currentForm = newCellsFromCenter;
    orientation = Orientation(newOrientation);
    return true;
}

bool RotatingObject::rotateCounterclockwiseOffField(int rotates) {
    if (isFieldAttached())
        return attachedField->rotateObjectCounterclockwise(this, rotates);
    rotates %= 4;
    if (!rotates)
        return true;

    return rotateClockwiseOffField(4 - rotates);
}

void RotatingObject::changePivot(const _offset& newPivot) {
    pivot = newPivot;
}

// FormChangingObject

FormChangingObject::FormChangingObject(Field &field, const Field::Position &gObjPos): FormChangingObject() {
    attachToField(field, gObjPos);
}

bool FormChangingObject::changeFormOffField(const _offsets_set& newCellsFromCenter) {
    if (isFieldAttached())
        return false;
    currentForm = newCellsFromCenter;
    return true;
}

bool FormChangingObject::nextFormOffField() {
    if (isFieldAttached())
        return false;
    for (std::size_t i = 0; i < formPresets.size(); ++i)
        if (formPresets[i] == currentForm)
            currentForm = formPresets[(i + 1) % formPresets.size()];
    return true;
}

// CreatorObject

CreatorObject::CreatorObject(Field &field, const Field::Position &gObjPos): CreatorObject() {
    attachToField(field, gObjPos);
}