/*
 * Description: class for creating an object that will
 * hold data for temperature sensors, high/low tolerances etc
 */

 class TempSensorMonitor{
  private:   
    String name_;//script output readable name
    String displayName_; //lcd friendly version of name
    int sensorPinNumber_;//pin number sensor is accessed from
    float readingTemp_;//most recent reading of the sensor
    float targetTemp_;//desired temperature outcome from the sensor
    float normalTolerance_; //number to use as range -+ of ideal temperature to call for heat, or turn heat off
    float warningTolerance_; //number to use as warning level range -+ of ideal temperature to put monitor into high or low warning state
    bool lowWarningState_; //true if temperature has exceeded warningTolerance on low end 
    bool highWarningState_;//true if temperature has exceeded warningTolerance on high end 
    bool callingForHeat_; //true if temperature has exceeded tolerance on low end

    
  public:
    TempSensorMonitor();
    TempSensorMonitor(String,String,int,float);
    TempSensorMonitor(String, String, int, float, float, float,bool,bool,bool);

    bool stateChanged; //true if value for callingForHeat_ has just been changed 
    
    String GetName();//return String of name_
    void SetName(String t);//set name_ from input String
    
    String GetDisplayName(); //return displayName_ 
    void SetDisplayName(String t); //set displayName_ from input String

    int GetPin();
    void SetPin(int);
    
    float GetReadingTemp(); //return value of readingTemp_ most recent measurement of sensor
    void SetReadingTemp(float val); //for testing only
    
    float GetTargetTemp(); //return value of targetTemp_
    void SetTargetTemp(float val); //set the value of targetTemp from input float val

    float GetTolerance();
    void SetTolerance(float val);
    
    float GetWarningTolerance();
    void SetWarningTolerance(float val);//set number to use -+ 

    bool LowTempWarning();//return true if value temp_ <= warningTolerance_
    
	bool HighTempWarning();//return true if value temp_ >= warningTolerance_
	
	bool RunningSmooth(); //return true if no errors or warnings are on at time accessed;
	
    bool IsCallingForHeat();
    void SetCallingForHeat(bool val);
    
    void Update();

    String ToStringShort();
	String ToStringLong();    
 };

 TempSensorMonitor::TempSensorMonitor(){
  name_ = "Temperature Sensor";
  displayName_ = "TempSens";
  sensorPinNumber_ = -1;
  readingTemp_ = -888;
  targetTemp_ = -888;
  normalTolerance_ = 3; //3 degrees above and below the targetTemp_, total range 6 degrees
  warningTolerance_ = 7; //7 degrees above and below the targetTemp_, total range 14 degrees
  lowWarningState_= false;
  highWarningState_ = false; 
  callingForHeat_ = false;
//  lastUpdate_ = millis();
//  updateInterval_ = 500;
  stateChanged = false;
 }
 
 TempSensorMonitor::TempSensorMonitor(String n,String dN,int pin,float tT){
  name_ = n;
  displayName_ = dN;
  sensorPinNumber_ = pin;
  readingTemp_ = tT; //set at targetTemp during creation
  targetTemp_ = tT;
  normalTolerance_ = 3; //3 degrees above and below the targetTemp_, total range 6 degrees
  warningTolerance_ = 7; //7 degrees above and below the targetTemp_, total range 14 degrees
  lowWarningState_= false; //
  highWarningState_ = false; 
  callingForHeat_ = false;
  stateChanged = false;
 }
 
TempSensorMonitor::TempSensorMonitor(String n,String dN,int pin,float tT, float nT,float wT,bool lWS, bool hWS, bool h){
  name_ = n;
  displayName_ = dN;
  sensorPinNumber_ = pin;
  readingTemp_ = -888;
  targetTemp_ = tT;
  normalTolerance_ = nT;
  warningTolerance_ = wT;
  lowWarningState_= hWS;
  highWarningState_ = hWS; 
  callingForHeat_ = h;
  stateChanged = false;
}
 
String TempSensorMonitor::GetName(){
  return name_;
}
  void TempSensorMonitor::SetName(String t){
    name_ = t;
  }
  
  String TempSensorMonitor::GetDisplayName(){
    return displayName_;
  }
  void TempSensorMonitor::SetDisplayName(String t){
    displayName_ = t;
  }
  
  int TempSensorMonitor::GetPin(){
    return sensorPinNumber_;
  }
  void TempSensorMonitor::SetPin(int val){
    sensorPinNumber_ = val;
  }  
  
  float TempSensorMonitor::GetReadingTemp(){
    return readingTemp_;
  }
  void TempSensorMonitor::SetReadingTemp(float val){
    readingTemp_ = val;
  }
  
  float TempSensorMonitor::GetTargetTemp(){
  	return targetTemp_;
  }
  
  void TempSensorMonitor::SetTargetTemp(float val){
  	targetTemp_ = val;
  }
  
  float TempSensorMonitor::GetTolerance(){
    return normalTolerance_;
  }
  void TempSensorMonitor::SetTolerance(float val){
    normalTolerance_ = val;
  }
  
  float TempSensorMonitor::GetWarningTolerance(){
    return warningTolerance_;
  }
  void TempSensorMonitor::SetWarningTolerance(float val){
    warningTolerance_ = val;
  }
  
  bool TempSensorMonitor::LowTempWarning(){
  	return lowWarningState_;
  }
  
  bool TempSensorMonitor::HighTempWarning(){
  	return highWarningState_;
  }
    
  bool TempSensorMonitor::RunningSmooth(){
//  	return (!lowWarningState_ && !highWarningState_);
	return true;
  }  
  
  bool TempSensorMonitor::IsCallingForHeat(){
    return callingForHeat_;
  }
  void TempSensorMonitor::SetCallingForHeat(bool val){
    callingForHeat_ = val;
  }
  
  void TempSensorMonitor::Update(){

	float rnd = (float)random(11)/10;
 	 	
  	if(callingForHeat_){//calling for heat true
  		readingTemp_ += rnd;//add random number to temp
  		if(readingTemp_ >= (targetTemp_ + normalTolerance_)){//reading temp is outside high tolerance
  			callingForHeat_ = false; //turn off heat
  			stateChanged = true; //show state just changed
		}else stateChanged = false;//reading temp still within high tolerance
	}else if(!callingForHeat_){ //not calling for heat
  		readingTemp_ -=rnd; //subtract random number from temp
   		if(readingTemp_ <= (targetTemp_ - normalTolerance_)){ //reading temp is outside low tolerance
  			callingForHeat_ = true; //turn on heat
  			stateChanged = true;// show state just changed
  		}else stateChanged = false; //reading temp still within low tolerance
  	}
  }
  
  String TempSensorMonitor::ToStringShort(){
    return "Name: "+ name_+" CurrentValue: "+ (String)readingTemp_;
  }

  String TempSensorMonitor::ToStringLong(){
  	return "name: "+ name_+"\n"
  	+ "displayName: "+ displayName_+"\n"
  	+ "pinNumber: "+ (String)sensorPinNumber_ +"\n"
  	+ "readingTemp: "+(String)readingTemp_ +"\n"
  	+ "targetTemp: "+(String)targetTemp_+"\n"
  	+ "normalTolerance: "+(String)normalTolerance_+"\n"
  	+ "warningTolerance: "+(String)warningTolerance_+"\n"
  	+ "lowWarningState: "+(String)lowWarningState_+"\n"
  	+ "highWarningState: "+(String)highWarningState_+"\n"
  	+ "callingForHeat: "+(String)callingForHeat_+"\n";
  }  
  
