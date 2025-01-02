#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#define DR (M_PI/180.0)   // one degree in radians

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080

#define NUM_RAYS 1920
#define MOVE_SPEED 0.05f
#define ROT_SPEED 0.01f

int mapX = 16, mapY = 16;
int mapS = 32; 
bool keys[256] = { false };

int map[] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1,
    1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

float px, py;   // player position
float pdx, pdy; // player delta x/y
float pa;       // player angle

float dist(float ax, float ay, float bx, float by)
{
    return sqrtf((bx - ax)*(bx - ax) + (by - ay)*(by - ay));
}

void drawMap2D()
{
    int x, y;
    for (y = 0; y < mapY; y++)
    
    {
        for (x = 0; x < mapX; x++)
        {
            if (map[y * mapX + x] == 1)
                glColor3f(1, 1, 1);  // wall
            else
                glColor3f(0, 0, 0);  // empty space

            int xo = x * mapS;
            int yo = y * mapS;

            glBegin(GL_QUADS);
                glVertex2i(xo + 1,     yo + 1);
                glVertex2i(xo + 1,     yo + mapS - 1);
                glVertex2i(xo + mapS - 1, yo + mapS - 1);
                glVertex2i(xo + mapS - 1, yo + 1);
            glEnd();
        }
    }
}

void drawPlayer2D()
{
    glColor3f(1,1,0);
    glPointSize(8);
    glBegin(GL_POINTS);
        glVertex2i((int)px, (int)py);
    glEnd();

    // draw facing direction line
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex2i((int)px, (int)py);
        glVertex2i((int)(px + pdx * 5), (int)(py + pdy * 5));
    glEnd();
}

void drawRays2D_3D()
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo, disH, hx, hy, disV, vx, vy;
    float nTan, aTan, disT;
    
    float fovHalf = 30 * DR;
    ra = pa - fovHalf;
    if (ra < 0)        ra += 2.0f * M_PI;
    if (ra > 2.0f * M_PI) ra -= 2.0f * M_PI;

    float columnWidth = (float)SCREEN_WIDTH / (float)NUM_RAYS;

    for (r = 0; r < NUM_RAYS; r++)
    {

        dof = 0;
        disH = 1e9; 
        hx = px; 
        hy = py;
        aTan = -1 / tanf(ra);

        if (ra > M_PI)  // looking up
        {
            ry = (((int)py >> 5) << 5) - 0.0001f;
            rx = (py - ry) * aTan + px;
            yo = -mapS;
            xo = -yo * aTan;
        }
        else if (ra < M_PI)  // looking down
        {
            ry = (((int)py >> 5) << 5) + mapS;
            rx = (py - ry) * aTan + px;
            yo = mapS;
            xo = -yo * aTan;
        }
        else  // looking straight left or right
        {
            rx = px;
            ry = py;
            dof = mapY; 
        }

        while (dof < mapY)
        {
            mx = (int)(rx) >> 5; 
            my = (int)(ry) >> 5;
            mp = my * mapX + mx;
            if (mp >= 0 && mp < mapX*mapY && map[mp] == 1)
            {
                hx = rx; 
                hy = ry; 
                disH = dist(px, py, hx, hy);
                break;
            }
            rx += xo;
            ry += yo;
            dof += 1;
        }

        dof = 0;
        disV = 1e9; 
        vx = px; 
        vy = py;
        nTan = -tanf(ra);

        if (ra > M_PI/2 && ra < 3*M_PI/2) // looking left
        {
            rx = (((int)px >> 5) << 5) - 0.0001f;
            ry = (px - rx) * nTan + py;
            xo = -mapS;
            yo = -xo * nTan;
        }
        else if (ra < M_PI/2 || ra > 3*M_PI/2) // looking right
        {
            rx = (((int)px >> 5) << 5) + mapS;
            ry = (px - rx) * nTan + py;
            xo = mapS;
            yo = -xo * nTan;
        }
        else // looking straight up or down
        {
            rx = px;
            ry = py;
            dof = mapX;
        }

        while (dof < mapX)
        {
            mx = (int)(rx) >> 5; 
            my = (int)(ry) >> 5;
            mp = my * mapX + mx;
            if (mp >= 0 && mp < mapX*mapY && map[mp] == 1)
            {
                vx = rx; 
                vy = ry; 
                disV = dist(px, py, vx, vy);
                break;
            }
            rx += xo;
            ry += yo;
            dof += 1;
        }

        if (disV < disH)
        {
            rx = vx; 
            ry = vy; 
            disT = disV;
        }
        else
        {
            rx = hx; 
            ry = hy; 
            disT = disH;
        }

        // (Optional) Draw the 2D ray line ? if you want to see it in the top-left mini-map.
        /*
        glColor3f(1, 0, 0);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2i((int)px, (int)py);
        glVertex2i((int)rx, (int)ry);
        glEnd();
        */

        float ca = pa - ra;
        if (ca < 0)        ca += 2.0f * M_PI;
        if (ca > 2.0f * M_PI) ca -= 2.0f * M_PI;
        disT = disT * cosf(ca);

        float lineH = (mapS * (SCREEN_HEIGHT / 2.0f)) / (disT);
        if (lineH > SCREEN_HEIGHT) lineH = SCREEN_HEIGHT;

        float lineOffset = (SCREEN_HEIGHT / 2.0f) - (lineH / 2.0f);

        glColor3f(1, 1, 1);
        float xPosition = r * columnWidth + (columnWidth * 0.5f); // center the column a bit
        glLineWidth(columnWidth);
        glBegin(GL_LINES);
        glVertex2i((int)xPosition, (int)lineOffset);
        glVertex2i((int)xPosition, (int)(lineOffset + lineH));
        glEnd();

        ra += (60.0f * DR) / (float)NUM_RAYS;
        if (ra < 0)        ra += 2.0f * M_PI;
        if (ra > 2.0f * M_PI) ra -= 2.0f * M_PI;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawRays2D_3D();

    glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y)
{
    keys[key] = true;
}

void keyUp(unsigned char key, int x, int y)
{
    keys[key] = false;
}

void update()
{
    // Turn left (A)
    if (keys['a'] || keys['A'])
    {
        pa -= ROT_SPEED;
        if (pa < 0) pa += 2.0f * M_PI;
        pdx = cosf(pa) * 5.0f;
        pdy = sinf(pa) * 5.0f;
    }

    // Turn right (D)
    if (keys['d'] || keys['D'])
    {
        pa += ROT_SPEED;
        if (pa > 2.0f * M_PI) pa -= 2.0f * M_PI;
        pdx = cosf(pa) * 5.0f;
        pdy = sinf(pa) * 5.0f;
    }

    // Move forward (W)
    if (keys['w'] || keys['W'])
    {
        px += pdx * MOVE_SPEED;
        py += pdy * MOVE_SPEED;
    }

    // Move backward (S)
    if (keys['s'] || keys['S'])
    {
        px -= pdx * MOVE_SPEED;
        py -= pdy * MOVE_SPEED;
    }

    glutPostRedisplay(); // request a redraw
}

void init()
{
    glClearColor(0.1f, 0.1f, 0.1f, 0);
    gluOrtho2D(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    px = 300;
    py = 300;
    pa = 0.0f;
    pdx = cosf(pa) * 5.0f;
    pdy = sinf(pa) * 5.0f;
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Raycaster");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(update);
    glutMainLoop();
    return 0;
}

