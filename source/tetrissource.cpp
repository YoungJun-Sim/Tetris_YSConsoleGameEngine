#ifdef _DEBUG
#pragma comment(lib, "YSConsoleGameEngineLib.lib")
#else
#pragma comment(lib, "YSConsoleGameEngineLib.lib")
#endif

#include <iostream>
#include "framework.h" 
#include <random>
#include <time.h>
#include <string>
#include <algorithm>

#define FRAME 1000/60
#define TICK 10
#define LEVEL 50

class Game {
public:
	Game() {
		start_S = clock(), start_T = clock(), start_F = clock(), start_mino = clock(), start_K = clock();
		s = std::make_unique<Screen>();
		s->setTitle(mTitle);
		s->disableResize();
		s->setFont(4, 4, L"Cascadia Code");
		s->setWindowSize(340 * 2, 190);
		s->setBackground(L' ');
		s->resetBuffer();

		font->LoadF("resources/YSfont.YSfont");
		font->setAllColor(15);

		sq->setSprite({
			L"████████████████" ,
			L"██    ████████  " ,
			L"██  ██████████  " ,
			L"██████████████  ",
			L"██████████████  ",
			L"██████████████  ",
			L"██████████████  ",
			L"                ", });
		sq->setAllColor(RED);

		sqblank->setSprite({
			L"█▀▀ ▀▀ ▀▀ ▀▀ ▀▀█",
			L"▄              ▄",
			L"▀              ▀",
			L"▄              ▄",
			L"▀              ▀",
			L"▄              ▄",
			L"▀              ▀",
			L"█▄▄ ▄▄ ▄▄ ▄▄ ▄▄█",
			});
		sqblank->setAllColor(RED);

		wall->setSprite({
			L"█▀▀▀▀▀▀▀▀▀▀▀▀▀▀█",
			L"█ ████████████ █",
			L"█ ████████████ █",
			L"█ ████████████ █",
			L"█ ████████████ █",
			L"█ ████████████ █",
			L"█ ████████████ █",
			L"█▄▄▄▄▄▄▄▄▄▄▄▄▄▄█",
			});
		wall->setAllColor(WHITE);

		delsq->setSprite({
			L"█▀▀▀▀▀▀▀▀▀▀▀▀▀▀█",
			L"█              █",
			L"█  ██████████  █",
			L"█  ██████████  █",
			L"█  ██████████  █",
			L"█  ██████████  █",
			L"█              █",
			L"█▄▄▄▄▄▄▄▄▄▄▄▄▄▄█",
			});
		delsq->setAllColor(GRAY);

		empty->setSprite({
			L"................",
			L"................",
			L"................",
			L"................",
			L"................",
			L"................",
			L"................",
			L"................", });
		empty->setAllColor(BLACK);
	}

	~Game() {};

