// 
// Laborator 8 -- EGC
// 

#include <windows.h>
#include <stdarg.h>
#include <freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Cube.h"
#include "Ground.h"

#define M_PI       3.14159265358979323846


// identificatori pentru stiva de obiecte (necesari pentru selectie)
#define NONE			0
#define CUB1			1
#define CUB2			2
#define GROUND_NAME		3

// tip vedere
#define VEDERE_FATA		0
#define VEDERE_SUS		1

// initial nici un obiect nu este selectat
int obiect = NONE;

// rotatia initiala a obiectelor
GLfloat spin=0.0;

GLfloat tr=0.0;

GLfloat dir=1;

int win_w = 1, win_h = 1;
GLfloat aspect = 1;

#define WIRE	0
#define SOLID	1

#define PROJECTIVE_SHADOW	0
#define VOLUME_SHADOW		1

// initial umbra este de tipul projective
int shadow_type = PROJECTIVE_SHADOW;
int shadow_enabled = 0;


// modul initial in care sunt desenate poligoanele
int mode = SOLID;

// vederea initiala
int vedere = VEDERE_FATA;

int mainWindow;

// solul
Ground *ground = NULL;

// vectorul suprafetelor 
Cube* objects[2];

// matricea folosita pentru calculul umbrelor proiectate
GLfloat groundshadow[4][4];
GLfloat plan[4];

GLfloat light_position0[] = { -2.0f, 9.5f, 0.0f, 1.0f };				//seteaza pozitia

// nume variabile pentru ecuatiile matematice

#define X 0
#define Y 1
#define Z 2
#define W 3

#define A 0
#define B 1
#define C 2
#define D 3


void
calcplan(GLfloat plane[4],
  GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
  GLfloat vec0[3], vec1[3];

  vec0[X] = v1[X] - v0[X];
  vec0[Y] = v1[Y] - v0[Y];
  vec0[Z] = v1[Z] - v0[Z];

  vec1[X] = v2[X] - v0[X];
  vec1[Y] = v2[Y] - v0[Y];
  vec1[Z] = v2[Z] - v0[Z];

  plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
  plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
  plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

  plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}

// calculeaza matricea care va proiecta umbra - umbre projective
void
shadowmatrix(GLfloat m[4][4],
  GLfloat groundplane[4],
  GLfloat lightpos[4])
{
  
	
  GLfloat dot;

  
    // produsul vectorial intre normala la plan si vectorul pozitiei luminei
  dot = groundplane[X] * lightpos[X] +
    groundplane[Y] * lightpos[Y] +
    groundplane[Z] * lightpos[Z] +
    groundplane[W] * lightpos[W];

  m[0][0] = dot - lightpos[X] * groundplane[X];
  m[1][0] = 0.f - lightpos[X] * groundplane[Y];
  m[2][0] = 0.f - lightpos[X] * groundplane[Z];
  m[3][0] = 0.f - lightpos[X] * groundplane[W];

  m[X][1] = 0.f - lightpos[Y] * groundplane[X];
  m[1][1] = dot - lightpos[Y] * groundplane[Y];
  m[2][1] = 0.f - lightpos[Y] * groundplane[Z];
  m[3][1] = 0.f - lightpos[Y] * groundplane[W];

  m[X][2] = 0.f - lightpos[Z] * groundplane[X];
  m[1][2] = 0.f - lightpos[Z] * groundplane[Y];
  m[2][2] = dot - lightpos[Z] * groundplane[Z];
  m[3][2] = 0.f - lightpos[Z] * groundplane[W];

  m[X][3] = 0.f - lightpos[W] * groundplane[X];
  m[1][3] = 0.f - lightpos[W] * groundplane[Y];
  m[2][3] = 0.f - lightpos[W] * groundplane[Z];
  m[3][3] = dot - lightpos[W] * groundplane[W];
  
  

}


// extrudere vertex folosind pozitia luminii si distanta t
void extinde(GLfloat n[3], GLfloat light[3], GLfloat vertex[3], GLfloat t)
{
  GLfloat delta[3];

  delta[X] = (vertex[X] - light[X]);
  delta[Y] = (vertex[Y] - light[Y]);
  delta[Z] = (vertex[Z] - light[Z]);

  
  n[X] = light[X] + delta[X] * t;
  n[Y] = light[Y] + delta[Y] * t;
  n[Z] = light[Z] + delta[Z] * t;
  
}

