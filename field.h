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
#include <functional>

#include <game_objects.h>
class GameObject;
class MovingObject;
class RotatingObject;
class CreatorObject;


class Field {
public:

    struct Position {
        std::size_t x;
        std::size_t y;
    };

    struct Cell {
    public:
        static std::unordered_set<std::string> possibleTags;

        const std::unordered_set<std::string> getTags();
        void addTag(std::string);

    private:
        Cell(const Position& _position, const bool _isAvalForObj, const std::unordered_set<std::string>& _tags);

        Position position;
        bool isAvalForObj;
        std::unordered_set<GameObject*> objects;

        std::unordered_set<std::string> tags;
    };

    Field() = delete;
    Field(std::size_t width, std::size_t height);
    Field(std::vector<std::vector<bool>> matrix);

    int changeSize(long long left, long long right, long long below, long long above);
    void rotate(int clockwiseRotatesNubmer);

    bool setObjectAtPos(GameObject* gObj, Position pos);
    bool setObjectAtPos(GameObject* gObj, std::size_t, std::size_t);
    bool setObjectRandPos(GameObject gObj);
    bool deleteObject(GameObject* gObj);
    bool deteleObjectAtPos(Position pos);
    bool changeObjectOrientation(GameObject* gObj, const RotatingObject::Orientation& newOrientation);
    bool rotateObjectClockwise(GameObject* gObj, int rotates);
    bool rotateObjectCounterclockwise(GameObject* gObj, int rotates);
    bool changeObjectForm(GameObject* gObj, const _offsets_set& newCellsFromCenter);
    bool nextObjectForm(GameObject* gObj);

    const std::unordered_set<const Cell&> getAvailableCells() const;

    std::unordered_set<const GameObject*> getObjectsAtPos(std::size_t x, std::size_t y) const;
    Position getPosOfObject(const GameObject& gObj) const;

    std::unordered_set<const GameObject*> getObjectsByTag(std::string tag) const;

    const std::unordered_set<const GameObject*> getObjectToInteract(const GameObject& gObj) const;
    const std::unordered_set<const Cell&> getCellActions(std::size_t x, std::size_t y) const;

private:
    std::vector<std::vector<Cell>> cells_matrix;
    std::unordered_set<GameObject*> activeObjects;
    std::unordered_set<GameObject*> inactiveObjects;
};


//struct GamePreset {
//public:
//    Field field;
//    std::string presetName;
//};

#endif //UNIVERSAL_GAMING_FIELD_FIELD_H
