#include "Jelly.h" 
#include "math.h"

using namespace cggl;

Jelly::Jelly(const Vector3& pos, const float _width, const float _height, const int _player) 
	: position(pos), width(_width),height(_height), player(_player) {
	bodyRadius = .75;
	//headRadius = .55;
	radiusShadow = bodyRadius * width*.8 + position.y*0.1;
	jump = false;
	points = 0;
	highJump = 20;
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
	  static GLint sphereSlicesAndStacks = 30;

	  glColor3fv((player==1) ? GreenPlayer : RedPlayer);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (player==1) ? GreenPlayer : RedPlayer);

	  // draw body sphere
	  glPushMatrix();
		  glTranslated(0, bodyRadius, 0);
		  glutSolidSphere(bodyRadius, sphereSlicesAndStacks, sphereSlicesAndStacks);
	  glPopMatrix();
		/*
	  // draw head 
	  glPushMatrix();
		  glTranslated(0, bodyRadius * 2 + headRadius/2, 0);
		  glColor3fv((player==1)?GreenPlayer:RedPlayer);
		  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (player==1)?GreenPlayer:RedPlayer);
		  glutSolidSphere(headRadius, sphereSlicesAndStacks, sphereSlicesAndStacks);
	  glPopMatrix();
	  */
	  glDisable(GL_LIGHTING);
	  glPopMatrix();

	  // draw shadow
	  glPushMatrix();
		  glColor3f(.2, .2, .2 );
		  glTranslated(position.x, .01, position.z);
		  glRotated(90,1,0,0);
		  glBegin(GL_POLYGON);
		  float numPoints = 20, aux;
		  for(int i = 0; i < numPoints; i++) {    
			  aux = i*2*3.14159/numPoints;
			  glVertex2f(cos(aux)*radiusShadow,sin(aux)*radiusShadow);
		  }
		  glEnd();
	   glPopMatrix();
}

void Jelly::Update(int deltaTimeMilis){
  Object::Update(deltaTimeMilis);
  if (jump){
		velocity.y = highJump;
		jump = false;
  }
  float velocityxz = (position.y > 0)? .3: .35;
  /*
  int xMax = 50;
  int xMin = 3.5;
  int zMax = 25;
  int zMin = -25;
  */
  int xMax = 80;
  int xMin = 3.5;
  int zMax = 40;
  int zMin = -40;
  

  printf("player %d z %f \n",player, position.z);
  if( player == 1 ){
	  if(App::Input->IsKeyPressed('a') && position.x >= - xMax) { position.x -= velocityxz; }
	  else if(App::Input->IsKeyPressed('d') && position.x <= -xMin) { position.x += velocityxz; }
	  if(App::Input->IsKeyPressed('w') && position.z >= zMin) { position.z -= velocityxz; }
	  else if(App::Input->IsKeyPressed('s') && position.z <= zMax) { position.z += velocityxz; }
	  if(App::Input->IsKeyPressed('e') && position.y == 0 && jump == false){ jump = true;}
 }
  else if( player == 2 ){
	  if(App::Input->IsSpecialKeyPressed(GLUT_KEY_LEFT) && position.x >= xMin) { position.x -= velocityxz; }
	  else if(App::Input->IsSpecialKeyPressed(GLUT_KEY_RIGHT) && position.x <= xMax) { position.x += velocityxz; }
	  if(App::Input->IsSpecialKeyPressed(GLUT_KEY_UP) && position.z >= zMin) { position.z -= velocityxz; }
	  else if(App::Input->IsSpecialKeyPressed(GLUT_KEY_DOWN) && position.z <= zMax) { position.z += velocityxz; }
	  if(App::Input->IsKeyPressed('ç') && position.y == 0 && jump == false){jump = true; }
  }

  double t = deltaTimeMilis/(float)1000;
  double mass = 0.1;
  double fg = mass * -9.8;
  double fdy = -6*3.14*bodyRadius*0.00002*velocity.y;
  double ay = (fg + fdy) / mass;

  if(position.y > 0 || velocity.y == highJump) {
	  float aux = (velocity.y > 0 )? 1.8 :5;
	  velocity += ay * t *aux;
	  position.y += velocity.y*t + 0.5 * ay * t * t;
  }
  if(position.y <0) position.y =0;
  radiusShadow = bodyRadius * width*.8 + position.y*0.1;
  
}

cggl::Vector3 Jelly::hitJelly(float x,float y, float z, float radius){
float eq;
	eq = pow((x - position.x),2) + pow((y - (position.y+height)),2) + pow((z - position.z),2) - (height*height);

	if(eq <= 0 ) {
	/*	printf("%f |Y %f | pos.y %f | height %f \n",eq,y,position.y,height);
		printf("pos.x %f | pos.y %f | pos.z %f \n",position.x,position.y,position.z);
		printf("ret x %f ref y %f ret z %f \n ", - (fabs(position.x)-fabs(x)),- (y/(position.y+height*2)),position.z-z);
		*/
		return Vector3(fabs(position.x)-fabs(x),-(y/(position.y+height*2)+0.1),position.z-z);
	}

else return Vector3(-1,-1,-1);

}

int Jelly::getPoints(){ return points;}
int Jelly::getPlayer(){	return player;}
int Jelly::getHeight(){ return height;}
int Jelly::getWidth (){ return width; }
void Jelly::setPoint(){	points++;}