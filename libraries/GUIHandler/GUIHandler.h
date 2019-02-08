//
//  GUIHandler.h
//  
//
//  Created by Joseph Cutro on 2/7/19.
//

#ifndef GUIHandler_h
#define GUIHandler_h
#include "DisplayHandler.h"

static const String PROGMEM _initialDisplayText = "Woodshop Computer v1.0";
static const String PROGMEM _initialTextRow1 = "Woodshop";
static const String PROGMEM _initialTextRow2 = "Computer v1.0";
static const char PROGMEM spaceChar = ' ';
static const char PROGMEM selectorChar = '>';
DisplayHandler displayHandler(16,2);
class GUIHandler{
    
private:
    enum menuModes {WELCOME, MAIN, HEATER, EDIT, RELAY};
    //enum buttons {NAV, SELECT, UP DOWN};
    //enum actions {PRESS, LONGPRESS};
    
    const String PROGMEM _mainMenu[2] = {"Heater","Relays"};
    const byte PROGMEM _mainMenuLength = 2;
    
    const String PROGMEM _heaterMenu[4] = {"Floor","Cabinet", "Room", "Floor Cab Room"};
    const byte PROGMEM _heaterMenuLength = 4;
    
    const String PROGMEM _zoneEditMenu[3] = {"Target Temp","Lower Limit", "Upper Limit"};
    const byte PROGMEM _zoneEditMenuLength = 3;
    
    const String PROGMEM _relayMenu[7] = {"relay_1", "relay_2", "relay_3", "relay_4", "relay_5", "relay_6", "relay_7"};
    const String _relayMenuLength = 7;
    
    byte _currentIndex;
    byte _heaterMenuIndex;
    byte _mainMenuIndex;
    menuModes _currentMenu;
    //buttons _currentButton;
    //actions _currentAction;
    
    String* _menuArray[4] = {_mainMenu, _heaterMenu, _zoneEditMenu, _relayMenu};
    
    void _navButtonPressed(String);
    void _selectButtonPressed(String);
    void _upButtonPressed(String);
    void _downButtonPressed(String);
    void _editHeaterSettings(int);
    void _advanceSelector();
    void _displayNextScreen();
    void _displayChildMenu();
    void _displayParentMenu();
    void _displayMainMenu();
    void _toggleRelay();
    

public:
    GUIHandler(){
        _currentMenu = WELCOME;
        _currentIndex = 0;
        _heaterMenuIndex = 0;
        _mainMenuIndex = 0;
    }
    void initiate();
    void updateHydroData();
    void updateButtonInput(String, String);
    
    
};
void GUIHandler::initiate(){
    displayHandler.initiate();
    displayHandler.setSelectorChar(selectorChar);
    displayHandler.setText(_initialTextRow1,0,0);
    displayHandler.setText(_initialTextRow2,0,1);
}
void GUIHandler::updateHydroData(){
    //Serial.print("DisplayHandler::mainMenu size: ");
    //Serial.println(sizeof(_mainMenu)/sizeof(_mainMenu[0]));
}

void GUIHandler::updateButtonInput(String name, String action){
  
    if(name =="navButton") _navButtonPressed(action);
    else if(name =="selectButton") _selectButtonPressed(action);
    else if(name =="upButton") _upButtonPressed(action);
    else if(name =="downButton") _downButtonPressed(action);
    
}

void GUIHandler::_navButtonPressed(String action){
/*
    Serial.println("GUIHANDLER::navButton PRESSED");
    Serial.print("action -- ");
    Serial.println(action);
    Serial.println();
  */
    if(action == "wasPressed"){
        Serial.println("GUIHANDLER::navButton wasPressed");
        switch(_currentMenu){
            case WELCOME:
                _displayMainMenu();
                break;
            case MAIN:
                _advanceSelector();
                break;
            case HEATER:
                _displayNextScreen();
                break;
            case EDIT:
                _displayNextScreen();
                break;
            case RELAY:
                _displayNextScreen();
                //advanceScrollMenu();
                break;
            default:
                break;
        }//switch
    }//if action == pressed
}//_navButtonPressed

