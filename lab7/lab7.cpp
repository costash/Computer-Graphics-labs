// EGC
// Laborator 7
//-------------------------------------------------
// W S    - miscare camera fata/spate
// A D    - miscare camera stanga/dreapta (strafe)

// [ ]    - selectie obiect stanga/dreapta
// < >    - modificare nivel de detaliu teren
// Sageti - miscare obiecte selectat

// Restul de actiuni : vezi meniul afisat in aplicatie

// ESC    - iesire din program
//-------------------------------------------------

// INCLUDES
//-------------------------------------------------
#include <stdlib.h>
#include <freeglut.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "Camera.h"
#include "Object3D.h"
#include "Plane.h"
#include "Light.h"
#include "Cube.h"
#include "Vector3D.h"
#include "Vector4D.h"

// tasta escape
#define ESC	27

float *Vector3D::arr;
float *Vector4D::arr;

// VARIABILE
//-------------------------------------------------
// numarul de obiecte
int objectCount;
// obiectul selectat
int selectedIndex = -1;

// camera
Camera *camera_front,*camera_top;
// vector de obiecte 3D
Cube *objects;
// planul de baza
Plane *ground;
// lumina omni
Light *light_o;
// lumina spot
Light *light_s;

// variabila pentru animatie
GLfloat spin=0.0;

// variabile necesare pentru listele de afisare
#define GROUND	1
int drawLists = 1;

// variabila folosita pentru a determina daca listele de afisare trebuiesc recalculate
int recomputeLists = 0;

// identificatori ferestre
// TODO : adaugati identificatori si pentru celelalte ferestre din scena
int fereastraStanga=-1,fereastraDreapta=-1,mainWindow;

// variabile necesare pentru picking
// TODO : adaugati definitii si pentru celelalte obiecte din scena
#define CUB1	1
#define CUB2	2
#define CUB3	3
#define PAMANT	4
#define LIGHTO	5
#define LIGHTS	6

// pe ce obiect a fost executat pick
int obiect = 0;

// FUNCTII
//-------------------------------------------------

// functie de initializare a setarilor ce tin de contextul OpenGL asociat ferestrei
void init(void)
{
	
	// Construieste listele de display
	glNewList(GROUND,GL_COMPILE);
	ground->Draw();
	glEndList();

	// pregatim o scena noua in opengl
	glClearColor(0.0, 0.0, 0.0, 0.0);	// stergem tot
	glEnable(GL_DEPTH_TEST);			// activam verificarea distantei fata de camera (a adancimii)
	glShadeModel(GL_SMOOTH);			// mod de desenare SMOOTH
	glEnable(GL_LIGHTING);				// activam iluminarea
	glEnable(GL_NORMALIZE);				// activam normalizarea normalelor
}

// functie de initializare a scenei 3D
void initScene(void)
{
	// initialize vector arrays
	Vector3D::arr = new float[3];
	Vector4D::arr = new float[4];

	// initializam camera pentru vedere top
	camera_top = new Camera();

	camera_top->SetPosition(new Vector3D(0,15,0));
	camera_top->SetForwardVector(new Vector3D(0,-1,0));
	camera_top->SetRightVector(new Vector3D(-1,0,0));
	camera_top->SetUpVector(new Vector3D(0,0,1));

	// initializam camera pentru vedere front ( cea default )
	camera_front = new Camera();

	// vrem 3 obiecte
	objectCount = 3;
	// initializam vectorul de 3 Object3D
	objects = new Cube[objectCount];

	// pentru fiecare obiect
	float offset = -2.0f;
	for( int index = 0; index < objectCount; index++ )
	{
		// setam culoarea rosie pentru Cubul 1
		if(index == 0)
			objects[index].SetDiffuse(new Vector4D(1.0,0.0,0.0,0.1));

		// setam culoarea galbena pentru Cubul 2
		if(index == 1)
			objects[index].SetDiffuse(new Vector4D(1.0,1.0,0.0,0.1));

		// setam culoarea albastra pentru Cubul 3
		if(index == 2)
			objects[index].SetDiffuse(new Vector4D(0.0,0.0,1.0,0.7));

		// pozitie relativa la centru
		objects[index].SetPosition(new Vector3D(-offset*(objectCount/2) + offset * index, 0, 1));
					
	}

	// initializam un plan de latura 5.0f
	ground = new Plane(5.0f);
	// culoare
	ground->SetColor(new Vector3D(0.0,0.05,0.0));
	// setam o grila de 1
	ground->SetLevelOfDetail(5);
	// sub nivelul obiectelor
	ground->SetPosition(new Vector3D(0,-2,0));
	// si wireframe
	ground->Wireframe = false;

	// initializam o noua lumina omnidirectionala
	light_o = new Light();
	// setam pozitia
	light_o->SetPosition(new Vector3D(-2, 0, 3));

	// initializam o noua lumina spot
	light_s = new Light();
	light_s->SetLightType(IlluminationType::Spot);
	// setam pozitia
	light_s->SetPosition(new Vector3D(0, -2.5, 0));
}

