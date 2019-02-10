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
    enum menuModes {WELCOME, MAIN, HEATER, EDIT, RELAY, BATTERY};
    //enum buttons {NAV, SELECT, UP DOWN};
    //enum actions {PRESS, LONGPRESS};
    
    const String PROGMEM _mainMenu[3] = {"Heater","Relays" ,"Batteries"};
    const byte PROGMEM _mainMenuLength = 3;
    
    const String PROGMEM _heaterMenu[4] = {"Floor","Cabinet", "Room", "Floor Cab   Room"};
    const byte PROGMEM _heaterMenuLength = 4;
    
    const String PROGMEM _zoneEditMenu[4] = {"Target Temp","Lower Limit", "Upper Limit", "Auto Control"};
    const byte PROGMEM _zoneEditMenuLength = 4;
    /*
    const String PROGMEM _relayMenu[7] = {"relay_1", "relay_2", "relay_3", "relay_4", "relay_5", "relay_6", "relay_7"};
    */
    String _relayMenu[7];
    const byte PROGMEM _relayMenuLength = 7;
    
    byte _currentIndex;
    byte _heaterMenuIndex;
    byte _mainMenuIndex;
    menuModes _currentMenu;
    
    String* _menuArray[4] = {_mainMenu, _heaterMenu, _zoneEditMenu, _relayMenu};
    
    HydronicDisplayData* _heaterData;
    HydronicDisplayData _heaterEditedByUser;
    
    RelayMessage* _relayData;
    RelayMessage _relayEditedByUser;
    
    void _navButtonPressed(String);
    void _selectButtonPressed(String);
    void _upButtonPressed(String);
    void _downButtonPressed(String);
    
    void _editHeaterSettings(byte);
    void _advanceSelector();
    void _displayNextScreen();
    void _displayChildMenu();
    void _displayParentMenu();
    void _displayMainMenu();
    void _displayHeaterItem();
    void _displayHeaterEditItem();
    void _updateHeaterItemValue();
    void _toggleZonePower();
    //void _displayBatteryItem();
    
//    void _editRelaySettings(byte);
    void _displayRelayItem();
    void _updateRelayItemValue();
    void _toggleRelay(byte);
    

public:
    GUIHandler(){
        _currentMenu = WELCOME;
        _currentIndex = 0;
        _heaterMenuIndex = 0;
        _mainMenuIndex = 0;
    }
    bool changeHeaterSettings;
    bool changeRelaySettings;
    void preloadRelayData(RelayMessage[]);
    
    void initiate();
    void updateHydroData(HydronicDisplayData[]);
    void updateRelayData(RelayMessage[]);
    void updateButtonInput(String, String);
    HydronicDisplayData getHeaterChanges();
    RelayMessage getRelayChanges(); //wont be void, must build relay communication struct
    
    
};
void GUIHandler::initiate(){
    displayHandler.initiate();
    displayHandler.setSelectorChar(selectorChar);
    displayHandler.setText(_initialTextRow1,0,0);
    displayHandler.setText(_initialTextRow2,0,1);
}

void GUIHandler::preloadRelayData(RelayMessage relayData[]){
    for(int i=0;i<_relayMenuLength;i++){
        _relayMenu[i] = relayData[i].name;
    }
    _relayData = relayData;
}
void GUIHandler::updateHydroData(HydronicDisplayData hydroData[]){
    //Serial.print("DisplayHandler::mainMenu size: ");
    //Serial.println(sizeof(_mainMenu)/sizeof(_mainMenu[0]));
    _heaterData = hydroData;
    if(_currentMenu == HEATER) _updateHeaterItemValue();
    if(_currentMenu == EDIT) _displayHeaterEditItem();
}

void GUIHandler::updateRelayData(RelayMessage relayData[]){
    _relayData = relayData;
    if(_currentMenu == RELAY) _updateRelayItemValue();
}

