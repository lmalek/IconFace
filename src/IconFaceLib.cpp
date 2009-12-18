#include "IconFaceLib.h"
#include <cmath>

#define PI 3.14159265

Eye_t::Eye_t():
  pupilRadius(0), pupilAngle(0), lidClosure(0), lidAngle(0)
{
}
 
Eye_t::Eye_t(char pupilRadiusInit, char pupilAngleInit, char lidClosureInit, char lidAngleInit):
  pupilRadius(pupilRadiusInit), pupilAngle(pupilAngleInit), lidClosure(lidClosureInit), lidAngle(lidAngleInit)
{
}

Eye_t Eye_t::operator+(const Eye_t &add)
{
  Eye_t out;
  out.pupilRadius = this->pupilRadius + add.pupilRadius;
  out.pupilAngle = this->pupilAngle + add.pupilAngle;
  out.lidClosure = this->lidClosure + add.lidClosure;
  out.lidAngle = this->lidAngle + add.lidAngle;
  return out;
}

CharVector2D_t::CharVector2D_t()
{
  x=0;
  y=0;
}

CharVector2D_t::CharVector2D_t(char xInit, char yInit)
{
  x=xInit;
  y=yInit;
}

CharVector2D_t::CharVector2D_t(const CharVector2D_t & init)
{
  x=init.x;
  y=init.y;
}

CharVector2D_t CharVector2D_t::operator*(char scale)
{
  CharVector2D_t out;
  out.x = this->x * scale / 100;
  out.y = this->y * scale /100;
  return out;
}

CharVector2D_t CharVector2D_t::operator+(const CharVector2D_t & add)
{
  CharVector2D_t out;
  out.x = this->x + add.x;
  out.y = this->y + add.y; 
  return out;
}


Mouth_t::Mouth_t():
  left(0,0), top(0,0), bottom(0,0), right(0,0)
{
}

Mouth_t::Mouth_t(CharVector2D_t leftInit, CharVector2D_t topInit, 
		 CharVector2D_t bottomInit, CharVector2D_t rightInit):
  left(leftInit), top(topInit), bottom(bottomInit),  right(rightInit)
{
}

Mouth_t::Mouth_t(const Mouth_t&init):
  left(init.left), top(init.top), bottom(init.bottom), right(init.right)
{
}

Mouth_t Mouth_t::operator*(char scale)
{
  Mouth_t out;
  out.left = this->left * scale;
  out.top = this->top * scale;
  out.bottom = this->bottom * scale;
  out.right = this->right * scale;
  return out;
}

Mouth_t Mouth_t::operator+(const Mouth_t & add)
{
  Mouth_t out;
  out.left = this->left + add.left;
  out.top = this->top + add.top;
  out.bottom = this->bottom + add.bottom;
  out.right = this->right + add.right;
  return out;
}


FaceParams_t::FaceParams_t():
  eyeL(), eyeR(), mouth()
{
}

FaceParams_t::FaceParams_t(Eye_t eyeLInit, Eye_t eyeRInit, Mouth_t mouthInit):
  eyeL(eyeLInit), eyeR(eyeRInit), mouth(mouthInit)
{
}

FaceParams_t FaceParams_t::operator*(char scale)
{
  FaceParams_t out;
  out.eyeL.pupilRadius = this->eyeL.pupilRadius;
  out.eyeL.pupilAngle = this->eyeL.pupilAngle;
  out.eyeL.lidClosure = this->eyeL.lidClosure*scale/100;
  out.eyeL.lidAngle = this->eyeL.lidAngle*scale/100;
  out.eyeR.pupilRadius = this->eyeR.pupilRadius;
  out.eyeR.pupilAngle = this->eyeR.pupilAngle;
  out.eyeR.lidClosure = this->eyeR.lidClosure*scale/100;
  out.eyeR.lidAngle = this->eyeR.lidAngle*scale/100;
  out.mouth = this->mouth*scale;
  return out;
}

FaceParams_t FaceParams_t::operator+(const FaceParams_t &add)
{
  FaceParams_t out;
  out.eyeL = this->eyeL + add.eyeL;
  out.eyeR = this->eyeR + add.eyeR;
  out.mouth = this->mouth + add.mouth;
  return out;
}


IconFace_t::IconFace_t(int width, int height):
  _params(Eye_t(0,0,10,-5),Eye_t(0,0,10,5),
	  Mouth_t(CharVector2D_t(50,0), CharVector2D_t(0,0),
		  CharVector2D_t(0,0),  CharVector2D_t(50,0)))
{
  SetDim(width,height);
}

char IconFace_t::SetDim(int width, int height){
  if (width>0 && height>0) {
    _width = width;
    _height = height;
    _minDim = _width < _height ? _width : _height;
    _scaleFactor = _minDim/5; 
    _offsetX = (_width - _minDim)/2;
    _offsetY = (_height - _minDim)/2;
    return 0;
  }
  return 1;
}

