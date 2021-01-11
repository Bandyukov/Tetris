#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
using namespace sf;

const int M = 20; //Высота игрового поля
const int N = 10; //Ширина игрового поля


//Массив фигурок-тетрамино
int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

class Position
{
	int x, y;

public:
	Position() : x(0), y(0) {};

	int getx() { return x; };
	int& setx() { return x; };
	int gety() { return y; };
	int& sety() { return y; };

};

class Tetramino
{
	int* arr;
	int size;

public:

	Tetramino()
	{
		size = 500;
		arr = new int[size];
		int n;
		for (int i = 0; i < size; i++)
		{
			if (i == 0)
				arr[i] = rand() % 7;
			else if (i == 1)
			{
				n = rand() % 7;
				while (n == arr[i - 1]) //Избегаем повторения фигурок
					n = rand() % 7;
				arr[i] = n;
			}
			else
			{
				n = rand() % 7;
				while (n == arr[i - 1] || n == arr[i - 2]) //Избегаем повторения фигурок
					n = rand() % 7;
				arr[i] = n;
			}
		}
	}

	/*void createArray()
	{
		int n;
		for (int i = 0; i < 500; i++)
		{
			if (i == 0)
				arr[i] = rand() % 7;
			else if (i == 1)
			{
				n = rand() % 7;
				while (n == arr[i - 1]) //Избегаем повторения фигурок
					n = rand() % 7;
				arr[i] = n;
			}
			else
			{
				n = rand() % 7;
				while (n == arr[i - 1] || n == arr[i - 2]) //Избегаем повторения фигурок
					n = rand() % 7;
				arr[i] = n;
			}
		}
	}*/

	void moveTetraminoDown(int field[20][10], bool& beginGame, float& timer, float delay,
		int& colorNum, int& k, Position a[4], Position b[4]);
	void moveTetraminoUp(int field[20][10], bool& beginGame, float& timer, float delay,
		int& colorNum, int& k, Position a[4], Position b[4]);
};


//Проверка на выход за границы игрового поля
bool check(int field[20][10], Position a[4])
{
	for (int i = 0; i < 4; i++)
		if (a[i].getx() < 0 || a[i].getx() >= N || a[i].gety() >= M) return false;
		else if (field[a[i].gety()][a[i].getx()]) return false;

	return true;
}

//Проверка для усложнённого режима
bool checkHardMode(int field[20][10], Position a[4])
{
	for (int i = 0; i < 4; i++)
		if (a[i].getx() < 0 || a[i].getx() >= N || a[i].gety() <= 0) return false;
		else if (field[a[i].gety()][a[i].getx()]) return false;

	return true;
}

////// Вышли за пределы поля
void outTheField(Position a[4], Position b[4])
{
	for (int i = 0; i < 4; i++)
		a[i] = b[i];
}

//Печатаем текст
void printText(RenderWindow& window, char str[], Position pos, int size, Color color)
{
	//Добавляем шрифт
	Font font;
	font.loadFromFile("..\\font\\BAUHS93.ttf");

	//Текст
	Text text(str, font, size);
	text.setFillColor(color);
	text.setPosition(pos.getx(), pos.gety());

	window.draw(text);
}

////// Движение тетрамино вниз («Тик» таймера) //////
void Tetramino::moveTetraminoDown(int field[20][10], bool& beginGame, float& timer, float delay,
	int& colorNum, int& k, Position a[4], Position b[4])
{
	if (timer > delay)
	{
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].sety() += 1;
		}
		if (!check(field, a))
		{
			for (int i = 0; i < 4; i++)
				field[b[i].gety()][b[i].getx()] = colorNum;

			colorNum = 1 + arr[k];

			for (int i = 0; i < 4; i++)
			{
				a[i].setx() = figures[arr[k]][i] % 2 + 4; //По центру
				a[i].sety() = figures[arr[k]][i] / 2 - 1;
			}

			k++;
			if (k == 500)
				k = 0;
		}
		timer = 0;

		//Первое появление тетрамино на поле?
		if (beginGame)
		{
			beginGame = false;

			colorNum = arr[0] + 1;
			for (int i = 0; i < 4; i++)
			{
				a[i].setx() = figures[arr[0]][i] % 2 + 4; //По центру
				a[i].sety() = figures[arr[0]][i] / 2 - 1;
			}
		}
	}
}

