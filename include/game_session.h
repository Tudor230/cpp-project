#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "question.h" // Assuming Question class is defined
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

using json = nlohmann::json;

class GameSession
{
private:
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

public:
    // Default constructor
    GameSession() = default;

    // Getters and setters for private attributes
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }
    const std::string &getPlayerName() const { return playerName; }
    void setPlayerName(const std::string &name) { playerName = name; }
    const std::string &getDifficulty() const { return difficulty; }
    void setDifficulty(const std::string &diff) { difficulty = diff; }
    float getCurrentScore() const { return currentScore; }
    void setCurrentScore(float score) { currentScore = score; }
    int getCurrentQuestionIndex() const { return currentQuestionIndex; }
    void setCurrentQuestionIndex(int index) { currentQuestionIndex = index; }
    const std::vector<Question> &getQuestionsForSession() const { return questionsForSession; }
    std::vector<Question> &getQuestionsForSession() { return questionsForSession; }
    void setQuestionsForSession(const std::vector<Question> &questions) { questionsForSession = questions; }
    bool hasUsed5050() const { return used5050; }
    void setUsed5050(bool used) { used5050 = used; }

    // For JSON serialization/deserialization - friend function approach
    friend void to_json(json &j, const GameSession &gs);
    friend void from_json(const json &j, GameSession &gs);
};

// JSON serialization functions
void to_json(json &j, const GameSession &gs);
void from_json(const json &j, GameSession &gs);

#endif // GAME_SESSION_H