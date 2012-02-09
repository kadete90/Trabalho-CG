#include <GL/glut.h>
#include <cggl/App.h>
#include <cggl/FPSCounter.h>

#include "JellyApp.h"
#include "Ball.h"
#include "SimplePlan.h"
#include "Jelly.h"
#include "Net.h"

using namespace cggl;

void JellyApp::CreateScene() {
	RegisterObject(p1 = new Jelly(Vector3(-15, 0, 0), 3.5, 4, 1,'a','d','w','s','e')) ;
	RegisterObject(p2 = new Jelly(Vector3(15, 0, 0), 3.5, 4, 2,(char)GLUT_KEY_LEFT,(char)GLUT_KEY_RIGHT,(char)GLUT_KEY_UP,(char)GLUT_KEY_DOWN,'-')) ;
	RegisterObject( net = new Net(Vector3(0, 6, 0), 20) ) ;
	RegisterObject( ball = new Ball(Vector3(0, 20, 0), 1.2,p1,p2) );
	RegisterObject(new SimplePlan(100, 100, Vector3(0,0,0), Vector3(0,0,0),"textures/sandBeach2.jpg", 0) ); // width, height, rotate, pos, texture
	RegisterObject( new SimplePlan(100, 65, Vector3(-1,0,0), Vector3(0,65,-100),"textures/background.jpg", 0) );
	RegisterObject( new SimplePlan(100, 65, Vector3(-1,0,1), Vector3(100,65,0),"textures/background.jpg", 0) );
	glPushMatrix();
	glRotated(180, 0,1,0);
	RegisterObject( new SimplePlan(100, 65, Vector3(-1,0,1), Vector3(-100,65,0),"textures/background.jpg", 1) );
	glPopMatrix();

	RegisterObject( new FPSCounter() );
}

void JellyApp::Draw() {
	App::Draw();
	int xScore = App::Writer->GetMaxX()/2, yScore = 60;
	char txtP1[5];
	char txtP2[5];
	sprintf(txtP1, "%d", p1->getPoints());
	sprintf(txtP2, "%d", p2->getPoints());
	glColor3f(1, 1, 1);
	App::Writer->BeginWriteText();
	App::Writer->WriteText(xScore-50,  yScore, txtP1, GLUT_BITMAP_TIMES_ROMAN_24);
	App::Writer->WriteImediateText(xScore, yScore, "-", GLUT_BITMAP_TIMES_ROMAN_24);
	App::Writer->WriteText(xScore+50,  yScore, txtP2, GLUT_BITMAP_TIMES_ROMAN_24);
	App::Writer->EndWriteText();
}

void JellyApp::Update(int deltaTimeMilis) {	
	if(App::Input->IsKeyPressed('x')) exit(0);
	if(App::Input->IsKeyPressed('c'))
		App::Camera->LookAt(Vector3(0, 20, 60), ball->getPosition(), Vector3(0,1,0));
	else if(App::Input->IsKeyPressed('0'))
		App::Camera->LookAt(Vector3(0, 120, 0.1), Vector3(0,0,0), Vector3::UP);
	else if(App::Input->IsKeyPressed('9')){
		App::Camera->LookAt(Vector3(50, 100, 0.3), Vector3(0,0,0), Vector3::UP);
		p1->leftKey = 'w';
		p1->downKey = 'a';
		p1->rightKey= 's';
		p1->upKey	= 'd';

		p2->leftKey = (char)GLUT_KEY_UP;
		p2->downKey = (char)GLUT_KEY_LEFT;
		p2->rightKey= (char)GLUT_KEY_DOWN;
		p2->upKey	= (char)GLUT_KEY_RIGHT;

	}
	else if(App::Input->IsKeyPressed('8'))
		App::Camera->LookAt(Vector3(50, 100, 300), Vector3(0,0,0), Vector3::UP);
	else if(App::Input->IsKeyPressed('7'))
		App::Camera->LookAt(Vector3(100, 100, 0.3), Vector3(0,0,0), Vector3::UP);
	else if(App::Input->IsKeyPressed('6'))
		App::Camera->LookAt(Vector3(000, 100, 90), Vector3(0,0,0), Vector3::UP);
	else if(App::Input->IsKeyPressed('5'))
		App::Camera->LookAt(Vector3(00, 20, 90), Vector3(0,20,0), Vector3::UP);
	else
	{
		App::Camera->LookAt(Vector3(0, 100, 90), Vector3(0,10,-20), Vector3(0,1,0));
		p1->leftKey = 'a';
		p1->downKey = 's';
		p1->rightKey= 'd';
		p1->upKey = 'w';

		p2->leftKey = (char)GLUT_KEY_LEFT;
		p2->downKey = (char)GLUT_KEY_DOWN;
		p2->rightKey= (char)GLUT_KEY_RIGHT;
		p2->upKey	= (char)GLUT_KEY_UP;
	}
	App::Update(deltaTimeMilis);
}

int main(int argc, char** argv){
	JellyApp app;
	boolean x = false;
	printf(" Teclas do jogador 1 (verde): \n \t Esquerda - a \n\t Direita - d \n\t baixo - s \n\t cima - w \n \t saltar e \n"); 
	printf(" Teclas do jogador 2 (vermelho): \n \t Esquerda - LeftArrow \n\t Direita - RightArrow \n\t baixo - DownArrow \n\t cima - UpArrow \n \t saltar e \n");


	printf("Press AnyKey to start the game\n");
	//system("PAUSE");
	app.Run(argc, argv);

}