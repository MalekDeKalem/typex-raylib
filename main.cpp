#include <iostream>
#include <stdio.h>      /* printf */
#include <math.h>       /* ceil */
#include <string>
#include <random>
#include "raylib.h"


	// g++ -I/usr/local/include -o main main.cpp -L/usr/local/lib -lraylib



	// What it should be int the game?
	/*
	    a sound effect when a word is correctly typed (positive) 
	    a sound effect when a typo is made (negative)
	    a sound effect when the word has not been typed before the timer (negative)
	    player has three lives when live count is zero than it is game over
	    lives are lost when word is not typed timely or they are lost when there is a typo 
	    the timer will get faster progressively the more words you type correctly the faster the timer gets 
	    each letter of a word is highlighted as you type them when you make a typo it will be shown via an animation 
	    when you lose a life it also will be shown via an animation
	    the background will change the further you go
	    timer will be represented as a bar that decreased as time goes on but will be restarted when a word is correctly typed
	    words to type will be randomly selected
	*/


typedef enum StateChar {
	DEFAULT,
	CORRECT,
	FALSE,
} StateChar;


const std::vector<std::string> Dict = {
	"interesting", "mother", "father", "gallant", "robber", "paleontologist", 
	"dinosaur", "mammoth", "merit", "accomplishment", "car", "house", "driver",
	"maggot", "dog", "god", "chicken", "beef", "limousine", "puzzle", "doodle",
	"corn", "book", "tiger", "cat", "computer", "system", "failure", "success",
	"browser", "eye", "egg", "cheese", "potato", "tomato", "bacon", "onion", 
	"garlic", "eggplant", "juice", "favorite", "phone", "wheel", "circle",
	"wife", "life", "laugh", "mathmatician", "physician", "aquarium", "auditorium",
	"game", "meat", "honey", "horse", "hour", "minute", "second", "hero", "molecule",
	"berserk", "bizarre", "adventure", "cucumber", "banana", "pizza", "vehicle", "plane",
	"aircraft", "air", "photosythesis", "synthesizer", "instrument", "vacuum", "dust",
	"fire", "earth", "mud", "drug", "frag", "fracture", "fruit", "gasoline", "neighborhood",
	"water", "machine", "head", "dragon", "master", "slave", "weapon", "gun", "beast", "ant", 
	"snow", "gas", "burgeoisie", "penicillin", "paper", "pear", "peach", "tree", "monad", "demon",
	"daemon", "baby", "casual", "mean", "villain", "fit", "fat", "fierce", "fury", "fear", "femur",
	"phallic", "dolphin", "monkey", "mason", "smith", "sloth", "dictator", "president", "author",
	"animal", "anime", "package", "pen", "pencil", "crayon", "ketchup", "mustard", "fearmonger",
	"capital", "mansion", "dust", "dusk", "diamond", "fitness", "surf", "mountain", "river", "coast"
};

int main(void) 
{
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 450;

	SetTargetFPS(60);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Typex");

	int timer = 60; 

	std::random_device rd;
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distr(0, Dict.size()-1);

	int rand = distr(gen);
	std::string selected_word = Dict[rand];

	int highlight_index = 0;
	int font_size = 120;
	int spacing = 10;

	Vector2 text_size = MeasureTextEx(GetFontDefault(), selected_word.c_str(), font_size, spacing);
	int start_x = SCREEN_WIDTH / 2 - text_size.x / 2;
	int start_y = SCREEN_HEIGHT / 2 - text_size.y / 2;

	std::vector<StateChar> state_of_char(selected_word.size(), StateChar::DEFAULT);


	bool is_correct = false;

	int score = 0;
	int lives = 3;

	while (!WindowShouldClose()) {

		// if the correct char is pressed increment the highlight_index and set is_correct to true
		// if the false char is pressed increment the highlight_index and set is_correct to false;
		
		int key_pressed = GetCharPressed();	

		if (key_pressed != 0) {

			if (key_pressed == selected_word[highlight_index]) {
				std::cout << "Pressed correct key" << std::endl;
				state_of_char[highlight_index] = StateChar::CORRECT;
			} else {
				std::cout << "Pressed incorrect key" << std::endl;
				state_of_char[highlight_index] = StateChar::FALSE;
				lives--;
			}

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
						DrawText(charStr.c_str(), start_x + x_offset, start_y, font_size, RAYWHITE);
						break;
					case StateChar::CORRECT:
						DrawText(charStr.c_str(), start_x + x_offset, start_y, font_size, GREEN);
						break;
					case StateChar::FALSE:
						DrawText(charStr.c_str(), start_x + x_offset, start_y, font_size, RED);
						break;
					default:
						std::cout << "Unknown state" << std::endl;
						break;
				}

				x_offset += char_width + spacing;
			}
			std::string score_str = std::to_string(score);
			const char* score_text = score_str.c_str();
			std::string lives_str = std::to_string(lives);
			const char* lives_text = lives_str.c_str();
			DrawText(score_text, 20, SCREEN_HEIGHT - 20, 20, RAYWHITE);
			DrawText(lives_text, SCREEN_WIDTH-20, SCREEN_HEIGHT - 20, 20, RAYWHITE);
		EndDrawing();



		if (highlight_index >= selected_word.size()) {
			highlight_index = 0;
			int rand_index = distr(gen);
			selected_word = Dict[rand_index];
			state_of_char.assign(selected_word.size(), StateChar::DEFAULT);
			text_size = MeasureTextEx(GetFontDefault(), selected_word.c_str(), font_size, spacing);
			start_x = SCREEN_WIDTH / 2 - text_size.x / 2;
			start_y = SCREEN_HEIGHT / 2 - text_size.y / 2;
			score++;
		}

		if (lives <= 0) break;



	}



	// Close the window and clean up resources
	CloseWindow();
	return 0;
}