	void run() {
		wallReset();
		minoPosX = 4, minoPosY = 0;
		static std::random_device rd;
		static std::mt19937 gen(rd());

		std::shuffle(std::begin(minoarray), std::end(minoarray), gen);
		std::shuffle(std::begin(minoarray2), std::end(minoarray2), gen);

		minoTrype = minoarray[0];
		getMino(minoTrype);
		bool isRun = true;

		while (isRun) {
			finish_time = clock();
			timeTick = (double)(finish_time - start_T);
			timeKey = (double)(finish_time - start_K);

			if (timeTick > TICK) {
				level = 1 + (double)(score + 1) / 5000;
				if (newMino == 1) {
					minoPosX = 4, minoPosY = 0;
					if (isDeadZone() == 0) {
						isRun = false;
						minoPosX = -10, minoPosY = -10;
					}
					endofdrop = 0;
					minoIndex = (minoIndex + 1) % 7;
					if (minoIndex == 0) {
						for (int i = 0; i < 7; i++) {
							minoarray[i] = minoarray2[i];
						}
						std::shuffle(std::begin(minoarray2), std::end(minoarray2), gen);
					}
					minoTrype = minoarray[minoIndex];
					getMino(minoTrype);
					newMino--;
				}

				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
					if (!isKeyPressedL) {
						isKeyPressedL = true;
						start_K = clock();
					}
					else {
						if (timeKey = (double)(finish_time - start_K) > 300) if (moveleft() == 1) {
							if (endofdrop > 0) endofdrop -= 5;
							minoPosX--;
						}
					}
				}
				else {
					if (isKeyPressedL == true) {
						if (endofdrop > 0) endofdrop -= 5;
						if (moveleft() == 1) minoPosX--;
					}
					isKeyPressedL = false;
				}

				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
					if (!isKeyPressedR) {
						isKeyPressedR = true;
						start_K = clock();
					}
					else {
						if (timeKey = (double)(finish_time - start_K) > 300) if (moveright() == 1) {
							if (endofdrop > 0) endofdrop -= 5;
							minoPosX++;
						}
					}
				}
				else {
					if (isKeyPressedR == true) {
						if (endofdrop > 0) endofdrop -= 5;
						if (moveright() == 1) minoPosX++;
					}
					isKeyPressedR = false;
				}

				if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
					if (!isKeyPressedD) {
						isKeyPressedD = true;
						start_K = clock();
					}
					else {
						if (timeKey = (double)(finish_time - start_K) > 200) if (movedown() == 1) minoPosY++;
					}
				}
				else {
					if (isKeyPressedD == true) if (movedown() == 1) { minoPosY++; if (endofdrop > 0) endofdrop -= 5; }
					isKeyPressedD = false;
				}

				if (GetAsyncKeyState(VK_UP) & 0x8000) {
					if (!isKeyPressedU) {
						isKeyPressedU = true;
						endofdrop = 10;
						start_K = clock();
					}
				}
				else {
					if (isKeyPressedU == true) while (movedown() == 1) { minoPosY++; endofdrop = 25; };
					isKeyPressedU = false;
				}

				if (GetAsyncKeyState('Z') & 0x8000) {
					if (!isKeyPressedZ) {
						isKeyPressedZ = true;
						start_K = clock();
					}
				}
				else {
					if (isKeyPressedZ == true) {
						leftKick(playMino);
						if (endofdrop > 0) endofdrop -= 5;
					}
					isKeyPressedZ = false;
				}

				if (GetAsyncKeyState('X') & 0x8000) {
					if (!isKeyPressedX) {
						isKeyPressedX = true;
						start_K = clock();
					}
				}
				else {
					if (isKeyPressedX == true) {
						rightKick(playMino);
						if (endofdrop > 0) endofdrop -= 5;
					}
					isKeyPressedX = false;
				}

				if (GetAsyncKeyState('Q') & 0x8000) {
					if (!isKeyPressedQ) {
						isKeyPressedQ = true;
						start_K = clock();
					}
					else if (timeKey = (double)(finish_time - start_K) > 9000) {
						if (views == false) {
							views = true; score = 100000;
						}
					}
				}
				else {
					if (isKeyPressedQ == true) {
					}
					isKeyPressedQ = false;
				}

				if (GetAsyncKeyState('C') & 0x8000) {
					if (!isKeyPressedC) {
						isKeyPressedC = true;
						start_K = clock();
					}
				}
				else {
					if (isKeyPressedC == true) {
						if (isChanged == false) {
							int tempindex = minoarray[minoIndex];
							if (minoholdIndex == -1) {
								minoholdIndex = minoTrype;
								newMino++;
							}
							else {
								int temp = minoTrype;
								minoTrype = minoholdIndex;
								minoholdIndex = temp;
								minoPosX = 4;
								minoPosY = 0;
								getMino(minoTrype);
							}
							isChanged = true;
						}
					}
					isKeyPressedC = false;
				}

				if (droptime > LEVEL / level && movedown() == 1) minoPosY++, droptime > level, endofdrop = 0, droptime = 0;
				else if (droptime > LEVEL / level && movedown() == 0) { endofdrop++; }
				else if (level > 50) droptime += 2;
				else droptime++;

