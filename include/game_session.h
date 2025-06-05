#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "question.h" // Assuming Question class is defined
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

using json = nlohmann::json;

struct GameSession
{
    bool active = false;                       // Is a game currently in progress?
    std::string playerName;                    // Name of the current player
    std::string difficulty;                    // Difficulty of the current game
    float currentScore = 0.0f;                 // Player's current score in this session
    int currentQuestionIndex = 0;              // Index of the current question in questionsForSession
    std::vector<Question> questionsForSession; // The actual questions selected for this game session
    bool used5050 = false;                     // Has the 50/50 lifeline been used?
    // To support 50/50 displaying modified options, we might need to store
    // the temporarily modified options or indices to hide.
    // For simplicity, apply5050 will modify the options in questionsForSession directly for the current question.

    // For JSON serialization/deserialization
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameSession, active, playerName, difficulty, currentScore, currentQuestionIndex, questionsForSession, used5050);
};

#endif // GAME_SESSION_H