// calculeaza fetele volumului de umbra in functie muchiile poligoanelor de umbra (shadower)
void makeShadowVolume(Cube * shadower, GLfloat light[3],
  GLfloat t, int lines)
{
  int i;
  GLfloat v1[3];
  GLfloat v2[3];
  GLfloat v3[3];
  GLfloat v4[3];

  GLfloat *vertices = shadower->getVertices();

  int n = shadower->getNrVertices();

  glPushMatrix();
  //glDisable(GL_LIGHTING);

  glColor3f(.2f, .8f, .4f);
  for (i = 0; i < n; i++) 
  {
	GLfloat tempx,tempz;
	GLfloat angle;

	angle = (shadower->rotation[1] * M_PI )/ 180.0f;

	// Trebuie rotite numai varfurile de sus care apoi se extind : adica v1 si v4 (v2 si v3 se vor extinde)
	v1[0] = vertices[i * 3];v1[1] = vertices[i * 3+1];v1[2] = vertices[i * 3+2];
	tempx = v1[0];tempz = v1[2];
	v1[0] = tempx*cos(angle)+tempz*sin(angle);
	v1[2] = tempz*cos(angle)-tempx*sin(angle);

	extinde(v2, light, v1, t);

	v4[0] = vertices[((i + 1) % 4 + i/4 * 4) * 3];
	v4[1] = vertices[((i + 1) % 4 + i/4 * 4 ) * 3+1];
	v4[2] = vertices[((i + 1) % 4 + i/4 * 4 ) * 3+2];
	tempx = v4[0];
	tempz = v4[2];
	v4[0] = tempx*cos(angle)+tempz*sin(angle);
	v4[2] = tempz*cos(angle)-tempx*sin(angle);

	extinde(v3, light, v4, t);

	if(!lines)
		glBegin(GL_QUADS);
	else
	 glBegin(GL_LINES);

    glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(v3);
    glVertex3fv(v4);

	glEnd();
	
  }
  
  
  glPopMatrix();
  
}


void init(void)
{
	glClearColor(0.5, 0.5, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

		
	GLfloat light_ambient0 [] = { 1.0, 1.0, 1.0, 1.0 };				//seteaza componenta ambientala
	GLfloat light_difusion0[] = { 1.0, 1.0, 1.0, 1.0 };				//seteaza componenta  de difuzie    
	GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };				//seteaza componenta speculara
	GLfloat direction0     [] = { 0.0, -1.0, 0.0};					//seteaza directia spotului
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);          
	glLightfv(GL_LIGHT0, GL_AMBIENT , light_ambient0);      
	glLightfv(GL_LIGHT0, GL_DIFFUSE , light_difusion0);          
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);      
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction0);      
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,5.0f);
	
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 30);


	// creaza si seteaza solul
	GLfloat specular[4]={0.5,0.5,0.5,1.0};	

	ground = new Ground(20);
	ground->setSpecular(specular);
	
	GLfloat color1[4]={0.0,1.0,0.0,1.0};
	ground->setColor(color1);

	// calculeaza matricile de umbrire pentru umbrele projective

	// trebuie sa avem ecuatia planului solului
	// pentru acest lucru avem nevoie de 3 puncte din planul acestuia
	// solul este desenat in planul Y = -5
	GLfloat P1[3], P2[3], P3[3];

	//P1
	P1[X] = 10.0f;
	P1[Y] = -5.0f;
	P1[Z] = 10.0f;

	//P3
	P2[X] = 10.0f;
	P2[Y] = -5.0f;
	P2[Z] = -10.0f;

	//P3
	P3[X] = -10.0f;
	P3[Y] = -5.0f;
	P3[Z] = -10.0f;

	
	calcplan(plan, P1, P2, P3);

	// calculeaza matricea necesara pentru proiectia umbrei
	// aceasta matrice va fi folosita alaturi de matricea de modelare a obiectului pentru a "proiecta" umbra
	// pe planul calculat
	
	shadowmatrix(groundshadow, plan, light_position0);
		
 
	// cub 1
	objects[0] = new Cube(0.0,0.0,0.0,4.0);
	objects[0]->setSpecular(specular);

	GLfloat color2[4]={1.0,1.0,0.0,1.0};
	objects[0]->setColor(color2);

	// cub 2 
	objects[1] = new Cube(0.0,5.3,0.0,2.0);
	objects[1]->setSpecular(specular);

	GLfloat color3[4]={1.0,0.0,0.0,1.0};
	objects[1]->setColor(color3);


}



