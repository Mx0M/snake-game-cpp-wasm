#include <emscripten.h>
#include <vector>
#include <cstdlib>
#include <ctime>

extern "C"
{

    struct Point
    {
        int x;
        int y;
    };

    // Grid size
    const int WIDTH = 20;
    const int HEIGHT = 20;

    static std::vector<Point> snake;
    static Point food;
    static int dirX = 1;
    static int dirY = 0;
    static bool gameOver = false;
    static int score = 0;

    // Helper to check equality of points
    bool pointsEqual(Point a, Point b)
    {
        return a.x == b.x && a.y == b.y;
    }

    EMSCRIPTEN_KEEPALIVE
    void start_game()
    {
        snake.clear();
        snake.push_back({WIDTH / 2, HEIGHT / 2});
        dirX = 1;
        dirY = 0;
        score = 0;
        gameOver = false;

        srand((unsigned)time(NULL));
        // Place food randomly, avoid snake start position
        do
        {
            food.x = rand() % WIDTH;
            food.y = rand() % HEIGHT;
        } while (pointsEqual(food, snake[0]));
    }

    EMSCRIPTEN_KEEPALIVE
    bool is_game_over()
    {
        return gameOver;
    }

    EMSCRIPTEN_KEEPALIVE
    int get_score()
    {
        return score;
    }

    // Update snake direction: dx, dy must be (-1,0,1) and not opposite of current
    EMSCRIPTEN_KEEPALIVE
    void change_direction(int dx, int dy)
    {
        if ((dx == -dirX && dy == -dirY) || (dx == 0 && dy == 0))
        {
            return; // Ignore reverse or zero change
        }
        dirX = dx;
        dirY = dy;
    }

    // Update game state by one tick
    EMSCRIPTEN_KEEPALIVE
    void update()
    {
        if (gameOver)
            return;

        Point newHead = {snake[0].x + dirX, snake[0].y + dirY};

        // Check wall collision
        if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT)
        {
            gameOver = true;
            return;
        }
        // Check self collision
        for (auto &s : snake)
        {
            if (pointsEqual(s, newHead))
            {
                gameOver = true;
                return;
            }
        }

        snake.insert(snake.begin(), newHead);

        if (pointsEqual(newHead, food))
        {
            // Eat food: increase score and place new food
            score += 10;
            do
            {
                food.x = rand() % WIDTH;
                food.y = rand() % HEIGHT;
            } while (std::any_of(snake.begin(), snake.end(), [&](Point p)
                                 { return pointsEqual(p, food); }));
        }
        else
        {
            // Move: remove tail
            snake.pop_back();
        }
    }

    // Get snake length
    EMSCRIPTEN_KEEPALIVE
    int get_snake_length()
    {
        return (int)snake.size();
    }

    // Get snake segment at index: returns x or y depending on coordinate (0 for x, 1 for y)
    EMSCRIPTEN_KEEPALIVE
    int get_snake_segment(int index, int coord)
    {
        if (index < 0 || index >= (int)snake.size())
            return -1;
        if (coord == 0)
            return snake[index].x;
        return snake[index].y;
    }

    // Get food position coord (0 = x, 1 = y)
    EMSCRIPTEN_KEEPALIVE
    int get_food(int coord)
    {
        if (coord == 0)
            return food.x;
        return food.y;
    }
}
