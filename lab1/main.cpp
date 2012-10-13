//-----------------------------------------------------------------------------------------------
//					LAB 1
//
//	Fisiere de interes: Transform2d.cpp main.cpp
//
//	Functii WorldDrawer2d:
//	-init e apelat o singura data, la inceput.
//	-idle e apelat per cadru de desenare
//	-onKey la apasarea unei taste.
//
//	Obiecte:
//	- un obiect este reprezentat prin punct si topologia punctelor (cum sunt legate pctele)
//	- obiectele sunt compuse din triunghiuri! de exemplu cu 4 puncte si 6 indici de topologie
//	pot crea 2 triunghiuri adiacente ce impreuna formeaza un dreptunghi.
//
//	Sisteme de coordonate:
//	- sunt 2 tipuri de sisteme de coordonate (fix - bleu&magenta) si dinamic(rosu&albastru)
//	- ca un obiect sa fie desenat trebuie sa fie atasat unui sistem de coordonate
//	- cand un obiect e atasat la un sistem de coordonate urmeaza transformarile sistemului.
//
//	Control obiecte:
//	- daca translatez/rotesc un obiect/punct direct (ex: o->translate(1,1)) o fac in coordonate globale
//	- daca translatez/rotesc un obiect printr-un sistem de coordonate o fac in coordonate locale
//	- pentru simplitate toate coordonatele mentinute in obiecte(de c++) sunt globale.
//
//	Happy coding.
//----------------------------------------------------------------------------------------------

#include "WorldDrawer2d.h"
bool WorldDrawer2d::animation=true;


//used global vars
CoordinateSystem2d *cs1, *cs2,*cs3;
Object2d *o1, *o2, *o3;


//add
void WorldDrawer2d::init(){
	//creeaza 2 sistem de coordonate client
	cs1 = new CoordinateSystem2d();
	cs2 = new CoordinateSystem2d();	
	cs3 = new CoordinateSystem2d();
	cs_used.push_back(cs1);
	cs_used.push_back(cs2);
	cs_used.push_back(cs3);

	//(triunghi rosu)
	std::vector<Point2d> points;
	std::vector<int> topology;
	points.push_back(Point2d(1,0));
	points.push_back(Point2d(-1,0));
	points.push_back(Point2d(0,1));
	topology.push_back(0);
	topology.push_back(1);
	topology.push_back(2);
	o1 = new Object2d(points,topology);
	o1->setcolor(1,0.8f,0.85f);
	//cs 1
	cs1->objectAdd(o1);
	cs1->translate(-5,5);
	cs1->rotateSelf(0.5);

	//(patrat albastru = 2 triunghiuri)	
	std::vector<Point2d> points2;
	std::vector<int> topology2;
	points2.push_back(Point2d(1,0));
	points2.push_back(Point2d(-1,0));
	points2.push_back(Point2d(-1,1));
	points2.push_back(Point2d(1,1));
	topology2.push_back(0);
	topology2.push_back(1);
	topology2.push_back(2);
	topology2.push_back(2);
	topology2.push_back(3);
	topology2.push_back(0);
	o2 = new Object2d(points2,topology2);
	o2->setcolor(0,0,1);
	//cs 2
	cs2->objectAdd(o2);
	cs2->translate(-10,-10);
	cs2->rotateSelf(-0.5);
	cs2->objectTranslate(o2,0,5);

}
void WorldDrawer2d::onIdle(){	//per frame
	static int iteration=1;
	static int direction = 1;
	static bool o1dir=true;
	static bool o2dir=true;
	static bool o3dir=true;
	static bool o3dir2=true;
	if(animation){
		if (iteration % 25 == 0)
			direction *= -1;
		
		o1->translate(direction, 0);

		o2->rotateSelf(0.05);

		o2->rotateRelativeToPoint(cs2->axiscenter, 0.01);

		iteration++;
	}
}

void WorldDrawer2d::onKey(unsigned char key){
	switch(key){
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;
		case KEY_LEFT:
			break;
		case KEY_RIGHT:
			break;
		case KEY_SPACE:
				animation=!animation;
			break;
		default:
			break;
	}
}


int main(int argc, char** argv){
	WorldDrawer2d wd2d(argc,argv,600,600,200,100,std::string("Lab 1"));
	wd2d.init();
	wd2d.run();
	return 0;
}