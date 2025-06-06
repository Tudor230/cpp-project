#include "game_session.h"

// JSON serialization functions implementation
void to_json(json &j, const GameSession &gs)
{
    j = json{
        {"active", gs.isActive()},
        {"playerName", gs.getPlayerName()},
        {"difficulty", gs.getDifficulty()},
        {"currentScore", gs.getCurrentScore()},
        {"currentQuestionIndex", gs.getCurrentQuestionIndex()},
        {"questionsForSession", gs.getQuestionsForSession()},
        {"used5050", gs.hasUsed5050()}};
}

void from_json(const json &j, GameSession &gs)
{
    bool active;
    std::string playerName, difficulty;
    float currentScore;
    int currentQuestionIndex;
    std::vector<Question> questionsForSession;
    bool used5050;

    j.at("active").get_to(active);
    j.at("playerName").get_to(playerName);
    j.at("difficulty").get_to(difficulty);
    j.at("currentScore").get_to(currentScore);
    j.at("currentQuestionIndex").get_to(currentQuestionIndex);
    j.at("questionsForSession").get_to(questionsForSession);
    j.at("used5050").get_to(used5050);

    gs.setActive(active);
    gs.setPlayerName(playerName);
    gs.setDifficulty(difficulty);
    gs.setCurrentScore(currentScore);
    gs.setCurrentQuestionIndex(currentQuestionIndex);
    gs.setQuestionsForSession(questionsForSession);
    gs.setUsed5050(used5050);
}