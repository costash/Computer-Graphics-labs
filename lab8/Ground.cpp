#include "Ground.h"

Ground::Ground (GLfloat lat)
{
	latura = lat;
	
	// culoarea speculara
	specular[0]=specular[1]=specular[2]=specular[3]=1.0;
	// culoarea ambientala si difuza
	color[0]=color[1]=color[2]=color[3]=1.0;

	// pasul cu care se construieste solul
	divStep = 0.5;
	factor = 1.0;
}


GLfloat Ground::getFactor()
{
	return factor;
}

void Ground::setFactor(GLfloat factor_)
{
	factor = factor_;
}

GLfloat* Ground::getSpecular()
{
	return specular;
}

void Ground::setSpecular(GLfloat specular_[])
{
	specular[0] = specular_[0];
	specular[1] = specular_[1];
	specular[2] = specular_[2];
	specular[3] = specular_[3];
}

GLfloat* Ground::getColor()
{
	return color;
}

void Ground::setColor(GLfloat color_[])
{
	color[0] = color_[0];
	color[1] = color_[1];
	color[2] = color_[2];
	color[3] = color_[3];
}


// functia de desenare a obiectului
void Ground::Draw ()
{
	glPushMatrix();

	// seteaza proprietati material
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);

	float leftMargin,rightMargin;
	
	leftMargin = -latura;
	rightMargin = latura;

	float i,j;

	// Ground
	glBegin(GL_QUADS);

	glNormal3f(0.0,1.0,0.0);

	for(i=leftMargin;i+(divStep*factor)<=rightMargin;i+=(divStep*factor))
	{
		glNormal3f(0.0,1.0,0.0);

		for(j=leftMargin;j+(divStep*factor)<=rightMargin;j+=(divStep*factor))
		{

			glVertex3f(i ,  -5, j);

			glVertex3f( i, -5, j+(divStep*factor));

			glVertex3f(i+(divStep*factor), -5, j+(divStep*factor));

			glVertex3f(i+(divStep*factor),  -5, j);
		}

	}
	
	glEnd();

	glPopMatrix();

}
