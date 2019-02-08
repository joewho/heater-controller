//
//  GUIHandler.h
//  
//
//  Created by Joseph Cutro on 2/7/19.
//

#ifndef GUIHandler_h
#define GUIHandler_h

static const String PROGMEM _initialDisplayText = "Woodshop Computer v1.0";

class GUIHandler{
    
private:
    enum menuModes {MAIN, HEATER, EDIT, RELAY};
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
    

public:
    GUIHandler(){
        _currentMenu = MAIN;
        _currentIndex = 0;
        _heaterMenuIndex = 0;
        _mainMenuIndex = 0;
    }
    void updateHydroData();
    void updateButtonInput(String, String);
    
    
};

void GUIHandler::updateHydroData(){
    Serial.print("DisplayHandler::mainMenu size: ");
    Serial.println(sizeof(_mainMenu)/sizeof(_mainMenu[0]));
}

void GUIHandler::updateButtonInput(String name, String action){
    //
    //_name = name;
    //_action = action;
    if(name =="navButton") _navButtonPressed(action);
    else if(name =="selectButton") _selectButtonPressed(action);
    else if(name =="upButton") _upButtonPressed(action);
    else if(name =="downButton") _downButtonPressed(action);
    
}

void GUIHandler::_navButtonPressed(String action){
    if(action == "pressed"){
        switch(_currentMenu){
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
    if(action == "pressed"){
        
    }
    else if(action == "longPress"){
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
    if(action == "pressed"){
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
    if(action == "pressed"){
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

void GUIHandler::_editHeaterSettings(int value){}

void GUIHandler::_advanceSelector(){
    switch(_currentMenu){
        case MAIN:
            if(_currentIndex == 0){
                _currentIndex = 1;
                //displayHandler.setSelector(1);
                
            }else{
                _currentIndex = 0;
                //displayHandler.setSelector(0);
            }
            break;
        default:
            break;
    }//switch
}
void GUIHandler::_displayNextScreen(){
    switch(_currentMenu){
        case HEATER:
            break;
        case EDIT:
            break;
        case RELAY:
            break;
        default:
            break;
    }//switch
}
void GUIHandler::_displayParentMenu(){
    switch(_currentMenu){
        case HEATER:
            break;
        case EDIT:
            break;
        case RELAY:
            break;
    }//switch
}
void GUIHandler::_displayChildMenu(){
    switch(_currentMenu){
        case MAIN:
            break;
        case HEATER:
            break;
        default:
            break;
    }//switch
}


#endif /* GUIHandler_h */
