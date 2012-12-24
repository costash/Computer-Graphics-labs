#ifndef CUBE_H
#define CUBE_H

#include <freeglut.h>

class Cube
{
  public:

	// Constructor
	Cube (GLfloat posx=0, GLfloat posy=0, GLfloat posz=0,GLfloat latura=1.0);
	  
	// functia de desenare
	void Draw();

	// partea de sus a cubului pentru a crea volumul de umbra
	GLint getNrVertices()
	{
		return 8;
	}

	GLfloat* getVertices()
	{
		return (GLfloat*)vertices;
	}

	// culori
	GLfloat* getSpecular();
	void setSpecular(GLfloat []);

	GLfloat* getColor();
	void setColor(GLfloat []);

	GLfloat * getPosition() const { return (GLfloat *)position; }
	void setPosition(GLfloat pos[]) { position[0]=pos[0];position[1]=pos[1];position[2]=pos[2]; }

	GLfloat * getRotation() const { return (GLfloat *)rotation; }
	void setRotation(GLfloat rot[]) { rotation[0]=rot[0];rotation[1]=rot[1];rotation[2]=rot[2]; }

	GLfloat * getScale() const { return (GLfloat *)scale; }
	void setScale(GLfloat sca[]) { scale[0]=sca[0];scale[1]=sca[1];scale[2]=sca[2]; }



  public:
	GLfloat		position[3];
	GLfloat		rotation[3];
	GLfloat		scale[3];
	
	GLfloat		specular[4];
	GLfloat		color[4];	
	
	GLfloat latura;
	GLfloat vertices[8][3];
	
	  
};


#endif
