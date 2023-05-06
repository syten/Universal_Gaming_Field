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
class RotatableObject;
class CreatorObject;


//struct GamePreset {
//public:
//    Field field;
//    std::string presetName;
//};

class Field {
public:

    static struct Position {
        std::size_t x;
        std::size_t y;
    };

    class Cell {
    public:
        static std::unordered_set<std::string> possibleTags;

        const std::unordered_set<std::string> getTags();
        void addTag(std::string);

    protected:
        Cell() = default;
        ~Cell() = default;

        std::size_t x, y;
        bool is_aval_for_obj;
        std::unordered_set<GameObject*> objects;

        std::unordered_set<std::string> tags;
    };

    Field() = delete;
    Field(std::size_t width, std::size_t height);
    Field(std::vector<std::vector<bool>> matrix);

    int changeSize(long long left, long long right, long long below, long long above);
    void rotate(int clockwiseRotatesNubmer);

    int setObject(const GameObject* gObj, Position pos);
    int setObjectRandPos(GameObject gObj);
    void deleteObject(const GameObject* gObj);
    void deteleObjectAtPos(Position pos);

    const std::unordered_set<const Cell&> getAvailableCells() const;

    std::unordered_set<const GameObject*> getObjectsAtPos(std::size_t x, std::size_t y) const;
    Position getPosOfObject(const GameObject& gObj) const;

    std::unordered_set<const GameObject*> getObjectsByTag(std::string tag) const;

    const std::unordered_set<const GameObject*> getObjectToInteract(const GameObject& gObj) const;
    const std::unordered_set<const Cell&> getCellActions(std::size_t x, std::size_t y) const; // закончили здесь.

private:
    std::vector<std::vector<Cell>> cells_matrix;
    std::unordered_set<GameObject*> activeObjects;
    std::unordered_set<GameObject*> inactiveObjects;
};

#endif //UNIVERSAL_GAMING_FIELD_FIELD_H
