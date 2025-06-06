#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Question
{
private:
    std::string text;                 // [cite: 5]
    std::vector<std::string> options; // [cite: 5]
    int correct_index;                // [cite: 5]
    std::string difficulty;           // Added based on usage in commands

public:
    Question(std::string txt = "",
             std::vector<std::string> opts = {},
             int correct_idx = -1,
             std::string diff = "easy");

    // Getters and setters for private attributes
    const std::string &getText() const { return text; }
    void setText(const std::string &t) { text = t; }
    const std::vector<std::string> &getOptions() const { return options; }
    std::vector<std::string> &getOptions() { return options; }
    void setOptions(const std::vector<std::string> &opts) { options = opts; }
    int getCorrectIndex() const { return correct_index; }
    void setCorrectIndex(int idx) { correct_index = idx; }
    const std::string &getDifficulty() const { return difficulty; }
    void setDifficulty(const std::string &diff) { difficulty = diff; }

    // For JSON serialization/deserialization - friend function approach
    friend void to_json(json &j, const Question &q);
    friend void from_json(const json &j, Question &q);
};

// JSON serialization functions
void to_json(json &j, const Question &q);
void from_json(const json &j, Question &q);

#endif // QUESTION_H