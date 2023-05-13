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
        struct pos_hash {
            std::size_t operator() (const Position& pos) const {
                return pos.vertical * 31 + pos.horizontal;
            }
        };

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

    bool limitLeftBorder(std::size_t minusColumns);
    bool limitRightBorder(std::size_t minusColumns);
    bool limitBelowBorder(std::size_t minusRows);
    bool limitAboveBorder(std::size_t minusRows);
    bool expandLeftBorder(std::size_t plusColumns);
    bool expandRightBorder(std::size_t plusColumns);
    bool expandBelowBorder(std::size_t plusRows);
    bool expandAboveBorder(std::size_t plusRows);

    bool isPosAvalForObj(GameObject* gObj, const Position& pos);
    bool setObjectAtPos(GameObject* gObj, Position pos);
    bool setObjectRandPos(GameObject* gObj);
    bool deleteObject(GameObject* gObj);
    std::unordered_set<GameObject*> getObjectAtPos(const Position& pos);
    void deleteObjectsAtPos(Position pos);
    std::unordered_set<Position, Position::pos_hash> getPositionsToMove(MovingObject* gObj);
    bool moveObjectAtPos(MovingObject* gObj, Position pos);
    bool rotateObjectClockwise(RotatingObject* gObj, int rotates);
    bool rotateObjectCounterclockwise(RotatingObject* gObj, int rotates);
    bool changeObjectForm(FormChangingObject* gObj, const _offsets_set& newCellsFromCenter);
    bool nextObjectForm(FormChangingObject* gObj);

    Position getPosOfObject(GameObject* gObj);

    std::unordered_set<GameObject*> getObjectsByTag(const std::string& tag) const;

    std::unordered_set<GameObject*> getObjectToInteract(GameObject* gObj);

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
    std::unordered_map<GameObject*, Position> objectPosition;
    std::unordered_map<Position, std::unordered_set<GameObject*>, Position::pos_hash> positionObject;
};

#endif //UNIVERSAL_GAMING_FIELD_FIELD_H
