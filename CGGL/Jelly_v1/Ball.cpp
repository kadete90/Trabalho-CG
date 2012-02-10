#include "ball.h" 
#include "Jelly.h"
#include <Windows.h>
#include <dos.h> // ter o SLEEP
using namespace cggl;

Ball::Ball(Vector3 pos, float rad, Jelly * _j1,Jelly * _j2,bool* _gameOver ): position(pos), radius(rad),j1(_j1),j2(_j2),gameOver(_gameOver){
	model = new ObjModel("models/whiteBall.obj");
	radiusShadow = radius * .8 + position.y*.05;
	positionToP1 = Vector3(-25,25,0);
	positionToP2 = Vector3(25,25,0);
	velocityToP1 = Vector3(4,6,0); 
	velocityToP2 = Vector3(-4,6,0);
	angleZ = 20;
	angleX =0;
	hitTimeBlock =0;
	strenghHit = 2,5;
	fstHitGround = false;
	startPlay = true;
	playerLastHit  =-1;
	startPlayP1='q';
	startPlayP2='l';
	maxHitsPerPlayer=5;
}

void Ball::InitGL() { 
	Object::InitGL();
	model->InitGL();
	int r = rand()%2;
	velocity = (r)? velocityToP1: velocityToP2;
	position = (r)? positionToP1: positionToP2;

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


	if (!(*gameOver)){
		double t = deltaTimeMilis/(float)1000;
		double mass = 1.5;
		double fg = mass * -9.8;
		double fdx = -6*3.14*radius*0.00001827*velocity.x;
		double fdy = -6*3.14*radius*0.00001827*velocity.y;
		double fdz = -6*3.14*radius*0.00001827*velocity.z;

		double ax = (fdx) / mass;
		double ay = (fg + fdy) / mass;
		double az = (fdz)/mass;

		Vector3 vj1 = NULL;
		Vector3 vj2 = NULL;

		if (startPlay && position.x == positionToP1.x && position.y == positionToP1.y && position.z == positionToP1.z){
			if(App::Input->IsKeyPressed(startPlayP1)) { startPlay=false;}
		}
		else if (startPlay &&  position.x == positionToP2.x && position.y == positionToP2.y && position.z == positionToP2.z){
			if(App::Input->IsKeyPressed(startPlayP2)) { startPlay=false;}
		}
		else{

			velocity.x += 2*(ax * t);
			velocity.y += 2*(ay * t);
			velocity.z += 2*(az * t);



			position.y = position.y + velocity.y*t + 0.5 * ay * t * t;
			position.x += velocity.x*t;
			position.z += velocity.z*t;

			boolean hitBlock=false;

			if(hitTimeBlock <= 0 && !fstHitGround ){
				vj1 = j1->hitJelly(position.x,position.y,position.z);
				vj2 = j2->hitJelly(position.x,position.y,position.z);

				if (!(vj1.x==-10 && vj1.y==-10 && vj1.z==-10)){
					hitBlock=true;
					if (playerLastHit ==1 ) lastPlayerNumberOfHits++;
					else					lastPlayerNumberOfHits=1;
					if (lastPlayerNumberOfHits == maxHitsPerPlayer){
						PlaySound(TEXT("SOUNDS\\FOUL.WAV"), NULL, SND_ASYNC);
						j2->setPoint();
						position = positionToP2; 
						velocity = velocityToP2;
						startPlay=true;
					}

					playerLastHit = 1;
					velocity.x = vj1.x * strenghHit;
					velocity.y*= vj1.y * strenghHit;
					velocity.z = -vj1.z * strenghHit;
				}

				if (!(vj2.x==-10 && vj2.y==-10 && vj2.z==-10)){ 
					hitBlock=true;

					if (playerLastHit ==2 ) lastPlayerNumberOfHits++;
					else					lastPlayerNumberOfHits=1;
					if (lastPlayerNumberOfHits == maxHitsPerPlayer){
						PlaySound(TEXT("SOUNDS\\FOUL.WAV"), NULL, SND_ASYNC);
						j1->setPoint();
						position = positionToP1; 
						velocity = velocityToP1;
						startPlay=true;
					}
					playerLastHit=2;
					if(velocity.x >0) velocity.x = vj2.x*-1; 
					else velocity.x = vj2.x*-1;
					velocity.x*= vj2.x * strenghHit;
					velocity.y*= vj2.y * strenghHit;
					velocity.z =-vj2.z * strenghHit;
				}
			}


			if(hitBlock) hitTimeBlock = 20;
			else --hitTimeBlock;

			angleZ -= velocity.x;
			angleX -= -velocity.z;
			radiusShadow = radius * .8 + position.y*.05;

			if(position.y <= radius){
				position.y = radius;
				velocity.y = -velocity.y/2.5 ;
				velocity.x = velocity.x/1.2 ;
				velocity.z = velocity.z/1.2 ;
			}
			if(position.x >= -radius && position.x <= radius){
				if( position.y < 15-radius && position.z >= -17 && position.z <=17){
					printf("Erro! 1");
					velocity.y = velocity.y*0.8;
					velocity.x = (position.y >10)?-velocity.x: -velocity.x*0.5;
				}
				else if(position.y >= 15-radius && position.y < 15 && position.z > -17 && position.z < 17){
					velocity.y = -velocity.y;
					printf("Erro! 2");
				}
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
						PlaySound(TEXT("SOUNDS\\REFEREEWHISTLE.WAV"), NULL, SND_ASYNC);

						fstHitGround = false;


						if (pontoJ1) {
							position = positionToP1; 
							velocity = velocityToP1;
							startPlay=true;
						}
						else {
							position = positionToP2; 
							velocity = velocityToP2;
							startPlay=true;
						}
				}
			}

			if(velocity.x > 15) velocity.x = 15;
			else if(velocity.x < -15) velocity.x = -15;
			if(velocity.y > 28) velocity.y = 28;
			if(velocity.z > 15) velocity.z = 15;
			else if(velocity.z < -15) velocity.z = -15;
		}
	}
}
