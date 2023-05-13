#include "field.h"

// Field::Cell

Field::Cell::Cell(const Field::Position& _position, const bool _isAvalForObj,
                  std::unordered_set<GameObject*> gObjs, std::unordered_set<std::string> _tags):
        position(_position), isAvalForObj(_isAvalForObj), objects(gObjs), tags(_tags) {};

Field::Position operator+ (const Field::Position& pos1, const Field::Position& pos2) {
    return Field::Position(pos1.vertical + pos2.vertical, pos2.horizontal + pos2.horizontal);
}

bool operator== (const Field::Position& pos1, const Field::Position& pos2) {
    return pos1.vertical == pos2.vertical && pos1.horizontal == pos2.horizontal;
}

Field::Position Field::Cell::getPosition() {
    return position;
}

std::unordered_set<std::string> Field::Cell::getTags() {
    return tags;
}

std::unordered_set<GameObject*> Field::Cell::getObjects() {
    return objects;
}

void Field::Cell::addObject(GameObject *gObj) {
    objects.insert(gObj);
}

void Field::Cell::deleteObject(GameObject* gObj) {
    objects.erase(objects.find(gObj));
}

void Field::Cell::addTag(std::string& tag) {
    tags.insert(tag);
}

void Field::Cell::setAvailable() {
    isAvalForObj = true;
}

void Field::Cell::setUnavailable() {
    isAvalForObj = false;
}

bool Field::Cell::isAvailable() {
    return isAvalForObj;
}

// Field

Field::Field(std::size_t width, std::size_t height) {
    cellsMatrix.resize(height);
    for (std::size_t i = 0; i < height; ++i) {
        cellsMatrix[i].resize(width);
        for (std::size_t j = 0; j < width; ++j)
            cellsMatrix[i][j] = Cell(Position(i, j), true, {}, {});
    }
}

Field::Field(const std::vector<std::vector<bool>>& matrix) {
    cellsMatrix.resize(matrix.size());
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        cellsMatrix.resize(matrix[i].size());
        for (std::size_t j = 0; j < matrix[i].size(); ++j)
            cellsMatrix[i][j] = Cell(Position(i, j), matrix[i][j], {}, {});
    }
}

bool Field::limitLeftBorder(std::size_t minusColumns) {
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i) {
        if (cellsMatrix[i].size() <= minusColumns)
            return false;
    }
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i) {
        for (std::size_t j = 0; j < minusColumns; ++j)
            for (auto gObj: cellsMatrix[i][j].objects)
                deleteObject(gObj);
        cellsMatrix[i].erase(cellsMatrix[i].begin(), cellsMatrix[i].begin() + minusColumns - 1);
    }
    return true;
}

bool Field::limitRightBorder(std::size_t minusColumns) {
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i) {
        if (cellsMatrix[i].size() <= minusColumns)
            return false;
    }
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i) {
        for (std::size_t j = cellsMatrix[i].size() - minusColumns; j < cellsMatrix[i].size(); ++j)
            for (auto gObj: cellsMatrix[i][j].objects)
                deleteObject(gObj);
        cellsMatrix[i].erase(cellsMatrix[i].begin() + (cellsMatrix[i].size() - minusColumns), (--cellsMatrix[i].end()));
    }
    return true;
}

bool Field::limitBelowBorder(std::size_t minusRows) {
    if (cellsMatrix.size() <= minusRows)
        return false;
    for (std::size_t i = cellsMatrix.size() - minusRows; i < cellsMatrix.size(); ++i) {
        for (std::size_t j = 0; j < cellsMatrix[i].size(); ++j)
            for (auto gObj: cellsMatrix[i][j].objects)
                deleteObject(gObj);
    }
    cellsMatrix.erase(cellsMatrix.begin() + (cellsMatrix.size() - minusRows), (--cellsMatrix.end()));
    return true;
}

bool Field::limitAboveBorder(std::size_t minusRows) {
    if (cellsMatrix.size() <= minusRows)
        return false;
    for (std::size_t i = 0; i < minusRows; ++i) {
        for (std::size_t j = 0; j < cellsMatrix[i].size(); ++j)
            for (auto gObj: cellsMatrix[i][j].objects)
                deleteObject(gObj);
    }
    cellsMatrix.erase(cellsMatrix.begin(), cellsMatrix.begin() + (minusRows - 1));
    return true;
}

bool Field::expandLeftBorder(std::size_t plusColumns) {
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i)
        if (cellsMatrix[i].size() + plusColumns <= cellsMatrix[i].size())
            return false;
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i)
        cellsMatrix[i].insert(cellsMatrix[i].begin(), plusColumns, Cell());
    return true;
}

