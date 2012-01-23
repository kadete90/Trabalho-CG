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
  RegisterObject(p1 = new Jelly(Vector3(-15, 0, 0), 3.5, 4, 1)) ;
  RegisterObject(p2 = new Jelly(Vector3(15, 0, 0), 3.5, 4, 2)) ;
  RegisterObject( net = new Net(Vector3(0, 6, 0), 40) ) ;
  RegisterObject( ball = new Ball(Vector3(10, 4, 0), 1.2) );
  RegisterObject( new SimplePlan(100, 100, 0, Vector3(0,0,0),Vector3(0,1,0),"textures/sandBeach2.jpg") ); // width, height, angle.x, pos, front, texture
  RegisterObject( new SimplePlan(100, 60, -90, Vector3(0,60,-100),Vector3(0,1,0),"textures/background.jpg") ); 
  RegisterObject( new FPSCounter() );
}

void JellyApp::Draw() {
  App::Draw();
}

void JellyApp::Update(int deltaTimeMilis) {	
	if(App::Input->IsKeyPressed('x')) exit(0);
	if(App::Input->IsKeyPressed('c'))
		App::Camera->LookAt(Vector3(0, 20, 50), ball->getPosition(), Vector3(0,1,0));
	else
		App::Camera->LookAt(Vector3(0, 20, 80), Vector3(0,20,0), Vector3(0,1,0));
	App::Update(deltaTimeMilis);
}

int main(int argc, char** argv){
  JellyApp app;
  app.Run(argc, argv);
}