void Tetramino::moveTetraminoUp(int field[20][10], bool& beginGame, float& timer, float delay,
	int& colorNum, int& k, Position a[4], Position b[4])
{
	if (timer > delay)
	{
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].sety() -= 1;
		}
		if (!checkHardMode(field, a))
		{
			for (int i = 0; i < 4; i++)
				field[b[i].gety()][b[i].getx()] = colorNum;

			colorNum = 1 + arr[k];

			for (int i = 0; i < 4; i++)
			{
				a[i].setx() = figures[arr[k]][i] % 2 + 4; //По центру
				a[i].sety() = figures[arr[k]][i] / 2 + 19;
			}

			k++;
			if (k == 500)
				k = 0;
		}
		timer = 0;

		//Первое появление тетрамино на поле?
		if (beginGame)
		{
			beginGame = false;

			colorNum = arr[0] + 1;
			for (int i = 0; i < 4; i++)
			{
				a[i].setx() = figures[arr[0]][i] % 2 + 4; //По центру
				a[i].sety() = figures[arr[0]][i] / 2 + 19;
			}
		}
	}
}

/////// Горизонтальное перемещение ////
void horizontalMovement(Position a[4], Position b[4], int dx)
{
	for (int i = 0; i < 4; i++)
	{
		b[i] = a[i];
		a[i].setx() += dx;
	}
}

/////// Вращение
void rotation(int field[20][10], Position a[4], Position b[4], bool mode)
{
	Position p = a[1]; //Задаем центр вращения
	for (int i = 0; i < 4; i++)
	{
		int x = a[i].gety() - p.gety(); //y-y0
		int y = a[i].getx() - p.getx(); //x-x0
		a[i].setx() = p.getx() - x;
		a[i].sety() = p.gety() + y;
	}
	//Вышли за пределы поля после поворота? Тогда возвращаем старые координаты 
	if (!check(field, a))
		outTheField(a, b);
	if (mode)
		if (!checkHardMode(field, a))
			outTheField(a, b);
}

/////// Проверяем линию на заполненность
void lineCheck(int field[20][10], int& score, bool &gameOver)
{
	int t = M - 1, k = 0;
	for (int i = M - 1; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < N; j++)
		{
			if (field[i][j]) 
				count++;
			field[t][j] = field[i][j];
		}
		if (count < N)
			t--;
		else 
			score += 10 * (++k);
	}
}

/////// Проверяем линию на заполненность
void lineCheckHardMode(int field[20][10], int& score)
{
	int t = 0, k = 0;
	for (int i = 0; i < M; i++)
	{
		int count = 0;
		for (int j = 0; j < N; j++)
		{
			if (field[i][j]) count++;
			field[t][j] = field[i][j];
		}
		if (count < N) 
			t++;
		else 
			score += 10 * (++k);
	}
}

/////// Рисуем тетрамино
void drawTetramino(RenderWindow& window, Sprite sprite, int field[20][10], int colorNum, Position a[4], int shiftX, int shiftY)
{
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
		{
			if (field[i][j] == 0) continue;
			sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
			sprite.setPosition(j * 18, i * 18);
			sprite.move(shiftX, shiftY); //Смещение
			window.draw(sprite);
		}
	for (int i = 0; i < 4; i++)
	{
		//Разукрашиваем тетрамино
		sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
		//Устанавливаем позицию каждого кусочка тетрамино
		sprite.setPosition(a[i].getx() * 18, a[i].gety() * 18);
		sprite.move(shiftX, shiftY); //Смещение
		//Отрисовка спрайта
		window.draw(sprite);
	}
}

