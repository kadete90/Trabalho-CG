#include "ball.h" 
#include "Jelly.h"
#include <Windows.h>
using namespace cggl;

Ball::Ball(Vector3 pos, float rad, Jelly * _j1,Jelly * _j2 ): position(pos), radius(rad),j1(_j1),j2(_j2){
	model = new ObjModel("models/whiteBall.obj");
	radiusShadow = radius * (position.y > 4) ? position.y*.1/radius : 1/position.y*radius*0.8;
	velocityToP1 = Vector3(-5,15,0);
	velocityToP2 = Vector3(5,15,0);
	angle = 20;
	hitTimeBlock =0;
}

void Ball::InitGL() { 
	Object::InitGL();
	velocity = velocityToP1;
	model->InitGL();
}

void Ball::Draw(){
	Object::Draw();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glRotated(angle, 0, 0, 1);
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
	double fdz = -6*3.14*radius*0.00001827*velocity.z;
	double fdy = -6*3.14*radius*0.00001827*velocity.y;
	double fdx = -6*3.14*radius*0.00001827*velocity.x;

	double ax = (fdx) / mass;
	double ay = (fg + fdy) / mass;
	double az = (fdz)/mass;

	velocity.z += az * t ;
	velocity.y += ay * t ;
	velocity.x += ax * t ;

	position.y = position.y + velocity.y*t + 0.5 * ay * t * t;
	position.x += (velocity.x == 0) ? 0 : velocity.x*t;

	boolean hitBlock=false;

	Vector3 vj1 = j1->hitJelly(position.x,position.y,position.z,radius);
	Vector3 vj2 = j2->hitJelly(position.x,position.y,position.z,radius);

		if(hitTimeBlock <= 0){
		
		if (!(vj1.x==-1 && vj1.y==-1 && vj1.z==-1)){
			hitBlock=true;

			if(vj1.x>0){
				if(velocity.x <0)velocity.x = velocity.x*vj1.x*-1;
				else velocity.x = velocity.x*vj1.x;
			}else{
				if(velocity.x <0)velocity.x = velocity.x*vj1.x*-1;
				else velocity.x = velocity.x*vj1.x;
			}
			
			velocity.y *= vj1.y;
			velocity.z *= vj1.z;
		}
		else if (!(vj2.x==-1 && vj2.y==-1 && vj2.z==-1)){ 

			if(vj2.x>0){
				if(velocity.x <0)velocity.x = velocity.x*vj2.x;
				else velocity.x = velocity.x*vj2.x*-1;
			}else{
				if(velocity.x <0)velocity.x = velocity.x*vj2.x;
				else velocity.x = velocity.x*vj2.x*-1;
			}
			velocity.y *= vj1.y;
			velocity.z *= vj1.z;
		}
	}

	if(hitBlock) hitTimeBlock = 30;
	else --hitTimeBlock;
	
	angle -= velocity.x;
	radiusShadow = radius * (position.y > 4) ? position.y*.1/radius : 1/position.y*radius*1;

	if(position.y <= radius){
		position.y = radius;
		velocity.y = -velocity.y/2.5 ;
		velocity.x = velocity.x/1.1 ;
	}

	if(App::Input->IsKeyPressed('r')) {	position.y= 25;position.x=15;velocity = Vector3(1,6,0); }
	if(App::Input->IsKeyPressed('t')) {	position.y= 25;position.x=-15;velocity = Vector3(1,6,0); }

	if(position.x >= -radius && position.x <= radius){
		if( position.y < 14.5){
			velocity.y = -velocity.y*0.8;
			velocity.x = -velocity.x*1;
		}
		if(position.y >= 14.5 && position.y < 15)
			velocity.y = -velocity.y*1.1;
	}
	

	//if(vj1.x != -1 || vj2.x != -1)printf("xpoint1: %f  || xpoint2: %f \n", vj1.x, vj2.x); 


	/* hitTimeBlock serve para que não seja possivel existirem vários eventos num curto espaço de tempo *resolve problema do salto */
	/* Re-posicionamento da bola e contagem dos pontos */
	if(position.y-radius==0) {
		boolean pontoJ1=false;
		if (position.x < -0 )
			if (position.x >=-35) j2->setPoint();
			else {j1->setPoint();pontoJ1=true;}
		else 
			if (position.x <= 35) {j1->setPoint();pontoJ1=true;}
			else j2->setPoint();

			/* #####		Apito do arbitro que indica que houve ponto		################
			PlaySound(TEXT("SOUNDS\\REFEREEWHISTLE.WAV"), NULL, SND_ASYNC);
			*/

			if (pontoJ1) {position.y= 25;position.x=-25;velocity = Vector3(-2,6,0); }
			else {position.y= 25;position.x=25;velocity = Vector3(2,6,0); }
	}
}