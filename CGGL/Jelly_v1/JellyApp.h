#include <GL/glut.h>
#include <cggl/App.h>

#include "Ball.h"
#include "Jelly.h"
#include "Net.h"

class JellyApp : public cggl::App {
	Ball* ball;
	Jelly * p1;
	Jelly * p2;
	Net * net;
	char txt[20];
	char txt2[20];
	bool gameOver;

public: 
  void CreateScene();
  void Draw();
  void Update(int deltaTimeMilis);
  void updateText(char * str);
  void setGameOver(bool b );
};