void GUIHandler::updateButtonInput(String name, String action){
  
    if(name =="navButton") _navButtonPressed(action);
    else if(name =="selectButton") _selectButtonPressed(action);
    else if(name =="upButton") _upButtonPressed(action);
    else if(name =="downButton") _downButtonPressed(action);
    
}

void GUIHandler::_navButtonPressed(String action){
    if(action == "wasPressed"){
        Serial.println("GUIHANDLER::navButton wasPressed");
        switch(_currentMenu){
            case WELCOME:
                _currentMenu = MAIN;
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
    if(action == "wasPressed"){
        Serial.println("GUIHANDLER::selectButton wasPressed");
        switch(_currentMenu){
            case MAIN:
                _displayChildMenu();
                break;
                
            case EDIT:
               // if(_currentIndex == _zoneEditMenuLength-1)
               //     _toggleZonePower();
                break;
            case RELAY:
                _toggleRelay(_currentIndex);
                break;
            case BATTERY:
                break;
                
        }
        
    }
    else if(action == "pressedFor"){
        Serial.println("GUIHANDLER::selectButton pressedFor");
        switch(_currentMenu){
            case HEATER:
                //_currentMenu = EDIT;
                _displayChildMenu();
                break;
            case EDIT:
                //_currentMenu = HEATER;
                _displayParentMenu();
                break;
            default:
                break;
        }//switch
        
    }//if longPress
}//_selectButtonPressed

void GUIHandler::_upButtonPressed(String action){
    if(action == "wasPressed"){
        Serial.println("GUIHANDLER::upButton wasPressed");

        switch(_currentMenu){
            case EDIT:
                if(_currentIndex != _zoneEditMenuLength-1)
                    _editHeaterSettings(1);
                else
                    _toggleZonePower();
                break;
            case RELAY:
                _toggleRelay(_currentIndex);
                break;
            default:
                break;
        }//switch
    }//if pressed
}//_upButtonPressed

void GUIHandler::_downButtonPressed(String action){
    if(action == "wasPressed"){
        Serial.println("GUIHANDLER::downButton wasPressed");
        switch(_currentMenu){
            case EDIT:
                if(_currentIndex != _zoneEditMenuLength-1)
                    _editHeaterSettings(-1);
                else
                    _toggleZonePower();
                break;
            case RELAY:
                _toggleRelay(_currentIndex);
                break;
            default:
                break;
        }//switch
    }//if pressed
}//_downButtonPressed

void GUIHandler::_displayMainMenu(){
    String stringArr[] ={_mainMenu[0],_mainMenu[1]};
    displayHandler.setMenuText(stringArr,2,_mainMenuIndex);
    /*for(int i=0;i<_mainMenuLength;i++){
     displayHandler.setMenuText(_mainMenu,_mainMenuLength,_mainMenuIndex);
     }
     */
}

void GUIHandler::_advanceSelector(){
    switch(_currentMenu){
        case MAIN:
            _currentIndex++;
            if(_currentIndex >= _mainMenuLength) _currentIndex = 0;
            _mainMenuIndex = _currentIndex;
            
            switch(_currentIndex){
                case 0:
                    //_currentIndex = 1;
                    //_mainMenuIndex = _currentIndex;
                    _displayMainMenu();
                    //displayHandler.setSelectorRow(0);
                    break;
                case 1:
                    //_currentIndex = 2;
                    //_mainMenuIndex = _currentIndex;
                    //String stringArr[] ={_mainMenu[1],_mainMenu[2]};
                    //displayHandler.setMenuText(stringArr,2,1);
                    displayHandler.setSelectorRow(1);
                    break;
                case 2:
                    String stringArr[] ={_mainMenu[1],_mainMenu[2]};
                    displayHandler.setMenuText(stringArr,2,1);
                    //_currentIndex = 0;
                    //_mainMenuIndex = _currentIndex;
                    //_displayMainMenu();
                    //displayHandler.setMenuText()
                    break;
            }
            /*
            if(_currentIndex == 0){
                _currentIndex = 1;
                displayHandler.setSelectorRow(1);
                //displayHandler.setSelector(spaceChar,0);
                
            }else if(){
                _currentIndex = 0;
                displayHandler.setSelectorRow(0);
                //displayHandler.setSelector(spaceChar,1);
            }
            */
            break;
        default:
            break;
    }//switch
}
void GUIHandler::_displayNextScreen(){
    switch(_currentMenu){
        case HEATER:
            //change display to view the next 'zone'
            _currentIndex++;
            if(_currentIndex >= _heaterMenuLength)
                _displayParentMenu();
            else
                _displayHeaterItem();//_currentIndex);
            break;
        case EDIT:
            //change display to view next settings to edit/view
            _currentIndex++;
            if(_currentIndex >= _zoneEditMenuLength)
                _currentIndex = 0;
            _displayHeaterEditItem();//_currentIndex);
            break;
        case RELAY:
            _currentIndex++;
            if(_currentIndex >= _relayMenuLength)
                _displayParentMenu();
            else
                _displayRelayItem();//_currentIndex);
            //change display to view next relay status
            break;
        default:
            break;
    }//switch
}
void GUIHandler::_displayParentMenu(){
    switch(_currentMenu){
        case HEATER:
            _currentMenu = MAIN;
            _currentIndex = _mainMenuIndex;
            _displayMainMenu();
            //return to main menu
            //with heater selected
            break;
        case EDIT:
            //return to 'zone' display
            //with zone that was being edited last
            _currentMenu = HEATER;
            _currentIndex = _heaterMenuIndex;
            _displayHeaterItem();//_heaterMenuIndex);
            break;
        case RELAY:
            _currentMenu = MAIN;
            _currentIndex = _mainMenuIndex;
            _displayMainMenu();
            //return to main menu
            //with relay selected
            break;
    }//switch
}
void GUIHandler::_displayChildMenu(){
    switch(_currentMenu){
        case MAIN:
            //display heater or relay based on selection
            //check which is selected heater || relays
            if(_currentIndex == 0){
                _mainMenuIndex = 0;
                _currentMenu = HEATER;
                //display heater menu items
                _displayHeaterItem();//0);
            }else if(_currentIndex == 1){
                _mainMenuIndex = _currentIndex;
                _currentMenu = RELAY;
                _currentIndex = 0;
                //display relays menu items
                _displayRelayItem();//0);
            }
            break;
        case HEATER:
            if(_currentIndex != _heaterMenuLength-1){
                _heaterMenuIndex = _currentIndex;
                _currentMenu = EDIT;
                _currentIndex = 0;
                _displayHeaterEditItem();//0);
            }
            //display edit menu
            //with last zone being viewed
            break;
        default:
            break;
    }//switch
}

void GUIHandler::_displayHeaterItem(){//byte index){
    /*
     display single heater item like so:
     Name  XX.XX(temperature)
     Auto ON  Rising||Falling
     */
    //build strings for line text
    //_currentIndex = index;
    String row1 = _heaterMenu[_currentIndex];
    String row2;
    //if not the 'multi' heater menu item
    if(_currentIndex != _heaterMenuLength-1){
        row1 += "  "+(String)_heaterData[_currentIndex].currentTemp;
        row2 = "Auto ";
        row2 += (_heaterData[_currentIndex].zoneControlOn)? "On  ":"Off ";
        row2 += (_heaterData[_currentIndex].waterFlowOpen)? "Rising":"Falling";
    }else{//index is equal to the 'multi' item format
        row2 = (String)_heaterData[0].currentTemp+' ';
        row2 += (String)_heaterData[1].currentTemp+' ';
        row2 += (String)_heaterData[2].currentTemp;
    }
    displayHandler.clearDisplay();
    displayHandler.setText(row1,0,0);
    displayHandler.setText(row2,0,1);
    
}

void GUIHandler::_updateHeaterItemValue(){
    //only update the value of the temp not the whole line of text
    String row2;
    if(_currentIndex != _heaterMenuLength-1){
        displayHandler.setText((String)_heaterData[_currentIndex].currentTemp,_heaterMenu[_currentIndex].length()+2,0);
        row2 = "Auto ";
        row2 += (_heaterData[_currentIndex].zoneControlOn)? "On  ":"Off ";
        row2 += (_heaterData[_currentIndex].waterFlowOpen)? "Rising":"Falling";
        displayHandler.setRowText(row2,1);
    }else{
        row2 = (String)_heaterData[0].currentTemp+' ';
        row2 += (String)_heaterData[1].currentTemp+' ';
        row2 += (String)_heaterData[2].currentTemp;
        displayHandler.setRowText(row2,1);
    }
}

void GUIHandler::_displayHeaterEditItem(){//byte index){
    String row1, row2;
    row1 = _heaterMenu[_heaterMenuIndex];
    row1 += "  Edit";
    row2 = _zoneEditMenu[_currentIndex]+' ';
    switch(_currentIndex){
        case 0:
            row2 += _heaterData[_heaterMenuIndex].targetTemp;
            break;
        case 1:
            row2+= _heaterData[_heaterMenuIndex].lowTemp;
            break;
        case 2:
            row2+= _heaterData[_heaterMenuIndex].highTemp;
            break;
        case 3:
            row2+= (_heaterData[_heaterMenuIndex].zoneControlOn)? "On":"Off";
            break;
    }
    
    displayHandler.clearDisplay();
    displayHandler.setText(row1,0,0);
    displayHandler.setText(row2,0,1);
}

void GUIHandler::_editHeaterSettings(byte value){
    //input is either 1 || -1 to inc || dec settings of currentValue displayed on LCD screen
    //update data
    _heaterEditedByUser = _heaterData[_heaterMenuIndex];
    switch(_currentIndex){
        case 0:
            //_heaterData[_heaterMenuIndex].targetTemp += value;
            _heaterEditedByUser.targetTemp += value;
            break;
        case 1:
            _heaterEditedByUser.lowTemp += value;
            break;
        case 2:
            _heaterEditedByUser.highTemp += value;
            break;
        defaut:
            break;
    }
    
    changeHeaterSettings = true;
    //_displayHeaterEditItem();

}

void GUIHandler::_toggleZonePower(){
    Serial.println("TOGGLE ZONE POWER");
    _heaterEditedByUser = _heaterData[_heaterMenuIndex];
    _heaterEditedByUser.zoneControlOn =! _heaterEditedByUser.zoneControlOn;
    
    changeHeaterSettings = true;
    //_displayHeaterEditItem();
    
    //update Displayed text
}

HydronicDisplayData GUIHandler::getHeaterChanges(){
    return _heaterEditedByUser;
    
}

void GUIHandler::_displayRelayItem(){
    String row1, row2;
    row1 = _relayMenu[_currentIndex];
    row2 = "Power ";
    row2 += (_relayData[_currentIndex].powerOn)? "On":"Off";
    
    displayHandler.clearDisplay();
    displayHandler.setText(row1,0,0);
    displayHandler.setText(row2,0,1);
}

void GUIHandler::_updateRelayItemValue(){
    String row2;
    row2 = "Power ";
    row2 += (_relayData[_currentIndex].powerOn)? "On":"Off";
    
    displayHandler.setRowText(row2,1);
}

void GUIHandler::_toggleRelay(byte index){
    _relayEditedByUser = _relayData[index];
    _relayEditedByUser.powerOn =! _relayEditedByUser.powerOn;
    
    changeRelaySettings = true;
}

RelayMessage GUIHandler::getRelayChanges(){
    return _relayEditedByUser;
}
#endif /* GUIHandler_h */
