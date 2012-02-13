#ifndef _JELLY_H_
#define _JELLY_H_

#include <GL/glut.h>
#include <cggl/App.h>
#include <cggl/MathUtils.h>

class Jelly : public cggl::Object {

  cggl::Vector3 position, velocity;
  float width, height, radiusShadow, heigthVariable, widthVariable, hitTimeGround;
  boolean jump, hitTheGround;
  int player, points, velocityJump,touchNumber,pointsToWin;
  GLfloat bodyRadius;
  GLint sphereSlicesAndStacks;
  bool * gameOver;
  
  
public: 
  char leftKey, rightKey, upKey, downKey, jumpKey;
  Jelly(const cggl::Vector3& pos, const float width, const float height, int player,bool * gameOver,int pointsToWin,char leftKey, char rightKey,char upKey, char downKey, char jumpKey);

  void Update(int deltaTimeMilis);
  void Draw();

  cggl::Vector3& GetPosition()  { return position; };
  cggl::Vector3 hitJelly(float x,float y, float z);
  void setPoint();
   void resetPoint();
  int getPoints();
  int getPlayer();
  int Jelly::getHeight();
  int Jelly::getWidth();
  void drawRectangle(float size, float angleXY, float x, float y);
  char getLeftKey();
  char getRightKey();
  char getUpKey();
  char getDownKey();
  char getJumpKey();
  void increaseTouchNumber();
  void resetTouchNumber();
  boolean hasWon();
  int getTouchNumber();
};

#endif