﻿#include "CGAME.h"

CGAME::CGAME()  {
	isPause = false;
	isPlaying = false;
	isPlayed = false;
	mConsole = Console::getConsole(WINDOW_BUFFER_WIDTH, WINDOW_BUFFER_HEIGHT, FONT_WIDTH, FONT_HEIGHT);
	
	mLevel = 1;
	mMinEnemies = 1;
	mMaxEnemies = mMinEnemies;

	// Set obj
	setMainMenu();
	setSettingMenu();
	setPlayingArea(SCALE_X, SCALE_Y);
	setScoreBoard();
	setObjects();
	setPeople();
}

CGAME* CGAME::getGame() {
	static CGAME mInstance;
	return &mInstance;
}

CGAME::~CGAME() {}

void CGAME::setSettingMenu() {
	Texture gameTitle = \
		" ######  ######## ######## ######## #### ##    ##  ######\n"
		"##    ## ##          ##       ##     ##  ###   ## ##    ##\n"
		"##       ##          ##       ##     ##  ####  ## ##\n"
		" ######  ######      ##       ##     ##  ## ## ## ##   ####\n"
		"      ## ##          ##       ##     ##  ##  #### ##    ##\n"
		"##    ## ##          ##       ##     ##  ##   ### ##    ##\n"
		" ######  ########    ##       ##    #### ##    ##  ######";

	mSettingMenu.setMenuTitle(gameTitle, COLOUR::PINK);
	mSettingMenu.setMarginTop(4);
	mSettingMenu.addOption("Audio");
	mSettingMenu.addOption("Back to main menu", bind(&CCenterMenu::QuitMenu, &mSettingMenu));
}

void CGAME::setMainMenu() {
	Texture gameTitle = \
		" ######  ########   #######   ######   ######  #### ##    ##  ######      ######      ###    ##     ## ########\n"
		"##    ## ##     ## ##     ## ##    ## ##    ##  ##  ###   ## ##    ##    ##    ##    ## ##   ###   ### ##       \n"
		"##       ##     ## ##     ## ##       ##        ##  ####  ## ##          ##         ##   ##  #### #### ##       \n"
		"##       ########  ##     ##  ######   ######   ##  ## ## ## ##   ####   ##   #### ##     ## ## ### ## ######   \n"
		"##       ##   ##   ##     ##       ##       ##  ##  ##  #### ##    ##    ##    ##  ######### ##     ## ##       \n"
		"##    ## ##    ##  ##     ## ##    ## ##    ##  ##  ##   ### ##    ##    ##    ##  ##     ## ##     ## ##       \n"
		" ######  ##     ##  #######   ######   ######  #### ##    ##  ######      ######   ##     ## ##     ## ######## \n";

	mMainMenu.setMenuTitle(gameTitle);
	mMainMenu.setMarginTop(4);
	mMainMenu.addOption("New Game", bind(&CGAME::StartGame, this));
	mMainMenu.addOption("Load Game", bind(&CGAME::loadGame, this));
	mMainMenu.addOption("Setting", bind(&CCenterMenu::Run, &mSettingMenu, ref(*mConsole)));
	mMainMenu.addOption("Quit", bind(&CCenterMenu::QuitMenu, &mMainMenu));
}

void CGAME::setPlayingArea(float scaleX, float scaleY) {
	// Set the size of the playing area
	// 0,0 is the coord of top left
	int playingAreaWidth = mConsole->Width() * scaleX;
	int playingAreaHeight = mConsole->Height() * scaleY;

	// Set the top left point of the playing area
	mTopLeft = CPOINT2D(TOP_LEFT_X, TOP_LEFT_Y);
	mBottomRight = CPOINT2D(TOP_LEFT_X + playingAreaWidth - 1, TOP_LEFT_Y + playingAreaHeight - 1);
}

void CGAME::setScoreBoard()
{
	CPOINT2D topLeftCoord = CPOINT2D(mBottomRight.getX() + 1, mTopLeft.getY());
	int playingAreaWidth = mBottomRight.getX() - mTopLeft.getX() + 1;

	int scoreBoardWidth = mConsole->Width() - playingAreaWidth;
	int scoreBoarHeight = mConsole->Height() * SCALE_Y;

	mScoreBoard = CScoreBoard::getScoreBoard();
	mScoreBoard->setPosTopLeftCorner(topLeftCoord);
	mScoreBoard->resize(scoreBoardWidth, scoreBoarHeight);
}

void CGAME::setObjects() {
	int left = mTopLeft.getX();
	int right = mBottomRight.getX();
	int y = mTopLeft.getY() + 1;

	for (const auto& LaneInfo : mLanes) {
		int randomQty = RandomInt(mMaxEnemies, mMinEnemies);
		CLANE mLane(left, right);
		mLane.setY(y);
		mLane.setSpeed(LaneInfo.first);
		mLane.generateObjectsOnLane(LaneInfo.second, randomQty);
		switch (LaneInfo.second)		
		{
		case ENEMY::CCAR:
		case ENEMY::CTRUCK:
			mLane.enableTrafficLight();
		default:
			break;
		}

		if(mLane.size() != 0)
			mLaneOfEnemies.push_back(mLane);
		y += LANE_SIZE;
	}
}

