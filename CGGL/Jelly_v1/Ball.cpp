#include "ball.h" 
#include "Jelly.h"
#include <Windows.h>
#include <dos.h> // ter o SLEEP
using namespace cggl;

Ball::Ball(Vector3 pos, float rad, Jelly * _j1,Jelly * _j2 ): position(pos), radius(rad),j1(_j1),j2(_j2){
	model = new ObjModel("models/whiteBall.obj");
	radiusShadow = radius * .8 + position.y*.05;
	velocity = Vector3(-5,15,0);
	angleZ = 20;
	angleX =0;
	hitTimeBlock =0;
	fstHitGround = false;
	playerLastHit  =-1;
}

void Ball::InitGL() { 
	Object::InitGL();
	model->InitGL();
}

void Ball::Draw(){
	Object::Draw();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glRotated(angleX,1,0,0);
	glRotated(angleZ,0,0,1);
	glScaled(radius,radius,radius);
	model->Draw();
	glPopMatrix();

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

	glDisable(GL_LIGHTING);
}

void Ball::Update(int deltaTimeMilis){
	Object::Update(deltaTimeMilis);

	double t = deltaTimeMilis/(float)1000;
	double mass = 1.5;
	double fg = mass * -9.8;
	double fdx = -6*3.14*radius*0.00001827*velocity.x;
	double fdy = -6*3.14*radius*0.00001827*velocity.y;
	double fdz = -6*3.14*radius*0.00001827*velocity.z;

	double ax = (fdx) / mass;
	double ay = (fg + fdy) / mass;
	double az = (fdz)/mass;

	velocity.x += ax * t ;
	velocity.y += ay * t ;
	velocity.z += az * t ;

	position.y = position.y + velocity.y*t + 0.5 * ay * t * t;
	position.x += velocity.x*t;

	if(App::Input->IsKeyPressed('g')) {position.z += 1; }
	else if(App::Input->IsKeyPressed('f')) {position.z -= 1; }
	else position.z += velocity.z*t;
	boolean hitBlock=false;

	Vector3 vj1 = j1->hitJelly(position.x,position.y,position.z);
	Vector3 vj2 = j2->hitJelly(position.x,position.y,position.z);

	float INCREASE_X_VELOCITY = 1.2;

	if(hitTimeBlock <= 0 && !fstHitGround ){

		if (!(vj1.x==-10 && vj1.y==-10 && vj1.z==-10)){
			hitBlock=true;
			playerLastHit = 1;
			velocity.x = vj1.x* INCREASE_X_VELOCITY;
			velocity.y *= vj1.y;
			velocity.z = -vj1.z;
		}

		if (!(vj2.x==-10 && vj2.y==-10 && vj2.z==-10)){ 
			hitBlock=true;
			playerLastHit=2;
			if(velocity.x >0) velocity.x = vj2.x*-1; 
			else velocity.x = vj2.x*-1;
			velocity.x*= INCREASE_X_VELOCITY;
			velocity.y *= vj2.y;
			velocity.z = -vj2.z;
		}
	}


	if(hitBlock) hitTimeBlock = 90;
	else --hitTimeBlock;

	angleZ -= velocity.x;
	angleX -= -velocity.z;
	radiusShadow = radius * .8 + position.y*.05;

	if(position.y <= radius){
		position.y = radius;
		velocity.y = -velocity.y/2.5 ;
		velocity.x = velocity.x/1.1 ;
		velocity.z = velocity.z/1.1 ;
	}

	if(App::Input->IsKeyPressed('r')) {	position.y= 25;position.x=15;position.z=0;velocity = Vector3(1,6,0); }
	if(App::Input->IsKeyPressed('t')) {	position.y= 25;position.x=-15;position.z=0;velocity = Vector3(1,6,0); }

	if(position.x >= -radius && position.x <= radius){
		if( position.y < 14.5 && position.z >= -17 && position.z <=17){
			velocity.y = velocity.y*0.8;
			velocity.x = -velocity.x*1;
		}
		if(position.y >= 14.5 && position.y < 15 && position.z >= -17 && position.z <=17)
			velocity.y = -velocity.y*1.1;
	}

	if(position.y - radius <= 0){
		if(!fstHitGround){
			lastPosHitGround = position;
			fstHitGround = true;
		}
		else if(velocity.x <= .15 && velocity.x >= -.15 && velocity.z <= .15 && velocity.z >= -.15 && fstHitGround) {
			boolean pontoJ1 = false;
			if (lastPosHitGround.x < 0 )
				if (lastPosHitGround.x >=-35 && lastPosHitGround.z >=-17 && lastPosHitGround.z<=17) j2->setPoint();
				else if (playerLastHit == 2) {j1->setPoint();pontoJ1 = true;}
				else j2->setPoint();
			else 
				if (lastPosHitGround.x <= 35 && lastPosHitGround.z >=-17 && lastPosHitGround.z<=17) {j1->setPoint(); pontoJ1 = true;}
				else if (playerLastHit == 2) {j1->setPoint();pontoJ1 = true;}
				else j2->setPoint();

				// #####		Apito do arbitro que indica que houve ponto		################
				//PlaySound(TEXT("SOUNDS\\REFEREEWHISTLE.WAV"), NULL, SND_ASYNC);

				fstHitGround = false;

				if (pontoJ1) {position.y= 25;position.x=-25;position.z=0;velocity = Vector3(-2,6,0); }
				else {position.y= 25;position.x=25;position.z=0;velocity = Vector3(2,6,0); }
		}
	}
}