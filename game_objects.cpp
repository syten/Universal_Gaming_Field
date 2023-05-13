#include "game_objects.h"

// GameObject

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

const _offsets_set& GameObject::getCurrentForm() {
    return currentForm;
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

void GameObject::changeCellsOfInteraction(const std::set<std::pair<long long int, long long int>> &cells) {
    _offsets_set _cells(cells.begin(), cells.end());
    changeCellsOfInteraction(_cells);
}

bool GameObject::operator() (Mode mode, GameObject* gObj, bool needToInteract) {
    return false;
}

bool GameObject::operator() (Mode mode, MovingObject* gObj, bool needToInteract) {
    return false;
}

bool GameObject::operator() (Mode mode, RotatingObject* gObj, bool needToInteract) {
    return false;
}

bool GameObject::operator() (Mode mode, FormChangingObject* gObj, bool needToInteract) {
    return false;
}

bool GameObject::operator() (Mode mode, CreatorObject* gObj, bool needToInteract) {
    return false;
}

double GameObject::getHealth() const {
    return health;
}

void GameObject::changeHealth(double shift) {
    health = health + shift < 0.0000001 ? 0. : health + shift;
}

void GameObject::attachToField(Field *field) {
    attachedField = field;
}

void GameObject::detachFromField() {
    attachedField = nullptr;
}

// MovingObject

_offsets_set MovingObject::getMovesFromCenter() const {
    return cellToMove;
}

int MovingObject::getMovesPossible() const {
    return movesPossible;
}

void MovingObject::changeMovesPossible(int shift) {
    movesPossible = movesPossible + shift >= 0 ? movesPossible + shift : 0;
}

// Rotating Object

RotatingObject::RotatingObject(): GameObject() {
    orientationsAvailable[UP] = true;
    orientationsAvailable[RIGHT] = true;
    orientationsAvailable[DOWN] = true;
    orientationsAvailable[LEFT] = true;
    pivot = { 0ll, 0ll };
}

RotatingObject::RotatingObject(const RotatingObject::Orientation& _orientation): RotatingObject() {
    if (orientationsAvailable.find(orientation) == orientationsAvailable.end())
        throw wrong_orientation_exception();
    orientation = _orientation;
}

bool RotatingObject::rotateClockwiseOffField(unsigned int rotates) {
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

bool RotatingObject::rotateCounterclockwiseOffField(unsigned int rotates) {
    rotates %= 4;
    return rotateClockwiseOffField(4 - rotates);
}

_offset RotatingObject::getPivot() {
    return pivot;
}

void RotatingObject::changePivot(const _offset& newPivot) {
    pivot = newPivot;
}

RotatingObject::Orientation RotatingObject::getOrientation() const {
    return orientation;
}

// FormChangingObject

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

template<class ObjectT, typename... Args>
ObjectT* CreatorObject::createObject(Args... args) {
    return new ObjectT(args...);
}