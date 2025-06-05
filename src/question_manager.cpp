#include "question_manager.h"

QuestionManager::QuestionManager(std::string fname) : filename(std::move(fname))
{
    load();
}

void QuestionManager::load()
{ // [cite: 7]
    std::ifstream file(filename);
    if (file.is_open())
    {
        try
        {
            json j;
            file >> j;
            questions = j.get<std::vector<Question>>();
        }
        catch (json::parse_error &e)
        {
            std::cerr << "Error parsing questions file " << filename << ": " << e.what() << std::endl;
            questions.clear();
        }
        file.close();
    }
    else
    {
        std::cerr << "Could not open questions file: " << filename << ". Starting with an empty question bank." << std::endl;
        questions.clear();
    }
}

void QuestionManager::save() const
{ // [cite: 7]
    std::ofstream file(filename);
    if (file.is_open())
    {
        json j = questions;
        file << j.dump(4); // Pretty print
        file.close();
    }
    else
    {
        std::cerr << "Error: Could not save questions to " << filename << std::endl;
    }
}

void QuestionManager::addQuestion(const std::string &text, const std::vector<std::string> &options,
                                  int correct_index, const std::string &difficulty)
{
    if (options.size() != 4)
    {
        std::cerr << "Error: A question must have exactly 4 options." << std::endl;
        return;
    }
    if (correct_index < 0 || correct_index >= 4)
    {
        std::cerr << "Error: Correct index must be between 0 and 3." << std::endl;
        return;
    }
    questions.emplace_back(text, options, correct_index, difficulty);
    save();
    std::cout << "Question added successfully." << std::endl;
}

void QuestionManager::showAllQuestions() const
{
    if (questions.empty())
    {
        std::cout << "No questions in the bank." << std::endl;
        return;
    }
    std::cout << "--- Question Bank ---" << std::endl;
    for (size_t i = 0; i < questions.size(); ++i)
    {
        const auto &q = questions[i];
        std::cout << "Q" << i + 1 << " (" << q.difficulty << "): " << q.text << std::endl;
        for (size_t j = 0; j < q.options.size(); ++j)
        {
            std::cout << "  " << char('A' + j) << ") " << q.options[j] << std::endl;
        }
        std::cout << "  Correct: " << char('A' + q.correct_index) << std::endl;
        std::cout << "---------------------" << std::endl;
    }
}