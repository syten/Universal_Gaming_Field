//
// Created by syten on 07.04.23.
//

#ifndef UNIVERSAL_GAMING_FIELD_FIELD_H
#define UNIVERSAL_GAMING_FIELD_FIELD_H

#include <array>
#include <vector>
#include <set>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "game_objects.h"
#include "interactions_caller.h"

class Field {

public:
    struct Position {
        Position() = default;
        ~Position() = default;
        friend Position operator+ (const Position& pos1, const Position& pos2);
        friend bool operator== (const Position& pos1, const Position& pos2);
        Position(std::size_t _vertical, std::size_t _horizontal): horizontal(_horizontal), vertical(_vertical) {};
        std::size_t horizontal;
        std::size_t vertical;
    };

    Field() = delete;
    Field(std::size_t width, std::size_t height);
    explicit Field(const std::vector<std::vector<bool>>& matrix);

    bool changeSize(long long left, long long right, long long below, long long above);

    bool canSetObjectAtPos(GameObject* gObj, const Position pos);
    bool setObjectAtPos(GameObject* gObj, Position pos);
    bool setObjectRandPos(GameObject* gObj);
    bool deleteObject(GameObject* gObj);
    bool deleteObjectsAtPos(Position pos);
    _offsets_set getPossibleMoves(const MovingObject* gObj);
    bool moveObjectAtPos(MovingObject* gObj, Position pos);
    bool rotateObjectClockwise(RotatingObject* gObj, int rotates);
    bool rotateObjectCounterclockwise(RotatingObject* gObj, int rotates);
    bool changeObjectForm(FormChangingObject* gObj, const _offsets_set& newCellsFromCenter);
    bool nextObjectForm(FormChangingObject* gObj);

    //const std::unordered_set<Cell&> getAvailableCells() const;

    std::unordered_set<const GameObject*> getObjectsAtPos(std::size_t x, std::size_t y) const;
    Position getPosOfObject(GameObject* gObj);

    std::unordered_set<GameObject*> getObjectsByTag(const std::string& tag) const;

    std::unordered_set<const GameObject*> getObjectToInteract(const GameObject* gObj) const;
    //std::unordered_set<Cell> getCellsToMove(const MovingObject& gObj) const;

private:
    struct Cell {
        friend Field;
    public:
        Cell() = default;
        Cell(const Position& _position, bool _isAvalForObj,
             std::unordered_set<GameObject*> gObjs, std::unordered_set<std::string> _tags);
        ~Cell() = default;

        Position getPosition();
        std::unordered_set<std::string> getTags();
        std::unordered_set<GameObject*> getObjects();
        bool canSetObject(GameObject* gObj) const;
        void addObject(GameObject* gObj);
        void deleteObject(GameObject* gObj);
        void addTag(std::string& tag);
        void setAvailable();
        void setUnavailable();
        bool isAvailable();

    private:
        Position position{};
        bool isAvalForObj{};
        std::unordered_set<GameObject*> objects;
        std::unordered_set<std::string> tags;
    };

    std::vector<std::vector<Cell>> cellsMatrix;
    std::unordered_set<GameObject*> objectsOnField;
    std::unordered_map<GameObject*, Position> objectsPosition;
};

#endif //UNIVERSAL_GAMING_FIELD_FIELD_H
