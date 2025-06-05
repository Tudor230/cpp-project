cmake_minimum_required(VERSION 3.10)
project(MillionaireProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory
include_directories(include)

# # --- nlohmann/json ---
# # Option 1: Add nlohmann/json directly (e.g., as a submodule or copy headers)
# # Assuming nlohmann/json/single_include/nlohmann/json.hpp is available
# # Create a directory named "nlohmann_json_include" in your project root.
# // Inside "nlohmann_json_include" create a directory "nlohmann"
# # Download json.hpp from https://github.com/nlohmann/json/releases (e.g., v3.11.3)
# # and place it in project_root/nlohmann_json_include/nlohmann/json.hpp
# include_directories(nlohmann_json_include)

# Option 2: FetchContent (CMake 3.11+)
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3 # Or latest version
)
FetchContent_MakeAvailable(nlohmann_json)

# --- Source Files ---
set(COMMON_SOURCES
    src/player.cpp
    src/question.cpp
    src/leaderboard.cpp
    src/question_manager.cpp
    src/game.cpp
)

set(COMMON_HEADERS
    include/player.h
    include/question.h
    include/leaderboard.h
    include/question_manager.h
    include/game.h
    include/colors.h
    include/game_session.h
    # Add any other common headers, e.g., for JSON parsing utilities if you make one
)

# --- Executable 1: millionaire_game ---
add_executable(millionaire_game src/main_game.cpp ${COMMON_SOURCES})
if(MSVC)
    target_compile_options(millionaire_game PRIVATE /W4)
else()
    target_compile_options(millionaire_game PRIVATE -Wall -Wextra -pedantic)
endif()

# --- Executable 2: millionaire_util ---
add_executable(millionaire_util src/main_util.cpp ${COMMON_SOURCES})
if(MSVC)
    target_compile_options(millionaire_util PRIVATE /W4)
else()
    target_compile_options(millionaire_util PRIVATE -Wall -Wextra -pedantic)
endif()

# If using FetchContent for nlohmann_json, link it:
target_link_libraries(millionaire_game PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(millionaire_util PRIVATE nlohmann_json::nlohmann_json)