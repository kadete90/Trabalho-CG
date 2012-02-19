#include "ball.h" 
#include "Jelly.h"
#include <Windows.h>
#include <time.h>
#include <dos.h> // ter o SLEEP

using namespace cggl;

Ball::Ball(Vector3 pos, float rad, Jelly * _j1,Jelly * _j2,bool* _gameOver,bool* _useSound,int _maxHitsPerPlayer ): position(pos), radius(rad),j1(_j1),j2(_j2),gameOver(_gameOver),useSound(_useSound),maxHitsPerPlayer(_maxHitsPerPlayer){
	model = new ObjModel("models/whiteBall.obj");
	radiusShadow = radius * .8 + position.y*.05;
	positionToP1 = Vector3(-25,25,0);
	positionToP2 = Vector3(25,25,0);
	velocityToP1 = Vector3(3,6,0); 
	velocityToP2 = Vector3(-3,6,0);
	hitTimeBlock = 0;
	strenghHit = 3;
	startPlay = true;
	fstHitGround = false;
	hasHitPlayer = false;
	hitTimePlayer = 80;
	angleZ = 0;
	angleX = 0;
	playerLastHit  =-1;
	startPlayP1='q';
	startPlayP2='.';
}

void Ball::InitGL() { 
	Object::InitGL();
	model->InitGL();
	srand(time(NULL));
	int r = rand()%2;
	velocity = (r)? velocityToP1: velocityToP2;
	position = (r)? positionToP1: positionToP2;
}

