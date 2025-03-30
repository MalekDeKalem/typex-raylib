#include "raylib.h"
#include <fstream>
#include <iostream>
#include <math.h> /* ceil */
#include <random>
#include <stdio.h> /* printf */
#include <string>
#include <algorithm>

#define MAX_ENTRIES 20

// What it should be int the game?
/*
    a sound effect when a word is correctly typed (positive)
    a sound effect when a typo is made (negative)
    a sound effect when the word has not been typed before the timer (negative)
    player has three lives when live count is zero than it is game over
    lives are lost when word is not typed timely or they are lost when there is
   a typo the timer will get faster progressively the more words you type
   correctly the faster the timer gets each letter of a word is highlighted as
   you type them when you make a typo it will be shown via an animation when you
   lose a life it also will be shown via an animation the background will change
   the further you go timer will be represented as a bar that decreased as time
   goes on but will be restarted when a word is correctly typed words to type
   will be randomly selected
*/

typedef struct {
  std::vector<std::pair<std::string, int>> entries;
} LeaderBoardData;

typedef enum StateChar {
  DEFAULT,
  CORRECT,
  FALSE,
} StateChar;

typedef enum GameScreen {
  TITLE,
  GAMEPLAY,
  LeaderBoard,
} GameScreen;

const std::vector<std::string> Dict = {"interesting",
                                       "mother",
                                       "father",
                                       "gallant",
                                       "robber",
                                       "paleontologist",
                                       "dinosaur",
                                       "mammoth",
                                       "merit",
                                       "accomplishment",
                                       "car",
                                       "house",
                                       "driver",
                                       "maggot",
                                       "dog",
                                       "god",
                                       "chicken",
                                       "beef",
                                       "limousine",
                                       "puzzle",
                                       "doodle",
                                       "corn",
                                       "book",
                                       "tiger",
                                       "cat",
                                       "computer",
                                       "system",
                                       "failure",
                                       "success",
                                       "browser",
                                       "eye",
                                       "egg",
                                       "cheese",
                                       "potato",
                                       "tomato",
                                       "bacon",
                                       "onion",
                                       "garlic",
                                       "eggplant",
                                       "juice",
                                       "favorite",
                                       "phone",
                                       "wheel",
                                       "circle",
                                       "wife",
                                       "life",
                                       "laugh",
                                       "mathmatician",
                                       "physician",
                                       "aquarium",
                                       "auditorium",
                                       "game",
                                       "meat",
                                       "honey",
                                       "horse",
                                       "hour",
                                       "minute",
                                       "second",
                                       "hero",
                                       "molecule",
                                       "berserk",
                                       "bizarre",
                                       "adventure",
                                       "cucumber",
                                       "banana",
                                       "pizza",
                                       "vehicle",
                                       "plane",
                                       "aircraft",
                                       "air",
                                       "photosynthesis",
                                       "synthesizer",
                                       "instrument",
                                       "vacuum",
                                       "dust",
                                       "fire",
                                       "earth",
                                       "mud",
                                       "drug",
                                       "frag",
                                       "fracture",
                                       "fruit",
                                       "gasoline",
                                       "neighborhood",
                                       "water",
                                       "machine",
                                       "head",
                                       "dragon",
                                       "master",
                                       "slave",
                                       "weapon",
                                       "gun",
                                       "beast",
                                       "ant",
                                       "snow",
                                       "gas",
                                       "burgeoisie",
                                       "penicillin",
                                       "paper",
                                       "pear",
                                       "peach",
                                       "tree",
                                       "monad",
                                       "demon",
                                       "daemon",
                                       "baby",
                                       "casual",
                                       "mean",
                                       "villain",
                                       "fit",
                                       "fat",
                                       "fierce",
                                       "fury",
                                       "fear",
                                       "femur",
                                       "phallic",
                                       "dolphin",
                                       "monkey",
                                       "mason",
                                       "smith",
                                       "sloth",
                                       "dictator",
                                       "president",
                                       "author",
                                       "animal",
                                       "anime",
                                       "package",
                                       "pen",
                                       "pencil",
                                       "crayon",
                                       "ketchup",
                                       "mustard",
                                       "fearmonger",
                                       "capital",
                                       "mansion",
                                       "dust",
                                       "dusk",
                                       "diamond",
                                       "fitness",
                                       "surf",
                                       "mountain",
                                       "river",
                                       "coast",
                                       "fahrenheit",
                                       "kilometer",
                                       "mallet",
                                       "bigot",
                                       "gril",
                                       "measles",
                                       "girl",
                                       "boy",
                                       "text",
                                       "editor",
                                       "manuscript",
                                       "shift",
                                       "pellet",
                                       "vallet",
                                       "chess",
                                       "chest",
                                       "cockroach"};

