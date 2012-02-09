#ifndef _SIMPLE_PLANE_H_
#define _SIMPLE_PLANE_H_

#include <cggl/App.h>

class SimplePlan : public cggl::Object {

  cggl::Vector3 position, rotate;
  float width, height;
  char fileName[FILENAME_MAX];
  cggl::glTexture texture;
  int invertImage;

public: 
  SimplePlan(float _width, float _height, cggl::Vector3 _rotate, cggl::Vector3 pos, char* _fileName, int _invertImage) 
	: width(_width), height(_height), rotate(_rotate), position(pos), invertImage(_invertImage) { strcpy(fileName, _fileName); }

  void Draw();
  void InitGL();
};

#endif