void Ball::Draw(){
	Object::Draw();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//draw ball
	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glRotated(angleX,1,0,0);
	glRotated(angleZ,0,0,1);
	glScaled(radius,radius,radius);
	model->Draw();
	glPopMatrix();

	static GLfloat shadowColor[3] = { .15, .15, .15 };
	static GLfloat redShadowColor[3] = { .6, .0, .0 };
	//draw shadow
	drawShadow(shadowColor, position, radiusShadow);
	//draw red shadow (when ball hit the ground)
	if(fstHitGround)
		drawShadow(redShadowColor, lastPosHitGround, lastRadiusShadow);

	glDisable(GL_LIGHTING);
}
void Ball::drawShadow(GLfloat *color, Vector3 pos, float _radiusShadow){
	glPushMatrix();
	glColor3fv(color);
	glTranslated(pos.x, .01, pos.z);
	glRotated(90,1,0,0);
	glBegin(GL_POLYGON);
	float numPoints = 20, aux;
	for(int i = 0; i < numPoints; i++) {    
		aux = i*2*3.14159/numPoints;
		glVertex2f(cos(aux)*_radiusShadow,sin(aux)*_radiusShadow);
	}
	glEnd(); 
	glPopMatrix();
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
			if(App::Input->IsKeyPressed(startPlayP1)) { 
				startPlay=false; 
				angleZ = 0; angleX = 0; 
				velocity = velocityToP1; 
			}
		}
		else if (startPlay &&  position.x == positionToP2.x && position.y == positionToP2.y && position.z == positionToP2.z){
			if(App::Input->IsKeyPressed(startPlayP2)) { 
				startPlay=false;
				angleZ = 0; angleX = 0; 
				velocity = velocityToP2; 
			}
		}
		else{
			velocity.x += 2*(ax * t);
			velocity.y += 2*(ay * t);
			velocity.z += 2*(az * t);

			position.y = position.y + velocity.y*t + 0.5 * ay * t * t;
			position.x += velocity.x*t;
			position.z += velocity.z*t;

			// Calculos para saber quando bate num jogador
			// Foram feitos bloco contadores para diminuir as chamadas ao hit e assim optimizar o jogo
			boolean hitBlock = false;
			if(hasHitPlayer){
				if(--hitTimePlayer <= 0){
					hasHitPlayer = false;
					hitTimePlayer = 80;
				}
			}
			if(hitTimeBlock <= 0 && !fstHitGround && !hasHitPlayer){	
				vj1 = j1->hitJelly(position.x,position.y,position.z);
				vj2 = j2->hitJelly(position.x,position.y,position.z);

				if (!(vj1.x==-10 && vj1.y==-10 && vj1.z==-10)){
					hitBlock=true; hasHitPlayer = true;
					if (playerLastHit == 1 ) lastPlayerNumberOfHits++;
					else					lastPlayerNumberOfHits = 1;
					if (lastPlayerNumberOfHits == maxHitsPerPlayer){
						position = positionToP2;  
						j2->setPoint();
						startPlay=true;
						if (*useSound) PlaySound(TEXT("SOUNDS\\FOUL.WAV"), NULL, SND_ASYNC);
					}
					playerLastHit = 1;
					if(j1->getJump()){  //quando salta proximo da rede
						if(j1->GetPosition().x < -10){
							velocity.x = (vj1.x > 0)? 15: -10;
							velocity.y*= vj1.y * strenghHit/3;
						}
						else{
							if(App::Input->IsKeyPressed('q')) 	// faz amorti
								velocity.x = (vj1.x > 0)? 5: -5;
							else // faz remate
								velocity.x = (vj1.x > 0)? 20: -10;
								velocity.y = 10;
						}
						velocity.z = -vj1.z * strenghHit*1.5;
					}	
					else{
						velocity.x = vj1.x * strenghHit;
						velocity.y*= vj1.y * strenghHit;
						velocity.z = -vj1.z * strenghHit;
					}
				}

				if (!(vj2.x==-10 && vj2.y==-10 && vj2.z==-10)){ 
					hitBlock=true; hasHitPlayer = true;

					if (playerLastHit == 2 ) lastPlayerNumberOfHits++;
					else					lastPlayerNumberOfHits=1;
					if (lastPlayerNumberOfHits == maxHitsPerPlayer){
						position = positionToP1;  
						j1->setPoint();
						startPlay=true;
						if (*useSound) PlaySound(TEXT("SOUNDS\\FOUL.WAV"), NULL, SND_ASYNC);
					}
					playerLastHit=2;
					//e-p . d - n
					velocity.x = vj2.x*-1;
					if(j2->getJump()){  
						if(j2->GetPosition().x > 10){
							velocity.x = (vj2.x > 0)? -15: 10;
							velocity.y*= vj2.y * strenghHit/3;
						}
						else{ //quando salta proximo da rede
							if(App::Input->IsKeyPressed('.')) 	// faz amorti
								velocity.x = (vj2.x > 0)? -5: 5;
							else // faz remate
								velocity.x = (vj2.x > 0)? -20: 10;
							velocity.y = 8;
						}
						velocity.z = -vj2.z * strenghHit*1.5;
					}	
					else{
						velocity.x = velocity.x * strenghHit;
						velocity.y*= vj2.y * strenghHit;
						velocity.z =-vj2.z * strenghHit;
					}
				}
			}
			if(hitBlock) hitTimeBlock = 20;
			else --hitTimeBlock;

			// Perde velocidade quando bate na areia
			if(position.y <= radius){
				position.y = radius;
				velocity.y /= -2.5 ;
				velocity.x /= 1.4 ;
				velocity.z /= 1.4 ;
			}

			// calculos para quando bate na rede
			if(position.x >= -(radius +.5) && position.x <= radius + .5){
				if( position.y < 16 && position.z >= -17 && position.z <=17 && velocity.y < 0){
					velocity.y = fabs(velocity.y)*.8;
					if(position.x > 0)
						velocity.x = (position.y > 10)? fabs(velocity.x) : 5 ;
					else
						velocity.x = (position.y > 10)? -fabs(velocity.x) : -5 ;
				}
				else if(position.y >= 16 && position.y < 16.5 && position.z > -17 && position.z < 17){
					velocity.y = fabs(velocity.y);
					velocity.x = (position.x > 0) ? fabs(velocity.x)*.8: -fabs(velocity.x)*.8;
				}
			}

			//calculos para saber quando e onde bate no chao
			if(position.y - radius <= 0 || fabs(position.x) > 65){
				if(!fstHitGround){
					lastPosHitGround = position;
					lastRadiusShadow = radiusShadow;
					fstHitGround = true;
				}
				else if((velocity.x <= .15 && velocity.x >= -.15 && velocity.z <= .15 && velocity.z >= -.15 && fstHitGround) || fabs(position.x) > 65) {
					boolean pontoJ1 = false;
					int aux = radiusShadow;
					if (lastPosHitGround.x < 0 ){
						if (lastPosHitGround.x >=-(38+aux) && lastPosHitGround.z >=-(17+aux) && lastPosHitGround.z<=17+aux) j2->setPoint();
						else if (playerLastHit == 2) {j1->setPoint(); pontoJ1 = true;}
						else j2->setPoint();
					}
					else{ 
						if (lastPosHitGround.x <= 38+aux && lastPosHitGround.z >=-(17+aux) && lastPosHitGround.z<=17+aux) {j1->setPoint(); pontoJ1 = true;}
						else if (playerLastHit == 2) {j1->setPoint(); pontoJ1 = true;}
						else j2->setPoint();
					}
					// #####		Apito do arbitro que indica que houve ponto		################
					if (*useSound) PlaySound(TEXT("SOUNDS\\REFEREEWHISTLE.WAV"), NULL, SND_ASYNC);

					fstHitGround = false;
					lastPlayerNumberOfHits=0;
					if (pontoJ1) {
						position = positionToP1; 
						startPlay = true;
					}
					else {
						position = positionToP2; 
						startPlay = true;
					}
					j1->resetPosition();
					j2->resetPosition();
				}
			}

			radiusShadow = radius * .8 + position.y*.05;

			if(velocity.x > 20) velocity.x = 20;
			else if(velocity.x < -20) velocity.x = -20;
			if(velocity.y > 30) velocity.y = 30;
			if(velocity.z > 15) velocity.z = 15;
			else if(velocity.z < -15) velocity.z = -15;

			angleZ -= velocity.x*.2;
			angleX -= -velocity.z*.2;
		}		
	}
}