// functie pentru output text
void output(GLfloat x, GLfloat y, char *format,...)
{
	va_list args;

	char buffer[1024],*p;

	va_start(args,format);

	vsprintf(buffer, format, args);

	va_end(args);

	glPushMatrix();
	
	glTranslatef(x,y,-15);

	//glRotatef(180,0,1,0);

	glScalef(0.0035, 0.0035, 0.0); /* 0.1 to 0.001 as required */

	for (p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);

	glPopMatrix();
}

// AFISARE SCENA
//-------------------------------------------------

// functie de desenare a scenei 3D
void drawScene(void)
{
	if(glutGetWindow() == fereastraStanga)
	{
		// desenare text
		glColor3f(1.0,1.0,1.0);

		glDisable(GL_LIGHTING);
		glLineWidth(1.0);
		output(-7,10,"q:wireframe");
		output(-7,9,"z:solid");
		output(-7,8,"<|>:creste/scade diviziuni sol");
		output(-7,7,"x:activare/dezactivare liste de display");
		output(-7,6,"click stanga:animatie/selectie obiecte");
		if(drawLists)
			output(-7,5,"afisare : cu liste de display");
		else
			output(-7,5,"afisare : fara liste de display");
		
		// TODO : afisati mesaje corespunzator pentru selectia diferitelor obiecte din scena
		switch(obiect)
		{
			case CUB1:
				output(-7,4,"obiect selectat : cub Red");
				break;
			case CUB2:
				output(-7,4,"obiect selectat : cub Yellow");
				break;
			case CUB3:
				output(-7,4,"obiect selectat : cub Blue");
				break;
			case PAMANT:
				output(-7,4,"obiect selectat : ground");
				break;
			case LIGHTO:
				output(-7,4,"obiect selectat : light_o");
				break;
			case LIGHTS:
				output(-7,4,"obiect selectat : light_s");
				break;
			default:
				output(-7,4,"obiect selectat : nici unul");
		
		}
		glEnable(GL_LIGHTING);
	}

	// vizualizare observator in functie de fereastra

	if(glutGetWindow() == fereastraDreapta)
	{
		camera_top->Render();
	}

	if(glutGetWindow() == fereastraStanga)
	{
		camera_front->Render();
	}

	// activare lumina omnidirectionala
	light_o->Render();
	// activare lumina spot
	light_s->Render();
	
	// desenare obiecte
	for ( int i = 0 ; i < objectCount ; i ++ )
	{
		// rotatie OX
		if(i == 0)
			objects[i].SetRotation(new Vector3D(spin,0.0,0.0));

		// rotatie OY
		if(i == 1)
			objects[i].SetRotation(new Vector3D(0.0,spin,0.0));

		// rotatie OZ
		if(i == 2)
			objects[i].SetRotation(new Vector3D(0.0,0.0,spin));

		// TODO : implementati mecanismul de picking folosind functiile deja definite in cod
		// TODO : hint ( implementati folosirea stivei de nume )
		glPushName(i + 1);
		objects[i].Draw();
		glPopName();

		// desenare
		objects[i].Draw();
	}

	// desenare plan
	// TODO: selectia trebuie implementata si pentru plan 
	glPushName(PAMANT);
	if(drawLists)
	{
		glCallList(GROUND);
	}
	else
	{
		ground->Draw();
	}
	glPopName();

	// desenare lumina (deseneaza o sfera acolo unde se afla lumina, nu face nimic altceva)
	// TODO: selectia trebuie implementata si pentru lumini
	glPushName(LIGHTO);
	light_o->Draw();
	glPopName();
	
	// dezactivare lumina
	light_o->Disable();

	// desenare lumina (deseneaza o sfera acolo unde se afla lumina, nu face nimic altceva)
	// TODO: selectia trebuie implementata si pentru lumini 
	glPushName(LIGHTS);
	light_s->Draw();
	glPopName();

	// dezactivare lumina
	light_s->Disable();

	// double buffering
	// glutSwapBuffers();

	// redeseneaza scena
	// glutPostRedisplay();
}

