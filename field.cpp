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

//bool Field::changeSize(long long int left, long long int right, long long int below, long long int above) {
//    // Check Possibility to change size
//    std::size_t cur_height = cellsMatrix.size();
//    if (above < 0 && -(long long)cur_height > above
//        || below < 0 && -(long long)cur_height > below
//        || below < 0 && above < 0 && (-(long long)cur_height > below + above || below + above >= 0)) {
//        return false;
//    }
//    std::size_t max_width = 0;
//    for (int i = 0; i < cellsMatrix.size(); ++i)
//        max_width = std::max(max_width, cellsMatrix[i].size());
//    if (right < 0 && -(long long)max_width > right
//        || left < 0 && -(long long)max_width > left
//        || left < 0 && right < 0 && (-(long long)max_width > left + right || left + right >= 0)) {
//        return false;
//    }
//    // Algorithm
//    if (left < 0) {
//
//    }
//    else
//    if ()
//}

bool Field::canSetObjectAtPos(GameObject* gObj, const Position pos) {
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
    if (!canSetObjectAtPos(gObj, pos))
        return false;
    for (auto it = gObj->getCurrentForm().begin(); it != gObj->getCurrentForm().end(); ++it)
        cellsMatrix[pos.horizontal + it->first][pos.vertical + it->second].addObject(gObj);
    objectsOnField.insert(gObj);
    objectsPosition[gObj] = pos;
    return true;
}

bool Field::setObjectRandPos(GameObject *gObj) {
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
    return objectsPosition[gObj];
}


bool Field::moveObjectAtPos(MovingObject *gObj, Field::Position pos) {
    if (!canSetObjectAtPos(gObj, pos))
        return false;
    Position curPos = getPosOfObject(gObj);
    for (auto moveFromCenter: gObj->getMovesFromCenter()) {
        Position newPos(curPos.vertical + moveFromCenter.first, curPos.horizontal + moveFromCenter.second);
        if (newPos == pos)
            return setObjectAtPos(gObj, pos);
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
    return true;
}