// functia pentru desenarea unui text 3D
void output(GLfloat x, GLfloat y, GLfloat z,GLfloat rotation[3],char *format,...)
{
	va_list args;

	char buffer[1024],*p;

	va_start(args,format);

	vsprintf(buffer, format, args);

	va_end(args);

	glPushMatrix();
	
	glTranslatef(x,y,z);

	glRotatef(rotation[0],1,0,0);
	glRotatef(rotation[1],0,1,0);
	glRotatef(rotation[2],0,0,1);

	glScalef(0.0045, 0.0045, 0.0); /* 0.1 to 0.001 as required */

	for (p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);

	glPopMatrix();
}

// deseneaza umbrele pentru obiectele din scena
void drawProjectiveShadows()
{

	// avem doar un cub
	int nr_obj = 2;
	if (shadow_enabled)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		ground->Draw();

		glNormal3f(0.f, 1.f, 0.f);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		// culoarea umbrei
		glColor3f(0.1, 0.1, 0.1);  

		glPushMatrix();
		glMultMatrixf((GLfloat *) groundshadow);
		
		for(int i=0;i<nr_obj;i++)
			objects[i]->Draw();
		
		glPopMatrix();
		

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

	}
}

void output2(GLfloat x, GLfloat y, char *format,...)
{
	va_list args;

	char buffer[1024],*p;

	va_start(args,format);

	vsprintf(buffer, format, args);

	va_end(args);

	glPushMatrix();
	
	glTranslatef(x,y,0);

	glRotatef(180,1,0,0);

	glScalef(0.08, 0.08, 0.0); /* 0.1 to 0.001 as required */

	for (p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);

	glPopMatrix();
}

void draw_my_hud(void)
{
	static int frame = 0, last_time = 0;
	static GLfloat fps = 0;
	int time;
	int startY = 13 , startX = 5;
	int pasY = 13;

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - last_time > 1000) {
		fps = frame * 1000.0 / (time - last_time);
		last_time = time;
		frame = 0;
	}

	/* begin text rendering mode */
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, win_w, win_h, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();
	glColor4f(1, 1, 1, 1);

	output2(win_w-26*4,startY, "FPS: %.1f",fps);

	output2(startX, startY, "Keys Usage  :");
	startY+=pasY;
	output2(startX, startY, "s/w         : Solid/Wireframe");
	startY+=pasY;

	if(shadow_enabled)
	{
		output2(startX, startY, "u           : Toggle shadows( current state ON )");
		startY+=pasY;
	}
	else
	{
		output2(startX, startY, "u           : Toggle shadows( current state OFF )");
		startY+=pasY;
	}

	output2(startX, startY, "q/a         : Creste/Scade diviziuni sol");
	startY+=pasY;
	
	output2(startX, startY, "1           : Privire \"Fata\"");
	startY+=pasY;
	output2(startX, startY, "2           : Privire \"Spate\"");
	startY+=pasY;
	output2(startX, startY, "Click stanga: Animatie/selectie obiecte");
	startY+=pasY;
	output2(startX, startY, "m           : Cycle shadow types");
	startY+=pasY;
	
	// shadow type
	switch (shadow_type)
	{
		case PROJECTIVE_SHADOW: 
			output2(startX, startY, "shadow type : Projective");
			startY+=pasY;
			break;
		case VOLUME_SHADOW :
			output2(startX, startY, "shadow type : Volume");
			startY+=pasY;
			break;

	}	

	// in functie de ce este selectat se afiseaza textul corespunzator
	switch (obiect)
	{
		case CUB1: 
			output2(startX, startY, "Obiect      : Cub 1");
			startY+=pasY;
			break;
		case CUB2: 
			output2(startX, startY, "Obiect      : Cub 2");
			startY+=pasY;
			break;
		case GROUND_NAME:
			output2(startX, startY, "Obiect      : Sol");
			startY+=pasY;
			break;
		case NONE:
			output2(startX, startY, "Obiect      : Nici unul");
			startY+=pasY;
			break;
		default:
			output2(startX, startY, "Obiect      : Altul(neprocesat in hit list)");
			startY+=pasY;
	}
	
	// Restore previous states
	glPopAttrib();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	/* end text rendering mode */
}