// functie de desenare (se apeleaza cat de repede poate placa video)
// se va folosi aceeasi functie de display pentru toate subferestrele ( se puteau folosi si functii distincte 
// pentru fiecare subfereastra )
void display(void)
{
	// stergere ecran
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(glutGetWindow() == mainWindow)
		return;

	// Reconstruieste listele de display pentru ambele subferestre
	if(recomputeLists)
	{
		glNewList(GROUND,GL_COMPILE);
		ground->Draw();
		glEndList();

		recomputeLists--;
	}

	// TODO : implementati mecanismul de transparenta folosind ALPHA TESTING / BLENDING
	// First Pass - alpha test
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL, GL_ONE);
	drawScene();
	
	// Second Pass - blending
	glAlphaFunc(GL_LESS, GL_ONE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	glDepthMask(GL_FALSE);
	drawScene();
	
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	// double buffering
	glutSwapBuffers();

	// redeseneaza scena
	glutPostRedisplay();
		
}

// PICKING
//-------------------------------------------------

// functia care proceseaza hitrecordurile pentru a vedea daca s-a click pe un obiect din scena
void processhits (GLint hits, GLuint buffer[])
{
   int i;
   GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

   // pointer la inceputul bufferului ce contine hit recordurile
   ptr = (GLuint *) buffer;
   // se doreste selectarea obiectului cel mai aproape de observator
   minZ = 0xffffffff;
   for (i = 0; i < hits; i++) 
   {
      // numarul de nume numele asociate din stiva de nume
      names = *ptr;
	  ptr++;
	  // Z-ul asociat hitului - se retine 
	  if (*ptr < minZ) {
		  numberOfNames = names;
		  minZ = *ptr;
		  // primul nume asociat obiectului
		  ptrNames = ptr+2;
	  }
	  
	  // salt la urmatorul hitrecord
	  ptr += names+2;
  }

  // identificatorul asociat obiectului
  ptr = ptrNames;
  
  obiect = *ptr;
     
}

// functie ce realizeaza picking la pozitia la care s-a dat click cu mouse-ul
void pick(int x, int y)
{
	// buffer de selectie
	GLuint buffer[1024];

	// numar hituri
	GLint nhits;

	// coordonate viewport curent
	GLint	viewport[4];

	// se obtin coordonatele viewportului curent
	glGetIntegerv(GL_VIEWPORT, viewport);
	// se initializeaza si se seteaza bufferul de selectie
	memset(buffer,0x0,1024);
	glSelectBuffer(1024, buffer);
	
	// intrarea in modul de selectie
	glRenderMode(GL_SELECT);

	// salvare matrice de proiectie curenta
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// se va randa doar intr-o zona din jurul cursorului mouseului de [1,1]
	glGetIntegerv(GL_VIEWPORT,viewport);
	gluPickMatrix(x,viewport[3]-y,1.0f,1.0f,viewport);

	gluPerspective(45,(viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]),0.1,1000);
	glMatrixMode(GL_MODELVIEW);

	// se "deseneaza" scena : de fapt nu se va desena nimic in framebuffer ci se va folosi bufferul de selectie
	drawScene();

	// restaurare matrice de proiectie initiala
	glMatrixMode(GL_PROJECTION);						
	glPopMatrix();				

	glMatrixMode(GL_MODELVIEW);
	// restaurarea modului de randare uzual si obtinerea numarului de hituri
	nhits=glRenderMode(GL_RENDER);	
	
	// procesare hituri
	if(nhits != 0)
		processhits(nhits,buffer);
	else
		obiect=0;

				
}

// handler pentru tastatura
void keyboard(unsigned char key , int x, int y)
{
	switch (key)
	{
		// la escape se iese din program
	case ESC : exit(0);
		// afisare obiecte wireframe
	case 'q': case 'Q':
		for ( int i = 0 ; i < objectCount ; i ++ )
			objects[i].Wireframe = true;
		
		break;
		// afisare obiecte solid
	case 'z': case 'Z':
		for ( int i = 0 ; i < objectCount ; i ++ )
			objects[i].Wireframe = false;
		
		break;
		// cu [ si ] se selecteaza urmatorul obiect sau predecesorul
	case ']' : 
		// deselectam vechiul obiect
		objects[selectedIndex].Deselect();
		// ciclam prin obiecte, daca am ajuns la 0, luam ultimul obiect din vector
		selectedIndex = (selectedIndex > 0) ? selectedIndex - 1 : objectCount - 1;
		// selectam obiectul nou
		objects[selectedIndex].Select();
		break;
	case '[' : 
		// deselectam vechiul obiect
		objects[selectedIndex].Deselect();
		// ciclam prin obiecte, daca am ajuns la ultimul obiect, luam obiectul de la adresa 0
		selectedIndex = (selectedIndex < objectCount - 1) ? selectedIndex + 1 : 0;
		// selectam obiectul nou
		objects[selectedIndex].Select();
		break;
		// cu < si > se modifica nivelul de detaliu al terenului
	case ',' : ground->SetLevelOfDetail(ground->GetLevelOfDetail()*2); 
			if(drawLists)
				recomputeLists = 2;
			
			break;
	case '.' : ground->SetLevelOfDetail(ground->GetLevelOfDetail()/2); 
			if(drawLists)
				recomputeLists = 2;
			
			break;
		// cu W A S D Q E observatorul se misca prin scena
	case 'w' : case 'W' : 
		camera_top->MoveForward(0.1); camera_front->MoveForward(0.1);break;
	case 's' : case 'S' : 
		camera_top->MoveBackward(0.1); camera_front->MoveBackward(0.1);break;
	case 'a' : case 'A' : 
		camera_top->MoveRight(0.1); camera_front->MoveRight(0.1);break;
		break;
	case 'd' : case 'D' : 
		camera_top->MoveLeft(0.1); camera_front->MoveLeft(0.1);break;
		break;
		// Activare/Dezactivare liste de display
	case 'x': case 'X':
			drawLists=(drawLists-1)*(-1);
			recomputeLists = 2;
			
			break;
	default: break;
	}
}

