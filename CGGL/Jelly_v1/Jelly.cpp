#include "Jelly.h" 
#include "math.h"

using namespace cggl;

Jelly::Jelly(const Vector3& pos, const float _width, const float _height, const int _player) 
	: position(pos), width(_width),height(_height), player(_player) {
		
		jump = false;
		up=true;
		points =0;
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

	 /* // draw head 
	  glPushMatrix();
		  glTranslated(0, bodyRadius * 2 + headRadius/2, 0);
		  glColor3fv((player==1)?GreenPlayer:RedPlayer);
		  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (player==1)?GreenPlayer:RedPlayer);
		  glutSolidSphere(headRadius, sphereSlicesAndStacks, sphereSlicesAndStacks);
	  glPopMatrix();
	  */
	  glDisable(GL_LIGHTING);
  glPopMatrix();
}

void Jelly::Update(int deltaTimeMilis){
  Object::Update(deltaTimeMilis);
  int MIN_Z = -15, MAX_Z=15,MIN_X=3.5,MAX_X=40;

  double t = deltaTimeMilis/(float)1000;

	if (jump){
		if (position.y<10 && up)position+=Vector3(0,.2,0);
		else if (position.y>0){ position-=Vector3(0,.2,0); up =false;}
		else if (position.y <=0) {position.y=0;jump=false;up=true;}
	}
  if( player == 1 ){
	  if(App::Input->IsKeyPressed('a') && position.x >= -MAX_X) { position.x -= 0.4; }
	  else if(App::Input->IsKeyPressed('d') && position.x <= -MIN_X) { position.x += 0.4; }
	  if(App::Input->IsKeyPressed('w') && position.z >= MIN_Z) { position.z -= 0.4; }
	  else if(App::Input->IsKeyPressed('s') && position.z <= MAX_Z) { position.z += 0.4; }

	  if(App::Input->IsKeyPressed('e') && jump == false) {jump = true; }
  }
  else if( player == 2 ){
	  if(App::Input->IsSpecialKeyPressed(GLUT_KEY_LEFT) && position.x >= MIN_X) {	position.x -= 0.4; }
	  else if(App::Input->IsSpecialKeyPressed(GLUT_KEY_RIGHT) && position.x <= MAX_X) { position.x += 0.4;}
	  if(App::Input->IsSpecialKeyPressed(GLUT_KEY_UP) && position.z >= MIN_Z) { position.z -= 0.4; }
	  else if(App::Input->IsSpecialKeyPressed(GLUT_KEY_DOWN) && position.z <= MAX_Z) { position.z += 0.4; }
	   if(App::Input->IsKeyPressed('ç') && jump == false) {jump = true; }
	 }
}

cggl::Vector3 Jelly::hitJelly(float x,float y, float z, float radius){
float eq;
	eq = pow((x - position.x),2) + pow((y - (position.y+height)),2) + pow((z - position.z),2) - (height*height);

	if(eq <= 0 ) {
		printf("%f |Y %f | pos.y %f | height %f \n",eq,y,position.y,height);
		printf("pos.x %f | pos.y %f | pos.z %f \n",position.x,position.y,position.z);
		printf("ret x %f ref y %f ret z %f \n ", - (fabs(position.x)-fabs(x)),- (y/(position.y+height*2)),position.z-z);
		
		return Vector3( fabs(position.x)-fabs(x),- (y/(position.y+height*2)+0.1),position.z-z);
	}

else return Vector3(-1,-1,-1);

}

int Jelly::getPoints(){ return points;}
int Jelly::getPlayer(){	return player;}
int Jelly::getHeight(){ return height;}
int Jelly::getWidth (){ return width; }
void Jelly::setPoint(){	points++;}