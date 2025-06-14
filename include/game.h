#ifndef GAME_H
#define GAME_H

#include "question.h"
#include "player.h"
#include "leaderboard.h"
#include "game_session.h" // Include the new session structure
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

class Game
{
private:
    std::vector<Question> all_questions; // All questions loaded from questions.json
    Leaderboard leaderboard;
    GameSession currentSession; // Holds the state of the active game

    const std::string questions_db_filename;
    const std::string session_state_filename = "game_session.json";

public:
    Game(const std::string &q_db_fname = "questions.json", const std::string &leaderboard_fname = "leaderboard.json");

    // Getters for private attributes
    const std::vector<Question> &getAllQuestions() const { return all_questions; }
    const Leaderboard &getLeaderboard() const { return leaderboard; }
    Leaderboard &getLeaderboard() { return leaderboard; }
    const GameSession &getCurrentSession() const { return currentSession; }
    GameSession &getCurrentSession() { return currentSession; }

    bool loadAllQuestions(); // Helper to load questions from DB

    // Session Management
    bool saveSessionState() const;
    bool loadSessionState();
    void clearSessionState();

    // Game Flow
    bool startNewGame(const std::string &playerName, const std::string &difficulty);
    bool processAnswer(int answer_index);
    void displayCurrentQuestion() const; // Displays question, options
    bool apply5050Lifeline();
    void endGame();

    void showInstructions() const;
};

#endif // GAME_H