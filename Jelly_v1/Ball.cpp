#include "ball.h" 

using namespace cggl;

Ball::Ball(Vector3 pos, float rad): position(pos), radius(rad){
	model = new ObjModel("models/whiteBall.obj");
	radiusShadow = radius*(position.y>4)? position.y*.12/radius: 1/position.y*radius;
	velocity = Vector3(-5,15,0);
	angle = 20;
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
	glRotated(angle, 0, 0, 1);
	glScaled(radius,radius,radius);
    model->Draw();
  glPopMatrix();
  
  glPushMatrix();
    const static float diffuseColor[3] = { .2, .2, .2 };
    glColor3fv(diffuseColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
	glTranslated(position.x,0.05,position.z);
	glRotated(-90,1,0,0);
	glBegin(GL_POLYGON);
	float numPoints = 20, aux;
    for(int i =0;i<numPoints;i++) {    
		aux = i*2*3.14159/numPoints;
		glVertex2f(cos(aux)*radius,sin(aux)*radius);
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

  if(position.y <= radius){
    position.y = radius;
    velocity.y = -velocity.y/2.5 ;
	velocity.x = velocity.x/1.1 ;
  }
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

  angle -= velocity.x;
  radiusShadow = radius*(position.y>4)? position.y*.1/radius: 1/position.y*radius*0.8;

}