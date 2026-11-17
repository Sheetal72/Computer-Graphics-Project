#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <dos.h>

#define PI 3.14159265

int cx, cy;
int frame = 0;
int paused = 0;
float speed = 1.0;
float moonAngle = 0;

struct Planet
{
    char name[10];
    int orbit;
    int size;
    int color;
    float angle;
    float rate;
};

struct Planet planets[8] =
{
    {"Mercury", 35, 6, LIGHTGRAY, 0, 4.0},
    {"Venus",   55, 9, LIGHTRED, 0, 2.5},
    {"Earth",   78, 12, LIGHTBLUE, 0, 2.0},
    {"Mars",   105, 10, RED, 0, 1.6},
    {"Jupiter",145, 18, LIGHTCYAN, 0, 1.0},
    {"Saturn", 190, 17, YELLOW, 0, 0.8},
    {"Uranus", 230, 14, CYAN, 0, 0.6},
    {"Neptune",270, 14, BLUE, 0, 0.4}
};

// Draw twinkling stars
void drawStars()
{
    int i, x, y, c;

    for(i = 0; i < 120; i++)
    {
        x = (i * 137 + 53) % (getmaxx() + 1);
        y = (i * 89 + 31) % (getmaxy() + 1);

        if((i + frame / 4) % 5 == 0)
            c = WHITE;
        else if((i + frame / 4) % 5 == 1)
            c = LIGHTGRAY;
        else
            c = DARKGRAY;

        putpixel(x, y, c);

        if(i % 15 == 0 && c == WHITE)
        {
            setcolor(LIGHTGRAY);
            line(x - 2, y, x + 2, y);
            line(x, y - 2, x, y + 2);
        }
    }
}

// Draw smaller Sun glow
void drawSun()
{
    setcolor(DARKGRAY);
    circle(cx, cy, 21);

    setcolor(LIGHTRED);
    circle(cx, cy, 17);

    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(cx, cy, 14, 14);

    setcolor(WHITE);
    putpixel(cx - 4, cy - 4, WHITE);
}

// Draw planetary orbits
void drawOrbits()
{
    int i;

    setcolor(DARKGRAY);

    for(i = 0; i < 8; i++)
    {
        ellipse(cx, cy, 0, 360,
                planets[i].orbit,
                planets[i].orbit / 2);
    }
}

// Asteroid belt between Mars and Jupiter
void drawAsteroids()
{
    int i, x, y, r;
    float a;

    for(i = 0; i < 110; i++)
    {
        a = i * 3.3 + frame * 0.12;

        r = 122 + (i % 11);

        x = cx + (int)(r * cos(a * PI / 180));

        y = cy + (int)(r * 0.5 *
                       sin(a * PI / 180));

        if(i % 4 == 0)
        {
            setcolor(LIGHTGRAY);
            circle(x, y, 2);
        }
        else
        {
            putpixel(x, y, LIGHTGRAY);
        }
    }
}

// Draw Earth Moon
void drawMoon()
{
    int ex, ey, mx, my;

    ex = cx + (int)(planets[2].orbit *
         cos(planets[2].angle * PI / 180));

    ey = cy + (int)(planets[2].orbit * 0.5 *
         sin(planets[2].angle * PI / 180));

    // Moon orbit
    setcolor(DARKGRAY);
    ellipse(ex, ey, 0, 360, 21, 11);

    mx = ex + (int)(21 * cos(moonAngle * PI / 180));

    my = ey + (int)(11 * sin(moonAngle * PI / 180));

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(mx, my, 3, 3);

    moonAngle += 3 * speed;

    if(moonAngle >= 360)
        moonAngle -= 360;
}

// Draw all planets
void drawPlanets()
{
    int i, x, y;

    for(i = 0; i < 8; i++)
    {
        x = cx + (int)(planets[i].orbit *
             cos(planets[i].angle * PI / 180));

        y = cy + (int)(planets[i].orbit * 0.5 *
             sin(planets[i].angle * PI / 180));

        // Saturn rings
        if(i == 5)
        {
            setcolor(LIGHTGRAY);
            ellipse(x, y, 0, 360, 34, 15);

            setcolor(LIGHTCYAN);
            ellipse(x, y, 0, 360, 30, 13);

            setcolor(WHITE);
            ellipse(x, y, 0, 360, 25, 11);
        }

        // Planet body
        setcolor(planets[i].color);
        setfillstyle(SOLID_FILL, planets[i].color);

        fillellipse(x, y,
                    planets[i].size,
                    planets[i].size);

        // Small surface detail
        if(i == 2 || i == 4)
        {
            putpixel(x - 4, y - 2, WHITE);
            putpixel(x + 3, y + 4, LIGHTGRAY);
        }


        // Planet name
        setcolor(WHITE);
        outtextxy(x + planets[i].size + 3,
                  y - 5, planets[i].name);

        // Move planets
        if(!paused)
        {
            planets[i].angle +=
                planets[i].rate * speed;

            if(planets[i].angle >= 360)
                planets[i].angle -= 360;
        }
    }
}

// Display information panel
void drawInfo()
{
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    outtextxy(130, 40, "SOLAR SYSTEM SIMULATION");

    setcolor(WHITE);

   // outtextxy(10, 30, "+ : Increase Speed");
   // outtextxy(10, 45, "- : Decrease Speed");
    //outtextxy(10, 60, "P : Pause / Resume");
    //outtextxy(10, 75, "ESC : Exit");

    if(paused)
        outtextxy(10, 95, "ANIMATION PAUSED");
}

// Main function
int main()
{
    int gd = DETECT, gm;
    int key;

    initgraph(&gd, &gm,
              "C:\\TURBOC3\\BGI");

    cx = getmaxx() / 2;
    cy = getmaxy() / 2 + 15;

    while(1)
    {
        if(kbhit())
        {
            key = getch();

            if(key == 27)
                break;

            if(key == 'p' || key == 'P')
                paused = !paused;

            if(key == '+' && speed < 4.0)
                speed += 0.2;

            if(key == '-' && speed > 0.2)
                speed -= 0.2;
        }

        cleardevice();

        drawStars();
        drawOrbits();
        drawAsteroids();
        drawSun();
        drawPlanets();
        drawMoon();
        drawInfo();

        if(!paused)
            frame++;

        delay(50);
    }
    closegraph();
    return 0;
}