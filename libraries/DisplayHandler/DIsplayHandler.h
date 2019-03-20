//
//  DisplayHandler.h
//  
//
//  Created by Joseph Cutro on 2/6/19.
//

#ifndef DisplayHandler_h
#define DisplayHandler_h
#include <LiquidCrystal.h>

LiquidCrystal lcd(LCD_rs, LCD_en, LCD_d4, LCD_d5, LCD_d6, LCD_d7);

class DisplayHandler{
private:
    byte _brightness = 255;
    byte _contrast = 100;
    byte _columnCount;
    byte _rowCount;
    char _selectorChar;
    void _clearRow(int row){
        lcd.setCursor(0,row);
        for(int i=0;i<_columnCount;i++){
            lcd.print(' ');
        }
    }
    void _setBacklight();
public:
    DisplayHandler(byte c=LCD_columnCount, byte r=LCD_rowCount){ _columnCount = c; _rowCount = r;}
    void initiate(){
        lcd.begin(_columnCount,_rowCount);
        analogWrite(LCD_Potentiometer,_contrast);
        pinMode(LCD_RED,OUTPUT);
        pinMode(LCD_GREEN,OUTPUT);
        pinMode(LCD_BLUE,OUTPUT);
        _brightness = 100;
        _setBacklight();
    }
    void clearDisplay(){
        lcd.clear();
        lcd.setCursor(0,0);
    }
    
    void setSelectorChar(char c){_selectorChar = c;}

    void setSelectorRow(byte row){
        for(int i=0;i<_rowCount;i++){
            lcd.setCursor(0,i);
            if(i == row)
                lcd.print(_selectorChar);
            else
                lcd.print(' ');
        }
    }
    
    void setRowText(String text, byte row){
        _clearRow(row);
        lcd.setCursor(0,row);
        lcd.print(text);
    }
    void setText(String text, byte column, byte row){
        lcd.setCursor(column,row);
        lcd.print(text);
    }
    
    void setMenuText(String* stringArray, byte arrayLength, byte rowSelected){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(' ');
        lcd.print(stringArray[0]);
        lcd.setCursor(0,1);
        lcd.print(' ');
        lcd.print(stringArray[1]);
        lcd.setCursor(0,2);
        lcd.print(' ');
        lcd.print(stringArray[2]);
        lcd.setCursor(0,3);
        lcd.print(' ');
        lcd.print(stringArray[3]);
        /*
        for(int i=0;i<arrayLength;i++){
            Serial.println("DisplayHandler::setMenuText - i: "+(String)i);
            Serial.println("stringArray[i]: "+stringArray[i]);
            lcd.setCursor(0,i);
            lcd.print(' ');
            lcd.print(stringArray[i]);
        }
         */
        setSelectorRow(rowSelected);
    }
};

void DisplayHandler::_setBacklight(){
    analogWrite(LCD_RED, 125);
    analogWrite(LCD_GREEN, 0);
    analogWrite(LCD_BLUE,0);
    
}
#endif /* DisplayHandler_h */
