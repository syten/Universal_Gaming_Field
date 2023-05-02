//
// Created by syten on 07.04.23.
//

#ifndef UNIVERSAL_GAMING_FIELD_FIELD_H
#define UNIVERSAL_GAMING_FIELD_FIELD_H

#include <game_object.h>
#include <array>
#include <vector>
#include <set>
#include <string>
#include <unordered_set>
#include <functional>

//struct GamePreset {
//public:
//    Field field;
//    std::string presetName;
//};

class Field {
public:
    enum RotationDirection {
        CLOCKWISE,
        COUNTERCLOCKWISE
    };

private:
    class Cell {
    public:
        static std::unordered_set<std::string> possibleTags;

    private:
        bool is_aval_for_obj;
    };

public:
    Field() = delete;
    Field(std::size_t width, std::size_t height);
    Field(std::vector<std::vector<bool>> matrix);

    int changeSize(int left, int right, int below, int above);
    void rotate(RotationDirection rdir, int number);

    int addObject(std::size_t x, std::size_t y, GameObject gObj);
    int addObjectRandPos(GameObject gObj);
    void deteleObject(std::size_t x, std::size_t y);

    const std::vector<Cell> getAvailableCells() const;

    std::vector<const GameObject*> getObjectsAtPos(std::size_t x, std::size_t y) const;
    std::pair<size_t, size_t> getPosOfObject(const GameObject& gObj) const;

    std::vector<const GameObject*> getObjectsByTag(std::string tag) const;

    const std::unordered_set<const std::function<void(const GameObject&)>&> getObjectActions(const GameObject& gObj) const;
    const std::vector<Action> getCellActions(std::size_t x, std::size_t y) const; // закончили здесь.

private:
    std::vector<std::vector<Cell>> cells_matrix;
    std::vector<GameObject> objects;
    std::vector<GameObject> activeObjects;
    std::vector<GameObject> inactiveObjects;
};

#endif //UNIVERSAL_GAMING_FIELD_FIELD_H
