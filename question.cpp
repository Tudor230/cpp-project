#include "question.h"

Question::Question(std::string txt, std::vector<std::string> opts, int correct_idx, std::string diff)
    : text(std::move(txt)), options(std::move(opts)), correct_index(correct_idx), difficulty(std::move(diff)) {}