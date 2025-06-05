#include "game.h"
#include "nlohmann/json.hpp" // For loading questions DB and session

using json = nlohmann::json;

Game::Game(const std::string &q_db_fname, const std::string &leaderboard_fname)
    : leaderboard(leaderboard_fname), questions_db_filename(q_db_fname)
{
    loadAllQuestions(); // Load all questions once at construction
    loadSessionState(); // Try to load an existing session
}

bool Game::loadAllQuestions()
{
    all_questions.clear();
    std::ifstream q_file(questions_db_filename);
    if (q_file.is_open())
    {
        try
        {
            json j;
            q_file >> j;
            all_questions = j.get<std::vector<Question>>();
            return true;
        }
        catch (json::parse_error &e)
        {
            std::cerr << "Error parsing questions DB " << questions_db_filename << ": " << e.what() << std::endl;
        }
        q_file.close();
    }
    else
    {
        std::cerr << "Could not open questions DB: " << questions_db_filename << std::endl;
    }
    return false;
}

bool Game::saveSessionState() const
{
    std::ofstream file(session_state_filename);
    if (file.is_open())
    {
        json j = currentSession;
        file << j.dump(4);
        file.close();
        return true;
    }
    std::cerr << "Error: Could not save game session to " << session_state_filename << std::endl;
    return false;
}

bool Game::loadSessionState()
{
    std::ifstream file(session_state_filename);
    if (file.is_open())
    {
        try
        {
            json j;
            file >> j;
            currentSession = j.get<GameSession>();
            file.close();
            return currentSession.active; // Return true if session is marked active
        }
        catch (json::parse_error &e)
        {
            std::cerr << "Error parsing game session file " << session_state_filename << ": " << e.what() << std::endl;
            // Corrupted session file, treat as no active session
            clearSessionState(); // and save cleared state
        }
        file.close(); // ensure file is closed even on error
    }
    // If file doesn't exist or couldn't be opened, it implies no active session.
    // currentSession will have its default (inactive) state.
    return false;
}

void Game::clearSessionState()
{
    currentSession = GameSession(); // Reset to default (active = false)
    saveSessionState();             // Persist the cleared state
}

bool Game::startNewGame(const std::string &playerName, const std::string &difficulty)
{
    currentSession = GameSession(); // Reset any existing session
    currentSession.playerName = playerName;
    currentSession.difficulty = difficulty;

    for (const auto &q : all_questions)
    {
        if (q.difficulty == difficulty)
        {
            currentSession.questionsForSession.push_back(q);
        }
    }

    if (currentSession.questionsForSession.empty())
    {
        std::cout << "No questions found for difficulty: " << difficulty << ". Cannot start game." << std::endl;
        return false;
    }

    // Optional: Shuffle questions for this game session
    // std::random_device rd;
    // std::mt19937 g(rd());
    // std::shuffle(currentSession.questionsForSession.begin(), currentSession.questionsForSession.end(), g);

    currentSession.active = true;
    currentSession.currentQuestionIndex = 0;
    currentSession.currentScore = 0.0f;
    currentSession.used5050 = false;

    if (saveSessionState())
    {
        std::cout << "New game started for " << playerName << " at difficulty " << difficulty << "." << std::endl;
        return true;
    }
    return false;
}

void Game::displayCurrentQuestion() const
{
    if (!currentSession.active || currentSession.questionsForSession.empty() ||
        currentSession.currentQuestionIndex >= static_cast<int>(currentSession.questionsForSession.size()))
    {
        std::cout << "No active question to display." << std::endl;
        if (currentSession.active && currentSession.currentQuestionIndex >= static_cast<int>(currentSession.questionsForSession.size()))
        {
            std::cout << "All questions answered. Game might be over." << std::endl;
        }
        return;
    }

    const auto &q = currentSession.questionsForSession[currentSession.currentQuestionIndex];
    std::cout << "\n------------------------------------------" << std::endl;
    std::cout << "Player: " << currentSession.playerName << " | Score: " << currentSession.currentScore << std::endl;
    std::cout << "Question " << (currentSession.currentQuestionIndex + 1) << "/" << currentSession.questionsForSession.size() << ": " << std::endl;
    std::cout << q.text << std::endl;
    for (size_t j = 0; j < q.options.size(); ++j)
    {
        if (!q.options[j].empty())
        { // Only display non-empty options (for 50/50)
            std::cout << "  " << j << ") " << q.options[j] << std::endl;
        }
    }
    std::cout << "------------------------------------------" << std::endl;
    if (!currentSession.used5050)
    {
        std::cout << "Lifeline available: 50/50 (use './millionaire_game lifeline 5050')" << std::endl;
    }
}

