#include "game.h" // Game.h now includes game_session.h implicitly or explicitly
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage:" << std::endl;
        std::cerr << "  ./millionaire_game start <player_name> <difficulty>" << std::endl;
        std::cerr << "  ./millionaire_game answer <answer_index (0-3)>" << std::endl;
        std::cerr << "  ./millionaire_game lifeline <type (e.g., 5050)>" << std::endl;
        std::cerr << "  ./millionaire_game instructions" << std::endl;
        return 1;
    }

    Game game; // Constructor loads all_questions and any existing session
    std::string command = argv[1];

    if (command == "instructions")
    {
        if (argc != 2)
        {
            std::cerr << "Usage: ./millionaire_game instructions" << std::endl;
            return 1;
        }
        game.showInstructions();
    }
    else if (command == "start")
    {
        if (argc != 4)
        {
            std::cerr << "Usage: ./millionaire_game start <player_name> <difficulty>" << std::endl;
            return 1;
        }
        std::string playerName = argv[2];
        std::string difficulty = argv[3];
        if (game.startNewGame(playerName, difficulty))
        {
            game.displayCurrentQuestion();
        }
    }
    else if (command == "answer")
    {
        if (!game.currentSession.active)
        {
            std::cout << "No active game. Use 'start' to begin a new game." << std::endl;
            return 1;
        }
        if (argc != 3)
        {
            std::cerr << "Usage: ./millionaire_game answer <answer_index>" << std::endl;
            return 1;
        }
        int answer_idx;
        try
        {
            answer_idx = std::stoi(argv[2]);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Invalid answer index: " << argv[2] << ". Must be an integer." << std::endl;
            return 1;
        }

        if (game.processAnswer(answer_idx))
        {
            // Check if game is over
            if (game.currentSession.currentQuestionIndex >= static_cast<int>(game.currentSession.questionsForSession.size()))
            {
                game.endGame(); // Finalizes score, updates leaderboard, clears session
            }
            else
            {
                game.displayCurrentQuestion(); // Display next question
            }
        }
    }
    else if (command == "lifeline")
    {
        if (!game.currentSession.active)
        {
            std::cout << "No active game. Use 'start' to begin a new game." << std::endl;
            return 1;
        }
        if (argc != 3)
        {
            std::cerr << "Usage: ./millionaire_game lifeline <type (e.g., 5050)>" << std::endl;
            return 1;
        }
        std::string type = argv[2];
        if (type == "5050")
        {
            if (game.apply5050Lifeline())
            {
                game.displayCurrentQuestion(); // Re-display question with fewer options
            }
        }
        else
        {
            std::cerr << "Unknown lifeline type: " << type << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Unknown command: " << command << std::endl;
        // (Usage message already printed if argc < 2)
        return 1;
    }

    return 0;
}