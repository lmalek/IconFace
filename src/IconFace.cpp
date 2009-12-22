#include "IconFaceLib.h"
#include <sys/time.h>
#include <cmath>
#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

class Dictionary {
public:
  std::map<std::string,std::string> translate;
  std::string language;
  Dictionary();
  Dictionary(std::string dictFileName);
  void loadFile(std::string dictFileName);
};

Dictionary::Dictionary(){
}

Dictionary::Dictionary(std::string dictFileName){
  loadFile(dictFileName);
}

void Dictionary::loadFile(std::string dictFileName){
  std::ifstream dictFile;
  dictFile.open(dictFileName.c_str());
  std::string phonem, shape;
  if (dictFile.is_open())
  {
    getline (dictFile,language);
    while (! dictFile.eof() )
    {
      dictFile>>phonem>>shape;
      translate.insert(std::pair<std::string, std::string>(phonem,shape));
    }
    dictFile.close();
  }
  /*
  std::map<std::string,std::string>::iterator it;
  for ( it=dict.begin() ; it != dict.end(); it++ )
    std::cout << (*it).first << " => " << (*it).second << std::endl;
  */
}




class FaceExpression_t {
public:
  FaceParams_t *face;
  double duration;

  FaceExpression_t();
  FaceExpression_t(FaceParams_t *faceInit,double durationInit); 
};

FaceExpression_t::FaceExpression_t():
  face(NULL),duration(0)
{
}

FaceExpression_t::FaceExpression_t(FaceParams_t *faceInit,double durationInit):
  face(faceInit), duration(durationInit)
{  
}

/**********************************************************************/

class Morphings:std::map<std::string, FaceParams_t> {
public:
  std::map<std::string, FaceParams_t> morph;
  Morphings();
  Morphings(std::string morphFileName);
  void loadFile(std::string morphFileName);
  void insert(std::string morphName, FaceParams_t morphShape);
  FaceParams_t& operator[](const std::string& morphName );
};

Morphings::Morphings()
{
}

Morphings::Morphings(std::string morphFileName){
  loadFile(morphFileName);
}

void Morphings::loadFile(std::string morphFileName){
  std::ifstream morphFile;
  morphFile.open(morphFileName.c_str());
  std::string morphName;
  Eye_t eyeLeft, eyeRight;
  Mouth_t mouth;
  if (morphFile.is_open()) {
    while (! morphFile.eof() ) {
      morphFile>>morphName;
      morphFile>>eyeLeft.pupilRadius>>eyeLeft.pupilAngle>>eyeLeft.lidClosure>>eyeLeft.lidAngle;
      morphFile>>eyeRight.pupilRadius>>eyeRight.pupilAngle>>eyeRight.lidClosure>>eyeRight.lidAngle;      
      morphFile>>mouth.left.x>>mouth.left.y;
      morphFile>>mouth.top.x>>mouth.top.y;
      morphFile>>mouth.bottom.x>>mouth.bottom.y;
      morphFile>>mouth.right.x>>mouth.right.y;
      /*
      std::cout<<morphName<<"|";
      std::cout<<eyeLeft.pupilRadius<<" "<<eyeLeft.pupilAngle<<" "<<eyeLeft.lidClosure<<" "<<eyeLeft.lidAngle<<"|";
      std::cout<<eyeRight.pupilRadius<<" "<<eyeRight.pupilAngle<<" "<<eyeRight.lidClosure<<" "<<eyeRight.lidAngle<<"|";      
      std::cout<<mouth.left.x<<" "<<mouth.left.y<<" ";
      std::cout<<mouth.top.x<<" "<<mouth.top.y<<" ";
      std::cout<<mouth.bottom.x<<" "<<mouth.bottom.y<<" ";
      std::cout<<mouth.right.x<<" "<<mouth.right.y;
      std::cout<<std::endl;
      */
      morph.insert(std::pair<std::string, FaceParams_t>(morphName,FaceParams_t(eyeLeft,eyeRight,mouth)));
    }
    morphFile.close();
  }
}

void Morphings::insert(std::string morphName, FaceParams_t morphShape){
  morph.insert(std::pair<std::string, FaceParams_t>(morphName,morphShape));
}

