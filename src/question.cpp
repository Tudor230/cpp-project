#include "question.h"

Question::Question(std::string txt, std::vector<std::string> opts, int correct_idx, std::string diff)
    : text(std::move(txt)), options(std::move(opts)), correct_index(correct_idx), difficulty(std::move(diff)) {}

// JSON serialization functions implementation
void to_json(json &j, const Question &q)
{
    j = json{
        {"text", q.getText()},
        {"options", q.getOptions()},
        {"correct_index", q.getCorrectIndex()},
        {"difficulty", q.getDifficulty()}};
}

void from_json(const json &j, Question &q)
{
    j.at("text").get_to(q.text);
    j.at("options").get_to(q.options);
    j.at("correct_index").get_to(q.correct_index);
    j.at("difficulty").get_to(q.difficulty);
}