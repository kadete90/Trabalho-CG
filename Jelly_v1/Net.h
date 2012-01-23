#ifndef _NET_H_
#define _NET_H_

#include <GL/glut.h>
#include <cggl/App.h>
#include <cggl/ObjModel.h>

class Net : public cggl::Object {
	
  cggl::Vector3 position; 
  const float size, widght, height;

  cggl::ObjModel* model;
  
public: 
  Net(cggl::Vector3 pos,  float size);

  void InitGL();
  void Draw();

  cggl::Vector3 getPosition() { return position; }
};

#endif