#include <iostream>
#include <MyLib/engine.hpp>
#include <MyLib/MathLib.hpp>
#include <vector>
#include <sys/time.h>
#include <unistd.h>



using Math::Vector2;
using std::vector;



void KeyHandler(Renderer *r, Vector2 *snakeV, const int &scl)
{
	if (r->GetKey(KEY_RIGHT) == KEY_PRESS)
		*snakeV = Vector2(scl, 0);
	if (r->GetKey(KEY_LEFT) == KEY_PRESS)
		*snakeV = Vector2(-scl, 0);
	if (r->GetKey(KEY_UP) == KEY_PRESS)
		*snakeV = Vector2(0, -scl);
	if (r->GetKey(KEY_DOWN) == KEY_PRESS)
		*snakeV = Vector2(0, scl);
}

void GenerateFood(Vector2 *food, int &rows, int &cols, const int &scl)
{
	food->x = Math::Random(0, cols);
	food->x *= scl;
	food->y = Math::Random(0, rows);
	food->y *= scl;
}


int main()
{
	Renderer r("Snake!", 600, 600);
	
	vector<Vector2> snake;

	int snakeSize = 3;

	const int scl = 10;
	int rows = r.width / scl, cols = r.height / scl;

	Vector2 snakePos(scl * 3, scl * 3), snakeVel(0, scl), food;

	for (int i = 0; i < snakeSize; i++)
		snake.push_back(snakePos - (snakeVel * i));
	
	int frameRate = 8;
	int frameDelay = 1000 / frameRate;

	unsigned int frameStart;
	int frameTime;

	timeval te;

	bool dead = false;

	GenerateFood(&food, rows, cols, scl);

	while (!r.WindowShouldClose())
	{
		gettimeofday(&te, nullptr);
		frameStart = te.tv_sec * 1000LL + te.tv_usec / 1000;
		r.Background({1, 1, 1, 1});

		for (auto part : snake)
			r.DrawRect(part.x, part.y, scl, scl, 
				Renderer::RECT_MODE_TOP_LEFT,  {0, 0, 0, 0});

		r.FillRect(food.x, food.y, scl, scl,
				Renderer::RECT_MODE_TOP_LEFT, {1, 0, 0, 0});

		KeyHandler(&r, &snakeVel, scl);
		snakePos += snakeVel;

		for (int i = snakeSize - 1; i > 0; i--)
			snake[i] = snake[i - 1];
		snake[0] = snakePos;

		if ((snakePos.x == food.x && snakePos.y == food.y) || 
			(r.GetMouseButton(MOUSE_BUTTON_LEFT) == KEY_PRESS))
		{
			snake.push_back(Vector2(snakePos - (snakeVel * snakeSize)));
			snakeSize++;
			GenerateFood(&food, rows, cols, scl);
		}

		for (int i = 1; i < snakeSize; i++)
			if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
				dead = true;

		if ((snakePos.x < 0 || snakePos.x > r.width - scl ||
			snakePos.y < 0 || snakePos.y > r.height - scl) ||
			(dead))
		{
			for (int i = 0; i < snakeSize - 3; i++)
				snake.pop_back();
			snakePos = Vector2(scl * 3, scl * 3);
			snakeVel = Vector2(0, scl);
			snakeSize = 3;
			dead = false;
		}

		r.SPollEvents();

		gettimeofday(&te, nullptr);
		frameTime = te.tv_sec * 1000LL + te.tv_usec / 1000 - frameStart;
		if (frameDelay > frameTime)
			usleep(1000 * (frameDelay - frameTime));
	}
}
