#include "player.h"

Player::Player(std::string n, float s) : name(std::move(n)), score(s) {}

// JSON serialization functions implementation
void to_json(json &j, const Player &p)
{
    j = json{{"name", p.getName()}, {"score", p.getScore()}};
}

void from_json(const json &j, Player &p)
{
    j.at("name").get_to(p.name);
    j.at("score").get_to(p.score);
}