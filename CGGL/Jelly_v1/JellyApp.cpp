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
	RegisterObject(p1 = new Jelly(Vector3(-15, 0, 0), 3.5, 4, 1,&gameOver,gamesToWin,'a','d','w','s','e')) ;
	RegisterObject(p2 = new Jelly(Vector3(15, 0, 0), 3.5, 4, 2 ,&gameOver,gamesToWin,(char)GLUT_KEY_LEFT,(char)GLUT_KEY_RIGHT,(char)GLUT_KEY_UP,(char)GLUT_KEY_DOWN,'0')) ;
	RegisterObject(net = new Net(Vector3(0, 5, 0), 20) ) ;
	RegisterObject(ball = new Ball(Vector3(0, 20, 0), 1.2, p1, p2, &gameOver,&useSound,maxHitsPerPlayer) );
	RegisterObject(new SimplePlan(100, 100, Vector3(0,0,0), Vector3(0,0,0),"textures/sandBeach2.jpg", 0) ); // width, height, rotate, pos, texture
	RegisterObject(new SimplePlan(100, 65, Vector3(-1,0,0), Vector3(0,65,-100),"textures/background.jpg", 0) );
	RegisterObject(new SimplePlan(100, 65, Vector3(-1,0,1), Vector3(100,65,0),"textures/background.jpg", 0) );
	glPushMatrix();
	glRotated(180, 0,1,0);
	RegisterObject(new SimplePlan(100, 65, Vector3(-1,0,1), Vector3(-100,65,0),"textures/background.jpg", 1) );
	glPopMatrix();
	RegisterObject(new FPSCounter() );
	setKeyBoardHorizontal();
	sprintf(txtCamera,"Camera [1]");
	_eye = Vector3(0, 60, 90); 
	_center = Vector3(0,0,-50);
	_up = Vector3::UP;
}

void JellyApp::Draw() {
	App::Draw();
	int xScore = App::Writer->GetMaxX()/2;
	bool aux = !p1->hasWon() && !p2->hasWon();
	if(aux){
		glColor3f(0, 0, 0);
		sprintf(txt, "%d - %d", p1->getPoints(),p2->getPoints());
	}
	else
		glColor3f(1, 0, 0);
	App::Writer->BeginWriteText();
	App::Writer->WriteText((aux)? xScore-30: xScore-80, 50, txt, GLUT_BITMAP_TIMES_ROMAN_24);
	if (txt2 != "") {
		glColor3f(0, 0, 0);
		App::Writer->WriteText(xScore-75,  80, txt2, GLUT_BITMAP_HELVETICA_18);
	}
	glColor3f(0, 0, 0);
	App::Writer->WriteText(20, 30, txtCamera, GLUT_BITMAP_HELVETICA_18);
	App::Writer->EndWriteText();
}

void JellyApp::Update(int deltaTimeMilis) {	
	App::Update(deltaTimeMilis);

	if (gameOver){
		sprintf(txt2,"Press k to play again");
		if ( App::Input->IsKeyPressed('k')){
			setGameOver(false);
			p1->resetPoint();
			p2->resetPoint();
		}
	}
	else{
		sprintf(txt2, "%s", "");
		if(App::Input->IsKeyPressed('x')) exit(0);
		if(App::Input->IsKeyPressed('b')){
			sprintf(txtCamera,"Camera following ball");
			_eye = Vector3(0, 20, 60); 
			_center = ball->getPosition(); 
			_up = Vector3(0,1,0);
		}
		else if(App::Input->IsKeyPressed('1')){
			setKeyBoardHorizontal();
			sprintf(txtCamera,"Camera [1]");
			_eye = Vector3(0, 60, 90); 
			_center = Vector3(0,0,-50);
			_up = Vector3::UP;
		}
		else if(App::Input->IsKeyPressed('2')){
			setKeyBoardHorizontal();
			sprintf(txtCamera,"Camera [2]");
			_eye = Vector3(0, 120, .1); 
			_center =  Vector3(0,0,0); 
			_up = Vector3::UP;
		}
		else if(App::Input->IsKeyPressed('3')){
			setKeyBoardVertical();
			sprintf(txtCamera,"Camera [3]");
			_eye = Vector3(50, 90, .3);
			_center = Vector3(0,0,0);
			_up = Vector3::UP;
		}
		else if(App::Input->IsKeyPressed('4')){
			setKeyBoardHorizontal();
			sprintf(txtCamera,"Camera [4]");
			_eye = Vector3(50, 100, 300);
			_center = Vector3(0,0,0);
			_up = Vector3::UP;
		}
		else if(App::Input->IsKeyPressed('5')){
			setKeyBoardVertical();
			sprintf(txtCamera,"Camera [5]");
			_eye = Vector3(100, 100, .3);
			_center = Vector3(0,0,0);
			_up = Vector3::UP;
		}
		else if(App::Input->IsKeyPressed('6')){
			setKeyBoardHorizontal();
			sprintf(txtCamera,"Camera [6]");
			_eye = Vector3(00, 20, 90); 
			_center = Vector3(0,20,0); 
			_up = Vector3::UP;
		}

		App::Camera->LookAt(_eye, _center,_up);

		if (p1->hasWon()){
			sprintf(txt, "%s", "PLAYER 1 WINS");
			setGameOver(true);
		}
		else if (p2->hasWon()){
			sprintf(txt, "%s", "PLAYER 2 WINS");
			setGameOver(true);
		}
	}
}