// functia pentru desenarea scenei 3D
void drawScene()
{
	// initilizeaza stiva de nume necesara pentru selectie
	glInitNames();

	// Shading model
	glShadeModel(GL_SMOOTH);

	// Activeaza iluminarea
	glEnable(GL_LIGHTING);

	// seteaza modul de desenare al poligoanelor
	if(mode == WIRE)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	if(mode == SOLID)
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// Desenare sol
	if(shadow_type != VOLUME_SHADOW )
		glEnable(GL_LIGHT0);
	

	if (shadow_type == PROJECTIVE_SHADOW)
	{
		glEnable(GL_STENCIL_TEST);
		// ceea ce se afiseaza va intra in stencil buffer (always pass)
		glStencilFunc(GL_ALWAYS, 1, 0xf);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	}

	glPushName(GROUND_NAME);
	ground->Draw();
	glPopName();

	//if (shadow_type == PROJECTIVE_SHADOW)
	//{
	//	// ceea ce se afiseaza va intra in stencil buffer (always pass)
	//	glStencilFunc(GL_ALWAYS, 0, 0x0);

	//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//}
	
	// operatiile cu umbre
	if(shadow_type == PROJECTIVE_SHADOW)
	{	
		glStencilFunc(GL_EQUAL, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawProjectiveShadows();

		glDisable(GL_STENCIL_TEST);
	}

	glPushName(CUB1);
	// Desenare cub 1
	objects[0]->Draw();
	glPopName();

	glPushName(CUB2);
	// Desenare cub 2
	objects[1]->Draw();
	glPopName();

		
	// draw sphere at light position
	glPushMatrix();
	glTranslatef(light_position0[X],light_position0[Y],light_position0[Z]);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(0.2,10,10);
	glPopMatrix();
	
}

// functia de display
void display(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glClearStencil(0);

	if(shadow_type == VOLUME_SHADOW && shadow_enabled )
	{
		// suntem in afara volumului de umbra
		//glClearStencil(0);
		glDisable(GL_LIGHT0);	
		drawScene();  
		
		// enable la stencil buffer
		glEnable(GL_STENCIL_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_CULL_FACE);

		// ceea ce se afiseaza va intra in stencil buffer (always pass)
		glStencilFunc(GL_ALWAYS, 0, 0x0);

		// raza din pixel care intra in volumul de umbra
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glCullFace(GL_BACK);  

		// nu se doreste afisarea quadurilor ce compun volumul de umbra
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		makeShadowVolume(objects[1], light_position0, 15.f,0);

		// raza din pixel care iese din volumul de umbra
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		glCullFace(GL_FRONT);  
		// nu se doreste afisarea quadurilor ce compun volumul de umbra
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		makeShadowVolume(objects[1], light_position0, 15.f,0);
		
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		// in afara volumului de umbra
		glStencilFunc(GL_EQUAL, 0, 0xf);  
		glEnable(GL_LIGHT0);
		drawScene();
						
		glDisable(GL_STENCIL_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
					
		makeShadowVolume(objects[1], light_position0, 15.f, 1);
		
	}
	else
		drawScene();

	draw_my_hud();
	glutSwapBuffers();
}


// functia de animatie ( functia de idle a GLUT-ului -- similara cu un thread separat)
void spinAnimation()
{
	spin=spin+0.5;

	if(spin>360.0)
		spin= spin -360.0;
		
	for(int i=0;i<2;i++)
	{
		GLfloat* rotation = objects[i]->getRotation();
		rotation[1]=spin;
		objects[i]->setRotation(rotation);
	}

	GLfloat* position= objects[0]->getPosition();

	if(position[0]>=8 || position[0]<=-8)
		dir *= -1;

	position[0]-=dir*0.1;
	
	objects[0]->setPosition(position);

  	glutPostRedisplay();
}

void vedereDinFata()
{
	int w,h;
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

	aspect = (GLfloat) w / (GLfloat) h;
	win_w = w;
	win_h = h;
	
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)w/h, 1.0, 40.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// vedere din FATA
	gluLookAt(0.0, 0.0, -15.0,   //observatorul este in origine departat pe Z
		      0.0, 0.0, 1.0,     //si priveste in directia pozitiva a axei oz
			  0.0, 1.0, 0.0);  

	init();

}

void vedereDeSus()
{
	int w,h;
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

	aspect = (GLfloat) w / (GLfloat) h;
	win_w = w;
	win_h = h;

	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)w/h, 1.0, 40.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// vedere de deasupra
	gluLookAt(0.0, 15.0, -5.0,   //observatorul este in origine departat pe Y
		      0.0, -1.0,  0.0,   //si priveste in directia negativa a axei oy
			  0.0, 0.0, 1.0);  

	init();

}

