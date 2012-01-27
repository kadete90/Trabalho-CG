#ifndef _JELLY_H_
#define _JELLY_H_

#include <GL/glut.h>
#include <cggl/App.h>
#include <cggl/MathUtils.h>

class Jelly : public cggl::Object {

  cggl::Vector3 position;
  float width, height;
  boolean jump,up;
  
public: 
  Jelly(const cggl::Vector3& pos, const float width, const float height, int player);

  void Update(int deltaTimeMilis);
  void Draw();

  cggl::Vector3& GetPosition()  { return position; };
  bool hitJelly(float x,float y, float z,int radius);
  void setPoint();
  int getPoints();
  void drawRectangle(float size, float angleXY, float x, float y);
  int player;
  int points;

};

#endif