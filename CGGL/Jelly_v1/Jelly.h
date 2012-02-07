#ifndef _JELLY_H_
#define _JELLY_H_

#include <GL/glut.h>
#include <cggl/App.h>
#include <cggl/MathUtils.h>

class Jelly : public cggl::Object {

  cggl::Vector3 position, velocity;
  float width, height, radiusShadow;
  boolean jump;
  int player, points, highJump;
  GLfloat bodyRadius;//, headRadius;
  
public: 
  Jelly(const cggl::Vector3& pos, const float width, const float height, int player);

  void Update(int deltaTimeMilis);
  void Draw();

  cggl::Vector3& GetPosition()  { return position; };
  cggl::Vector3 hitJelly(float x,float y, float z,float radius);
  void setPoint();
  int getPoints();
  int getPlayer();
  int Jelly::getHeight();
  int Jelly::getWidth();
  void drawRectangle(float size, float angleXY, float x, float y);

};

#endif