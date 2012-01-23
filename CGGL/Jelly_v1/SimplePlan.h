#ifndef _SIMPLE_PLANE_H_
#define _SIMPLE_PLANE_H_

#include <cggl/App.h>

class SimplePlan : public cggl::Object {

  cggl::Vector3 position, front;
  float width, height, angle;
  char fileName[FILENAME_MAX]; 
  cggl::glTexture texture;

public: 
  SimplePlan(float _width, float _height, float _angle, cggl::Vector3 pos, cggl::Vector3 _front, char* _fileName) 
	: width(_width), height(_height), angle(_angle), position(pos), front(_front) 
	{ strcpy(fileName, _fileName);  front.y = 0; front.Normalize(); }

  void Draw();
  void InitGL();
};

#endif