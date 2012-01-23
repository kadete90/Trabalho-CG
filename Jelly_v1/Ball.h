#ifndef _BALL_H_
#define _BALL_H_

#include <GL/glut.h>
#include <cggl/App.h>
#include <cggl/ObjModel.h>

class Ball : public cggl::Object {
	
  cggl::Vector3 position, velocity; 
  float radius, radiusShadow, angle;

  cggl::ObjModel* model;
  
public: 
  Ball(cggl::Vector3 pos, float rad);

  void InitGL();
  void Update(int deltaTimeMilis);
  void Draw();

  cggl::Vector3 getPosition() { return position; }
  cggl::Vector3& GetVelocity() { return velocity; };
};

#endif