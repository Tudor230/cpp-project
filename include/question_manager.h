#ifndef QUESTION_MANAGER_H
#define QUESTION_MANAGER_H

#include "question.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class QuestionManager
{
private:
    std::vector<Question> questions; // [cite: 7] Composition of Question objects [cite: 18]
    std::string filename;

public:
    QuestionManager(std::string fname = "questions.json");

    // Getters for private attributes
    const std::vector<Question> &getQuestions() const { return questions; }
    const std::string &getFilename() const { return filename; }

    void load();       // Renamed from load(string filename) [cite: 7]
    void save() const; // Renamed from save(string filename) [cite: 7]
    void addQuestion(const std::string &text, const std::vector<std::string> &options,
                     int correct_index, const std::string &difficulty); // Signature based on CLI [cite: 13]
    void showAllQuestions() const;                                      // Helper to see questions, not in PDF but useful
};

#endif // QUESTION_MANAGER_H