FaceParams_t& Morphings::operator[](const std::string& morphName ){
  return morph[morphName];
}


/**********************************************************************/

class PhoScenario{
public:
  std::string name;
  PhoScenario(Dictionary *dictInit,Morphings *wzorceFonemowInit);
  void load(std::string phoFileName);
  std::vector<FaceExpression_t> pho;
  Dictionary *dict;
  Morphings *wzorceFonemow;
};

PhoScenario::PhoScenario(Dictionary *dictInit,Morphings *wzorceFonemowInit)
{
  dict=dictInit;
  wzorceFonemow=wzorceFonemowInit;
}

void PhoScenario::load(std::string phoFileName){
  std::ifstream phoFile;
  phoFile.open(phoFileName.c_str());
  std::string phoName;
  float phoDur;
  if (phoFile.is_open()) {
    while (! phoFile.eof() ) {
      phoFile>>phoName>>phoDur;
      std::cout<<dict->translate[phoName]<<" "<<phoDur<<std::endl;
      pho.push_back(FaceExpression_t(&(wzorceFonemow->morph[dict->translate[phoName]]),phoDur));  
    }
    phoFile.close();
  }
}


/**********************************************************************/
void init(int width, int height) {
  int depth, res;
  allegro_init();
  depth = desktop_color_depth();
  if (depth == 0) depth = 32;
  set_color_depth(depth);
  res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
  //res = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, width, height, 0, 0);
  if (res != 0) {
    allegro_message("Allegro error : %s\n",allegro_error);
    exit(-1);
  }

  install_timer();
  install_keyboard();
  install_mouse();
  printf("sound card detection = [ %s ]\n",install_sound(DIGI_OSS,MIDI_NONE,"")==-1?"FAILED":"OK");
  set_volume(255,255);
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
  SAMPLE *dzwiek = NULL;
  dzwiek = load_sample("test.wav"); 
  
  IconFace_t face(width,height);

  FaceParams_t *twarz;
  FaceParams_t *oczy;
  FaceParams_t *usta;
  FaceParams_t *usta_old;
  FaceParams_t *usta_new;

  FaceParams_t params;

  Morphings wzorceEmocji;
  Morphings wzorceFonemow;

  std::vector<FaceExpression_t> emocje;
  //  std::vector<FaceExpression_t> fonemy;
  int currPhonIter;

  wzorceEmocji.loadFile("emocje.dat");
  wzorceFonemow.loadFile("fonemy.dat");
  Dictionary dict("english.dic");

  PhoScenario scenariusz(&dict,&wzorceFonemow);
  scenariusz.load("test.pho");
  FaceParams_t *previousPhoneme, *currentPhoneme, *nextPhoneme;
  double previousPhonemeEndTime;

  FaceParams_t wynik;
  char skalaTwarz=100;
  char skalaOczy=100;
  char skalaUsta=100;
  bool zmiana=true;

  bool active=true;
  FaceParams_t *paramPtr=&wzorceEmocji["default"];

  twarz=&wzorceEmocji["default"];
  usta=&wzorceFonemow["default"];
  usta_old=&wzorceFonemow["default"];
  usta_new=&wzorceFonemow["default"];


  clear_to_color(background,TRANSPARENT_COLOR);

  int refresh_rate = 1;

  char prePhoneme;
  char postPhoneme;
  

  timeval time_curr; 
  timeval time_start;
  timeval time_diff;
  gettimeofday(&time_start, NULL);
  double dtime;   
  double playTime=-1;
  double timeOffset;
  double voiceDur;
  double phoneme_dur=0;
  double phoneme_start=0;
  char tmp;

  int voice;
  int counter;
  
  //play_sample(dzwiek, 255,127,1000,0);
  voice=allocate_voice(dzwiek);
  voice_set_frequency(voice,voice_get_frequency(voice));
  voiceDur=1.0*dzwiek->len/voice_get_frequency(voice);


  while (active) {
    clear_keybuf();    
    acquire_screen(); 

    gettimeofday(&time_curr, NULL);       
    time_diff.tv_sec = time_curr.tv_sec - time_start.tv_sec;
    time_diff.tv_usec = time_curr.tv_usec - time_start.tv_usec;
    dtime = time_diff.tv_sec + (double) time_diff.tv_usec / 1e6;
    if (voice_get_position(voice)!=-1)
      playTime=1.0*voice_get_position(voice)/voice_get_frequency(voice);
    else
      playTime=0;
    /*
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
    */
    if (key[KEY_F1]) {twarz=&wzorceEmocji["default"];}
    else if (key[KEY_F2]) {twarz=&wzorceEmocji["happy"];}
    else if (key[KEY_F3]) {twarz=&wzorceEmocji["sad"];}
    else if (key[KEY_F4]) {twarz=&wzorceEmocji["angry"];}
    //else if (key[KEY_F12]) {twarz=&params;}
    //twarz=params;

    if (key[KEY_RIGHT]) {skalaTwarz+=1;}
    if (key[KEY_LEFT]) {skalaTwarz-=1;}
    if (skalaTwarz>100) skalaTwarz=100;
    if (skalaTwarz<0) skalaTwarz=0;

    if (key[KEY_UP]) {skalaUsta+=1;}
    if (key[KEY_DOWN]) {skalaUsta-=1;}
    if (skalaUsta>100) skalaUsta=100;
    if (skalaUsta<0) skalaUsta=0;


    if (key[KEY_SPACE] && voice_get_position(voice)<0) {
      voice_start(voice);
      playTime=0;
      currPhonIter=0;
      zmiana=true;
      counter=0;
      previousPhoneme=scenariusz.pho[0].face;
      previousPhonemeEndTime=0;
    }
    
    while (playTime>=scenariusz.pho[currPhonIter].duration && currPhonIter<scenariusz.pho.size() &&  playTime<=voiceDur){
      previousPhonemeEndTime=scenariusz.pho[currPhonIter].duration;
      previousPhoneme=scenariusz.pho[currPhonIter].face;
      currPhonIter++;
    }
    currentPhoneme=scenariusz.pho[currPhonIter].face;
    tmp=100.0*(scenariusz.pho[currPhonIter].duration-playTime)/(scenariusz.pho[currPhonIter].duration-previousPhonemeEndTime);
    if (tmp<0) tmp=0;
    if (tmp>100) tmp=100;
    tmp=(1-cos(3.14/100*tmp))*100;
    if (tmp<0) tmp=0;
    if (tmp>100) tmp=100;

    //usta =new  FaceParams_t((*previousPhoneme*(100-tmp)) +(*currentPhoneme*(tmp)));
    usta =new  FaceParams_t((*currentPhoneme));
    
    wynik=*twarz*skalaTwarz + *usta*skalaUsta;
    face.SetParams(wynik);
    face.Draw(background);

    /*
      if (currPhonIter>=scenariusz.pho.size() && playTime<voiceDur){
      currPhonIter=1;
    }
    */

    
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
    textprintf_ex( background, font, 10,60, makecol(255,255,255),-1, "time = %f",dtime);
    textprintf_ex( background, font, 10,70, makecol(255,255,255),-1, "tmp = %d",tmp);

    textprintf_ex( background, font, 10,90, makecol(255,255,255),-1, "pos = %d",voice_get_position(voice));
    textprintf_ex( background, font, 10,100, makecol(255,255,255),-1, "freq = %d",voice_get_frequency(voice));
    textprintf_ex( background, font, 10,110, makecol(255,255,255),-1, "T = %f",1.0*voice_get_position(voice)/voice_get_frequency(voice));
    
    textprintf_ex( background, font, 10,120, makecol(255,255,255),-1, "currPhonIter = %d/%d",currPhonIter,scenariusz.pho.size());
    textprintf_ex( background, font, 10,130, makecol(255,255,255),-1, "counter = %d",counter);
    textprintf_ex( background, font, 10,140, makecol(255,255,255),-1, "currDuration = %f",scenariusz.pho[currPhonIter].duration);
    

    blit( background, screen, 0,0,0,0, width,height);

    release_screen();

    delete usta;
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
