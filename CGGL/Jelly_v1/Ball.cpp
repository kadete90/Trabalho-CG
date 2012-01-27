#include "ball.h" 
#include "Jelly.h"
using namespace cggl;

Ball::Ball(Vector3 pos, float rad, Jelly * _j1,Jelly * _j2 ): position(pos), radius(rad),j1(_j1),j2(_j2){
	model = new ObjModel("models/whiteBall.obj");
	radiusShadow = radius * (position.y > 4) ? position.y*.1/radius : 1/position.y*radius*0.8;
	velocityToP1 = Vector3(-5,15,0);
	velocityToP2 = Vector3(5,15,0);
	angle = 20;

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
  double fdy = -6*3.14*radius*0.00001827*velocity.y;
  double fdx = -6*3.14*radius*0.00001827*velocity.x;

  double ax = (fdx) / mass;
  double ay = (fg + fdy) / mass;

  velocity.y += ay * t ;
  velocity.x += ax * t ;

  position.y = position.y + velocity.y*t + 0.5 * ay * t * t;
  position.x += (velocity.x == 0) ? 0 : velocity.x*t;

  angle -= velocity.x;
  radiusShadow = radius * (position.y > 4) ? position.y*.1/radius : 1/position.y*radius*1;

  if(position.y <= radius){
    position.y = radius;
    velocity.y = -velocity.y/2.5 ;
	velocity.x = velocity.x/1.1 ;
  }
   if(App::Input->IsKeyPressed('r')) {	position.y= 25;position.x=0;velocity = Vector3(1,6,0); }

  if(position.x >= 3.5 - radius){
	  //check position player2
  }

  else if(position.x <= -3.5 + radius){ 
	   //check position player2
  }

  else if(position.x >= -radius && position.x <= radius){
	  if( position.y < 14.5){
		  velocity.y = -velocity.y*1.3;
		  velocity.x = -velocity.x*1.2;
	  }
	  if(position.y >= 14.5 && position.y < 15)
		  velocity.y = -velocity.y*1.3;
  }

  if (j1->hitJelly(position.x,position.y,position.z,radius) || j2->hitJelly(position.x,position.y,position.z,radius)){
		velocity.x*=-1;
		if (App::Input->IsKeyPressed('p')) velocity.y*=-1.3;
		else velocity.y *=-1.05;
		
  }
}