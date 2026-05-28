#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 600

int score = 0;
int x_offset = 20;
int q = 1;

typedef struct
{
    float x;
    float y;
    int w;
    int h;
} Player;

typedef struct
{
    float x;
    float y;
    int r;
    int vx;
    int vy;
} Ball;

void draw_line()
{
    Vector2 start = {WIDTH / 2, 0};
    Vector2 end = {WIDTH / 2, HEIGHT};
    int dash_size = 26;
    int space_size = 10;

    DrawLineDashed(start, end, dash_size, space_size, WHITE);
}

void draw_score()
{
    const char *score_value = TextFormat("%d", score);
    float pos_x = WIDTH - x_offset;
    float pos_y = 10;
    int font_size = 20;

    DrawText(score_value, pos_x, pos_y, font_size, WHITE);
}

void draw_player_right(float pos_x, float pos_y, int width, int height)
{
    DrawRectangle(pos_x, pos_y, width, height, WHITE);
}

void update_player_right(Player *p_right, Ball *ball)
{
    if (ball->x > WIDTH / 2)
    {
        float mouse_y = GetMouseY();
        p_right->y = mouse_y - p_right->h / 2;
    }

    float y = p_right->y;
    int h = p_right->h;

    if (y + h > HEIGHT)
    {
        p_right->y = HEIGHT - h;
    }

    if (y < 0)
    {
        p_right->y = 0;
    }

    draw_player_right(p_right->x, p_right->y, p_right->w, p_right->h);
}

void draw_player_left(float pos_x, float pos_y, int width, int height)
{
    DrawRectangle(pos_x, pos_y, width, height, WHITE);
}

void update_player_left(Player *p_left, Ball *ball)
{
    if (ball->x < WIDTH / 2)
    {
        float mouse_y = GetMouseY();
        p_left->y = mouse_y - p_left->h / 2;
    }

    float y = p_left->y;
    int h = p_left->h;

    if (y + h > HEIGHT)
    {
        p_left->y = HEIGHT - h;
    }

    if (y < 0)
    {
        p_left->y = 0;
    }

    draw_player_left(p_left->x, p_left->y, p_left->w, p_left->h);
}

void draw_ball(int position_x, int position_y, int radius, Color color)
{
    DrawCircle(position_x, position_y, radius, color);
}

void update_ball(Ball *ball, Player *p_right, Player *p_left)
{
    ball->x += ball->vx;
    ball->y += ball->vy;

    float b_x = ball->x;
    float b_y = ball->y;
    int r = ball->r;

    float pr_x = p_right->x;
    float pr_y = p_right->y;
    int w = p_right->w;
    int h = p_right->h;

    float pl_x = p_left->x;
    float pl_y = p_left->y;

    if (b_x - r < 0 || b_x + r > WIDTH)
    {
        ball->x = WIDTH / 2;
        ball->y = HEIGHT / 2;

        // handle slowdown
        if (score > 0)
        {
            ball->vx = -ball->vx / 2;
        }
        else
        {
            ball->vx = -ball->vx;
        }

        score = 0;

        x_offset = 20;
        q = 1;
    }

    if (b_y + r > HEIGHT || b_y - r < 0)
    {
        ball->vy = -ball->vy;
    }

    int y_offset = r;

    if (ball->vy != 0)
    {
        y_offset *= 2;
    }

    // handle right player bounce
    if (b_x + r == pr_x && (b_y >= pr_y - y_offset && b_y <= pr_y + h + y_offset))
    {
        // middle bounce
        if (b_y >= (pr_y + h / 2) - r && b_y <= (pr_y + h / 2) + r)
        {
            ball->vx = -ball->vx;
            ball->vy = 0;
        }

        // top bounce
        if (b_y < (pr_y + h / 2) - r && b_y >= pr_y - y_offset)
        {
            ball->vx = -ball->vx;
            ball->vy = -5;
        }

        // bottom bounce
        if (b_y > (pr_y + h / 2) + r && b_y <= pr_y + h + y_offset)
        {
            ball->vx = -ball->vx;
            ball->vy = 5;
        }

        // handle speed
        if (ball->vx == -5)
        {
            ball->vx *= 2;
        }

        score++;

        // dynamic score alignment
        if (score % (10 * q) == 0)
        {
            x_offset += 6;
            q *= 10;
        }

        if (score == 20)
        {
            x_offset += 3;
        }
    }

    // handle left player bounce
    if (b_x - r == pl_x + w && (b_y >= pl_y - y_offset && b_y <= pl_y + h + y_offset))
    {
        // middle bounce
        if (b_y >= (pl_y + h / 2) - r && b_y <= (pl_y + h / 2) + r)
        {
            ball->vx = -ball->vx;
            ball->vy = 0;
        }

        // top bounce
        if (b_y < (pl_y + h / 2) - r && b_y >= pl_y - y_offset)
        {
            ball->vx = -ball->vx;
            ball->vy = -5;
        }

        // bottom bounce
        if (b_y > (pl_y + h / 2) + r && b_y <= pl_y + h + y_offset)
        {
            ball->vx = -ball->vx;
            ball->vy = 5;
        }

        // handle speed
        if (ball->vx == 5)
        {
            ball->vx *= 2;
        }

        score++;

        // dynamic score alignment
        if (score % (10 * q) == 0)
        {
            x_offset += 6;
            q *= 10;
        }

        if (score == 20)
        {
            x_offset += 6;
        }
    }

    draw_ball(ball->x, ball->y, ball->r, WHITE);
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "Pong");

    SetTargetFPS(60);

    float pr_pos_x = WIDTH - 40;
    float pr_pos_y = HEIGHT / 2;
    float pl_pos_x = 40;
    float pl_pos_y = HEIGHT / 2;

    int p_width = 10;
    int p_height = 60;

    float b_pos_x = WIDTH / 2;
    float b_pos_y = HEIGHT / 2;
    int b_rad = 10;
    int b_vel_x = 5;
    int b_vel_y = 5;

    Player p_right = {pr_pos_x, pr_pos_y, p_width, p_height};
    Player p_left = {pl_pos_x, pl_pos_y, p_width, p_height};
    Ball ball = {b_pos_x, b_pos_y, b_rad, b_vel_x, b_vel_y};

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        draw_line();
        draw_score();

        update_player_right(&p_right, &ball);
        update_player_left(&p_left, &ball);
        update_ball(&ball, &p_right, &p_left);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}