bool Game::processAnswer(int answer_index)
{
    if (!currentSession.active)
    {
        std::cout << "No active game session. Start a new game first." << std::endl;
        return false;
    }
    if (currentSession.currentQuestionIndex >= static_cast<int>(currentSession.questionsForSession.size()))
    {
        std::cout << "All questions have been answered. Game is over." << std::endl;
        endGame();
        return false; // No more questions to process
    }

    const auto &q = currentSession.questionsForSession[currentSession.currentQuestionIndex];

    std::cout << "\nPlayer's answer: " << answer_index;
    if (answer_index >= 0 && answer_index < static_cast<int>(q.options.size()) && !q.options[answer_index].empty())
    {
        std::cout << " (" << q.options[answer_index] << ")" << std::endl;
    }
    else
    {
        std::cout << " (Invalid or unavailable option)" << std::endl;
        // if answer is invalid, treat as wrong
        std::cout << "Incorrect. The correct answer was: " << q.correct_index << ") " << q.options[q.correct_index] << std::endl;
        currentSession.currentQuestionIndex++; // Move to next question or end game
        saveSessionState();
        return true; // Processed, but was incorrect.
    }

    if (answer_index == q.correct_index)
    {
        std::cout << "Correct!" << std::endl;
        currentSession.currentScore += 1.0f; // Or your scoring logic
    }
    else
    {
        std::cout << "Incorrect. The correct answer was: " << q.correct_index << ") " << q.options[q.correct_index] << std::endl;
        // Depending on rules, game might end here. For now, we continue.
    }

    currentSession.currentQuestionIndex++;
    saveSessionState();
    return true;
}

void Game::endGame()
{
    if (!currentSession.active)
        return;

    std::cout << "\n--- Game Over, " << currentSession.playerName << "! ---" << std::endl;
    std::cout << "Your final score: " << currentSession.currentScore << std::endl;

    leaderboard.addPlayer(Player(currentSession.playerName, currentSession.currentScore));
    std::cout << "Score has been updated on the main leaderboard." << std::endl;
    leaderboard.show(); // Show updated leaderboard

    clearSessionState(); // Marks session inactive and saves
}

bool Game::apply5050Lifeline()
{
    if (!currentSession.active)
    {
        std::cout << "No active game to apply lifeline." << std::endl;
        return false;
    }
    if (currentSession.used5050)
    {
        std::cout << "50/50 lifeline has already been used." << std::endl;
        return false;
    }
    if (currentSession.currentQuestionIndex >= static_cast<int>(currentSession.questionsForSession.size()))
    {
        std::cout << "No current question to apply lifeline to." << std::endl;
        return false;
    }

    Question &current_q = currentSession.questionsForSession[currentSession.currentQuestionIndex];
    if (current_q.options.size() < 4)
    { // Should be 4 for 50/50 to make sense
        std::cout << "Cannot apply 50/50: Not enough options." << std::endl;
        return false;
    }

    std::vector<int> incorrect_options_indices;
    for (size_t i = 0; i < current_q.options.size(); ++i)
    {
        if (static_cast<int>(i) != current_q.correct_index)
        {
            incorrect_options_indices.push_back(i);
        }
    }

    if (incorrect_options_indices.size() < 2)
    {
        std::cout << "Cannot apply 50/50: Not enough incorrect options to remove." << std::endl;
        return false; // Should not happen with standard questions
    }

    // Shuffle incorrect options and pick two to remove
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(incorrect_options_indices.begin(), incorrect_options_indices.end(), g);

    // Mark two incorrect options as empty.
    // The displayCurrentQuestion method will need to handle empty strings.
    current_q.options[incorrect_options_indices[0]] = "";
    current_q.options[incorrect_options_indices[1]] = "";

    currentSession.used5050 = true;
    std::cout << "50/50 lifeline applied. Two incorrect options removed." << std::endl;
    saveSessionState();
    return true;
}

void Game::showInstructions() const
{
    // (Keep existing instructions, but you might want to update command examples)
    std::cout << "\n--- How to Play Who Wants to Be a Millionaire (CLI Iterative Version) ---" << std::endl;
    std::cout << "The game proceeds one question at a time." << std::endl;
    std::cout << "1. Start a game: ./millionaire_game start <player_name> <difficulty>" << std::endl;
    std::cout << "   The first question will be displayed." << std::endl;
    std::cout << "2. Answer a question: ./millionaire_game answer <answer_index>" << std::endl;
    std::cout << "   (where answer_index is 0, 1, 2, or 3)" << std::endl;
    std::cout << "   The result and next question (or game end) will be shown." << std::endl;
    std::cout << "3. Use a lifeline (before answering): ./millionaire_game lifeline 5050" << std::endl;
    std::cout << "   The current question will be re-displayed with fewer options." << std::endl;
    std::cout << "\n--- Lifelines ---" << std::endl;
    std::cout << "  50/50: Removes two incorrect answers." << std::endl;
    std::cout << "\n--- Scoring ---" << std::endl;
    std::cout << "You get 1 point for each correct answer." << std::endl;
    std::cout << "Your score will be added to the leaderboard upon game completion." << std::endl;
    std::cout << "\n--- Other Commands ---" << std::endl;
    std::cout << "  ./millionaire_game instructions" << std::endl;
    std::cout << "    - Shows these instructions." << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;
}