void JellyApp::setKeyBoardHorizontal(){
	p1->leftKey = 'a';
	p1->downKey = 's';
	p1->rightKey= 'd';
	p1->upKey = 'w';

	p2->leftKey = (char)GLUT_KEY_LEFT;
	p2->downKey = (char)GLUT_KEY_DOWN;
	p2->rightKey= (char)GLUT_KEY_RIGHT;
	p2->upKey	= (char)GLUT_KEY_UP;
}
void JellyApp::setKeyBoardVertical(){
	p1->leftKey = 'w';
	p1->downKey = 'a';
	p1->rightKey= 's';
	p1->upKey	= 'd';

	p2->leftKey = (char)GLUT_KEY_UP;
	p2->downKey = (char)GLUT_KEY_LEFT;
	p2->rightKey= (char)GLUT_KEY_DOWN;
	p2->upKey	= (char)GLUT_KEY_RIGHT;
}

void JellyApp::setGameOver(bool b ){
	gameOver=b;
}
void JellyApp::setUseSound(bool b ){
	useSound=b;
}
void JellyApp::setGamesToWin(int i ){
	gamesToWin=i;
}
void JellyApp::setMaxHitsPerPlayer(int i ){
	maxHitsPerPlayer=i;
}
int JellyApp::getGamesToWin(){ return gamesToWin;}
int JellyApp::getMaxHitsPerPlayer(){ return maxHitsPerPlayer;}
bool JellyApp::getUseSound(){return useSound;}

int main(int argc, char** argv){
	JellyApp app;
	app.setGameOver(false);
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
	printf("...............................................................................\n\n");
	printf(" Teclas do jogador 1 (verde): \n\t LEFT: A | RIGHT: D | DOWN: S | UP: W \n\t JUMP: E | LetBall: Q \n\n"); 
	printf(" Teclas do jogador 2 (vermelho): \n\t LEFT : LEFT KEY | RIGHT : RIGHT KEY | DOWN : DOWN KEY | UP: UP KEY \n\t JUMP: 0 | LetBall: . \n\n");

	app.setUseSound(true);
	app.setGamesToWin(10);
	app.setMaxHitsPerPlayer(5);

	int op;
	int aux =0;
	printf(" MENU : (Insert number of option)\n Option 0 :Exit menu \n Option 1 : Enable sound \n Option 2 : Disable sound \n ");
	printf("Option 3 : Change number of points to win the match \n Option 4 : Change number of hits per player\n");
		
	do{
		fscanf(stdin,"%d",&op);
		switch (op){
		case 0:
			printf("\n \n Configuration sucessfull !\n ");
			break;
		case 1:
			app.setUseSound(true);
			printf("Sound enabled !\n");
			break;
		case 2:
			app.setUseSound(false);
			printf("Sound disabled!\n");
			break;
		case 3:
			printf("Set number of points to win match.\n");
			fscanf(stdin,"%d",&aux);
			if (aux >=1 ){
				app.setGamesToWin(aux);
				printf("The player that scores %d points first, win. \n",aux);}
			else
				printf("Number of points invalid. The player that scores %d points first, win. \n",app.getGamesToWin());
			break;
		case 4:
			printf("Set how many times a player can touch the ball per turn.\n");
			fscanf(stdin,"%d",&aux);
			if (aux >2 ){
				app.setMaxHitsPerPlayer(aux);
				printf("One player can only touch the ball %d times per turn\n",aux);}
			else 
				printf("That value is too low. The maximum touch per turn will be %d.\n",app.getMaxHitsPerPlayer());
			break;
		default:
			printf("Invalid Option");
		}
	}while (op!=0);

	printf("\n\n Sound %s \n PointsToWin %d \n Touch per Player %d \n\n",(app.getUseSound())? "enable" : "disable",app.getGamesToWin(),app.getMaxHitsPerPlayer());



	system("PAUSE");
	app.Run(argc, argv);
}