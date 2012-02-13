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
	char txt2[25];
	char txtCamera[20];
	bool gameOver, useSound;
	int gamesToWin, maxHitsPerPlayer;
	cggl::Vector3 _eye;
	cggl::Vector3 _center;
	cggl::Vector3 _up;

public: 
  void CreateScene();
  void Draw();
  void Update(int deltaTimeMilis);
  void updateText(char * str);
  void setGameOver(bool b );
  void setKeyBoardVertical();
  void setKeyBoardHorizontal();
  void setUseSound(bool);
  void setGamesToWin(int);
  void setMaxHitsPerPlayer(int);
  int getGamesToWin();
  int getMaxHitsPerPlayer();
  bool getUseSound();
};
