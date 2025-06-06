#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "nlohmann/json.hpp" // For JSON serialization

// Forward declaration for nlohmann/json
using json = nlohmann::json;

class Player
{
private:
    std::string name;
    float score; // [cite: 5]

public:
    Player(std::string n = "", float s = 0.0f);

    // Getters and setters for private attributes
    const std::string &getName() const { return name; }
    void setName(const std::string &n) { name = n; }
    float getScore() const { return score; }
    void setScore(float s) { score = s; }

    // For JSON serialization/deserialization - friend function approach
    friend void to_json(json &j, const Player &p);
    friend void from_json(const json &j, Player &p);
};

// JSON serialization functions
void to_json(json &j, const Player &p);
void from_json(const json &j, Player &p);

#endif // PLAYER_H