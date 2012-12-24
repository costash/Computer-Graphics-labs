#include <stdlib.h>
#include <freeglut.h>
#include <stdio.h>
#include <stdarg.h>

GLuint delay = 25;	// delay 25 msec = 40 fps (max)

GLfloat aspect = 1.0;
int win_w = 1, win_h = 1;

// Pentru generare sol
GLfloat leftMargin = -2.2;
GLfloat rightMargin = 2.2;
GLfloat step = 0.05;
int levelOfDetail = 1;


	// componente material
	GLfloat diffuse4f [] = {0.5, 0.5, 0.5, 1.0};
	GLfloat ambient4f [] = {0.2, 0.2, 0.2, 1.0};
	GLfloat specular4f [] = {1.0, 1.0, 1.0, 1.0};
	GLfloat shininess = 64.0;
	
	// componente lumina
	// position = x,y,z,w -> w = 0 lumina omnidirectionala pozitionata la infinit; w = 1 lumina directionala la care poate fi setat spotul
	GLfloat position4f [] = {1.0, 1.0, 0.5, 0.0};
	GLfloat color4f [] = {1.0, 1.0, 1.0, 1.0};

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	const GLfloat globalAmbientColor4f [] = {0.2, 0.2, 0.2, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor4f);
	
	glEnable(GL_DEPTH_TEST);	// pornire depth test

	glShadeModel(GL_SMOOTH);	// model iluminare

	// setare parametrii lumina 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, color4f);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color4f);
	glLightfv(GL_LIGHT0, GL_POSITION, position4f);

	glEnable(GL_LIGHT0); // pornire lumina 0
}

void display(void)
{
	//Render objects
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// aplicare material
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse4f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient4f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular4f);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	// Desenare suprafata
	glBegin(GL_QUADS);

	for( float i = leftMargin ; i+(levelOfDetail*step)<=rightMargin; i+=(levelOfDetail*step))
		for(float j=leftMargin;j+(levelOfDetail*step)<=rightMargin; j+=(levelOfDetail*step))
		{
			glNormal3f(0.0,1.0,0.0);
			glVertex3f(i ,  -1, j);
			glVertex3f(i, -1, j+(levelOfDetail*step));
			glVertex3f(i+(levelOfDetail*step), -1, j+(levelOfDetail*step));
			glVertex3f(i+(levelOfDetail*step),  -1, j);
		}

	glEnd();
	
	static float spin = 0;
	spin += 0.1;

	//Desenare matrice de sfere
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			glPushMatrix();
				glTranslatef(-1.5 + i * 1, 0, -1.5 + j * 1.0);
				glRotatef(spin, 0,1,0);
				glutSolidSphere(0.45, 20, 20);
			glPopMatrix();
		}
	}
	
	glutSwapBuffers();
}

void timer(int value)
{
	glutTimerFunc(value, timer,value);
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0,0, (GLsizei) w, (GLsizei) h);

	aspect = (GLfloat) w / (GLfloat) h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 60.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(5.0, 5.0, 0.0,   // observator
		      0.0, 0.0, 0.0,   // centru
			  0.0, 1.0, 0.0);  

	win_w = w; 
	win_h = h;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: exit(0);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Lighting");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(delay, timer, delay);
	glutMainLoop();
	return 0;
}