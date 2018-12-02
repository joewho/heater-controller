//
//  ButtonController2.h
//  
//
//  Created by Joseph Cutro on 12/1/18.
//

//#ifndef ButtonController2_h
//#define ButtonController2_h

//using namespace std;
#include "string.h"
class ButtonController2 {
private:
    int _pin, _length;
    String _str;
    String names[100];
public:
    ButtonController2(String, int);
    String getName();//{return _str;}
    int getPin();//{return _pin;}
    int getLength();
    void addNames(String);
    String toString();
};
// #endif /* ButtonController2_h*/
ButtonController2::ButtonController2(String s, int i){
    _str = s;
    _pin = i;
    //_length = 0;
    names[0] = s;
    _length=1;
}
String ButtonController2::getName(){return _str;}
int ButtonController2::getPin(){return _pin;}
int ButtonController2::getLength(){return _length;}
void ButtonController2::addNames(String s){names[_length] = s; _length++;}
String ButtonController2::toString(){String s; for(int i=0;i<_length;i++){s+= names[i]+" ";} return s;}
