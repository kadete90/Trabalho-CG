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
	RegisterObject(p1 = new Jelly(Vector3(-15, 0, 0), 3.5, 4, 1,&gameOver,'a','d','w','s','e')) ;
	RegisterObject(p2 = new Jelly(Vector3(15, 0, 0), 3.5, 4, 2 ,&gameOver,(char)GLUT_KEY_LEFT,(char)GLUT_KEY_RIGHT,(char)GLUT_KEY_UP,(char)GLUT_KEY_DOWN,'-')) ;
	RegisterObject( net = new Net(Vector3(0, 6, 0), 20) ) ;
	RegisterObject( ball = new Ball(Vector3(0, 20, 0), 1.2,p1,p2,&gameOver) );
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
	sprintf(txt, "%d - %d", p1->getPoints(),p2->getPoints());
	glColor3f(1, 1, 1);
	App::Writer->BeginWriteText();
	App::Writer->WriteText(xScore -20,  yScore, txt, GLUT_BITMAP_TIMES_ROMAN_24);
	if (txt2 != "") App::Writer->WriteText(xScore-50,  yScore+50, txt2, GLUT_BITMAP_HELVETICA_18);
	App::Writer->EndWriteText();
}

void JellyApp::Update(int deltaTimeMilis) {	
	App::Update(deltaTimeMilis);

	if (gameOver){
		sprintf(txt2,"Press k to play again");
		printf("CICLO GameOver %d \n",gameOver);


		printf("%d | %d \n",p1->getPoints(),p2->getPoints());

		if ( App::Input->IsKeyPressed('k')){
			setGameOver(false);
			sprintf(txt2, "%s", "");
			p1->resetPoint();
			p2->resetPoint();
		}
	}
	else{

		if(App::Input->IsKeyPressed('x')) exit(0);
		if(App::Input->IsKeyPressed('c'))
			App::Camera->LookAt(Vector3(0, 20, 60), ball->getPosition(), Vector3(0,1,0));
		else if(App::Input->IsKeyPressed('0'))
			App::Camera->LookAt(Vector3(0, 120, 0.1), Vector3(0,0,0), Vector3::UP);
		else if(App::Input->IsKeyPressed('9')){
			App::Camera->LookAt(Vector3(50, 90, 0.3), Vector3(0,0,0), Vector3::UP);
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
		else if(App::Input->IsKeyPressed('5'))
			App::Camera->LookAt(Vector3(00, 20, 90), Vector3(0,20,0), Vector3::UP);
		else
		{
			App::Camera->LookAt(Vector3(0, 60, 90), Vector3(0,0,-50), Vector3::UP);
			p1->leftKey = 'a';
			p1->downKey = 's';
			p1->rightKey= 'd';
			p1->upKey = 'w';

			p2->leftKey = (char)GLUT_KEY_LEFT;
			p2->downKey = (char)GLUT_KEY_DOWN;
			p2->rightKey= (char)GLUT_KEY_RIGHT;
			p2->upKey	= (char)GLUT_KEY_UP;
		}


		if (p1->hasWon()){
			sprintf(txt, "%s", "GANHOU PLAYER 1");
			setGameOver(true);
		}
		else if (p2->hasWon()){
			sprintf(txt, "%s", "GANHOU PLAYER 2");
			setGameOver(true);
		}

		printf("T1 %d T2 %d\n",p1->getTouchNumber(),p2->getTouchNumber());
		printf("\n gameOver %d \n",gameOver);
	}
}
void JellyApp::setGameOver(bool b ){
	gameOver=b;
}

int main(int argc, char** argv){
	JellyApp app;

	app.setGameOver(false);

	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	printf("...............................................................................\n");                                                                                                                                                       
	printf("      . .              .                                .                      \n");
	printf("      MM       . OMZ  MM ..       . MM     MM~ .       MM OMZ   .   .          \n");
	printf("      =M          MO  MM.            MM    MM .        MM  MO..    .           \n");
	printf("      IM   ,MMN   M8  MM 7M   ,M     ?M+  ,M7   MMO    MM  M8 . ,MMN  M,   M.  \n");
	printf("      ZM  .MM.M8. MM  MM  M~  MM.     NM .MM  .MMIMM,  MM. MM  DMZZM  MM  +M   \n");
	printf("      ZM. MM .MM  M8  MM  DM.+M .      M .MZ  ZM  .MM. MM  M8  MM .MN  MZ MN   \n");
	printf("      OM  MMMMMM  MN  MM  .MDMM .      MMMM   MM   OM  MM  MN. MMMMMN  MM$M    \n");
	printf("    . DM  MM    . MO  MM   MMMI        =MM+   MM   MM  MM .MO. MM      =MMM.   \n");
	printf("   =MMMM   MMOMM MMM  MM    MM .        MM     MMMMM,  MM.MMM  DMMDMM   DM7    \n");
	printf("    IN,..   :N+~  .  .. . .IM~               .   =:     .  .     7O+   .MM .   \n");
	printf("     . .        .    . .  .MM           .     .  .      .          .   MM      \n");
	printf("                        MMMN                                       .?MMM,      \n");                                                                    
	printf("...............................................................................\n");
	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\n");
	printf(" Teclas do jogador 1 (verde): \n\t Esquerda: A | Direita: D | Baixo: S | Cima: W | Saltar: E \n\n"); 
	printf(" Teclas do jogador 2 (vermelho): \n\t Esquerda: LEFT | Direita: RIGHT | Baixo: DOWN | Cima: UP | Saltar: - \n\n");
	//system("PAUSE");
	app.Run(argc, argv);

}