				if (endofdrop > 20 && movedown() == 0) {
					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 4; j++) {
							if (playMino[i][j] != 0 && map[(minoPosX + i)][(minoPosY + j)] == 0) {
								map[(minoPosX + i)][(minoPosY + j)] = playMino[i][j];
							}
						}
					}
					checkClear();
					newMino++;
					endofdrop = 0;
					isChanged = false;
				}

				tick++;
				start_T = clock();
			}

			finish_time = clock();
			timeFrame = (double)(finish_time - start_F);

			if (1 == 1) {
				bool pointerMinoarray2 = false;
				isTspin = false;
				s->resetBuffer();
				for (int i = 0; i < 12; i++) {
					for (int j = 0; j < 21; j++) {
						if (map[i][j] == 99) {
							wall->setX(startX + (i * 16));
							wall->setY(startY + (j * 8));
							wall->Draw(s.get());
						}
						else if (map[i][j] > 0) {
							sq->setX(startX + (i * 16));
							sq->setY(startY + (j * 8));
							sq->setAllColor(map[i][j]);
							sq->Draw(s.get());
						}
						else if (map[i][j] == 0) {
							empty->setX(startX + (i * 16));
							empty->setY(startY + (j * 8));
							empty->Draw(s.get());
						}
					}
				}

				int holdBaseX = startX - (6 * 16);
				int holdBaseY = startY;
				for (int i = 0; i < 7; i++) {
					for (int j = 0; j < 7; j++) {
						int currentX = holdBaseX + (i * 16);
						int currentY = holdBaseY + (j * 8);

						if (i == 0 || i == 6 || j == 0 || j == 6) {
							wall->setX(currentX);
							wall->setY(currentY);
							wall->Draw(s.get());
						}
						else {
							empty->setX(currentX);
							empty->setY(currentY);
							empty->Draw(s.get());
						}
					}
				}

				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						if (playMino[i][j] != 0) {
							sq->setAllColor(playMino[i][j]);
							sq->setX(((minoPosX + i) * 16) + startX);
							sq->setY(((minoPosY + j) * 8) + startY);
							sq->Draw(s.get());
						}
					}
				}

				short playMinoCopy[4][4] = { 0 };
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) playMinoCopy[i][j] = playMino[i][j];
				}

				int minoPosYCopy = minoPosY;
				while (movedownCopy(playMinoCopy, minoPosYCopy) == 1) {
					minoPosYCopy++;
				}
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						if (playMinoCopy[i][j] != 0) {
							sqblank->setAllColor(playMinoCopy[i][j]);
							sqblank->setX(((minoPosX + i) * 16) + startX);
							sqblank->setY(((minoPosYCopy + j) * 8) + startY);
							sqblank->Draw(s.get());
						}
					}
				}

				if (minoholdIndex != -1) {
					getSpolierMino(minoholdIndex);
					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 4; j++) {
							if (spoilerMino[i][j] != 0) {
								sq->setAllColor(spoilerMino[i][j]);
								sq->setX(((i) * 16) + startX - 70);
								sq->setY(((2 + j) * 8) + startY);
								sq->Draw(s.get());
							}
						}
					}
				}
				font->inputStr(s.get(), ((1) * 16) + startX - 75, (((2 + 1) * 8) + startY + 12), "HOLD");
				font->inputStr(s.get(), ((1) * 16) + startX + 200, (((2 + 1) * 8) + startY + 12), "NEXT");
				int nextBaseX = startX + (11 * 16);
				int nextBaseY = startY;
				int nextWidth = 8;
				int nextHeight = 21;

				for (int i = 0; i < nextWidth; i++) {
					for (int j = 0; j < nextHeight; j++) {
						int cx = nextBaseX + (i * 16);
						int cy = nextBaseY + (j * 8);

						if (i == 0 || i == nextWidth - 1 || j == 0 || j == nextHeight - 1) {
							wall->setX(cx);
							wall->setY(cy);
							wall->Draw(s.get());
						}
						else if (j == 6) {
							wall->setX(cx);
							wall->setY(cy);
							wall->Draw(s.get());
						}
						else {
							empty->setX(cx);
							empty->setY(cy);
							empty->Draw(s.get());
						}
					}
				}

				for (int k = 0; k < 5; k++) {
					int targetIndex = minoIndex + 1 + k;
					int nextMinoType = 0;

					if (targetIndex < 7) {
						nextMinoType = minoarray[targetIndex];
					}
					else {
						nextMinoType = minoarray2[targetIndex - 7];
					}

					getSpolierMino(nextMinoType);

					int yOffset = 2 + (k * 3);
					if (k > 0) yOffset += 3;

					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 4; j++) {
							if (spoilerMino[i][j] != 0) {
								sq->setAllColor(spoilerMino[i][j]);
								sq->setX(((13 + i) * 16) + startX);
								sq->setY(((yOffset + j) * 8) + startY);
								sq->Draw(s.get());
							}
						}
					}
				}

				font->inputStr(s.get(), ((1) * 16) + startX + 300, (((2 + 1) * 8) + startY + 12), "Score : " + std::to_string(score));
				font->inputStr(s.get(), ((1) * 16) + startX + 300, (((2 + 1) * 8) + startY + 18), "Level : " + std::to_string((int)(level)));

				if (views == false) {
					font->inputStr(s.get(), ((1) * 16) + startX - 200, (((2 + 1) * 8) + startY + 40), "Left, Right : Move");
					font->inputStr(s.get(), ((1) * 16) + startX - 200, (((2 + 1) * 8) + startY + 50), "Down Key : Fast drop");
					font->inputStr(s.get(), ((1) * 16) + startX - 200, (((2 + 1) * 8) + startY + 60), "Up Key : Quick drop");
					font->inputStr(s.get(), ((1) * 16) + startX - 200, (((2 + 1) * 8) + startY + 70), "Z, X : Rotation drop");
					font->inputStr(s.get(), ((1) * 16) + startX - 200, (((2 + 1) * 8) + startY + 80), "C Key : Hold");
				}
				else {
					font->setAllColor(RED);
					font->inputStr(s.get(), ((1) * 16) + startX + 300, (((2 + 1) * 8) + startY + 24), "Hard Mode On...");
					font->setAllColor(WHITE);
				}

				s->Display();
				start_F = clock();
				framecount++;
			}

			finish_time = clock();
			double duration = (double)(finish_time - start_S) / CLOCKS_PER_SEC;
			if (duration > 1) {
				s->setTitle(L"");
				s->setTitle(mTitle + L" | FPS : " + std::to_wstring(framecount) + L"  Tick : " + std::to_wstring(tick));
				tick = 0;
				framecount = 0;
				start_S = clock();
			}
		}
		isRun = true;
		bool isReset = false;
		int tempX = 100;
		int tempY = 10;
		while (isRun) {
			if (isReset == false) {
				delsq->setAllColor(DARK_GRAY);
				for (int j = 20; j >= 0; j--) {
					for (int i = 0; i < 12; i++) {
						Sleep(10);
						if (map[i][j] == 99) {
							wall->setX(startX + (i * 16));
							wall->setY(startY + (j * 8));
							wall->Draw(s.get());
						}
						else if (map[i][j] > 0) {
							empty->setX(startX + (i * 16));
							empty->setY(startY + (j * 8));
							empty->Draw(s.get());
							s->Display();
							delsq->setX(startX + (i * 16));
							delsq->setY(startY + (j * 8));
							delsq->Draw(s.get());
							s->Display();
						}
						else if (map[i][j] == 0) {
							empty->setX(startX + (i * 16));
							empty->setY(startY + (j * 8));
							empty->Draw(s.get());
						}
					}
				}
			}

			if (isReset == false) {
				wall->setAllColor(GRAY);
				for (int i = 0; i < 12; i++) {
					for (int j = 0; j < 10; j++) {
						if (i == 0 || i == 11) {
							wall->setX(((8 + i) * 16) + tempX);
							wall->setY(((5 + j) * 8) + tempY);
							wall->Draw(s.get());
						}
						else if (j == 0 || j == 9) {
							wall->setX(((8 + i) * 16) + tempX);
							wall->setY(((5 + j) * 8) + tempY);
							wall->Draw(s.get());
						}
						else {
							empty->setX(((8 + i) * 16) + tempX);
							empty->setY(((5 + j) * 8) + tempY);
							empty->Draw(s.get());
						}
						s->Display();
					}
				}
				isReset = true;
			}

			font->inputStr(s.get(), 280, 70, "Game Over");
			font->inputStr(s.get(), 270, 90, "Your Score");
			font->inputStr(s.get(), 270, 100, std::to_string(score));
			s->Display();

			if (GetAsyncKeyState('Z') & 0x8000) {
				if (!isKeyPressedZ) {
					isKeyPressedZ = true;
					start_K = clock();
				}
			}
			else {
				if (isKeyPressedZ == true) {
					isRun = false;
				}
				isKeyPressedZ = false;
			}
		}
	}

	bool isAllC() {
		for (int i = 1; i <= 10; i++) {
			for (int j = 0; j < 21; j++) {
				if (map[i][j] > 0) return false;
			}
		}
		return true;
	}

	void checkClear() {
		int deltime = 0;

		for (int y = 19; y >= 0; y--) {
			bool isFull = true;
			for (int x = 1; x <= 10; x++) {
				if (map[x][y] == 0) {
					isFull = false;
					break;
				}
			}
			if (isFull) {
				deltime++;
				for (int ky = y; ky > 0; ky--) {
					for (int kx = 1; kx <= 10; kx++) {
						map[kx][ky] = map[kx][ky - 1];
					}
				}
				for (int kx = 1; kx <= 10; kx++) {
					map[kx][0] = 0;
				}
				y++;
			}
		}

		if (deltime == 0) return;

		int multiplier = (LEVEL / level);
		if (multiplier < 1) multiplier = 1;

		int currentScore = 0;
		bool isDifficult = false;

		if (isTspin) {
			isDifficult = true;
			switch (deltime) {
			case 1: currentScore = 800; break;
			case 2: currentScore = 1200; break;
			case 3: currentScore = 1600; break;
			default: currentScore = 100; break;
			}
		}
		else {
			switch (deltime) {
			case 1: currentScore = 100; isDifficult = false; break;
			case 2: currentScore = 300; isDifficult = false; break;
			case 3: currentScore = 500; isDifficult = false; break;
			case 4: currentScore = 800; isDifficult = true;  break;
			}
		}

		if (isBacktoBack && isDifficult) {
			currentScore = currentScore + (currentScore / 2);
		}

		if (isAllC()) {
			switch (deltime) {
			case 1: currentScore += 800; break;
			case 2: currentScore += 1200; break;
			case 3: currentScore += 1800; break;
			case 4: currentScore += 2000; break;
			}
		}

		score += currentScore * multiplier;

		if (isDifficult) {
			isBacktoBack = true;
		}
		else {
			isBacktoBack = false;
		}
		isTspin = false;
	}

	void changeMap() {
		for (int i = 1; i < 11; i++) {
			for (int j = 0; j < 20; j++) {
				map[i][j] = tempmap[i][j];
			}
		}
	}

	void delLine(int line) {
		for (int i = 1; i < 11; i++) {
		}
	}

	void playerset(short mino[3][3]) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				playMino[i][j] = 0;
			}
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (mino[i][j] > 0) {
					playMino[j][i] = mino[i][j];
				}
			}
		}
	}

	void playerset(short* mino) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				playMino[i][j] = 0;
			}
		}
		for (int i = 0; i < 8; i++) {
			if (i < 4) playMino[(i % 4)][1] = mino[i];
			else       playMino[(i % 4)][2] = mino[i];
		}
	}

	void spoilerset(short mino[3][3]) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				spoilerMino[i][j] = 0;
			}
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (mino[i][j] > 0) {
					spoilerMino[j][i] = mino[i][j];
				}
			}
		}
	}

	void spoilerset(short* mino) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				spoilerMino[i][j] = 0;
			}
		}
		for (int i = 0; i < 8; i++) {
			if (i < 4) spoilerMino[(i % 4)][1] = mino[i];
			else       spoilerMino[(i % 4)][2] = mino[i];
		}
	}

	void wallReset() {
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 21; j++) {
				if (i == 0 || i == 11) map[i][j] = 99;
				if (j == 20) map[i][j] = 99;
			}
		}
	}

	char moveleft() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (playMino[i][j] != 0 && map[(minoPosX + i) - 1][(minoPosY + j)] != 0) return 0;
			}
		}
		return 1;
	}

	struct Point { int x; int y; };

	bool checkCollision(short mino[4][4], int tx, int ty) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (mino[i][j] != 0) {
					if (map[tx + i][ty + j] != 0) {
						return true;
					}
				}
			}
		}
		return false;
	}

	void leftKick(short originalMino[4][4]) {
		short rotatedMino[4][4] = { 0 };
		int n = (minoTrype == 0) ? 4 : 3;
		if (minoTrype == 1) return;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				rotatedMino[n - 1 - j][i] = originalMino[i][j];
			}
		}

		Point kicks[] = {
			{0, 0},
			{-1, 0}, {1, 0},
			{0, -1},
			{-1, -1}, {1, -1},
			{0, 1},
			{-2, 0}, {2, 0},
			{0, -2},
			{-2, -1}, {2, -1}, {-1, -2}, {1, -2}
		};

		for (int k = 0; k < sizeof(kicks) / sizeof(Point); k++) {
			int testX = minoPosX + kicks[k].x;
			int testY = minoPosY + kicks[k].y;

			if (checkCollision(rotatedMino, testX, testY) == false) {
				minoPosX = testX;
				minoPosY = testY;
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						playMino[i][j] = rotatedMino[i][j];
					}
				}
				isTspin = true;
				return;
			}
		}
		isTspin = false;
	}

	char moveright() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (playMino[i][j] != 0 && map[(minoPosX + i) + 1][(minoPosY + j)] != 0) return 0;
			}
		}
		return 1;
	}

	void rightKick(short originalMino[4][4]) {
		short rotatedMino[4][4] = { 0 };
		int n = (minoTrype == 0) ? 4 : 3;
		if (minoTrype == 1) return;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				rotatedMino[j][n - 1 - i] = originalMino[i][j];
			}
		}

		Point kicks[] = {
			{0, 0},
			{-1, 0}, {1, 0},
			{0, -1},
			{-1, -1}, {1, -1},
			{0, 1},
			{-2, 0}, {2, 0},
			{0, -2},
			{-2, -1}, {2, -1}, {-1, -2}, {1, -2}
		};

		for (int k = 0; k < sizeof(kicks) / sizeof(Point); k++) {
			int testX = minoPosX + kicks[k].x;
			int testY = minoPosY + kicks[k].y;
			if (checkCollision(rotatedMino, testX, testY) == false) {
				minoPosX = testX;
				minoPosY = testY;
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						playMino[i][j] = rotatedMino[i][j];
					}
				}
				isTspin = true;
				return;
			}
		}
		isTspin = false;
	}

	char movedown() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (playMino[i][j] != 0 && map[(minoPosX + i)][(minoPosY + j) + 1] != 0) return 0;
			}
		}
		return 1;
	}

	char movedownCopy(const short copyMino[4][4], int posYnow) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (copyMino[i][j] != 0 && map[(minoPosX + i)][(posYnow + j) + 1] != 0) return 0;
			}
		}
		return 1;
	}

	char isBlocked(short temp[4][4]) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (temp[i][j] != 0 && map[(minoPosX + i)][(minoPosY + j)] != 0) return 0;
			}
		}
		return 1;
	}

	char isDeadZone() {
		for (int i = 0; i < 4; i++) {
			if (map[4 + i][1] != 0) return 0;
		}
		return 1;
	}

	void rotation(int direction, int minoIndex) {
		if (minoIndex == 1) return;
		short temp[4][4] = { 0 };
		int n = (minoIndex == 0) ? 4 : 3;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (direction == 1) {
					temp[j][n - 1 - i] = playMino[i][j];
				}
				else if (direction == -1) {
					temp[n - 1 - j][i] = playMino[i][j];
				}
			}
		}
		if (isBlocked(temp) == 1) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					playMino[i][j] = temp[i][j];
				}
			}
		}
	}

	void getMino(short index) {
		if (index == 0) playerset(Imino);
		if (index == 1) playerset(Omino);
		if (index == 2) playerset(Zmino);
		if (index == 3) playerset(Smino);
		if (index == 4) playerset(Jmino);
		if (index == 5) playerset(Lmino);
		if (index == 6) playerset(Tmino);
	};

	void getSpolierMino(short index) {
		if (index == 0) spoilerset(Imino);
		if (index == 1) spoilerset(Omino);
		if (index == 2) spoilerset(Zmino);
		if (index == 3) spoilerset(Smino);
		if (index == 4) spoilerset(Jmino);
		if (index == 5) spoilerset(Lmino);
		if (index == 6) spoilerset(Tmino);
	};

