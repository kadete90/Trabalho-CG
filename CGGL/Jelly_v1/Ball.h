#ifndef _BALL_H_
#define _BALL_H_

#include <GL/glut.h>
#include <cggl/App.h>
#include <cggl/ObjModel.h>
#include "Jelly.h"

class Ball : public cggl::Object {
	
  cggl::Vector3 position,positionToP1,positionToP2 ;
  cggl::Vector3 velocity,velocityToP1,velocityToP2 ; 
  cggl::Vector3 lastPosHitGround;
  float radius, radiusShadow, lastRadiusShadow, angleX,angleZ;
  Jelly * j1;
  Jelly * j2;
  cggl::ObjModel* model;
  int hitTimeBlock, hitTimePlayer;
  int playerLastHit,lastPlayerNumberOfHits;
  bool fstHitGround, hasHitPlayer;
  bool * gameOver,startPlay,*useSound;
  char startPlayP1,startPlayP2;
  float strenghHit;
  int maxHitsPerPlayer;

public: 
  Ball(cggl::Vector3 pos, float rad, Jelly * _j1,Jelly * _j2,bool* _gameOver,bool* _useSound,int maxHitsPerPlayer);

  void InitGL();
  void Update(int deltaTimeMilis);
  void Draw();
  void drawShadow(GLfloat *color, cggl::Vector3 pos, float _radiusShadow);
  cggl::Vector3 getPosition() { return position; }
  cggl::Vector3& GetVelocity() { return velocity; }


};

#endif