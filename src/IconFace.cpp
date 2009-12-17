#include <allegro.h>
#include <cmath>

#define PI 3.14159265

struct Eye_t
{
  char pupilRadius; // pupil radius in polar cordr [0,100]
  char pupilAngle; // pupil angle in polar cord [0,100]
  char lidClosure; // eyelid closure factor [0,100]
  char lidAngle; // eyelid angle factor [-100,100]
  Eye_t();
  Eye_t(char pupilRadiusInit, char pupilAngleInit, char lidClosureInit, char lidAngleInit);
  Eye_t operator+(const Eye_t &add);
};

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

struct CharVector2D_t 
{
  char x;
  char y;
  CharVector2D_t();
  CharVector2D_t(char xInit, char yInit);
  CharVector2D_t(const CharVector2D_t & init);
  CharVector2D_t operator*(char scale);
  CharVector2D_t operator+(const CharVector2D_t &add);
};

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



struct FaceParams_t{
  Eye_t eyeL;
  Eye_t eyeR;
  Mouth_t mouth;
  FaceParams_t();
  FaceParams_t(Eye_t eyeLInit, Eye_t eyeRInit, Mouth_t mouthInit);
  FaceParams_t operator*(char scale);
  FaceParams_t operator+(const FaceParams_t &add);
};

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

void init(int width, int height) {
  int depth, res;
  allegro_init();
  depth = desktop_color_depth();
  if (depth == 0) depth = 32;
  set_color_depth(depth);
  res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
  if (res != 0) {
    allegro_message("Allegro error : %s\n",allegro_error);
    exit(-1);
  }

  install_timer();
  install_keyboard();
  install_mouse();
}


void deinit() {
  clear_keybuf();
}