protected:

private:
	std::unique_ptr<Screen> s;
	std::unique_ptr<Asset> sq = std::make_unique<Asset>();
	std::unique_ptr<Asset> sqblank = std::make_unique<Asset>();
	std::unique_ptr<Asset> wall = std::make_unique<Asset>();
	std::unique_ptr<Asset> delsq = std::make_unique<Asset>();
	std::unique_ptr<Asset> empty = std::make_unique<Asset>();
	std::unique_ptr<Asset> font = std::make_unique<Asset>();
	short playMino[4][4] = { 0 };
	short spoilerMino[4][4] = { 0 };
	short minoIndex = 0;
	short minoholdIndex = -1;
	short minoarray[7] = { 0,1,2,3,4,5,6 };
	short minoarray2[7] = { 0,1,2,3,4,5,6 };
	short map[12][21] = { 0 };
	short tempmap[12][20] = { 0 };
	short* minoShape;
	int minoPosX;
	int minoPosY;
	clock_t  start_S, start_T, start_F, start_mino, start_K, finish_time;
	double timeSec, timeTick, timeFrame, timeKey;
	int startX = 200, startY = 10;
	std::wstring mTitle = L"테트리스";
	bool isKeyPressedL = false;
	bool isKeyPressedR = false;
	bool isKeyPressedD = false;
	bool isKeyPressedU = false;
	bool isKeyPressedZ = false;
	bool isKeyPressedX = false;
	bool isKeyPressedC = false;
	bool isKeyPressedQ = false;
	bool isTspin = false;
	bool isBacktoBack = false;
	bool isChanged = false;
	bool views = false;
	short minonum = 0;
	int framecount = 0;
	int tick = 0;
	int droptime = 0;
	int endofdrop = 0;
	int newMino = 0;
	int minoTrype = 0;
	int score = 0;
	double level = 50;

	short Imino[8] = { 11,11,11,11, 0,0,0,0 };
	short Omino[3][3] = {
		{14, 14, 0},
		{14, 14, 0},
		{ 0,  0, 0}
	};
	short Zmino[3][3] = {
		{12, 12, 0},
		{ 0, 12, 12},
		{ 0,  0, 0}
	};
	short Smino[3][3] = {
		{ 0, 10, 10},
		{ 10,  10, 0},
		{ 0,  0, 0}
	};
	short Jmino[3][3] = {
		{ 9,  0, 0},
		{ 9,  9, 9},
		{ 0,  0, 0}
	};
	short Lmino[3][3] = {
		{ 0,  0, 6},
		{6, 6, 6},
		{ 0,  0,  0}
	};
	short Tmino[3][3] = {
		{ 0, 13, 0},
		{13, 13, 13},
		{ 0,  0, 0}
	};
};

int main() {
	Game g;
	g.run();
	return 0;
}