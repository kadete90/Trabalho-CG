#ifndef _BALL_H_
#define _BALL_H_

#include <GL/glut.h>
#include <cggl/App.h>
#include <cggl/ObjModel.h>
#include "Jelly.h"

class Ball : public cggl::Object {
	
  cggl::Vector3 position ;
  cggl::Vector3 velocity; 
  cggl::Vector3 lastPosHitGround;
  
  float radius, radiusShadow, angleX,angleZ;
  Jelly * j1;
  Jelly * j2;
  cggl::ObjModel* model;
  int hitTimeBlock;
  int playerLastHit;
  boolean fstHitGround;

public: 
  Ball(cggl::Vector3 pos, float rad, Jelly * _j1,Jelly * _j2 );

  void InitGL();
  void Update(int deltaTimeMilis);
  void Draw();

  cggl::Vector3 getPosition() { return position; }
  cggl::Vector3& GetVelocity() { return velocity; }
};

#endif