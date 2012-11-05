//programul deseneaza figuri care incep sa se roteasca la apasarea
//butonului din stanga al mouse-ului si se opresc la apasarea
//butonului din dreapta.
#include <cstdlib>
#include "dependente\freeglut.h"
#include <iostream>

//pasul unghiular de rotatie
GLfloat spin=0.0;
//pasul de scalare
GLfloat pas=0.0;
//factorul de scalare
GLfloat factor=1.0;
GLfloat aspect = 1;
//mode=0 => wireframe, mode=1 =>solid
GLint mode=0;
//pozitia obiectului selectat
GLint pos_x=1,pos_y=1;

int win_w = 1, win_h = 1;


void init(void)
{	//specifica valorile r,g,b,a folosite atunci cand se sterge bufferul de culoare cu glClear
	glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
}

//functia de afisare
void display(void)
{
	//initializeaza bufferul de culoare
	glClear(GL_COLOR_BUFFER_BIT);

	int i,j;

	//Desenare matrice de cuburi
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
		{
			glPushMatrix();
			
			if(i==0)
				//rosu
				glColor3f(1.0, 0.0, 0.0);
			if(i==1)
				//verde
				glColor3f(0.0, 1.0, 0.0);
			if(i==2)
				//cyan
				glColor3f(0.0, 1.0, 1.0);

			//obiectul selectat e colorat in alb
			if (i==pos_x && j==pos_y)
				glColor3f(1.0,1.0,1.0);

			//Pozitia initiala a obiectului
			glTranslatef((float)(3-(i*3)),(float)(2-(j*2.5)),5.0f);

			
			
			
			if (i==0)
			{
				//animatia - data de unghiul spin care se modifica in functia animatieDisplay()
				glRotatef(spin, 0.0, 1.0, 0.0);

				if (mode==0)
					//se deseneaza un cub wireframe de latura 1
					glutWireCube(1);
				else
					//se deseneaza un cub solid de latura 1
					glutSolidCube(1);
			}
			if (i==1)
			{
				glTranslatef(0,pas,0);
				if (mode==0)
					glutWireSphere(0.75,20,20);
				else
					glutSolidSphere(0.75,20,20);
			}
			if (i==2)
			{
				glScalef(factor,factor,factor);
				if (mode==0)
					glutWireTorus(0.25,0.5,20,20);
				else
					glutSolidTorus(0.25,0.5,20,20);
			}
					
			glPopMatrix();
		
		}

	glutSwapBuffers();
}

void animatieDisplay()
{
	static int iteratie=0;
	static int dir=1;
	spin=spin+1;

	if(spin>360.0)
		spin= spin -360.0f;

	iteratie++;
	if (iteratie==40)
	{
		iteratie=0;
		dir*=-1;
	}
	if (dir==1)
	{
		pas+=0.05;
		factor+=0.01;
	}
	else
	{
		pas-=0.05;
		factor-=0.01;
	}

	



	
	glutPostRedisplay();
}


void reshape(int w, int h)
{
	//transformarea in poarta de vizualizare
	glViewport(0,0, (GLsizei) w, (GLsizei) h);

	aspect = (GLfloat) w / (GLfloat) h;

	//se lucreaza pe matricea de proiectie
	glMatrixMode(GL_PROJECTION);
	//se porneste de la matricea identitate
	glLoadIdentity();
	//transformarea de proiectie - specifica si volumul de vizualizare
	gluPerspective(90.0f, aspect, 0.1f, 60.0f);

	//se lucreaza pe matricea de modelare vizualizare
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 0.0,   //observatorul este in origine
		      0.0, 0.0, 1.0,   //si priveste in directia pozitiva a axei oz
			  0.0, 1.0, 0.0);  

	win_w = w;
	win_h = h;


}

void special(int key,int x,int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		pos_y--;
		if (pos_y<0)
			pos_y=2;
		break;
	case GLUT_KEY_DOWN:
		pos_y++;
		if (pos_y>2)
			pos_y=0;
		break;
	case GLUT_KEY_LEFT:
		pos_x--;
		if (pos_x<0)
			pos_x=2;
		break;
	case GLUT_KEY_RIGHT:
		pos_x++;
		if (pos_x>2)
			pos_x=0;
		break;
	}
	//cere redesenarea scenei
	glutPostRedisplay();
}

void keyboard(unsigned char key,int x,int y)
{
	
	switch(key)
	{
	case 27:
		exit(0);
	case 's':
		mode=0;
		break;
	case 'w':
		mode=1;
		break;
	}
	//cere redesenarea scenei
	glutPostRedisplay();
}

//functie apelata la actionarea mouse-ului
void mouse(int buton, int stare, int x, int y)
{
	switch(buton)
	{
	case GLUT_LEFT_BUTTON:
		if(stare == GLUT_DOWN)
			//functia idle animatieDisplay() se apeleaza oricand nu sunt evenimente
			glutIdleFunc(animatieDisplay);
		break;
	case GLUT_RIGHT_BUTTON:
		if(stare== GLUT_DOWN)
			//functia idle este dezactivata 
			glutIdleFunc(NULL);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Animatie");
	init();
	
	//functia de afisare se numeste display()
	glutDisplayFunc(display);
	
	//functia reshape() este apelata la redimensionarea ferestrei de afisare 
	//sau cand sistemul determina ca s-a acoperit (partial) fereastra
	glutReshapeFunc(reshape);
	
	//functia mouse() se apeleaza la actionarea mouse-ului
	glutMouseFunc(mouse);

	//functia keyboard() se apeleaza la apasarea unei taste
	glutKeyboardFunc(keyboard);

	//functia special() se apeleaza la apasarea unei taste speciale
	glutSpecialFunc(special);



	//contine o bucla in care aplicatia asteapta evenimente
	glutMainLoop();

	
	return 0;
}