/*
** IconFaceLib.h
** 
** Made by 
** Login   <lmalek@tornado>
** 
** Started on  Thu Dec 17 09:12:04 2009 
** Last update Thu Dec 17 09:12:04 2009 
*/

#ifndef   	ICONFACELIB_H_
# define   	ICONFACELIB_H_
#include <allegro.h>

#define PI 3.14159265

struct Eye_t
{
  short pupilRadius; // pupil radius in polar cordr [0,100]
  short pupilAngle; // pupil angle in polar cord [0,100]
  short lidClosure; // eyelid closure factor [0,100]
  short lidAngle; // eyelid angle factor [-100,100]
  Eye_t();
  Eye_t(short pupilRadiusInit, short pupilAngleInit, short lidClosureInit, short lidAngleInit);
  Eye_t operator+(const Eye_t &add);
};


struct CharVector2D_t 
{
  short x;
  short y;
  CharVector2D_t();
  CharVector2D_t(short xInit, short yInit);
  CharVector2D_t(const CharVector2D_t & init);
  CharVector2D_t operator*(char scale);
  CharVector2D_t operator+(const CharVector2D_t &add);
};

struct Mouth_t{
  CharVector2D_t left; // position factor 
  CharVector2D_t top; // position factor  
  CharVector2D_t bottom; // position factor 
  CharVector2D_t right; // position factor 
  Mouth_t();
  Mouth_t(CharVector2D_t leftInit, CharVector2D_t topInit,
	  CharVector2D_t bottomInit, CharVector2D_t rightInit);
  Mouth_t(const Mouth_t & init);
  Mouth_t operator*(char scale);
  Mouth_t operator+(const Mouth_t & add);
};

struct FaceParams_t{
  Eye_t eyeL;
  Eye_t eyeR;
  Mouth_t mouth;
  FaceParams_t();
  FaceParams_t(Eye_t eyeLInit, Eye_t eyeRInit, Mouth_t mouthInit);
  FaceParams_t operator*(char scale);
  FaceParams_t operator+(const FaceParams_t &add);
};

class IconFace_t { // iconic head is represented in factor dimmention
		 // and is scaled while drawing
public:
  IconFace_t(int width, int height);
  void Draw(BITMAP *image); // width and height of the image
  char SetDim(int width, int height);
  void SetParams(FaceParams_t params);
  FaceParams_t _params;
  int _minDim;
  float _scaleFactor;
  int _offsetX;
  int _offsetY;
  int _width; 
  int _height;
private:
};

#define TRANSPARENT_COLOR makecol(255,0,255)



#endif 	    /* !ICONFACELIB_H_ */
