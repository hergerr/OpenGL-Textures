#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int model = 1;
static GLfloat theta[] = {0.0, 0.0, 0.0}; // trzy kąty obrotu

int getRand(int a, int b)
{
    return a + rand() % (b - a);
}

struct Point
{
    float x, y, z;
    int a, b, c;
};

const int N = 21;
vector<vector<Point>> points;

void spinEgg()
{

    theta[0] -= 0.5;
    if (theta[0] > 360.0)
        theta[0] -= 360.0;

    theta[1] -= 0.5;
    if (theta[1] > 360.0)
        theta[1] -= 360.0;

    theta[2] -= 0.5;
    if (theta[2] > 360.0)
        theta[2] -= 360.0;

    glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);

    glBegin(GL_TRIANGLES); // Narysowanie zielonego trójkąta
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 7.0f);
    glVertex2f(7.0f, 0.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);

    glFlush();
    glutSwapBuffers();
}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania

void MyInit(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
    // współczynniki ka =[kar,kag,kab] dla światła otoczenia

    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    // współczynniki kd =[kdr,kdg,kdb] światła rozproszonego

    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    // współczynniki ks =[ksr,ksg,ksb] dla światła odbitego

    GLfloat mat_shininess = {20.0};
    // współczynnik n opisujący połysk powierzchni

    /*************************************************************************************/
    // Definicja źródła światła

    GLfloat light_position[] = {0.0, 0.0, 10.0, 1.0};
    // położenie źródła

    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
    // składowe intensywności świecenia źródła światła otoczenia
    // Ia = [Iar,Iag,Iab]

    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    // składowe intensywności świecenia źródła światła powodującego
    // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    // składowe intensywności świecenia źródła światła powodującego
    // odbicie kierunkowe Is = [Isr,Isg,Isb]

    GLfloat att_constant = {1.0};
    // składowa stała ds dla modelu zmian oświetlenia w funkcji
    // odległości od źródła

    GLfloat att_linear = {0.05};
    // składowa liniowa dl dla modelu zmian oświetlenia w funkcji
    // odległości od źródła

    GLfloat att_quadratic = {0.001};
    // składowa kwadratowa dq dla modelu zmian oświetlenia w funkcji
    // odległości od źródła

    /*************************************************************************************/
    // Ustawienie parametrów materiału i źródła światła

    /*************************************************************************************/
    // Ustawienie patrametrów materiału

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    /*************************************************************************************/
    // Ustawienie parametrów źródła

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

    /*************************************************************************************/
    // Ustawienie opcji systemu oświetlania sceny

    glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
    glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny
    glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
    glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora

    // Kolor czyszcący (wypełnienia okna) ustawiono na czarny
}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    GLfloat AspectRatio;
    if (vertical == 0) // Zabezpieczenie przed dzieleniem przez 0
        vertical = 1;
    glViewport(0, 0, horizontal, vertical);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    if (horizontal <= vertical)

        glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
    else

        glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keys(unsigned char key, int x, int y)
{

    if (key == 'p')
        model = 1;
    if (key == 'w')
        model = 2;
    if (key == 's')
        model = 3;

    RenderScene(); // przerysowanie obrazu sceny
}

int main(int argc, char *argv[])
{

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(300, 300);

    glutInit(&argc, argv);
    glutCreateWindow("Uklad wspolrzednych 3D");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    MyInit();
    glutKeyboardFunc(keys);
    glutIdleFunc(spinEgg);

    glEnable(GL_DEPTH_TEST);
    // Włączenie mechanizmu usuwania powierzchni niewidocznych

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT

    return 0;
}
