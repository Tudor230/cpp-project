#include "leaderboard.h"

Leaderboard::Leaderboard(std::string fname) : filename(std::move(fname))
{
    loadFromFile();
}

void Leaderboard::loadFromFile()
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        try
        {
            json j;
            file >> j;
            players = j.get<std::vector<Player>>();
        }
        catch (json::parse_error &e)
        {
            std::cerr << "Error parsing leaderboard file " << filename << ": " << e.what() << std::endl;
            // Optionally, start with an empty leaderboard
            players.clear();
        }
        file.close();
    }
    else
    {
        std::cerr << "Could not open leaderboard file: " << filename << ". Starting with an empty leaderboard." << std::endl;
        players.clear(); // Ensure players is empty if file doesn't exist or can't be opened
    }
}

void Leaderboard::saveToFile()
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        json j = players;
        file << j.dump(4); // Pretty print JSON with 4 spaces indent
        file.close();
    }
    else
    {
        std::cerr << "Error: Could not save leaderboard to " << filename << std::endl;
    }
}

void Leaderboard::addPlayer(const Player &p)
{
    bool found = false;
    for (auto &player : players)
    {
        if (player.name == p.name)
        {
            // Update score if new score is higher (or just update regardless, depending on desired logic)
            // PDF does not specify, let's update if higher.
            if (p.score > player.score)
            {
                player.score = p.score;
            }
            found = true;
            break;
        }
    }
    if (!found)
    {
        players.push_back(p);
    }
    // Sort players by score descending
    std::sort(players.begin(), players.end(), [](const Player &a, const Player &b)
              { return a.score > b.score; });
    saveToFile();
}

void Leaderboard::show() const
{ // [cite: 6]
    if (players.empty())
    {
        std::cout << "Leaderboard is empty." << std::endl;
        return;
    }
    std::cout << "--- Leaderboard ---" << std::endl;
    for (const auto &player : players)
    {
        std::cout << player.name << ": " << player.score << " points" << std::endl;
    }
    std::cout << "-------------------" << std::endl;
}

void Leaderboard::showPlayer(const std::string &playerName) const
{
    bool found = false;
    for (const auto &player : players)
    {
        if (player.name == playerName)
        {
            std::cout << "--- Player Score ---" << std::endl;
            std::cout << player.name << ": " << player.score << " points" << std::endl;
            std::cout << "--------------------" << std::endl;
            found = true;
            break;
        }
    }
    if (!found)
    {
        std::cout << "Player '" << playerName << "' not found on the leaderboard." << std::endl;
    }
}