bool Field::expandRightBorder(std::size_t plusColumns) {
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i)
        if (cellsMatrix[i].size() + plusColumns <= cellsMatrix[i].size())
            return false;
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i)
        cellsMatrix[i].insert(cellsMatrix[i].end(), plusColumns, Cell());
    return true;
}

bool Field::expandBelowBorder(std::size_t plusRows) {
    if (cellsMatrix.size() + plusRows <= cellsMatrix.size())
        return false;
    std::size_t lastRowSize = cellsMatrix[cellsMatrix.size() - 1].size();
    cellsMatrix.insert(cellsMatrix.end(), plusRows, std::vector<Cell>(lastRowSize));
    return true;
}

bool Field::expandAboveBorder(std::size_t plusRows) {
    if (cellsMatrix.size() + plusRows <= cellsMatrix.size())
        return false;
    std::size_t firstRowSize = cellsMatrix[0].size();
    cellsMatrix.insert(cellsMatrix.begin(), plusRows, std::vector<Cell>(firstRowSize));
    return true;
}

bool Field::isPosAvalForObj(GameObject* gObj, const Position& pos) {
    if (pos.vertical > cellsMatrix.size())
    for (auto it = gObj->getCurrentForm().begin(); it != gObj->getCurrentForm().end(); ++it) {
        if (pos.vertical + it->first > cellsMatrix.size() - 1 ||
            pos.horizontal + it->second > cellsMatrix[pos.vertical + it->first].size() ||
            !cellsMatrix[pos.vertical + it->first][pos.horizontal + it->second].isAvailable())
            return false;
        for (auto objOnCell: cellsMatrix[pos.vertical + it->first][pos.horizontal + it->second].getObjects()) {
            if (!gObj->canIntersectWith(objOnCell))
                return false;
        }
    }
    for (auto it = gObj->getCurrentForm().begin(); it != gObj->getCurrentForm().end(); ++it)
        cellsMatrix[pos.horizontal + it->first][pos.vertical + it->second].addObject(gObj);
    objectsOnField.insert(gObj);
    return true;
}

bool Field::setObjectAtPos(GameObject* gObj, Field::Position pos) {
    if (!isPosAvalForObj(gObj, pos) || gObj->getAttachedField() != nullptr)
        return false;
    for (auto it = gObj->getCurrentForm().begin(); it != gObj->getCurrentForm().end(); ++it)
        cellsMatrix[pos.horizontal + it->first][pos.vertical + it->second].addObject(gObj);
    gObj->attachToField(this);
    objectsOnField.insert(gObj);
    objectPosition[gObj] = pos;
    positionObject[pos].insert(gObj);
    return true;
}

bool Field::setObjectRandPos(GameObject *gObj) {
    if (gObj->getAttachedField() != nullptr)
        return false;
    std::unordered_map<std::size_t, Cell*> numberActiveCell;
    std::size_t cur_ind = 0;
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i) {
        for (std::size_t j = 0; j < cellsMatrix[i].size(); ++j) {
            if (cellsMatrix[i][j].isAvailable()) {
                numberActiveCell[cur_ind] = &cellsMatrix[i][j];
                cur_ind++;
            }
        }
    }
    cur_ind--;
    if (numberActiveCell.empty())
        return false;
    unsigned long long number = (((unsigned long long)(unsigned int)rand() << 32) +
            (unsigned long long)(unsigned int)rand()) % (cur_ind);
    return setObjectAtPos(gObj, numberActiveCell[number]->getPosition());
}

Field::Position Field::getPosOfObject(GameObject* gObj) {
    return objectPosition[gObj];
}

bool Field::moveObjectAtPos(MovingObject *gObj, Field::Position pos) {
    if (!isPosAvalForObj(gObj, pos) || gObj->getAttachedField() != this)
        return false;
    Position curPos = getPosOfObject(gObj);
    for (auto moveFromCenter: gObj->getMovesFromCenter()) {
        Position newPos(curPos.vertical + moveFromCenter.first, curPos.horizontal + moveFromCenter.second);
        if (newPos == pos) {
            deleteObject(gObj);
            if (setObjectAtPos(gObj, pos))
                return true;
            setObjectAtPos(gObj, curPos);
            return false;
        }
    }
    return false;
}

bool Field::deleteObject(GameObject *gObj) {
    if (objectsOnField.find(gObj) == objectsOnField.end())
        return false;
    Position currentPos = getPosOfObject(gObj);
    for (auto offsetFromCenter: gObj->getCurrentForm()) {
        Position objectFormPosition(currentPos.vertical + offsetFromCenter.first, currentPos.horizontal + offsetFromCenter.second);
        cellsMatrix[objectFormPosition.vertical][objectFormPosition.horizontal].deleteObject(gObj);
    }
    gObj->detachFromField();
    objectPosition.erase(gObj);
    positionObject.erase(getPosOfObject(gObj));
    objectsOnField.erase(gObj);
    return true;
}

