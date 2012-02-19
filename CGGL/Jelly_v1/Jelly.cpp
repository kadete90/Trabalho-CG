#include "Jelly.h" 
#include "math.h"

using namespace cggl;

Jelly::Jelly(const Vector3& pos, const float _width, const float _height, const int _player,bool * _gameOver,int _pointsToWin,char _leftKey, char _rightKey,char _upKey, char _downKey,char _jumpKey) 
	: position(pos),resetPos(pos), width(_width),height(_height), player(_player),leftKey(_leftKey),rightKey(rightKey),upKey(_upKey),downKey(_downKey), jumpKey(_jumpKey),gameOver(_gameOver),pointsToWin(_pointsToWin) {
		bodyRadius = .75; sphereSlicesAndStacks = 30;
		radiusShadow = bodyRadius * width*.8 + position.y*0.1;
		jump = false, hitTheGround = false;
		points = 0; velocityJump = 30, jellyTimeShake = 0; 
		heigthVariable = height; widthVariable = width;
		touchNumber=0;
}

void Jelly::Draw(){
	Object::Draw();

	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glScaled(widthVariable, heigthVariable ,widthVariable);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	static GLfloat GreenPlayer[3] = { 0.1, 0.9, 0.1 };
	static GLfloat RedPlayer[3]   = { 0.9, 0.1, 0.1 };

	glColor3fv((player==1) ? GreenPlayer : RedPlayer);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (player==1) ? GreenPlayer : RedPlayer);

	// draw body sphere
	glPushMatrix();
	glTranslated(0, bodyRadius, 0);
	glutSolidSphere(bodyRadius, sphereSlicesAndStacks, sphereSlicesAndStacks);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glPopMatrix();

	// draw shadow
	radiusShadow = bodyRadius * width*.8 + position.y*0.1;
	glPushMatrix();
		glColor3f(.15, .15, .15 );
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

	if (!(*gameOver)){
		float velocityxz = (position.y > 0)? .1: .2; //movimento do player quando esta no ? ar : chao
		int xMax = 52;
		int xMin = 3.5;
		int zMax = 40;
		int zMin = -40;

		if( player == 1 ){
			if(App::Input->IsKeyPressed(leftKey) && position.x >= - xMax) { position.x -= velocityxz; }
			else if(App::Input->IsKeyPressed(rightKey) && position.x <= -xMin) { position.x += velocityxz; }
			if(App::Input->IsKeyPressed(upKey) && position.z >= zMin) { position.z -= velocityxz; }
			else if(App::Input->IsKeyPressed(downKey) && position.z <= zMax) { position.z += velocityxz; }
			if(App::Input->IsKeyPressed(jumpKey) && position.y == 0 && jump == false){ jump = true; velocity.y = velocityJump;}
		}
		else if( player == 2 ){
			if(App::Input->IsSpecialKeyPressed(leftKey) && position.x >= xMin) { position.x -= velocityxz; }
			else if(App::Input->IsSpecialKeyPressed(rightKey) && position.x <= xMax) { position.x += velocityxz; }
			if(App::Input->IsSpecialKeyPressed(upKey) && position.z >= zMin) { position.z -= velocityxz; }
			else if(App::Input->IsSpecialKeyPressed(downKey) && position.z <= zMax) { position.z += velocityxz; }
			if(App::Input->IsKeyPressed(jumpKey) && position.y == 0 && jump == false){jump = true;  velocity.y = velocityJump;}
		}

		double t = deltaTimeMilis/(float)1000;
		double mass = 0.1;
		double fg = mass * -9.8;
		double fdy = -6*3.14*bodyRadius*0.00002*velocity.y;
		double ay = (fg + fdy) / mass;

		if(position.y > 0 || velocity.y == velocityJump) {
			float aux = (velocity.y > 0 )? 4 :10;
			velocity += ay * t *aux;
			position.y += velocity.y*t + 0.5 * ay * t * t;
			if(position.y < 0) position.y = 0;
		}
		shakeJelly(false);
	}
	radiusShadow = bodyRadius * width*.8 + position.y*0.1;
}

void Jelly::shakeJelly(bool hitByBall){
	if((jump || hitByBall) && position.y == 0){
		hitTheGround = true;
		jellyTimeShake = 36;
		jump = false;
	}
	if((hitTheGround||hitByBall)  && jellyTimeShake > 0 ){
		if(!jump){
			if(jellyTimeShake <= 18){
				heigthVariable +=.025;
				widthVariable -= .04;
			}
			else{
				heigthVariable -= .025;
				widthVariable += .04;
			}
			--jellyTimeShake;
			if(jellyTimeShake == 0)
				hitTheGround = false;
		}
		else{
			heigthVariable = height;
			widthVariable = width;
			jellyTimeShake = -1;
		}
	}
	else if(hitByBall && jellyTimeShake < 1){
		widthVariable = width;
		hitByBall = false;
	}
	else
		heigthVariable = (position.y == 0)? height  : heigthVariable + velocity.y/1000;
}

cggl::Vector3 Jelly::hitJelly(float x,float y, float z){
	float eq;
	eq = pow((x - position.x),2) + pow((y - (position.y+height)),2) + pow((z - position.z),2) - (height*height);
	if(eq <= 0 ) {
		shakeJelly(true);
		return Vector3(fabs(position.x)-fabs(x),-(y/(position.y+height*2) + .1),position.z-z);
	}
	else return Vector3(-10,-10,-10);

}
void Jelly::resetPosition(){position=resetPos;}
int Jelly::getPoints(){ return points;}
int Jelly::getPlayer(){	return player;}
int Jelly::getHeight(){ return height;}
int Jelly::getWidth (){ return width; }
void Jelly::setPoint(){	points++;}
void Jelly::resetPoint(){points =0;}
char Jelly::getLeftKey(){return leftKey;}
char Jelly::getRightKey(){return rightKey;}
char Jelly::getUpKey(){return upKey;}
char Jelly::getDownKey(){return downKey;}
char Jelly::getJumpKey(){return jumpKey;}
void Jelly::resetTouchNumber(){touchNumber=1;}
void Jelly::increaseTouchNumber(){touchNumber++;}
bool Jelly::getJump(){return jump;}
bool Jelly::hasWon(){return points == pointsToWin;}
int Jelly::getTouchNumber(){return touchNumber;}