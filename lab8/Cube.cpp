#include "Cube.h"

Cube::Cube (GLfloat posx,GLfloat posy,GLfloat posz,GLfloat lat)
{
	position[0] = posx;
	position[1] = posy;
	position[2] = posz;

	latura = lat;

	rotation[0] = rotation[1] = rotation[2] = 0.0f;

	scale[0] = scale[1] = scale[2] = 1.0f;

	// culoarea speculara
	specular[0]=specular[1]=specular[2]=specular[3]=1.0;
	// culoarea ambientala si difuza
	color[0]=color[1]=color[2]=color[3]=1.0;

	
	vertices[0][0] = position[0]+latura/2;vertices[0][1] = position[1]+latura/2;vertices[0][2] = position[2]-latura/2;
	vertices[1][0] = position[0]-latura/2;vertices[1][1] = position[1]+latura/2;vertices[1][2] = position[2]-latura/2;
	vertices[2][0] = position[0]-latura/2;vertices[2][1] = position[1]+latura/2;vertices[2][2] = position[2]+latura/2;
	vertices[3][0] = position[0]+latura/2;vertices[3][1] = position[1]+latura/2;vertices[3][2] = position[2]+latura/2;

	
	vertices[4][0] = position[0]+latura/2;vertices[4][1] = position[1]-latura/2;vertices[4][2] = position[2]-latura/2;
	vertices[5][0] = position[0]-latura/2;vertices[5][1] = position[1]-latura/2;vertices[5][2] = position[2]-latura/2;
	vertices[6][0] = position[0]-latura/2;vertices[6][1] = position[1]-latura/2;vertices[6][2] = position[2]+latura/2;
	vertices[7][0] = position[0]+latura/2;vertices[7][1] = position[1]-latura/2;vertices[7][2] = position[2]+latura/2;
	

	/*
	// bottom face
	glNormal3f(0.0,-1.0,0.0);
	glVertex3f( position[0]+latura/2,  position[1]-latura/2, position[2]-latura/2);

	glNormal3f(0.0,-1.0,0.0);
	glVertex3f( position[0]-latura/2, position[1]-latura/2, position[2]-latura/2);

	glNormal3f(0.0,-1.0,0.0);
	glVertex3f(position[0]-latura/2, position[1]-latura/2, position[2]+latura/2);

	glNormal3f(0.0,-1.0,0.0);
	glVertex3f(position[0]+latura/2,  position[1]-latura/2, position[2]+latura/2);
	*/


}

GLfloat* Cube::getSpecular()
{
	return specular;
}

void Cube::setSpecular(GLfloat specular_[])
{
	specular[0] = specular_[0];
	specular[1] = specular_[1];
	specular[2] = specular_[2];
	specular[3] = specular_[3];
}

GLfloat* Cube::getColor()
{
	return color;
}

void Cube::setColor(GLfloat color_[])
{
	color[0] = color_[0];
	color[1] = color_[1];
	color[2] = color_[2];
	color[3] = color_[3];
}


// functia de desenare a obiectului
void Cube::Draw ()
{
	glPushMatrix();

	// seteaza proprietati material
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
	float shininess = 10.0f;
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shininess);

	// draw here
	glTranslatef(position[0],position[1],position[2]);
	glRotatef(rotation[0],1.,0.,0.);
	glRotatef(rotation[1],0.,1.,0.);
	glRotatef(rotation[2],0.,0.,1.);
	glTranslatef(-position[0],-position[1],-position[2]);
	
	glScalef(scale[0],scale[1],scale[2]); 

	
	glBegin(GL_QUADS);

	// Back face
	glNormal3f(0.0,0.0,-1.0);
	glVertex3f( position[0]+latura/2,  position[1]+latura/2, position[2]-latura/2);

	glNormal3f(0.0,0.0,-1.0);
	glVertex3f( position[0]-latura/2, position[1]+latura/2, position[2]-latura/2);

	glNormal3f(0.0,0.0,-1.0);
	glVertex3f(position[0]-latura/2, position[1]-latura/2, position[2]-latura/2);

	glNormal3f(0.0,0.0,-1.0);
	glVertex3f(position[0]+latura/2,  position[1]-latura/2, position[2]-latura/2);

	// Front face
	glNormal3f(0.0,0.0,1.0);
	glVertex3f( position[0]+latura/2,  position[1]+latura/2, position[2]+latura/2);

	glNormal3f(0.0,0.0,1.0);
	glVertex3f( position[0]-latura/2, position[1]+latura/2, position[2]+latura/2);

	glNormal3f(0.0,0.0,1.0);
	glVertex3f(position[0]-latura/2, position[1]-latura/2, position[2]+latura/2);

	glNormal3f(0.0,0.0,1.0);
	glVertex3f(position[0]+latura/2,  position[1]-latura/2, position[2]+latura/2);

	// left face
	glNormal3f(-1.0,0.0,0.0);
	glVertex3f( position[0]-latura/2,  position[1]+latura/2, position[2]-latura/2);

	glNormal3f(-1.0,0.0,0.0);
	glVertex3f( position[0]-latura/2, position[1]+latura/2, position[2]+latura/2);

	glNormal3f(-1.0,0.0,0.0);
	glVertex3f(position[0]-latura/2, position[1]-latura/2, position[2]+latura/2);

	glNormal3f(-1.0,0.0,0.0);
	glVertex3f(position[0]-latura/2,  position[1]-latura/2, position[2]-latura/2);

	// right face
	glNormal3f(1.0,0.0,0.0);
	glVertex3f( position[0]+latura/2,  position[1]+latura/2, position[2]-latura/2);

	glNormal3f(1.0,0.0,0.0);
	glVertex3f( position[0]+latura/2, position[1]+latura/2, position[2]+latura/2);

	glNormal3f(1.0,0.0,0.0);
	glVertex3f(position[0]+latura/2, position[1]-latura/2, position[2]+latura/2);

	glNormal3f(1.0,0.0,0.0);
	glVertex3f(position[0]+latura/2,  position[1]-latura/2, position[2]-latura/2);

	// top face
	glNormal3f(0.0,1.0,0.0);
	glVertex3f( position[0]+latura/2,  position[1]+latura/2, position[2]-latura/2);

	glNormal3f(0.0,1.0,0.0);
	glVertex3f( position[0]-latura/2, position[1]+latura/2, position[2]-latura/2);

	glNormal3f(0.0,1.0,0.0);
	glVertex3f(position[0]-latura/2, position[1]+latura/2, position[2]+latura/2);

	glNormal3f(0.0,1.0,0.0);
	glVertex3f(position[0]+latura/2,  position[1]+latura/2, position[2]+latura/2);

	// bottom face
	glNormal3f(0.0,-1.0,0.0);
	glVertex3f( position[0]+latura/2,  position[1]-latura/2, position[2]-latura/2);

	glNormal3f(0.0,-1.0,0.0);
	glVertex3f( position[0]-latura/2, position[1]-latura/2, position[2]-latura/2);

	glNormal3f(0.0,-1.0,0.0);
	glVertex3f(position[0]-latura/2, position[1]-latura/2, position[2]+latura/2);

	glNormal3f(0.0,-1.0,0.0);
	glVertex3f(position[0]+latura/2,  position[1]-latura/2, position[2]+latura/2);


	glEnd();
	
	  

	glPopMatrix();
}