void _drawTetramino_(RenderWindow& window, Sprite sprite, int field[20][10], int colorNum, Position a[4], int shiftX, int shiftY)
{
	for (int i = M - 1; i > 0; i--)
		for (int j = 0; j < N; j++)
		{
			if (field[i][j] == 0) continue;
			sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
			sprite.setPosition(j * 18, i * 18);
			sprite.move(shiftX, shiftY); //Смещение
			window.draw(sprite);
		}
	for (int i = 0; i < 4; i++)
	{
		//Разукрашиваем тетрамино
		sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
		//Устанавливаем позицию каждого кусочка тетрамино
		sprite.setPosition(a[i].getx() * 18, a[i].gety() * 18);
		sprite.move(shiftX, shiftY); //Смещение
		//Отрисовка спрайта
		window.draw(sprite);
	}
}

/////// Переход на следующий уровень
void nextLevel(int score, float& wave, bool& flag, bool& _flag_)
{
	if (score >= 100 * wave) // Количество очков для перехода на следующий уровень
		wave++, flag = true, _flag_ = true;
}

/////// Время показа текста
void timeForText(float& timer, bool& flag)
{
	if (timer > 2)
	{
		flag = false;
		timer = 0;
	}
}

/////// Проверяем на конец игры
bool checkGameEnd(int field[20][10], bool gameOver)
{
	
	for (int i = 0; i < N; i++)
		if (field[1][i])
			gameOver = true;

	return gameOver;
}

/////// Проверяем на конец игры
bool checkGameEndHardMode(int field[20][10], bool gameOver)
{
	for (int i = 0; i < N; i++)
		if (field[M - 1][i])
			gameOver = true;

	return gameOver;
}

