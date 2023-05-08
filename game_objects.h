//
// Created by dronl on 18.04.2023.
//

#ifndef UNIVERSAL_GAMING_FIELD_GAME_OBJECTS_H
#define UNIVERSAL_GAMING_FIELD_GAME_OBJECTS_H

#include <unordered_set>
#include <string>
#include <set>
#include <exception>

#include "field.h"
class Field;


class wrong_orientation_exception: public std::exception {
    const char* what() const throw() {
        return "Wrong object orientation exception";
    }
};

struct pair_hash {
    inline std::size_t operator() (const std::pair<long long, long long>& v) const {
        return v.first * 31 + v.second;
    }
};

typedef std::pair<long long, long long> _offset;
typedef std::unordered_set<_offset, pair_hash> _offsets_set;


class GameObject {
    friend class FormChangingObject;
    friend class RotatingObject;

public:
    GameObject() = default;
    GameObject(const GameObject& otherGObj) = default;
    GameObject(GameObject&& otherGObj) = default;
    GameObject& operator=(const GameObject& otherGObj) = default;
    GameObject& operator=(GameObject&& otherGObj) = default;
    ~GameObject() = default;
    GameObject(Field& field, const Field::Position& gObjPos);

    void attachToField(Field& field, const Field::Position& gObjPos);
    Field* getAttachedField();
    bool isFieldAttached();

    const std::unordered_set<std::string>& getTags() const;
    void addTag(std::string tag);
    void deleteTag(std::string tag);
    bool hasTag(std::string tag) const;

    std::size_t getSize() const;

    //Interaction
    virtual _offsets_set getPositionsOfInteraction() const;
    void changeCellsOfInteraction(const _offsets_set& cells);
    void changeCellsOfInteraction(const std::vector<std::pair<long long, long long>>& cells);
    void changeCellsOfinteraction(const std::set<std::pair<long long, long long>>& cells);

    virtual bool canIntersectWith(const GameObject* gObj) const;
    virtual bool canInteractWith(const GameObject* gObj) const;
    struct Interactor {
        void operator() () {};
    };

protected:
    Field* attachedField{};

    double health{};
    int interactionsPossible{};

    std::unordered_set<std::string> tags;

    _offsets_set cellsOfInteraction;
    Interactor interactor;

private:
    _offsets_set currentForm;
};


class MovingObject: virtual public GameObject {
public:
    MovingObject() = default;
    MovingObject(const MovingObject& otherGObj) = default;
    MovingObject(MovingObject&& otherGObj) = default;
    MovingObject& operator=(const MovingObject& otherGObj) = default;
    MovingObject& operator=(MovingObject&& otherGObj) = default;
    ~MovingObject() = default;
    MovingObject(Field& field, const Field::Position& gObjPos);

    virtual _offsets_set getMovesFromCenter() const;

protected:
    int movesPossible;
};


class RotatingObject: virtual public GameObject {

public:
    enum Orientation {
        UP,
        RIGHT,
        DOWN,
        LEFT,
    };

    RotatingObject(const RotatingObject& otherGObj) = default;
    RotatingObject(RotatingObject&& otherGObj) = default;
    RotatingObject& operator=(const RotatingObject& otherGObj) = default;
    RotatingObject& operator=(RotatingObject&& otherGObj) = default;
    ~RotatingObject() = default;
    explicit RotatingObject(const Orientation& _orientation);
    RotatingObject(Field& field, const Field::Position& gObjPos, const Orientation& _orientation);

    bool rotateClockwiseOffField(int rotates);
    bool rotateCounterclockwiseOffField(int rotates);

    void changePivot(const _offset& newPivot);

protected:
    RotatingObject();

    std::unordered_set<Orientation> orientationsAvailable;
    Orientation orientation;
    _offset pivot;
};


class FormChangingObject: virtual public GameObject {
public:
    FormChangingObject();
    FormChangingObject(const FormChangingObject& otherGObj) = default;
    FormChangingObject(FormChangingObject&& otherGObj) = default;
    FormChangingObject& operator=(const FormChangingObject& otherGObj) = default;
    FormChangingObject& operator=(FormChangingObject&& otherGObj) = default;
    ~FormChangingObject() = default;
    FormChangingObject(Field& field, const Field::Position& gObjPos);

    bool changeFormOffField(const _offsets_set& newCellFromCenter);
    bool nextFormOffField();

protected:
    std::vector<_offsets_set> formPresets;
};


class CreatorObject: virtual public GameObject {
public:
    CreatorObject() = default;
    CreatorObject(const CreatorObject& otherGObj) = default;
    CreatorObject(CreatorObject&& otherGObj) = default;
    CreatorObject& operator=(const CreatorObject& otherGObj) = default;
    CreatorObject& operator=(CreatorObject&& otherGObj) = default;
    ~CreatorObject() = default;
    CreatorObject(Field& field, const Field::Position& gObjPos);

    template<class ObjectT, typename... Args>
    ObjectT* createObject(Args... args) {
        return new ObjectT(args...);
    };
};

#endif //UNIVERSAL_GAMING_FIELD_GAME_OBJECTS_H
