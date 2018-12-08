//
//  ButtonTalker.h
//  
//
//  Created by Joseph Cutro on 12/7/18.
//

#ifndef ButtonTalker_h
#define ButtonTalker_h

class ButtonTalker{
private:
    ButtonListener listener
    String _name;
    int _pin;
    String _action;
    float _lastChange;
    bool _hasChanged;
public:
    ButtonTalker(ButtonListener listener){
        _name = listener->getName();
        _pin = pin;
        _action = action;
        _lastChange = lastChange;
        _hasChanged = hasChanged;
    }
    String getMessage(){
        //return stringified from all members except for ButtonListener
    }
    
};

#endif /* ButtonTalker_h */