void processhits (GLint hits, GLuint buffer[])
{
   int i;
   GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

   ptr = (GLuint *) buffer;
   minZ = 0xffffffff;
   for (i = 0; i < hits; i++) {	
      names = *ptr;
	  ptr++;
	  if (*ptr < minZ) {
		  numberOfNames = names;
		  minZ = *ptr;
		  ptrNames = ptr+2;
	  }
	  
	  ptr += names+2;
  }

  ptr = ptrNames;
  
  obiect = *ptr;
     
}

void pick(int x, int y)
{
	GLuint buffer[1024];
	GLint nhits;

	GLint	viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(1024, buffer);
	
	// se intra in modul de selectie
	glRenderMode(GL_SELECT);

	// se renderizeaza doar o zona mica din jurul cursorului mouseului
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGetIntegerv(GL_VIEWPORT,viewport);
	gluPickMatrix(x,viewport[3]-y,1.0f,1.0f,viewport);

	gluPerspective(90,(viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]),0.1,1000);
	glMatrixMode(GL_MODELVIEW);

	// se va desena scena in modul de selectie (de fapt nimic nu este desenat propriu-zis : "desenarea" se face
	// in bufferul de selectie si nu pe ecran
	drawScene();

	glMatrixMode(GL_PROJECTION);						
	glPopMatrix();								
	glMatrixMode(GL_MODELVIEW);						
	nhits=glRenderMode(GL_RENDER);	
	
	// initial nici un obiect selectat
	obiect = NONE;

	// proceseaza bufferul de selectie si alege obiectul cel mai apropiat (daca exista)
	if(nhits != 0)
		processhits(nhits,buffer);
				
}

// functia pentru procesarea evenimentelor de la mouse
void mouse(int buton, int stare, int x, int y)
{
	switch(buton)
	{
	case GLUT_LEFT_BUTTON:
		if(stare == GLUT_DOWN)
		{
			// se efectueaza selectia unde a avut loc click-ul
			pick(x,y);
			// se porneste animatia
			glutIdleFunc(spinAnimation);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if(stare== GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	}
}

// functia pentru procesarea evenimentelor de la tastatura
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
			exit(0);
			break;
		case '1':
			vedereDinFata();
			vedere = VEDERE_FATA;
			glutPostRedisplay();
			break;
		case '2':
			vedereDeSus();
			vedere = VEDERE_SUS;
			glutPostRedisplay();
			break;
		case 'w':
			mode = WIRE;
			glutPostRedisplay();
			break;
		case 'W':
			mode = WIRE;
			glutPostRedisplay();
			break;
		case 's':
			mode = SOLID;
			glutPostRedisplay();
			break;
		case 'S':
			mode = SOLID;
			glutPostRedisplay();
			break;
		case 'a':
			if(ground != NULL)
				ground->setFactor(ground->getFactor()*2);
			glutPostRedisplay();
			break;
		case 'A':
			if(ground != NULL)
				ground->setFactor(ground->getFactor()*2);
			glutPostRedisplay();
			break;
		case 'q':
			if(ground != NULL)
				ground->setFactor(ground->getFactor()/2);
			glutPostRedisplay();
			break;
		case 'Q':
			if(ground != NULL)
				ground->setFactor(ground->getFactor()/2);
			glutPostRedisplay();
			break;
		case 'm':
			if(shadow_type == VOLUME_SHADOW)
				shadow_type = PROJECTIVE_SHADOW;
			else
				shadow_type = VOLUME_SHADOW;
			glutPostRedisplay();
			break;
		case 'M':
			if(shadow_type == VOLUME_SHADOW)
				shadow_type = PROJECTIVE_SHADOW;
			else
				shadow_type = VOLUME_SHADOW;
			glutPostRedisplay();
			break;
		case 'u':
			if(shadow_enabled)
				shadow_enabled = 0;
			else
				shadow_enabled = 1;
			glutPostRedisplay();
			break;
		
	
	}
}


void reshape(int w, int h)
{
	if(vedere==VEDERE_FATA)
		vedereDinFata();
	if(vedere==VEDERE_SUS)
		vedereDeSus();

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(100,100);
	
	mainWindow = glutCreateWindow("Animatie");
	
	init();
	glClearColor(0.5, 0.5, 1.0, 1.0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}