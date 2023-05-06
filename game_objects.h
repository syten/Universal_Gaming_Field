//
// Created by dronl on 18.04.2023.
//

#ifndef UNIVERSAL_GAMING_FIELD_GAME_OBJECTS_H
#define UNIVERSAL_GAMING_FIELD_GAME_OBJECTS_H

#include <unordered_set>
#include <string>
#include <set>

#include "field.h"
class Field;


struct pair_hash {
    inline std::size_t operator() (const std::pair<long long, long long>& v) const {
        return v.first * 31 + v.second;
    }
};

typedef std::pair<long long, long long> _offset;
typedef std::unordered_set<_offset, pair_hash> _offsets_set;

class GameObject {
    friend class MovingObject;
    friend class RotatableObject;
    friend class ResizingObject;
    friend class CreatorObject;

public:
    GameObject();
    GameObject(Field& field, const Field::Position& gObjPos);
    void attachToField(Field& field, const Field::Position& gObjPos);

    Field& getAttachedField();
    std::unordered_set<Field::Position> getPositions() const;

    const std::unordered_set<std::string>& getTags() const;
    void addTag(std::string tag);
    void deleteTag(std::string tag);
    bool hasTag(std::string tag) const;

    //Interaction
    virtual _offsets_set getCellsOfInteraction();
    struct Interactions; // declare functions of interations between Objects in this structure

private:
    Field* attachedField;

    double health;
    static int size;
    std::set<std::pair<long long, long long>> cellsFromCenter;
    int movesPossible;

    std::unordered_set<std::string> tags;
    static std::unordered_set<GameObject*> interactableObjects;
    static std::unordered_set<GameObject*> ignorableObjects; // objects with which current object can be in the same cell
};


class MovingObject: virtual public GameObject {
public:
    bool checkMovePossibility(const Field::Cell& cell) const;
    std::set<std::pair<Field::Cell, Field::Cell>> getPossibleMoves() const;

protected:
    const static _offsets_set movesFromCenter;
};


class RotatableObject: virtual public GameObject {
public:
    void rotateObject(int rotatesClockwise);

protected:
    std::pair<long long, long long> pivot;
};


class ResizingObject: virtual public GameObject {
public:
    void changeSize(const std::set<std::pair<long long, long long>>& newCellFromCenter);
    void changeCenter(const std::pair<long long, long long>& newCenter);
};


class CreatorObject: virtual public GameObject {
public:
    template<class ObjectT, typename... Args>
    ObjectT* createObject(Args... args) {
        return new ObjectT(args...);
    }
};

class SomeObject2: public MovingObject {
public:
    int x = 3;
    SomeObject2(int a, int b, int c){};
    SomeObject2& operator=(const SomeObject2&) = default;
};

class SomeObject: public CreatorObject {
public:
    template<class ObjectT>
    const ObjectT* createObject() {
        SomeObject2* obj = CreatorObject::createObject<ObjectT, int, int, int>(1, 2, 3);
        obj->addTag("cringe");
        obj->attachToField(getAttachedField(), this->getCenterPosition());
        return obj;
    }
};

#endif //UNIVERSAL_GAMING_FIELD_GAME_OBJECTS_H