// handler taste speciale
void keyboard(int key , int x, int y)
{
	// incercam sa obtinem un pointer la obiectul selectat
	Object3D *selected;
	// daca nu exista un astfel de obiect
	if( selectedIndex >= 0 && selectedIndex < objectCount )
		selected = &objects[selectedIndex];
	else
		// se iese din functie
		return;

	// cu stanga/dreapta/sus/jos se misca obiectul curent
	switch (key)
	{
	case GLUT_KEY_RIGHT : 
		selected->SetPosition(&(selected->GetPosition() + Vector3D(0.2,0,0))); break;
	case GLUT_KEY_LEFT : 
		selected->SetPosition(&(selected->GetPosition() + Vector3D(-0.2,0,0))); break;
	case GLUT_KEY_DOWN : 
		selected->SetPosition(&(selected->GetPosition() + Vector3D(0,-0.2,0))); break;
	case GLUT_KEY_UP : 
		selected->SetPosition(&(selected->GetPosition() + Vector3D(0,0.2,0))); break;
	}
}

// Functia de idle a GLUT - folosita pentru animatia de rotatie a cuburilor
void spinDisplay()
{
	spin=spin+1;
	if(spin>360.0)
		spin= spin -360.0;
		
	
}

// Callback pentru a procesa inputul de mouse
void mouse(int buton, int stare, int x, int y)
{
	switch(buton)
	{
	// Am apasat click stanga : porneste animatia si realizeaza picking
	case GLUT_LEFT_BUTTON:
		if(stare == GLUT_DOWN)
		{
			// in aceasta variabila se va intoarce obiectul la care s-a executat pick
			obiect = 0;

			pick(x,y);

			glutIdleFunc(spinDisplay);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if(stare== GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	}
}

// Functie pentru redimensionarea subferestrei dreapta
void reshapeDreapta(int w, int h)
{
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w/h, 1.0, 40.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Initializeaza contextul OpenGL asociat ferestrei
	init();

}

// Functie pentru redimensionarea subferestrei stanga
void reshapeStanga(int w, int h)
{
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w/h, 1.0, 60.0); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Initializeaza contextul OpenGL asociat ferestrei
	init();
}

// functie de proiectie
void reshape(int w, int h)
{
	// Main Window
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	// calculare aspect ratio ( Width/ Height )
	GLfloat aspect = (GLfloat) w / (GLfloat) h;

	// intram in modul proiectie
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// incarcam matrice de perspectiva 
	gluPerspective(45, aspect, 1.0, 60);

	// Initializeaza contextul OpenGL asociat ferestrei
	init();

	// Fereastra aplicatiei a fost redimensionata : trebuie sa recream subferestrele

	if(fereastraDreapta != -1)
		glutDestroyWindow(fereastraDreapta);
	if(fereastraStanga != -1)
		glutDestroyWindow(fereastraStanga);

	// Creeaza fereastra stanga
	fereastraStanga = glutCreateSubWindow(mainWindow,0,0,w/2,h);

 	glutDisplayFunc(display);
	glutReshapeFunc(reshapeStanga);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard);

	// Creeaza fereastra dreapta
	fereastraDreapta = glutCreateSubWindow(mainWindow,w/2,0,w/2,h);

	glutDisplayFunc(display);
	glutReshapeFunc(reshapeDreapta);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	int w = 800, h= 600;
	glutInitWindowSize(w,h);
	glutInitWindowPosition(100,100);
	
	// Main window
	mainWindow = glutCreateWindow("Lab7");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);

	// Initializeaza scena 3D
	initScene();

	glutMainLoop();
	return 0;
}