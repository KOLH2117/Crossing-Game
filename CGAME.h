﻿#ifndef CGAME_H
#define CGAME_H

#include "CCenterMenu.h"
#include "CScoreBoard.h"
#include "CAlienShip.h"
#include "CPEOPLE.h"
#include "CLANE.h"

class CGAME {
private:
	Console* mConsole;

	// Game nenu
	CCenterMenu mMainMenu;
	CCenterMenu mSettingMenu;

	// Score
	CScoreBoard* mScoreBoard;

	// Enemies obj
	vector<CLANE> mLaneOfEnemies;

	CAlienShip mAlienShip;

	// Player
	CPEOPLE mPeople;

	// Playing area setting
	CPOINT2D mTopLeft;
	CPOINT2D mBottomRight;

	// Render game thread
	thread mRenderGame;

	// Game state
	bool isPlaying; // True when player is playing
	bool isPause; // True when player pause game
	bool isPlayed; // True when player make new game

	int mLevel;
	int mMaxEnemies;
	int mMinEnemies;

	// Default game setting 
	vector<pair<float, ENEMY>> mLanes = {
		{0.0f,ENEMY::NO_ENEMY},
		{-1.0f,ENEMY::CDOG},
		{1.0f,ENEMY::CBIRD},
		{-1.0f,ENEMY::CDOG},
		{1.0f,ENEMY::CBIRD},
		{0.0f,ENEMY::NO_ENEMY},
		{-1.0f,ENEMY::CCAR},
		{1.0f,ENEMY::CTRUCK},
		{-1.0f,ENEMY::CCAR},
		{1.0f,ENEMY::CTRUCK},
		{-1.0f,ENEMY::CCAR},
		{0.0f,ENEMY::NO_ENEMY},
	};
private:	
	CGAME(); // Chuẩn bị dữ liệu cho tất cả các đối tượng
	CGAME(const CGAME&);
	CGAME& operator=(const CGAME&);

	// Post process
	void setSettingMenu();
	void setMainMenu();
	void setPlayingArea(float = 1.0f, float = 1.0f);
	void setScoreBoard();
	void setObjects();
	void setPeople();
	void setAlienShip();

	// Getter
	CPEOPLE getPeople() const; // Lấy thông tin người
	CLANE getEnemyLane(); // Lấy danh sách các đồ vật trên 1 lane

	// Draw part
	void drawPlayingArea(); // Vẽ khu vực chơi
	void drawEnemies(); // Vẽ xe
	void drawGame(); // Thực hiện vẽ trò chơi ra màn hình sau khi có dữ liệu

	// Game flow
	void renderWhenPlayerDie();
	void renderGameThread(KEY*);
	void resetGame(); // Thực hiện thiết lập lại toàn bộ dữ liệu như lúc đầu
	void nextLevel();
	void playGame(); // Trong qua trinh choi
	

	// Save and load game
	void loadGame(); // Thực hiện tải lại trò chơi đã lưu
	void saveGame(); // Thực hiện lưu lại dữ liệu trò chơi

	void pauseGame(); // Tạm dừng Thread
	void resumeGame(); // Quay lai Thread
	void exitGame(); // Thực hiện thoát Thread

	void updatePosPeople(KEY); // Thực hiện điều khiển di chuyển của CPEOPLE
	void updatePosEnemies(); // Thực hiện cho tất cả các xe va thú di chuyển

	void StartGame(); // Thực hiện bắt đầu vào trò chơi
public:
	static CGAME* getGame();
	~CGAME(); // Hủy tài nguyên đã cấp phát
	void Run();
};

#endif // CGAME_H
