#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "player.h"
#include <vector>
#include <string>
#include <fstream>   // For file operations
#include <iostream>  // For output
#include <algorithm> // For sorting
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Leaderboard
{
public:
    std::vector<Player> players; // [cite: 1] Aggregation of Player records [cite: 19]
    std::string filename;

    Leaderboard(std::string fname = "leaderboard.json");

    void loadFromFile();             // Renamed from loadFromFile(string filename) [cite: 1] for consistency
    void saveToFile();               // Renamed from saveToFile(string filename) [cite: 1]
    void addPlayer(const Player &p); // [cite: 1]
    void show() const;               // [cite: 6]
    void showPlayer(const std::string &playerName) const;
};

#endif // LEADERBOARD_H