void CGAME::setPeople() {
	// Set the position of people
	// Generate randomly X by multiplying a floating number between 0.2 to 0.8
	float randomPercent = (float) RandomInt(8, 2) / 10.0f;
	int x = mBottomRight.getX() - mTopLeft.getX();
	int y = mBottomRight.getY() - mPeople.Height();

	// X is random, Y must at the end of the last square
	x = x * randomPercent;
	mPeople = CPEOPLE(x, y, mTopLeft, mBottomRight);
}

void CGAME::setAlienShip()
{
	int alienPosX = RandomInt(mBottomRight.getX(), mTopLeft.getY() + 1);
	int alienPosY = RandomInt(mTopLeft.getY(), mTopLeft.getY() - 10);

	mAlienShip.setXY(alienPosX, alienPosY);
}

CPEOPLE CGAME::getPeople() const {
	return mPeople;
}

CLANE CGAME::getEnemyLane()
{
	for (const auto& Lane : mLaneOfEnemies) {
		if (Lane.getY() == mPeople.getY())
			return Lane;
	}
	return CLANE();
}

void CGAME::drawPlayingArea() {
	// Draw the border line
	mConsole->DrawBorder(mTopLeft, mBottomRight, PLAYING_AREA_COLOUR);
}

void  CGAME::drawEnemies() {
	for (auto& Lane : mLaneOfEnemies) {
		Lane.drawObjectsOnLane(*mConsole);
	}
}

void CGAME::drawGame() {
	// Draw border
	drawPlayingArea();
	mScoreBoard->drawScoreBoard(*mConsole);
	// Draw people
	mPeople.drawPeople(*mConsole);

	// Draw object
	drawEnemies();
}

void CGAME::renderWhenPlayerDie() {
	while (!mAlienShip.isCapturePeople()) {
		mConsole->ClearScreen();

		drawGame();
		mAlienShip.drawToConsole(*mConsole, mTopLeft.getX(), mBottomRight.getY());
		mAlienShip.updatePos();
		
		mConsole->Render();

		Sleep(30);
	}
}

void CGAME::renderGameThread(KEY* MOVING) {
	while (isPlaying) {
		if (isPause)
			continue;

		// Clear the old screen
		mConsole->ClearScreen();

		if (!mPeople.isDead()) {
			updatePosPeople(*MOVING);
			updatePosEnemies();

			if (mPeople.isImpact(getEnemyLane())) {
			/*	mAlienShip.setPeoplePos(getPeople().getX(), getPeople().getY());

				thread t = thread(&CGAME::renderWhenPlayerDie, this);
				t.join();

				mPeople.Dead();*/
			}
		}
		*MOVING = KEY::SPACE;

		drawGame();

		if (mPeople.isFinish()) {
			nextLevel();
		}

		// Render out console
		mConsole->Render();
		Sleep(30);
	}
}

void CGAME::resetGame() {
	mLaneOfEnemies.clear();

	setObjects();
	setPeople();
	setAlienShip();
}

void CGAME::nextLevel() {
	if (mLevel == 3){
		mLevel = 1;
		mMinEnemies = 1;
		mMaxEnemies = mMinEnemies;
	}
	else {
		mLevel++;
		mMinEnemies++;
		mMaxEnemies = mMinEnemies + 1;
	}
	
	resetGame();
}

void CGAME::playGame() {
	isPlaying = true;
	isPause = false;
	isPlayed = true;

	KEY MOVING = KEY::SPACE;
	KEY temp;

	mRenderGame = thread(&CGAME::renderGameThread, this, &MOVING);

	while (isPlaying) {
		temp = (KEY)toupper(_getch());
		if (!getPeople().isDead()) {
			if (temp == KEY::ESC) {
				exitGame();
				return;
			}
			else if (temp == KEY::PAUSE_GAME) {
				pauseGame();
			}
			else {
				resumeGame();
				MOVING = (KEY)temp;
			}
		}
		else {
			if (temp == KEY::YES) {
				resetGame();
			}
			else {
				exitGame();
				return;
			}
		}
	}
}

void CGAME::StartGame() {
	if (isPlayed) {
		int result = MessageBox(
			NULL,
			L"You will lost all the unsaved process.\nContine to create new game???", // Text in msg box
			L"New Game", // Msg box title
			MB_YESNO | MB_ICONWARNING | MB_TOPMOST | MB_APPLMODAL // Mode ask yes no
		);

		if (result == IDYES) {
			resetGame();
			playGame();
		}
	}else {
		if (mMainMenu.isHasOption("Continue") == false)
			mMainMenu.insertOption(1, "Continue", bind(&CGAME::playGame, this));
		playGame();
	}
}


void CGAME::loadGame() {}

void CGAME::saveGame() {}

void CGAME::pauseGame() {
	isPause = true;
}

void CGAME::resumeGame() {
	isPause = false;
}

void CGAME::exitGame() {
	if (isPlaying) {
		isPlaying = false;
		mRenderGame.join();
	}
}

void CGAME::updatePosPeople(KEY direction) {
	mPeople.Move(direction,LANE_SIZE);
}

void CGAME::updatePosEnemies() {
	for (auto& Lane : mLaneOfEnemies) {
		Lane.updateObjectsOnLane();
	}
}

void CGAME::Run() {
	mMainMenu.Run(*mConsole);
}
