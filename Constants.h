#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <thread>
#include <fstream>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace std;

// Default setting for game
constexpr short WINDOW_BUFFER_HEIGHT = 52;
constexpr short WINDOW_BUFFER_WIDTH = 150;
constexpr short FONT_HEIGHT = 12;
constexpr short FONT_WIDTH = 12;
constexpr short MAX_LANE = 10;
constexpr int LANE_SIZE = 4;

// Position of top left corner of playing area
constexpr short TOP_LEFT_X = 0;
constexpr short TOP_LEFT_Y = 0;

// Scale of the playing area
constexpr float SCALE_X = 0.65;
constexpr float SCALE_Y = 1;

// Border char
constexpr char TOP_LEFT_CORNER = char(201);
constexpr char TOP_RIGHT_CORNER = char(187);
constexpr char BOTTOM_LEFT_CORNER = char(200);
constexpr char BOTTOM_RIGHT_CORNER = char(188);
constexpr char HORIZONTAL_OUTLINE = char(205);
constexpr char VERTICAL_OUTLINE = char(186);
constexpr char LANE_ROAD = char(196);

// Constant color for game
#define DEFAULT_COLOUR COLOUR::WHITE
#define PLAYING_AREA_COLOUR COLOUR::GREEN
#define LANE_COLOUR COLOUR::CYAN
#define PEOPLE_COLOUR COLOUR::RED
#define TRUCK_COLOUR COLOUR::PINK

enum class DIRECTION : char {
	UP = 'W',
	DOWN = 'S',
	LEFT = 'A',
	RIGHT = 'D'
};

enum class COLOUR : int {
	GREEN = 10,
	CYAN = 11,
	RED = 12,
	PINK = 13,
	WHITE = 15,
};

enum class Level {
	EASY,
	MEDIUM,
	HARD
};

#endif // CONSTANTS_H