void IconFace_t::SetParams(FaceParams_t params){
  _params=params;
}

void IconFace_t::Draw(BITMAP *image){
  int lipsWidth = _scaleFactor/6;
  int lipsWidth_2 = lipsWidth/2;

  int eyeLeft_X = _offsetX + _scaleFactor;
  int eyeLeft_Y = _offsetY + _scaleFactor;
  int eyeRight_X = _offsetX + 3*_scaleFactor;
  int eyeRight_Y = _offsetY + _scaleFactor;
  int mouth_X = _offsetX + _scaleFactor;
  int mouth_Y = _offsetY + 2*_scaleFactor;

  int eyelidLeft_Closure = _params.eyeL.lidClosure;
  int eyelidRight_Closure = _params.eyeR.lidClosure;
  int eyelidLeft_Angle = 128 * _params.eyeL.lidAngle / 100; //in Allegro
						       //255 is a 2Pi
  int eyelidRight_Angle = 128 * _params.eyeR.lidAngle / 100;
  int pupilLeft_Radius = _params.eyeL.pupilRadius;
  int pupilLeft_Angle = _params.eyeL.pupilAngle+25;
  int pupilLeft_DX = _scaleFactor/3 * pupilLeft_Radius*cos(1.0*pupilLeft_Angle/50*3.14) / 100;
  int pupilLeft_DY = _scaleFactor/3 * pupilLeft_Radius*sin(1.0*pupilLeft_Angle/50*3.14) / 100;
  int pupilRight_Radius = _params.eyeR.pupilRadius;
  int pupilRight_Angle = _params.eyeR.pupilAngle+25;
  int pupilRight_DX = _scaleFactor/3 * pupilRight_Radius*cos(1.0*pupilRight_Angle/50*3.14) / 100;
  int pupilRight_DY = _scaleFactor/3 * pupilRight_Radius*sin(1.0*pupilRight_Angle/50*3.14) / 100;

  int eyePupil_X = _scaleFactor/3;
  int eyePupil_Y = _scaleFactor/3;

  int mouthLeftDX = _scaleFactor * _params.mouth.left.x/100/2 + _scaleFactor;
  int mouthLeftDY = (_scaleFactor - lipsWidth_2) * _params.mouth.left.y/100/4;
  int mouthTopDX = 0;
  int mouthTopDY = (_scaleFactor - lipsWidth_2) * (((int)_params.mouth.top.y+100)*175/200-100)/100;
  int mouthBottomDX = 0;
  int mouthBottomDY = (_scaleFactor - lipsWidth_2) * (((int)_params.mouth.bottom.y+100)*175/200-75)/100;
  int mouthRightDX = _scaleFactor*_params.mouth.right.x/100/2 +  _scaleFactor;
  int mouthRightDY = (_scaleFactor - lipsWidth_2) * _params.mouth.right.y/100/4;
  
  int lipsCenter_X = 3*_scaleFactor/2;
  int lipsCenter_Y = _scaleFactor;

  int mouthLeftX = lipsCenter_X - mouthLeftDX; 
  int mouthLeftY = lipsCenter_Y + mouthLeftDY;
  int mouthTopX = lipsCenter_X + mouthTopDX;
  int mouthTopY = lipsCenter_Y + mouthTopDY;
  int mouthBottomX = lipsCenter_X + mouthBottomDX;
  int mouthBottomY = lipsCenter_Y + mouthBottomDY; 
  int mouthRightX = lipsCenter_X + mouthRightDX;
  int mouthRightY = lipsCenter_Y + mouthRightDY;

  static BITMAP *eyelid_l = create_bitmap_ex(32,_scaleFactor+1,_scaleFactor+1);
  static BITMAP *eyelid_r = create_bitmap_ex(32,_scaleFactor+1,_scaleFactor+1);
  static BITMAP *eye = create_bitmap_ex(32,_scaleFactor+1,_scaleFactor+1);
  static BITMAP *pupil = create_bitmap_ex(32,_scaleFactor/3+1,_scaleFactor/3+1);
  static BITMAP *mouth = create_bitmap_ex(32,4*_scaleFactor+1,3*_scaleFactor+1);

  clear_to_color(eyelid_l,TRANSPARENT_COLOR);// set to transparent
  clear_to_color(eyelid_r,TRANSPARENT_COLOR);// set to transparent
  clear_to_color(eye,TRANSPARENT_COLOR);   // set to transparent
  clear_to_color(pupil,TRANSPARENT_COLOR); // set to transparent
  clear_to_color(mouth,TRANSPARENT_COLOR);

  // lips contour
  int toplip_1[8] = {
    mouthLeftX,  mouthLeftY - lipsWidth_2,
    mouthTopX,   mouthTopY - lipsWidth_2,
    mouthTopX,   mouthTopY - lipsWidth_2,
    mouthRightX, mouthRightY - lipsWidth_2,
  }; // bottom line of top lip
  spline(mouth,toplip_1,makecol(255,255,255));

  int toplip_2[8] = {
    mouthLeftX,  mouthLeftY,
    mouthTopX,   mouthTopY + lipsWidth_2,
    mouthTopX,   mouthTopY + lipsWidth_2,
    mouthRightX, mouthRightY,
  }; // top line of top lip
  spline(mouth,toplip_2,makecol(255,255,255)); 
 
  int bottomlip_1[8] = {
    mouthLeftX,   mouthLeftY,
    mouthBottomX, mouthBottomY - lipsWidth_2,
    mouthBottomX, mouthBottomY - lipsWidth_2,
    mouthRightX,  mouthRightY,
  };  // top line of bottom lip
  spline(mouth,bottomlip_1,makecol(255,255,255));

  int bottomlip_2[8] = {
    mouthLeftX,   mouthLeftY + lipsWidth_2,
    mouthBottomX, mouthBottomY + lipsWidth_2,
    mouthBottomX, mouthBottomY + lipsWidth_2,
    mouthRightX,  mouthRightY + lipsWidth_2,
  }; // bottom line of bottom lip
  spline(mouth,bottomlip_2,makecol(255,255,255)); 
  
  line(mouth, mouthLeftX, mouthLeftY-lipsWidth_2, mouthLeftX, mouthLeftY+lipsWidth_2, makecol( 255, 255, 255));
  line(mouth, mouthRightX, mouthRightY-lipsWidth_2, mouthRightX, mouthRightY+lipsWidth_2, makecol( 255, 255, 255));
  
  // lips collor filling
  floodfill(mouth, mouthLeftX+1, mouthLeftY+lipsWidth_2/2, makecol(255,255,255));
  floodfill(mouth, mouthLeftX+1, mouthLeftY-lipsWidth_2/2, makecol(255,255,255));
   
  // eyes
  rectfill(image,0,0,_width, _height, makecol(0,0,0));
  rectfill(eyelid_l, 0, 0, _scaleFactor, eyelidLeft_Closure*_scaleFactor/100,
	   makecol(0,0,0));  // black  rectangle
  rectfill(eyelid_r, 0, 0, _scaleFactor, eyelidRight_Closure*_scaleFactor/100, 
	   makecol(0,0,0));  // black rectangle
  circlefill(eye, _scaleFactor/2, _scaleFactor/2, _scaleFactor/2, 
	     makecol(255,255,255)); // white circle
  circlefill(pupil, _scaleFactor/6, _scaleFactor/6, _scaleFactor/6,
	     makecol(0,0,0)); //black circle


#if 0   // termporary lines
  line(image,_offsetX,0,_offsetX,_height,makecol( 255, 0, 0));
  line(image,_width-_offsetX,0,_width-_offsetX,_height,makecol( 255, 0, 0));

  line(image,0,_offsetY,_width,_offsetY,makecol( 255, 0, 0));
  line(image,0,_height-_offsetY,_width,_height-_offsetY,makecol( 255, 0, 0));

  for(int i=1;i<6;i++){
    line(image,0,_offsetY+i*_scaleFactor,_width,_offsetY+i*_scaleFactor,makecol( 0, 255, 0));
  }

  for(int i=1;i<6;i++){
    line(image,_offsetX+i*_scaleFactor,0,_offsetX+i*_scaleFactor,_height,makecol( 0, 255, 0));
  }
#endif

  // left eye
  rotate_sprite(image, eye, eyeLeft_X, eyeLeft_Y, ftofix(0));
  // left pupil
  rotate_sprite(image, pupil, eyeLeft_X + eyePupil_X + pupilLeft_DX, // modification
		eyeLeft_Y + eyePupil_Y + pupilLeft_DY, //modification
		ftofix(0));
  // left eyelid
  rotate_sprite(image, eyelid_l, eyeLeft_X, eyeLeft_Y, ftofix(eyelidLeft_Angle));  
  // right eye
  rotate_sprite(image, eye, eyeRight_X, eyeRight_Y, ftofix(0));
  // right pupil
  rotate_sprite(image ,pupil, eyeRight_X + eyePupil_X + pupilRight_DX, // modification
		eyeRight_Y + eyePupil_Y + pupilRight_DY, //modification
		ftofix(0));
  // right eyelid
  rotate_sprite(image, eyelid_r, eyeRight_X, eyeRight_Y, ftofix(eyelidRight_Angle));  
  
  // mouth
  rotate_sprite(image, mouth, mouth_X, mouth_Y, ftofix(0));  
}