void SaveLeaderBoard(const std::string &fileName,
                     const LeaderBoardData &leaderboard) {
  std::ofstream file(fileName, std::ios::binary);
  if (file) {
    size_t size = leaderboard.entries.size();
    file.write(reinterpret_cast<const char *>(&size), sizeof(size));
    for (const auto &entry : leaderboard.entries) {
      size_t nameLength = entry.first.size();
      file.write(reinterpret_cast<const char *>(&nameLength),
                 sizeof(nameLength));
      file.write(entry.first.data(), nameLength);
      file.write(reinterpret_cast<const char *>(&entry.second),
                 sizeof(entry.second));
    }
    file.close();
  } else {
    std::cerr << "Error: Could not save leaderboard.\n";
  }
}

void LoadLeaderboard(const std::string &filename,
                     LeaderBoardData &leaderboard) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Error: No leaderboard file found. Initializing empty "
                 "leaderboard.\n";
    return;
  }

  size_t size;
  file.read(reinterpret_cast<char *>(&size), sizeof(size));

  leaderboard.entries.clear();
  for (size_t i = 0; i < size; ++i) {
    size_t nameLength;
    file.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));

    std::string name(nameLength, '\0');
    file.read(&name[0], nameLength);

    int score;
    file.read(reinterpret_cast<char *>(&score), sizeof(score));

    leaderboard.entries.emplace_back(name, score);
  }

  file.close();
}

