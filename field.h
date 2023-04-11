//
// Created by syten on 07.04.23.
//

#ifndef UNIVERSAL_GAMING_FIELD_FIELD_H
#define UNIVERSAL_GAMING_FIELD_FIELD_H

#include <vector>
#include <set>
#include <string>

class Field {
public:
    Field() = delete;
    Field(std::size_t width, std::size_t height);
    Field(std::vector<std::vector<bool>> matrix);

    int changeSize(int left, int right, int below, int avard);

    int addObject(std::size_t x, std::size_t y, GameObject obj);
    void deteleObject(std::size_t x, std::size_t y);



    class Cell {
    public:
        bool isAvalForObj();

    private:
        bool is_aval_for_obj;
        static std::set<std::string> possible_tags;
    };

private:


};

#endif //UNIVERSAL_GAMING_FIELD_FIELD_H
