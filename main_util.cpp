#include "leaderboard.h"
#include "question_manager.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept> // For std::stoi exception handling

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage:" << std::endl;
        std::cerr << "  ./millionaire_util leaderboard" << std::endl;
        std::cerr << "  ./millionaire_util player <player_name>" << std::endl;
        std::cerr << "  ./millionaire_util add_question \"<text>\" \"<optA>\" \"<optB>\" \"<optC>\" \"<optD>\" <correct_idx> <difficulty>" << std::endl;
        std::cerr << "  ./millionaire_util show_questions (Bonus: to view all questions)" << std::endl;
        return 1;
    }

    std::string command = argv[1];
    Leaderboard leaderboard; // Uses default "leaderboard.json"
    QuestionManager qm;      // Uses default "questions.json"

    if (command == "leaderboard")
    { // [cite: 13, 17]
        if (argc != 2)
        {
            std::cerr << "Usage: ./millionaire_util leaderboard" << std::endl;
            return 1;
        }
        leaderboard.show();
    }
    else if (command == "player")
    { // [cite: 14, 17]
        if (argc != 3)
        {
            std::cerr << "Usage: ./millionaire_util player <player_name>" << std::endl;
            return 1;
        }
        std::string playerName = argv[2];
        leaderboard.showPlayer(playerName);
    }
    else if (command == "add_question")
    { // [cite: 13, 17]
        if (argc != 9)
        {
            std::cerr << "Usage: ./millionaire_util add_question \"<text>\" \"<optA>\" \"<optB>\" \"<optC>\" \"<optD>\" <correct_idx> <difficulty>" << std::endl;
            std::cerr << "Example: ./millionaire_util add_question \"Capital of C++?\" \"Bjarne\" \"Java\" \"Pointers\" \"Runtime\" 0 easy" << std::endl;
            return 1;
        }
        std::string q_text = argv[2];
        std::vector<std::string> options = {argv[3], argv[4], argv[5], argv[6]};
        int correct_idx;
        try
        {
            correct_idx = std::stoi(argv[7]);
        }
        catch (const std::invalid_argument &ia)
        {
            std::cerr << "Error: Invalid correct index '" << argv[7] << "'. Must be an integer." << std::endl;
            return 1;
        }
        catch (const std::out_of_range &oor)
        {
            std::cerr << "Error: Correct index '" << argv[7] << "' is out of range." << std::endl;
            return 1;
        }
        std::string difficulty = argv[8];

        qm.addQuestion(q_text, options, correct_idx, difficulty);
    }
    else if (command == "show_questions")
    { // Bonus command, not in PDF but useful for testing
        if (argc != 2)
        {
            std::cerr << "Usage: ./millionaire_util show_questions" << std::endl;
            return 1;
        }
        qm.showAllQuestions();
    }
    else
    {
        std::cerr << "Unknown command: " << command << std::endl;
        // (Usage message already printed if argc < 2)
        return 1;
    }

    return 0;
}