int main(void) {
  const float aspectRatio = 16.0f / 9.0f;
  const int SCREEN_WIDTH = 1200;
  const int SCREEN_HEIGHT = static_cast<int>(SCREEN_WIDTH / aspectRatio);

  SetTargetFPS(60);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Typex");

  int timer = 60;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, Dict.size() - 1);

  int rand = distr(gen);
  std::string selected_word = Dict[rand];

  int highlight_index = 0;
  int font_size = 120;
  int spacing = 10;

  Vector2 text_size = MeasureTextEx(GetFontDefault(), selected_word.c_str(),
                                    font_size, spacing);
  int start_x = SCREEN_WIDTH / 2 - text_size.x / 2;
  int start_y = SCREEN_HEIGHT / 2 - text_size.y / 2;

  std::vector<StateChar> state_of_char(selected_word.size(),
                                       StateChar::DEFAULT);
  GameScreen currentScreen = GameScreen::TITLE;
  LeaderBoardData ld;

  LoadLeaderboard("score", ld);

  bool is_correct = false;


  int score = 0;
  int lives = 3;

  InitAudioDevice();

  // load sounds
  Sound click = LoadSound("resources/click.wav");

  while (!WindowShouldClose()) {

    // if the correct char is pressed increment the highlight_index and set
    // is_correct to true if the false char is pressed increment the
    // highlight_index and set is_correct to false;

    switch (currentScreen) {
    case TITLE: {

      Vector2 text_width = MeasureTextEx(GetFontDefault(), "Typex", 100, 0);
      BeginDrawing();
      ClearBackground(SKYBLUE);
      DrawText("Typex", SCREEN_WIDTH / 2 - text_width.x / 2,
               SCREEN_HEIGHT / 2 - text_width.y / 2, 100, RAYWHITE);
      DrawText("Press ENTER to play", SCREEN_WIDTH / 2 - text_width.x / 2,
               SCREEN_HEIGHT / 2 - text_width.y / 2 - 20, 20, RAYWHITE);
      EndDrawing();

      if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = GameScreen::GAMEPLAY;
      }
    } break;
    case GAMEPLAY: {
      int key_pressed = GetCharPressed();

      if (key_pressed != 0) {

        if (key_pressed == selected_word[highlight_index]) {
          state_of_char[highlight_index] = StateChar::CORRECT;
        } else {
          state_of_char[highlight_index] = StateChar::FALSE;
          lives--;
        }
        PlaySound(click);
        highlight_index++;
      }

      BeginDrawing();
      ClearBackground(SKYBLUE);
      int x_offset = 0;
      for (size_t i = 0; i < selected_word.length(); i++) {
        std::string charStr(1, selected_word[i]);
        int char_width = MeasureText(charStr.c_str(), font_size);

        switch (state_of_char[i]) {
        case StateChar::DEFAULT:
          DrawText(charStr.c_str(), start_x + x_offset, start_y, font_size,
                   RAYWHITE);
          break;
        case StateChar::CORRECT:
          DrawText(charStr.c_str(), start_x + x_offset, start_y, font_size,
                   GREEN);
          break;
        case StateChar::FALSE:
          DrawText(charStr.c_str(), start_x + x_offset, start_y, font_size,
                   RED);
          break;
        default:
          std::cout << "Unknown state" << std::endl;
          break;
        }

        x_offset += char_width + spacing;
      }
      std::string score_str = std::to_string(score);
      const char *score_text = score_str.c_str();
      std::string lives_str = std::to_string(lives);
      const char *lives_text = lives_str.c_str();
      DrawText(score_text, 20, SCREEN_HEIGHT - 20, 20, RAYWHITE);
      DrawText(lives_text, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20, 20, RAYWHITE);
      EndDrawing();

      if (lives <= 0) {
        ld.entries.push_back(std::pair<std::string, int>("ACK", score));
        highlight_index = 0;
        int rand_index = distr(gen);
        selected_word = Dict[rand_index];
        state_of_char.assign(selected_word.size(), StateChar::DEFAULT);
        text_size = MeasureTextEx(GetFontDefault(), selected_word.c_str(),
                                  font_size, spacing);
        start_x = SCREEN_WIDTH / 2 - text_size.x / 2;
        start_y = SCREEN_HEIGHT / 2 - text_size.y / 2;
        currentScreen = GameScreen::LeaderBoard;
        lives = 3;
        score = 0;
        SaveLeaderBoard("score", ld);
      }

      if (highlight_index >= selected_word.size()) {
        highlight_index = 0;
        int rand_index = distr(gen);
        selected_word = Dict[rand_index];
        state_of_char.assign(selected_word.size(), StateChar::DEFAULT);
        text_size = MeasureTextEx(GetFontDefault(), selected_word.c_str(),
                                  font_size, spacing);
        start_x = SCREEN_WIDTH / 2 - text_size.x / 2;
        start_y = SCREEN_HEIGHT / 2 - text_size.y / 2;
        score++;
      }


    } break;
      case LeaderBoard: {

        int padding = 50;
        BeginDrawing();
        ClearBackground(SKYBLUE);

        std::sort(ld.entries.begin(), ld.entries.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b){
          return a.second > b.second;
        });

        int i = 0;
        for (auto entry : ld.entries) {
          if (i >= 10) break;
          Vector2 name_size = MeasureTextEx(GetFontDefault(), entry.first.c_str(), 50, 50);

          std::string score_str = std::to_string(entry.second);
          DrawText(entry.first.c_str(), SCREEN_WIDTH / 2 - name_size.x / 2, padding, 50, RAYWHITE);
          DrawText(score_str.c_str(), SCREEN_WIDTH / 2 - name_size.x / 2 + name_size.x, padding, 50, RAYWHITE);
          padding += 50;
          i++;
        }


        std::string enter_text = "Press Enter to play again";
        int enter_size = MeasureText(enter_text.c_str(), 50);
        std::cout << enter_size << std::endl;
        std::cout << SCREEN_WIDTH << std::endl;
        DrawText(enter_text.c_str(), (SCREEN_WIDTH / 2) - (enter_size / 2), 250, 50, RAYWHITE);


        EndDrawing();

      if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = GameScreen::GAMEPLAY;
      }
 

    } break;

    default:
      break;
    }
  }
  
  UnloadSound(click);
  CloseAudioDevice();

  CloseWindow();
  return 0;
}
