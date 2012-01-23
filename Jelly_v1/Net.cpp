#include "Net.h" 

using namespace cggl;

Net::Net(Vector3 pos, float _size): position(pos), size(_size), widght (10), height(10) {
	model = new ObjModel("models/netVolley2.obj");
}

void Net::InitGL() { 
  Object::InitGL();
  model->InitGL();
}

void Net::Draw(){
  Object::Draw();

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glPushMatrix();
    glTranslated(position.x, position.y, position.z);
	glScaled(size,size,size);
    model->Draw();
  glPopMatrix();

  glDisable(GL_LIGHTING);
}