std::unordered_set<GameObject*> Field::getObjectAtPos(const Field::Position& pos) {
    return positionObject[pos];
}

void Field::deleteObjectsAtPos(Field::Position pos) {
    for (auto object: getObjectAtPos(pos))
        deleteObject(object);
}

std::unordered_set<Field::Position, Field::Position::pos_hash> Field::getPositionsToMove(MovingObject* gObj) {
    std::unordered_set<Field::Position, Field::Position::pos_hash> result;
    for (auto moveFromCenter: gObj->getMovesFromCenter()) {
        Position newPos = Position(getPosOfObject(gObj).vertical + moveFromCenter.first,
                                   getPosOfObject(gObj).horizontal + moveFromCenter.second);
        if (isPosAvalForObj(gObj, newPos))
            result.insert(newPos);
    }
    return result;
}

bool Field::rotateObjectClockwise(RotatingObject* gObj, int rotates) {
    if (objectsOnField.find(gObj) == objectsOnField.end())
        return false;
    auto rotatedObj = new RotatingObject(*gObj);
    rotatedObj->detachFromField();
    if (!rotatedObj->rotateClockwiseOffField(rotates))
        return false;
    Position prevObjPos = getPosOfObject(gObj);
    Position newObjPos(gObj->getPivot().first - rotatedObj->getPivot().first,
                               gObj->getPivot().second - rotatedObj->getPivot().second);
    deleteObject(gObj);
    if (!setObjectAtPos(rotatedObj, newObjPos)) {
        setObjectAtPos(gObj, prevObjPos);
        delete rotatedObj;
        return false;
    }
    delete gObj;
    return true;
}

bool Field::rotateObjectCounterclockwise(RotatingObject *gObj, int rotates) {
    rotates %= 4;
    return rotateObjectClockwise(gObj, 4 - rotates);
}

bool Field::changeObjectForm(FormChangingObject* gObj, const _offsets_set& newCellsFromCenter) {
    if (objectsOnField.find(gObj) == objectsOnField.end())
        return false;
    auto newFormObject = new FormChangingObject(*gObj);
    newFormObject->detachFromField();
    if (!newFormObject->changeFormOffField(newCellsFromCenter))
        return false;
    Position objPos = getPosOfObject(gObj);
    deleteObject(gObj);
    if (!setObjectAtPos(newFormObject, objPos)) {
        setObjectAtPos(gObj, objPos);
        delete newFormObject;
        return false;
    }
    delete gObj;
    return true;
}

bool Field::nextObjectForm(FormChangingObject *gObj) {
    if (objectsOnField.find(gObj) == objectsOnField.end())
        return false;
    auto newFormObject = new FormChangingObject(*gObj);
    newFormObject->detachFromField();
    if (!newFormObject->nextFormOffField()) {
        delete newFormObject;
        return false;
    }
    _offsets_set newForm = newFormObject->getCurrentForm();
    delete newFormObject;
    return changeObjectForm(gObj, newForm);
}

std::unordered_set<GameObject*> Field::getObjectsByTag(const std::string &tag) const {
    std::unordered_set<GameObject*> res;
    for (std::size_t i = 0; i < cellsMatrix.size(); ++i) {
        for (std::size_t j = 0; j < cellsMatrix[i].size(); ++j) {
            for (auto gObj: cellsMatrix[i][j].objects) {
                if (gObj->hasTag(tag))
                    res.insert(gObj);
            }
        }
    }
    return res;
}

std::unordered_set<GameObject*> Field::getObjectToInteract(GameObject* gObj) {
    if (objectsOnField.find(gObj) == objectsOnField.end())
        return {};
    std::unordered_set<GameObject*> res;
    Position objPos = getPosOfObject(gObj);
    for (auto offsetOfInteraction: gObj->getPositionsOfInteraction()) {
        Position interactionPos(objPos.vertical + offsetOfInteraction.first, objPos.horizontal + offsetOfInteraction.second);
        if (interactionPos.vertical >= cellsMatrix.size() || interactionPos.horizontal >= cellsMatrix[interactionPos.vertical].size())
            continue;
        for (auto objToInteract: cellsMatrix[interactionPos.vertical][interactionPos.horizontal].objects) {
            if (gObj->canInteractWith(objToInteract)) {
                unsigned long long ignorableCounter = 0;
                for (auto objToIgnore: cellsMatrix[interactionPos.vertical][interactionPos.horizontal].objects) {
                    if (objToInteract != objToIgnore) {
                        if (gObj->canIntersectWith(objToIgnore))
                            ignorableCounter++;
                    }
                }
                if (ignorableCounter == cellsMatrix[interactionPos.vertical][interactionPos.horizontal].objects.size() - 1)
                    res.insert(objToInteract);
            }
        }
    }
    return res;
}