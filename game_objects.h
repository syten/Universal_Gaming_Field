#ifndef UNIVERSAL_GAMING_FIELD_GAME_OBJECTS_H
#define UNIVERSAL_GAMING_FIELD_GAME_OBJECTS_H

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <set>
#include <exception>
#include <cmath>
#include <vector>

struct pair_hash {
    inline std::size_t operator() (const std::pair<long long, long long>& v) const {
        return v.first * 31 + v.second;
    }
};

typedef std::pair<long long, long long> _offset;
typedef std::unordered_set<_offset, pair_hash> _offsets_set;

class Field;
class MovingObject;
class RotatingObject;
class FormChangingObject;
class CreatorObject;

enum Mode {
    INTERACTION,
    INTERSECTION
};

class GameObject {
    friend class Field;

public:
    GameObject() = default;
    GameObject(const GameObject& otherGObj) = default;
    GameObject(GameObject&& otherGObj) = delete;
    GameObject& operator=(const GameObject& otherGObj) = default;
    GameObject& operator=(GameObject&& otherGObj) = delete;
    virtual ~GameObject() = default;

    double getHealth() const;
    void changeHealth(double shift);

    Field* getAttachedField();
    bool isFieldAttached();

    const std::unordered_set<std::string>& getTags() const;
    void addTag(std::string tag);
    void deleteTag(std::string tag);
    bool hasTag(std::string tag) const;

    std::size_t getSize() const;
    const _offsets_set& getCurrentForm();

    //Interaction
    virtual _offsets_set getPositionsOfInteraction() const;
    void changeCellsOfInteraction(const _offsets_set& cells);
    void changeCellsOfInteraction(const std::vector<std::pair<long long, long long>>& cells);
    void changeCellsOfInteraction(const std::set<std::pair<long long, long long>>& cells);

    virtual bool operator() (Mode mode, GameObject* gObj, bool needToInteract);
    virtual bool operator() (Mode mode, MovingObject* gObj, bool needToInteract);
    virtual bool operator() (Mode mode, RotatingObject* gObj, bool needToInteract);
    virtual bool operator() (Mode mode, FormChangingObject* gObj, bool needToInteract);
    virtual bool operator() (Mode mode, CreatorObject* gObj, bool needToInteract);

protected:

    Field* attachedField{};

    double health{};
    int interactionsPossible{};

    std::unordered_set<std::string> tags;

    _offsets_set cellsOfInteraction;

    _offsets_set currentForm;

private:
    void attachToField(Field* field);
    void detachFromField();
};


class MovingObject: virtual public GameObject {
public:
    MovingObject() = default;
    MovingObject(const MovingObject& otherGObj) = default;
    MovingObject(MovingObject&& otherGObj) = delete;
    MovingObject& operator=(const MovingObject& otherGObj) = default;
    MovingObject& operator=(MovingObject&& otherGObj) = delete;
    virtual ~MovingObject() = default;

    _offsets_set getMovesFromCenter() const;
    int getMovesPossible() const;
    void changeMovesPossible(int shift);

protected:
    _offsets_set cellToMove{};
    int movesPossible{};
};


class RotatingObject: virtual public GameObject {

public:
    class wrong_orientation_exception: public std::exception {
        const char* what() const throw() {
            return "Wrong object orientation exception";
        }
    };

    enum Orientation {
        UP,
        RIGHT,
        DOWN,
        LEFT,
    };

    RotatingObject(const RotatingObject& otherGObj) = default;
    RotatingObject(RotatingObject&& otherGObj) = delete;
    RotatingObject& operator=(const RotatingObject& otherGObj) = default;
    RotatingObject& operator=(RotatingObject&& otherGObj) = delete;
    virtual ~RotatingObject() = default;
    explicit RotatingObject(const Orientation& _orientation);

    bool rotateClockwiseOffField(unsigned int rotates);
    bool rotateCounterclockwiseOffField(unsigned int rotates);

    _offset getPivot();
    void changePivot(const _offset& newPivot);

    Orientation getOrientation() const;

protected:
    RotatingObject();

    std::unordered_map<Orientation, bool> orientationsAvailable;
    Orientation orientation;
    _offset pivot;
};


class FormChangingObject: virtual public GameObject {
public:
    FormChangingObject() = default;
    FormChangingObject(const FormChangingObject& otherGObj) = default;
    FormChangingObject(FormChangingObject&& otherGObj) = delete;
    FormChangingObject& operator=(const FormChangingObject& otherGObj) = default;
    FormChangingObject& operator=(FormChangingObject&& otherGObj) = delete;
    virtual ~FormChangingObject() = default;

    bool changeFormOffField(const _offsets_set& newCellFromCenter);
    bool nextFormOffField();

protected:
    std::vector<_offsets_set> formPresets;
};


class CreatorObject: virtual public GameObject {
public:
    CreatorObject() = default;
    CreatorObject(const CreatorObject& otherGObj) = default;
    CreatorObject(CreatorObject&& otherGObj) = delete;
    CreatorObject& operator=(const CreatorObject& otherGObj) = default;
    CreatorObject& operator=(CreatorObject&& otherGObj) = delete;
    virtual ~CreatorObject() = default;

    virtual GameObject* createObject(const std::type_info& info);
};

#endif //UNIVERSAL_GAMING_FIELD_GAME_OBJECTS_H