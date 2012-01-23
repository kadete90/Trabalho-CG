#include "Jelly.h" 

using namespace cggl;

Jelly::Jelly(const Vector3& pos, const float _width, const float _height, const int _player) 
	: position(pos), width(_width),height(_height), player(_player) {
}

void Jelly::Draw(){
  Object::Draw();

  glPushMatrix();
	  glTranslated(position.x, position.y, position.z);
	  glScaled(width, height ,width);

	  glEnable(GL_LIGHTING);
	  glEnable(GL_LIGHT0);

	  static GLfloat GreenPlayer[3] = { 0.1, 0.9, 0.1 };
	  static GLfloat RedPlayer[3]   = { 0.9, 0.1, 0.1 };
	  static GLfloat bodyRadius = .75;
	  static GLfloat headRadius = .55;
	  static GLint sphereSlicesAndStacks = 30;

	  glColor3fv((player==1)?GreenPlayer:RedPlayer);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (player==1)?GreenPlayer:RedPlayer);

	  // draw body sphere
	  glPushMatrix();
		  glTranslated(0, bodyRadius, 0);
		  glutSolidSphere(bodyRadius, sphereSlicesAndStacks, sphereSlicesAndStacks);
	  glPopMatrix();

	  // draw head 
	  glPushMatrix();
		  glTranslated(0, bodyRadius * 2 + headRadius/2, 0);
		  glColor3fv((player==1)?GreenPlayer:RedPlayer);
		  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (player==1)?GreenPlayer:RedPlayer);
		  glutSolidSphere(headRadius, sphereSlicesAndStacks, sphereSlicesAndStacks);
	  glPopMatrix();

	  glDisable(GL_LIGHTING);
  glPopMatrix();
}

void Jelly::Update(int deltaTimeMilis){
  Object::Update(deltaTimeMilis);

  double t = deltaTimeMilis/(float)1000;
  if( player == 1 ){
	  if(App::Input->IsKeyPressed('a') && position.x >= -35) { position.x -= 0.4; }
	  else if(App::Input->IsKeyPressed('d') && position.x <= -3.5) { position.x += 0.4; }
	  if(App::Input->IsKeyPressed('w') && position.z >= -14) { position.z -= 0.4; }
	  else if(App::Input->IsKeyPressed('s') && position.z <= 12) { position.z += 0.4; }
  }
  else if( player == 2 ){
	  if(App::Input->IsSpecialKeyPressed(GLUT_KEY_LEFT) && position.x >= 3.5) {	position.x -= 0.4; }
	  else if(App::Input->IsSpecialKeyPressed(GLUT_KEY_RIGHT) && position.x <= 35) { position.x += 0.4;}
	  if(App::Input->IsSpecialKeyPressed(GLUT_KEY_UP) && position.z >= -14) { position.z -= 0.4; }
	  else if(App::Input->IsSpecialKeyPressed(GLUT_KEY_DOWN) && position.z <= 12) { position.z += 0.4; }
  }
}

bool Jelly::hitJelly(int y){
	return (y < (position.y + height/2)) && (y > (position.y - height/2));
}

int Jelly::getPoints() {
	return 0;
}

void Jelly::setPoint(){
	points++;
}