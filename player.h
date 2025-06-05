#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "nlohmann/json.hpp" // For JSON serialization

// Forward declaration for nlohmann/json
using json = nlohmann::json;

class Player
{
public:
    std::string name;
    float score; // [cite: 5]

    Player(std::string n = "", float s = 0.0f);

    // For JSON serialization/deserialization
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Player, name, score);
};

#endif // PLAYER_H