int main(int argc, char *argv[])
{
  int width=800;
  int height=600;
  init(width,height);
  static BITMAP *background = create_bitmap_ex(32,width,height);
  
  IconFace_t face(width,height);

  FaceParams_t twarz;
  FaceParams_t oczy;
  FaceParams_t usta;
  FaceParams_t usta_old;
  FaceParams_t usta_new;

  FaceParams_t params;
  FaceParams_t domyslny(Eye_t(0,0,10,-5),Eye_t(0,0,10,5),
		      Mouth_t(CharVector2D_t(0,0), CharVector2D_t(0,0),
			      CharVector2D_t(0,0), CharVector2D_t(0,0)));
  FaceParams_t smutek(Eye_t(0,0,45,-21),Eye_t(0,0,45,21),
		      Mouth_t(CharVector2D_t(-80,80), CharVector2D_t(0,-30),
			      CharVector2D_t(0,-30), CharVector2D_t(-80,80)));
  FaceParams_t radosc(Eye_t(0,0,0,0),Eye_t(0,0,0,0),
		      Mouth_t(CharVector2D_t(80,-80), CharVector2D_t(0,80),
			      CharVector2D_t(0,80), CharVector2D_t(80,-80)));
  FaceParams_t zlosc(Eye_t(0,0,45,21),Eye_t(0,0,45,-21),
		      Mouth_t(CharVector2D_t(80,80), CharVector2D_t(0,0),
			      CharVector2D_t(0,0), CharVector2D_t(80,80)));

  FaceParams_t fonem_o(Eye_t(0,0,0,0),Eye_t(0,0,0,0),
		       Mouth_t(CharVector2D_t(-30,0), CharVector2D_t(0,-30),
			       CharVector2D_t(0,30), CharVector2D_t(-30,0)));

  FaceParams_t fonem_w(Eye_t(0,0,0,0),Eye_t(0,0,0,0),
		       Mouth_t(CharVector2D_t(0,0), CharVector2D_t(0,-30),
			       CharVector2D_t(0,30), CharVector2D_t(0,0)));

  FaceParams_t fonem_e(Eye_t(0,0,0,0),Eye_t(0,0,0,0),
		       Mouth_t(CharVector2D_t(-20,0), CharVector2D_t(0,-20),
			       CharVector2D_t(0,10), CharVector2D_t(-20,0)));

  FaceParams_t fonem_l(Eye_t(0,0,0,0),Eye_t(0,0,0,0),
		       Mouth_t(CharVector2D_t(0,0), CharVector2D_t(0,-30),
			       CharVector2D_t(0,0), CharVector2D_t(0,0)));

  FaceParams_t fonem_null(Eye_t(0,0,0,0),Eye_t(0,0,0,0),
		       Mouth_t(CharVector2D_t(0,0), CharVector2D_t(0,00),
			       CharVector2D_t(0,0), CharVector2D_t(0,0)));
 
  

  FaceParams_t wynik;
  char skalaTwarz=100;
  char skalaOczy=100;
  char skalaUsta=100;

  bool active=true;
  FaceParams_t *paramPtr=&domyslny;

  twarz=domyslny;
  usta=fonem_null;
  usta_old=fonem_null;
  usta_new=fonem_null;


  clear_to_color(background,TRANSPARENT_COLOR);

  int refresh_rate = 1;

  long tmp;

  while (active) {
    clear_keybuf();    
    acquire_screen();        

    if (key[KEY_Q]) {params.eyeL.pupilRadius+=1;}        
    if (key[KEY_A]) {params.eyeL.pupilRadius-=1;}
    if (key[KEY_W]) {params.eyeL.pupilAngle+=1;}
    if (key[KEY_S]) {params.eyeL.pupilAngle-=1;}
    if (key[KEY_E]) {params.eyeL.lidClosure+=1;}
    if (key[KEY_D]) {params.eyeL.lidClosure-=1;}
    if (key[KEY_R]) {params.eyeL.lidAngle+=1;}
    if (key[KEY_F]) {params.eyeL.lidAngle-=1;}

    if (key[KEY_P]) {params.eyeR.pupilRadius+=1;}        
    if (key[KEY_L]) {params.eyeR.pupilRadius-=1;}
    if (key[KEY_O]) {params.eyeR.pupilAngle+=1;}
    if (key[KEY_K]) {params.eyeR.pupilAngle-=1;}
    if (key[KEY_I]) {params.eyeR.lidClosure+=1;}
    if (key[KEY_J]) {params.eyeR.lidClosure-=1;}
    if (key[KEY_U]) {params.eyeR.lidAngle+=1;}
    if (key[KEY_H]) {params.eyeR.lidAngle-=1;}

    if (key[KEY_1]) {params.mouth.left.x+=1;}        
    if (key[KEY_2]) {params.mouth.left.x-=1;}
    if (key[KEY_3]) {params.mouth.left.y+=1;}
    if (key[KEY_4]) {params.mouth.left.y-=1;}
    if (key[KEY_5]) {params.mouth.right.x+=1;}
    if (key[KEY_6]) {params.mouth.right.x-=1;}
    if (key[KEY_7]) {params.mouth.right.y+=1;}
    if (key[KEY_8]) {params.mouth.right.y-=1;}
 
    if (key[KEY_Z]) {params.mouth.top.x+=1;}        
    if (key[KEY_X]) {params.mouth.top.x-=1;}
    if (key[KEY_C]) {params.mouth.top.y+=1;}
    if (key[KEY_V]) {params.mouth.top.y-=1;}
    if (key[KEY_B]) {params.mouth.bottom.x+=1;}
    if (key[KEY_N]) {params.mouth.bottom.x-=1;}
    if (key[KEY_M]) {params.mouth.bottom.y+=1;}
    if (key[KEY_COMMA]) {params.mouth.bottom.y-=1;}
 
    if (params.mouth.bottom.y < params.mouth.top.y) {params.mouth.bottom.y = params.mouth.top.y;}

    if (params.eyeL.pupilRadius>100) params.eyeL.pupilRadius=100;
    if (params.eyeL.pupilRadius<0) params.eyeL.pupilRadius=0;
    if (params.eyeL.pupilAngle>100) params.eyeL.pupilAngle=0;
    if (params.eyeL.pupilAngle<0) params.eyeL.pupilAngle=100;
    if (params.eyeL.lidClosure>100) params.eyeL.lidClosure=100;
    if (params.eyeL.lidClosure<0) params.eyeL.lidClosure=0;
    if (params.eyeL.lidAngle>100) params.eyeL.lidAngle=100;
    if (params.eyeL.lidAngle<-100) params.eyeL.lidAngle=-100;

    if (params.eyeR.pupilRadius>100) params.eyeR.pupilRadius=100;
    if (params.eyeR.pupilRadius<0) params.eyeR.pupilRadius=0;
    if (params.eyeR.pupilAngle>100) params.eyeR.pupilAngle=0;
    if (params.eyeR.pupilAngle<0) params.eyeR.pupilAngle=100;
    if (params.eyeR.lidClosure>100) params.eyeR.lidClosure=100;
    if (params.eyeR.lidClosure<0) params.eyeR.lidClosure=0;
    if (params.eyeR.lidAngle>100) params.eyeR.lidAngle=100;
    if (params.eyeR.lidAngle<-100) params.eyeR.lidAngle=-100;

    if (params.mouth.left.x>100) params.mouth.left.x=100;
    if (params.mouth.left.x<-100) params.mouth.left.x=-100;
    if (params.mouth.left.y>100) params.mouth.left.y=100;
    if (params.mouth.left.y<-100) params.mouth.left.y=-100;
    if (params.mouth.right.x>100) params.mouth.right.x=100;
    if (params.mouth.right.x<-100) params.mouth.right.x=-100;
    if (params.mouth.right.y>100) params.mouth.right.y=100;
    if (params.mouth.right.y<-100) params.mouth.right.y=-100;

    if (params.mouth.top.x>100) params.mouth.top.x=100;
    if (params.mouth.top.x<0) params.mouth.top.x=0;
    if (params.mouth.top.y>100) params.mouth.top.y=100; //bylo 75
    if (params.mouth.top.y<-100) params.mouth.top.y=-100;
    if (params.mouth.bottom.x>100) params.mouth.bottom.x=100;
    if (params.mouth.bottom.x<0) params.mouth.bottom.x=0;
    if (params.mouth.bottom.y>100) params.mouth.bottom.y=100;
    if (params.mouth.bottom.y<-100) params.mouth.bottom.y=-100; // bylo -75

    if (key[KEY_F1]) {twarz=domyslny;}
    else if (key[KEY_F2]) {twarz=radosc;}
    else if (key[KEY_F3]) {twarz=smutek;}
    else if (key[KEY_F4]) {twarz=zlosc;}
    else if (key[KEY_F12]) {twarz=params;}
    //twarz=params;

    if (key[KEY_RIGHT]) {skalaTwarz+=1;}
    if (key[KEY_LEFT]) {skalaTwarz-=1;}
    if (skalaTwarz>100) skalaTwarz=100;
    if (skalaTwarz<0) skalaTwarz=0;

    if (key[KEY_UP]) {skalaUsta+=1;}
    if (key[KEY_DOWN]) {skalaUsta-=1;}
    if (skalaUsta>100) skalaUsta=100;
    if (skalaUsta<0) skalaUsta=0;
    
    tmp++;

    //skalaUsta=100*sin(1.0 * tmp++/100)*sin(1.0 * tmp++/100);
    switch (tmp) {
    case 0: usta_old=usta_new; usta_new=fonem_null; break;
    case 20: usta_old=usta_new; usta_new=fonem_e; break;
    case 40: usta_old=usta_new; usta_new=fonem_l; break;
    case 60: usta_old=usta_new; usta_new=fonem_o; break;
    case 80: usta_old=usta_new; usta_new=fonem_w; break;
    case 100: usta_old=usta_new; usta_new=fonem_null; break;
    case 200:
      usta_old=fonem_null; usta_new=fonem_null;
      tmp=0;
    }
    if (tmp<120){
      usta=usta_old*(100-(tmp%20)*5) +  usta_new*((tmp%20)*5);
    }
    else usta=fonem_null;
    
    wynik=twarz*skalaTwarz + usta*skalaUsta;
    face.SetParams(wynik);
    face.Draw(background);

    textprintf_ex( background, font, 10,10, makecol(255,255,255),-1, "eyeL");
    textprintf_ex( background, font, 10,20, makecol(255,255,255),-1, "pupil[R,A] = [%d, %d]",params.eyeL.pupilRadius,params.eyeL.pupilAngle);
    textprintf_ex( background, font, 10,30, makecol(255,255,255),-1, "lid[C,A]   = [%d, %d]",params.eyeL.lidClosure, params.eyeL.lidAngle);
    textprintf_ex( background, font, 200,10, makecol(255,255,255),-1,"eyeR");
    textprintf_ex( background, font, 200,20, makecol(255,255,255),-1,"pupil[R,A] = [%d, %d]",params.eyeR.pupilRadius,params.eyeR.pupilAngle);
    textprintf_ex( background, font, 200,30, makecol(255,255,255),-1,"lid[C,A]   = [%d, %d]",params.eyeR.lidClosure, params.eyeR.lidAngle);
    textprintf_ex( background, font, 400,10, makecol(255,255,255),-1,"mouth");
    textprintf_ex( background, font, 400,20, makecol(255,255,255),-1,"left[X,Y] = [%d, %d]",params.mouth.left.x,params.mouth.left.y);
    textprintf_ex( background, font, 400,30, makecol(255,255,255),-1,"top[X,Y]  = [%d,% d]",params.mouth.top.x,params.mouth.top.y);
    textprintf_ex( background, font, 600,20, makecol(255,255,255),-1,"bottom[X,Y] = [%d, %d]",params.mouth.bottom.x,params.mouth.bottom.y);
    textprintf_ex( background, font, 600,30, makecol(255,255,255),-1,"right[X,Y]  = [%d, %d]",params.mouth.right.x,params.mouth.right.y);

    textprintf_ex( background, font, 10,40, makecol(255,255,255),-1, "skalaTwarz = %d",skalaTwarz);
    textprintf_ex( background, font, 10,50, makecol(255,255,255),-1, "skalaUsta = %d",skalaUsta);

    blit( background, screen, 0,0,0,0, width,height);

    release_screen();

    /*
     *  WYJSCIE
     */
    rest(10);
    if (key[KEY_ESC]){
      active=false;  
      continue;
    }

  }

  destroy_bitmap(background); 
  deinit();

  return 0;
}
END_OF_MAIN()