void GUIHandler::_selectButtonPressed(String action){
    /*
     Serial.println("GUIHANDLER::selectButton PRESSED");
    Serial.print("action -- ");
    Serial.println(action);
    Serial.println();
    */
    if(action == "wasPressed"){
        Serial.println("GUIHANDLER::selectButton wasPressed");

        switch(_currentMenu){
            case MAIN:
                break;
            case RELAY:
                _toggleRelay();
                break;
        }
        
    }
    else if(action == "pressedFor"){
        Serial.println("GUIHANDLER::selectButton pressedFor");
        switch(_currentMenu){
            case HEATER:
                _displayChildMenu();
                break;
            case EDIT:
                _displayParentMenu();
                break;
            default:
                break;
        }//switch
        
    }//if longPress
}//_selectButtonPressed

void GUIHandler::_upButtonPressed(String action){
    /*
    Serial.println("GUIHANDLER::upButton PRESSED");
    Serial.print("action -- ");
    Serial.println(action);
    Serial.println();
    */
    if(action == "wasPressed"){
        Serial.println("GUIHANDLER::upButton wasPressed");

        switch(_currentMenu){
            case EDIT:
                _editHeaterSettings(1);
                break;
            //only for scroll version of relay menu
            //case RELAY:
            //    break;
            default:
                break;
        }//switch
    }//if pressed
}//_upButtonPressed

void GUIHandler::_downButtonPressed(String action){
    /*
    Serial.println("GUIHANDLER::downButton PRESSED");
    Serial.print("action -- ");
    Serial.println(action);
    Serial.println();
     */
    if(action == "wasPressed"){
        Serial.println("GUIHANDLER::downButton wasPressed");
        switch(_currentMenu){
            case EDIT:
                _editHeaterSettings(-1);
                break;
                //only for scroll version of relay menu
                //case RELAY:
                //    break;
            default:
                break;
        }//switch
    }//if pressed
}//_downButtonPressed

void GUIHandler::_editHeaterSettings(int value){
    //input is either 1 || -1 to inc || dec settings of currentValue displayed on LCD screen
}

void GUIHandler::_advanceSelector(){
    switch(_currentMenu){
        case MAIN:
            if(_currentIndex == 0){
                _currentIndex = 1;
                displayHandler.setSelectorRow(1);
                //displayHandler.setSelector(spaceChar,0);
                
            }else{
                _currentIndex = 0;
                displayHandler.setSelectorRow(0);
                //displayHandler.setSelector(spaceChar,1);
            }
            break;
        default:
            break;
    }//switch
}
void GUIHandler::_displayNextScreen(){
    switch(_currentMenu){
        case HEATER:
            //change display to view the next 'zone'
            break;
        case EDIT:
            //change display to view next settings to edit/view
            break;
        case RELAY:
            //change display to view next relay status
            break;
        default:
            break;
    }//switch
}
void GUIHandler::_displayParentMenu(){
    switch(_currentMenu){
        case HEATER:
            //return to main menu
            //with heater selected
            break;
        case EDIT:
            //return to 'zone' display
            //with zone that was being edited last
            break;
        case RELAY:
            //return to main menu
            //with relay selected
            break;
    }//switch
}
void GUIHandler::_displayChildMenu(){
    switch(_currentMenu){
        case MAIN:
            //display heater or relay based on selection
            break;
        case HEATER:
            //display edit menu
            //with last zone being viewed
            break;
        default:
            break;
    }//switch
}

void GUIHandler::_displayMainMenu(){
    for(int i=0;i<_mainMenuLength;i++){
        displayHandler.setMenuText(_mainMenu,_mainMenuLength,0);
        _currentMenu = MAIN;
    }
}

void GUIHandler::_toggleRelay(){
    
}

#endif /* GUIHandler_h */