int main()
{
	srand(time(0));

	int field1[M][N] = { 0 }; //Игровое поле
	int field2[M][N] = { 0 };

	int score1 = 0, score2 = 0;

	Position  pos1, pos2, pos3, pos4, pos5, pos6, pos7, pos8, pos9, pos10, pos11, pos12, pos13, pos14,  a[4], b[4], 
		c[4], d[4], posText1, posText2, _posText1_;

	RenderWindow window(VideoMode(720, 950), "Tetri$!");

	//Создание и загрузка текстуры
	Texture texture, texture_background, texture_frame;
	texture.loadFromFile("..\\images\\tiles.png");

	//Добавляем музыку
	Music music;
	music.openFromFile("..\\music\\from_Bass_with_love.ogg");
	music.play();
	music.setLoop(true);

	//Создание спрайта
	Sprite sprite(texture);

	//Вырезаем из спрайта отдельный квадратик размером 18х18 пикселей
	sprite.setTextureRect(IntRect(0, 0, 18, 18));


	//Создаём рамки
	RectangleShape box1(Vector2f(180, 396)), box2(Vector2f(180, 396)), box3(Vector2f(180, 792));
	box1.setOutlineThickness(3.f);
	box1.setOutlineColor(Color(20, 48, 51));
	box1.setFillColor(Color(204, 167, 232));
	box1.setPosition(480.f, 300.f);
	box2.setOutlineThickness(3.f);
	box2.setOutlineColor(Color(20, 48, 51));
	box2.setFillColor(Color(163, 240, 139));
	box2.setPosition(60.f, 300.f);
	box3.setOutlineThickness(3.f);
	box3.setOutlineColor(Color(128, 128, 128));
	box3.setFillColor(Color(0, 49, 49));
	box3.setPosition(270.f, 79.f);

	//Переменные для горизонтального перемещения и вращения
	int dx1 = 0, dx2 = 0, n = rand() % 7, colorNum1 = 1 + n, colorNum2 = 1 + n, i, m = 1, k = 1;
	bool rotate1 = false, rotate2 = false, beginGame1 = true, beginGame2 = true, start1player = false, start2player = false, hardMode = false,
		demonstarationText1 = false, demonstarationText2 = false, flag1 = false, flag2 = false, gameOver1 = false, gameOver2 = false;
	float wave1 = 1, wave2 = 1;

	Tetramino arrayOfTetramino1, arrayOfTetramino2;

	//Переменные для таймера и задержки
	float timer1 = 0, delay1 = 0.7, timer2 = 0, delay2 = 0.7, timerText1 = 0, timerText2 = 0;

	//Сдвиг появления тетрамино
	int shiftX1 = 480, shiftX2 = 60, shiftY_default = 336, shiftY1 = 511, shiftY2 = 61;

	//Часы (таймер)
	Clock clock1;

	Color colorLevel(187, 7, 232);
	char strLevel[] = "Next level!", str5[] = "Scores: ", sc1[100], sc2[100], strOver[] = "GAME OVER";
	
	//Главный цикл приложения. Выполняется, пока открыто окно.
	while (window.isOpen())
	{
		//Получаем время, прошедшее с начала отсчета, и конвертируем его в секунды
		float time1 = clock1.getElapsedTime().asSeconds();
		clock1.restart();
		timer1 += time1;
		timer2 += time1;

		timerText1 += time1;
		timerText2 += time1;

		//Начало игры
		if (!start1player && !hardMode)
		{
			if (Keyboard::isKeyPressed(Keyboard::F1))
			{
				start1player = true;
				box1.setPosition(270.f, 300.f);
				shiftX1 = 270;
			}
			if (Keyboard::isKeyPressed(Keyboard::F2))
				start1player = true, start2player = true;
			if (Keyboard::isKeyPressed(Keyboard::F3))
			{
				hardMode = true;
				shiftX1 = 270;
			}
		}
		//Обрабатываем очередь событий в цикле
		Event event;
		while (window.pollEvent(event))
		{
			//Пользователь нажал на «крестик» и хочет окно закрыть?
			if (event.type == Event::Closed)
				//Тогда закрываем его
				window.close();
			//Была нажата кнопка на клавиатуре?
			if (event.type == Event::KeyPressed)
			{
				//Эта кнопка – стрелка вверх?…
				if (event.key.code == Keyboard::Up)
					rotate1 = true;
				//…или же стрелка влево?…
				else if (event.key.code == Keyboard::Left)
					dx1 = -1;
				////…ну тогда может это стрелка вправо?
				else if (event.key.code == Keyboard::Right)
					dx1 = 1;
				//Эта кнопка – w?…
				
				//…или же a?…
				if (event.key.code == Keyboard::A)
					dx2 = -1;
				////…ну тогда может это d?
				else if (event.key.code == Keyboard::D)
					dx2 = 1;
				else if (hardMode)
					{
						if (event.key.code == Keyboard::S)
							rotate2 = true;
					}
					else
						if (event.key.code == Keyboard::W)
							rotate2 = true;
			}
		}

		//Нажали кнопку "вниз"? Ускоряем падение тетрамино
		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay1 = 0.05;
		if (hardMode)
		{
			if (Keyboard::isKeyPressed(Keyboard::W))
				delay2 = 0.05;
		}
		else
			if (Keyboard::isKeyPressed(Keyboard::S))
				delay2 = 0.05;
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		//// Горизонтальное перемещение ////
		if (start1player && !gameOver1)
			horizontalMovement(a, b, dx1), gameOver1 = checkGameEnd(field1, gameOver1);
		if (start2player && !gameOver2)
			horizontalMovement(c, d, dx2), gameOver2 = checkGameEnd(field2, gameOver2);;
		if (hardMode && !gameOver1)
		{
			horizontalMovement(a, b, dx1);
			horizontalMovement(c, d, dx2);
			gameOver1 = checkGameEnd(field1, gameOver1);
			gameOver1 = checkGameEndHardMode(field2, gameOver1);
		}

		//Вышли за пределы поля после перемещения? Тогда возвращаем старые координаты
		if (start1player)
			if (!check(field1, a))
				outTheField(a, b);
		if (start2player)
			if (!check(field2, c))
				outTheField(c, d);
		if (hardMode)
		{
			if (!check(field1, a))
				outTheField(a, b);
			if (!checkHardMode(field2, c))
				outTheField(c, d);
		}

		/////// Вращение
		if (rotate1)
			rotation(field1, a, b, hardMode);
		if (start2player || hardMode)
			if (rotate2)
				rotation(field2, c, d, hardMode);

		
		////// Движение тетрамино вниз («Тик» таймера) //////
		if (start1player && !gameOver1)
			arrayOfTetramino1.moveTetraminoDown(field1, beginGame1, timer1, delay1, colorNum1, k, a, b);
		if (start2player && !gameOver2)
			arrayOfTetramino1.moveTetraminoDown(field2, beginGame2, timer2, delay2, colorNum2, m, c, d);
		if (hardMode && !gameOver1)
		{
			arrayOfTetramino1.moveTetraminoDown(field1, beginGame1, timer1, delay1, colorNum1, k, a, b);
			arrayOfTetramino2.moveTetraminoUp(field2, beginGame2, timer2, delay2, colorNum2, m, c, d);
		}

		//Возвращаем исходные значения
		dx1 = 0; rotate1 = false; delay1 = 0.70 - 0.05 * (wave1 - 1.0);
		dx2 = 0; rotate2 = false; 
		(hardMode) ? delay2 = 0.7 - 0.05 * (wave1 - 1.0) : delay2 = 0.70 - 0.05 * (wave2 - 1.0);

		//----ПРОВЕРКА ЛИНИИ И ПЕРЕХОД НА СЛЕДУЮЩИЙ УРОВЕНЬ----//
		if (start1player)
		{
			lineCheck(field1, score1, gameOver1);
			nextLevel(score1, wave1, demonstarationText1, flag1);
			
		}
		if (start2player)
		{
			lineCheck(field2, score2, gameOver2);
			nextLevel(score2, wave2, demonstarationText2, flag2);
		}
		if (hardMode)
		{
			lineCheck(field1, score1, gameOver1);
			lineCheckHardMode(field2, score1); 
			nextLevel(score1, wave1, demonstarationText1, flag1);
		}


		//----ОТРИСОВКА----//
		//Задаем цвет фона (Белый)
		window.clear(Color(233, 233, 172));
		if (start1player)
		{
			if (!gameOver1)
			_itoa_s(score1, sc1, 10);

			window.draw(box1);
			drawTetramino(window, sprite, field1, colorNum1, a, shiftX1, shiftY_default);

			if (!start2player)
			{
				pos5.setx() = 270;
				pos8.setx() = 400;
			}
			else
			{
				pos5.setx() = 480;
				pos8.setx() = 610;
			}

			pos5.sety() = 245;
			pos8.sety() = 245;
			pos1.setx() = 605;
			pos1.sety() = 245;

			Color color2(230, 49, 49);
			printText(window, str5, pos5, 40, color2);
			printText(window, sc1, pos8, 40, color2);
		}
		if (start2player)
		{
			if (!gameOver2)
				_itoa_s(score2, sc2, 10), _itoa_s(score1, sc1, 10);

			window.draw(box2);
			drawTetramino(window, sprite, field2, colorNum2, c, shiftX2, shiftY_default);

			pos6.setx() = 480;
			pos6.sety() = 245;
			pos6.setx() = 60;
			pos6.sety() = 245;
			pos7.setx() = 190;
			pos7.sety() = 245;
			pos8.setx() = 610;
			pos8.sety() = 245;

			Color color2(230, 49, 49);
			printText(window, str5, pos6, 40, color2);
			printText(window, sc2, pos7, 40, color2);
		}
		if (hardMode)
		{
			if (!gameOver1)
				_itoa_s(score1, sc1, 10);

			window.draw(box3);
			drawTetramino(window, sprite, field1, colorNum1, a, shiftX1, shiftY1);
			_drawTetramino_(window, sprite, field2, colorNum2, c, shiftX1, shiftY2);

			pos5.setx() = 270;
			pos8.setx() = 400;
			pos5.sety() = 15;
			pos8.sety() = 15;

			Color color2(230, 49, 49);
			printText(window, str5, pos5, 40, color2);
			printText(window, sc1, pos8, 40, color2);
		}


		//Отрисовка текста
		if (!start1player && !hardMode)
		{
			char str1[] = "Press F1 to play alone", str2[] = "or F2 to start PVP!",
				str3[] = "F3 - hard mode", str4[] = "Move player 1: arrows",
				str5[] = "Move player 2: W A S D";
			pos1.setx() = 160; //Устанавливаем позицию каждого текста
			pos1.sety() = 100;
			pos2.setx() = 200;
			pos2.sety() = 145;
			pos9.setx() = 230;
			pos9.sety() = 190;
			pos3.setx() = 10;
			pos3.sety() = 400;
			pos4.setx() = 10;
			pos4.sety() = 435;
			Color color1(28, 147, 215), color2(230, 49, 49); //Окрашиваем текст
			printText(window, str1, pos1, 40, color1);
			printText(window, str2, pos2, 40, color1);
			printText(window, str3, pos9, 40, color1);
			printText(window, str4, pos3, 30, color2);
			printText(window, str5, pos4, 30, color2);
		}

		if (demonstarationText1)
		{
			(start2player) ? posText1.setx() = 480 : posText1.setx() = 265;
			(hardMode) ? posText1.setx() = 40, posText1.sety() = 437 : posText1.sety() = 200;

			printText(window, strLevel, posText1, 38, colorLevel);

			if (hardMode)
			{
				_posText1_.setx() = 500;
				_posText1_.sety() = posText1.gety();
				printText(window, strLevel, _posText1_, 38, colorLevel);
			}
			if (flag1)
				timerText1 = 0, flag1 = false;
		}
		if (demonstarationText2)
		{
			posText2.setx() = 60;
			posText2.sety() = 200;

			printText(window, strLevel, posText2, 38, colorLevel);
			if (flag2)
				timerText2 = 0, flag2 = false;
		}
		
		timeForText(timerText1, demonstarationText1);
		if (start2player)
			timeForText(timerText2, demonstarationText2);

		if (gameOver1)
		{
			Color color3(255, 0, 47) ;
			if (hardMode)
			{
				pos10.setx() = 20;
				pos10.sety() = 300;
				pos12.setx() = 470;
				pos12.sety() = 600;
				printText(window, strOver, pos10, 45, color3);
				printText(window, strOver, pos12, 45, color3);
			}
			else if (!start2player)
			{
				pos10.setx() = 240;
				pos10.sety() = 180;
				printText(window, strOver, pos10, 45, color3);
			}
			else
			{
				pos10.setx() = 470;
				pos10.sety() = 180;
				printText(window, strOver, pos10, 45, color3);
			}
		}

		if (gameOver2)
		{
			Color color3(255, 0, 47);
			pos12.setx() = 40;
			pos12.sety() = 180;
			printText(window, strOver, pos12, 45, color3);
		}

		if (gameOver1 && gameOver2)
		{
			pos13.setx() = 140;
			pos13.sety() = 700;
			pos14.setx() = 180;
			pos14.sety() = 750;
			Color color4(61, 2, 120);
			if (score1 > score2)
			{
				char strWin[] = "this -> player is good", strLose[] = "ans this <- is not";
				printText(window, strWin, pos13, 50, color4);
				printText(window, strLose, pos14, 50, color4);
			}
			else if (score1 < score2)
			{
				char strWin[] = "this <- player is good", strLose[] = "ans this -> is not";
				printText(window, strWin, pos13, 50, color4);
				printText(window, strLose, pos14, 50, color4);
			}
			else
			{
				pos13.setx() = 290;
				pos14.setx() = 290;
				char strWin[] = " Wow ", strLose[] = "Draw!";
				printText(window, strWin, pos13, 50, color4);
				printText(window, strLose, pos14, 50, color4);
			}
		}
		window.display();
	}

	return 0;
}
