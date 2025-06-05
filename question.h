#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Question
{
public:
    std::string text;                 // [cite: 5]
    std::vector<std::string> options; // [cite: 5]
    int correct_index;                // [cite: 5]
    std::string difficulty;           // Added based on usage in commands

    Question(std::string txt = "",
             std::vector<std::string> opts = {},
             int correct_idx = -1,
             std::string diff = "easy");

    // For JSON serialization/deserialization
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Question, text, options, correct_index, difficulty);
};

#endif // QUESTION_H