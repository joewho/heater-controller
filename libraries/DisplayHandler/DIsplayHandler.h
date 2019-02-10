//
//  DisplayHandler.h
//  
//
//  Created by Joseph Cutro on 2/6/19.
//

#ifndef DisplayHandler_h
#define DisplayHandler_h
#include <LiquidCrystal.h>

const int rs=42,en=43,d4=22,d5=23,d6=24,d7=25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

class DisplayHandler{
private:
    byte _columnCount;
    byte _rowCount;
    char _selectorChar;
    void _clearRow(int row){
        lcd.setCursor(0,row);
        for(int i=0;i<_columnCount;i++){
            lcd.print(' ');
        }
    }
public:
    DisplayHandler(byte c=16, byte r=2){ _columnCount = c; _rowCount = r;}
    void initiate(){
        lcd.begin(_columnCount,_rowCount);
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
        for(int i=0;i<arrayLength;i++){
            lcd.setCursor(0,i);
            lcd.print(' ');
            lcd.print(stringArray[i]);
        }
        setSelectorRow(rowSelected);
    }
};
#endif /* DisplayHandler_h */
