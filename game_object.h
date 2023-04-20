//
// Created by dronl on 18.04.2023.
//

#ifndef UNIVERSAL_GAMING_FIELD_GAME_OBJECT_H
#define UNIVERSAL_GAMING_FIELD_GAME_OBJECT_H

#include <unordered_set>
#include <string>

class GameObject {

public:

    const std::unordered_set<std::string>& getTags() const;
    const bool hasTag(std::string tag) const;

private:

    static std::unordered_set<std::string> tags;
    static std::unordered_set<std::string> possibleTags;

};


#endif //UNIVERSAL_GAMING_FIELD_GAME_OBJECT_H
