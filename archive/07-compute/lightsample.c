// code taken from: https://compgraphics.info/OpenGL/lighting/light_sources.php

#include <glut.h>
#include <math.h>

#define PI 3.141592653

int light_sample = 1;

void init() {
    glClearColor(0.3, 0.3, 0.3, 0.0);
    glEnable(GL_LIGHTING);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.2, 1.2, -1.2, 1.2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat material_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
    switch (light_sample) {
    case 1: {
        // directed light
        GLfloat light0_diffuse[] = { 0.4, 0.7, 0.2 };
        GLfloat light0_direction[] = { 0.0, 0.0, 1.0, 0.0 };
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
        break;
    }
    case 2: {
        // point light
        GLfloat light1_diffuse[] = { 0.4, 0.7, 0.2 };
        GLfloat light1_position[] = { 0.0, 0.0, 1.0, 1.0 };
        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
        glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
        break;
    }
    case 3: {
        // point light      
        // f(d) = 1.0 / (0.4 * d * d + 0.2 * d)
        GLfloat light2_diffuse[] = { 0.4, 0.7, 0.2 };
        GLfloat light2_position[] = { 0.0, 0.0, 1.0, 1.0 };
        glEnable(GL_LIGHT2);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
        glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.0);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.2);
        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.4);
        break;
    }
    case 4: {
        // light spot, half angle of 30 deg
        GLfloat light3_diffuse[] = { 0.4, 0.7, 0.2 };
        GLfloat light3_position[] = { 0.0, 0.0, 1.0, 1.0 };
        GLfloat light3_spot_direction[] = { 0.0, 0.0, -1.0 };
        glEnable(GL_LIGHT3);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
        glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
        glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30);
        glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_spot_direction);
        break;
    }
    case 5: {
        // light spot
        GLfloat light4_diffuse[] = { 0.4, 0.7, 0.2 };
        GLfloat light4_position[] = { 0.0, 0.0, 1.0, 1.0 };
        GLfloat light4_spot_direction[] = { 0.0, 0.0, -1.0 };
        glEnable(GL_LIGHT4);
        glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
        glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
        glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 30);
        glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light4_spot_direction);
        glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 15.0);
        break;
    }
    case 6: {
        // several sources
        GLfloat light5_diffuse[] = { 1.0, 0.0, 0.0 };
        GLfloat light5_position[] =
            { 0.5 * cos(0.0), 0.5 * sin(0.0), 1.0, 1.0 };
        glEnable(GL_LIGHT5);
        glLightfv(GL_LIGHT5, GL_DIFFUSE, light5_diffuse);
        glLightfv(GL_LIGHT5, GL_POSITION, light5_position);
        glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.0);
        glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.4);
        glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.8);
        GLfloat light6_diffuse[] = { 0.0, 1.0, 0.0 };
        GLfloat light6_position[] =
            { 0.5 * cos(2 * PI / 3), 0.5 * sin(2 * PI / 3), 1.0, 1.0 };
        glEnable(GL_LIGHT6);
        glLightfv(GL_LIGHT6, GL_DIFFUSE, light6_diffuse);
        glLightfv(GL_LIGHT6, GL_POSITION, light6_position);
        glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 0.0);
        glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.4);
        glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.8);
        GLfloat light7_diffuse[] = { 0.0, 0.0, 1.0 };
        GLfloat light7_position[] =
            { 0.5 * cos(4 * PI / 3), 0.5 * sin(4 * PI / 3), 1.0, 1.0 };
        glEnable(GL_LIGHT7);
        glLightfv(GL_LIGHT7, GL_DIFFUSE, light7_diffuse);
        glLightfv(GL_LIGHT7, GL_POSITION, light7_position);
        glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, 0.0);
        glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.4);
        glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, 0.8);
        break;
    }
    }

    // plane
    GLfloat x, y;
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, -1.0);
    for (x = -1.0; x < 1.0; x += 0.005) {
        for (y = -1.0; y < 1.0; y += 0.005) {
            glVertex3f(x, y, 0.0);
            glVertex3f(x, y + 0.005, 0.0);
            glVertex3f(x + 0.005, y + 0.005, 0.0);
            glVertex3f(x + 0.005, y, 0.0);
        }
    }

    glEnd();

    // disable lights
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);

    glutSwapBuffers();
}

void keyboard_function(unsigned char key, int x, int y) {
    if (key > '0' && key < '7')
      light_sample = key - '0';
  glutPostRedisplay();
}

void main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow
        ("Light sources in openGL (с) compgraphics.info");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard_function);
    glutMainLoop();
}
