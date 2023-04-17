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

class Field {
public:
    Field() = delete;
    Field(std::size_t width, std::size_t height);
    Field(std::vector<std::vector<bool>> matrix);

    int changeSize(int left, int right, int below, int avard);

    int addObject(std::size_t x, std::size_t y, GameObject gObj);
    int addObjectRandPos(GameObject gObj);
    void deteleObject(std::size_t x, std::size_t y);
    std::vector<GameObject::Action> getPossibleActions(GameObject gObj);

    const GameObject* getObjectAtPos(std::size_t x, std::size_t y);
    const GameObject* getObjectByTag(std::string tag);

    class Cell {
    public:
        static std::unordered_set<std::string> possible_tags;

        bool isAvalForObj();

    private:
        std::unordered_set<std::string> tags;
        bool is_aval_for_obj;
    };

private:
    std::vector<std::vector<Cell>> cells_matrix;
    std::vector<GameObject> objects;
    std::vector<GameObject> activeObjects;
    std::vector<GameObject> inactiveObjects;
};

#endif //UNIVERSAL_GAMING_FIELD_FIELD_H
