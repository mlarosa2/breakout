//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 500
#define WIDTH 405

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// size of bricks in pixels
#define BRICKWIDTH 35
#define BRICKHEIGHT 10

// radius of ball in pixels
#define RADIUS 7

// lives
#define LIVES 3

//size of paddle in pixels
#define PADWIDTH 65
#define PADHEIGHT 10

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    //ball velocity
    double velocityY = (drand48()*3) + 1;
    double velocityX = 0;

    //lives symbols
    GOval lifeCount1 = newGOval(5,HEIGHT-(RADIUS*2) , RADIUS,RADIUS);
    setFilled(lifeCount1,true);
    setColor(lifeCount1,"BLACK");
    add(window,lifeCount1);

    GOval lifeCount2 = newGOval((RADIUS*2) + 5,HEIGHT-(RADIUS*2) , RADIUS,RADIUS);
    setFilled(lifeCount2,true);
    setColor(lifeCount2,"BLACK");
    add(window,lifeCount2);

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        char s[3];
        // sprintf(s,"%i", bricks);
        // setLabel(label, s);
        // setLocation(label, WIDTH - getWidth(label) - 5, HEIGHT - 5);
        GObject object = detectCollision(window, ball);
        GEvent paddleMove = getNextEvent(MOUSE_EVENT);

        if (paddleMove != NULL)
        {
            if (getEventType(paddleMove) == MOUSE_MOVED)
            {
                double paddleX = getX(paddleMove) - PADWIDTH/2;
                setLocation(paddle, paddleX, (HEIGHT-(PADHEIGHT*4)));
            }
        }

        move(ball, velocityX, velocityY);

        if (getX(ball) < 0 ||  getX(ball) + getWidth(ball) > getWidth(window))
        {
            if (velocityX == 0)
            {
                velocityX = drand48()*3 + 1;
            }
            velocityX = -velocityX;
        }

        if(getY(ball) < 0)
        {
            velocityY = -velocityY;
        }
        if (getY(ball) + RADIUS*2 >= HEIGHT)
        {
            velocityX = 0;
            lives--;
            if (lives == 2)
            {
                removeGWindow(window, lifeCount2);
            }
            if (lives == 1)
            {
                removeGWindow(window, lifeCount1);
            }
            pause(1000);
            setLocation(paddle, ((WIDTH/2) - PADWIDTH/2 ) , (HEIGHT-(PADHEIGHT*4)));
            setLocation(ball, WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS);
            waitForClick();
        }

        if (object != NULL)
        {
            if (object == paddle && getX(ball) + RADIUS < getX(paddle) + PADWIDTH/2)
            {
                velocityX = drand48()*-3 - 1;
                if(velocityY > 0)
                {
                    velocityY = -velocityY;
                }
                if (velocityX == 0)
                {
                    velocityX = drand48()*3 + 1;
                }
            }

            if (object == paddle && getX(ball) + RADIUS >= getX(paddle) + PADWIDTH/2)
            {
                velocityX = drand48()*3 + 1;
                if(velocityY > 0)
                {
                    velocityY = -velocityY;
                }
                if (velocityX == 0)
                {
                    velocityX = drand48()*3 + 1;
                }

            }
            if ((strcmp(getType(object), "GRect") == 0 && object != paddle && getX(ball) + RADIUS > getX(object) + BRICKWIDTH/2) || (strcmp(getType(object), "GRect") == 0 && object != paddle && getX(ball) + RADIUS <= getX(object) + BRICKWIDTH/2) )
            {
                removeGWindow(window, object);
                bricks--;
                velocityY = -velocityY;
                if (velocityX < 0)
                {
                    velocityX = (drand48()*3 + 1);
                }
            }
        }
        pause(10);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int space = 5;
    int brickSpotY =  BRICKHEIGHT*2;
    int brickSpotX = space;

    for (int brickX = 0; brickX < ROWS; brickX++)
    {
        for (int brickY = 0; brickY < COLS; brickY++)
        {
            if (brickX == 0)
            {
                GRect brick = newGRect(brickSpotX, brickSpotY, BRICKWIDTH, BRICKHEIGHT);
                setFilled(brick,true);
                setColor(brick,"RED");
                add(window,brick);
            }

            if (brickX == 1)
            {
                GRect brick = newGRect(brickSpotX, brickSpotY, BRICKWIDTH, BRICKHEIGHT);
                setFilled(brick,true);
                setColor(brick,"Orange");
                add(window,brick);
            }

            if (brickX == 2)
            {
                GRect brick = newGRect(brickSpotX, brickSpotY, BRICKWIDTH, BRICKHEIGHT);
                setFilled(brick,true);
                setColor(brick,"YELLOW");
                add(window,brick);
            }

            if (brickX == 3)
            {
                GRect brick = newGRect(brickSpotX, brickSpotY, BRICKWIDTH, BRICKHEIGHT);
                setFilled(brick,true);
                setColor(brick,"GREEN");
                add(window,brick);
            }

            if (brickX == 4)
            {
                GRect brick = newGRect(brickSpotX, brickSpotY, BRICKWIDTH, BRICKHEIGHT);
                setFilled(brick,true);
                setColor(brick,"BLUE");
                add(window,brick);
            }

            brickSpotX += BRICKWIDTH + space;
        }

        brickSpotY += BRICKHEIGHT + space;
        brickSpotX = space;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval((WIDTH/2)-RADIUS,(HEIGHT/2)-RADIUS, RADIUS*2,RADIUS*2);
    setFilled(ball,true);
    setColor(ball,"BLACK");
    add(window,ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(((WIDTH/2)-(PADWIDTH/2)),(HEIGHT-(PADHEIGHT*4)),PADWIDTH,PADHEIGHT);
    setFilled(paddle,true);
    setColor(paddle,"BLACK");
    add(window,paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel scoreBoard = newGLabel("");
    setFont(scoreBoard, "SansSerif-36");
    add(window, scoreBoard);
    return scoreBoard;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
