#include <cggl/App.h>
#include <cggl/MathUtils.h>
#include <cggl/TextureLoader.h>
#include "SimplePlan.h"

using namespace cggl;

void SimplePlan::Draw() {
  Object::Draw();

	Vector3 drawingFront(0,0,1);
	float dot = drawingFront.DotProduct(front);
	float angleRAD = acos( dot );
	float angleDEG = RAD_TO_DEG(angleRAD);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture.TextureID);
    //glEnable(GL_LIGHTING);

    glPushMatrix();
	glTranslated(position.x,position.y, position.z);
	glRotated(angle, 1, 0, 0);
	
    const static float diffuseColor[3] = { 1, 1, 1 };
    const static float ambientColor[3] = { 1, 1, 1 };
    glColor3f(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
    glNormal3f(0, 1, 0);
    glBegin(GL_QUADS);
	  glTexCoord2f(1, 1);
      glVertex3f(+width, 0, +height);
	  glTexCoord2f(1, 0);
      glVertex3f(+width, 0, -height);
	  glTexCoord2f(0, 0);
      glVertex3f(-width, 0, -height);
	  glTexCoord2f(0, 1);
      glVertex3f(-width, 0, +height);
    glEnd();
    glPopMatrix();

    //glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
  }

void SimplePlan::InitGL() {
  Object::InitGL();
  App::TextureLoader->LoadTextureFromDisk(fileName, &texture);
}