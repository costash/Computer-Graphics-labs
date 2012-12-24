#ifndef Ground_H
#define Ground_H

#include <freeglut.h>

class Ground
{
  public:

	// Constructor
	Ground (GLfloat latura=10.0);
	  
	// functia de desenare
	void Draw();

	// culori
	GLfloat* getSpecular();
	void setSpecular(GLfloat []);

	GLfloat* getColor();
	void setColor(GLfloat []);

	GLfloat getFactor();
	void setFactor(GLfloat factor);

  public:
	GLfloat	position[3];
	GLfloat latura;
	
	GLfloat specular[4];
	GLfloat color[4];

	GLfloat divStep;
	GLfloat factor;
  
};


#endif
