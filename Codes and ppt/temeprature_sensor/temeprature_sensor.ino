const int analogInPin = A0;  // Analog input pin that the LM35 is attached to
const int analogOutPin1 = 9; // Analog output pin that the Red LED is attached to
const int analogOutPin2 = 10; // Analog output pin that the Blue LED is attached to
double sense=
1.0;        // value read from the sensor

float low, norm, high, diff;
float dlow, dnorm, dhigh;
float flow, fnorm, fhigh;
float temp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
float difft[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
float rule[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int out;


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  
high=0; low=0; norm=0; diff=0;
dlow=0; dnorm=0; dhigh=0;
flow=0; fnorm-0; fhigh=0;


for(int i=0; i<=9;i++)
  temp[i]=difft[i]=rule[i]=0;

  sense = analogRead(analogInPin);             
  Serial.print("sensor = " );                       
  Serial.println(sense);
  
  if(sense<=50)
    low=1.0;
  else if(sense>50 && sense<=60)
  {
    low=(-(sense)/10)+6.0;
    norm=(sense/10)-5.0;
  }
  else if(sense>60 && sense<=70)
  {
    norm=((-(sense))/10)+7.0;
    high=(sense/10)-6.0;
  }
  
  else if(sense>70)
    high=1.0;
   Serial.print("low= ");    
   Serial.print(low);  
   Serial.print(" high= ");  
   Serial.print(high);  
   Serial.print(" norm= ");  
   Serial.println(norm);
   
   diff=sense-60.0; //since normal=60
   Serial.print("diff= ");    
   Serial.println(diff);  
   
  if(diff<=-10.0)
    dlow=1;
  else if(diff>-10.0 && diff<=0.0)
  {
    dlow=-diff/10;
    dnorm=diff/10+1.0;
  }
  else if(diff>0.0 && diff<=10.0)
  {
    dnorm=-diff/10+1.0;
    dhigh=diff/10;
  }
  else if(diff>10.0)
    dhigh=1;
    
   Serial.print("dlow= ");    
   Serial.print(dlow);  
   Serial.print(" dhigh= ");  
   Serial.print(dhigh);  
   Serial.print(" dnorm= ");  
   Serial.println(dnorm);
  
  //rule base for temp  
  if(low>0.0)
    temp[1]=temp[4]=temp[7]=low;
  if(norm>0.0)
    temp[2]=temp[5]=temp[8]=norm;
  if(high>0.0)
    temp[3]=temp[6]=temp[9]=high; 
  
  //rule base for error
  if(dlow>0.0)
    difft[1]=difft[2]=difft[3]=dlow;
  if(dnorm>0.0)
    difft[4]=difft[5]=difft[6]=dnorm;
  if(dhigh>0.0)
    difft[7]=difft[8]=difft[9]=dhigh; 
    
  //intersection
      for(int i=0; i<=9; i++)
      {
        if(temp[i]>0.0 && difft[i]>0.0)
        
          {
            rule[i]=min(temp[i], difft[i]);
              Serial.print("rule= ");
              
             Serial.println(i);
          }
      }
  
  //calculation
  flow=sqrt(sq(rule[1])+sq(rule[2])+sq(rule[4])+sq(rule[7]));
  fhigh=sqrt(sq(rule[3])+sq(rule[6])+sq(rule[8])+sq(rule[9]));
  fnorm=rule[5];
  
  out=(flow*(-255) + fnorm*0 + fhigh*255)/(flow + fhigh + fnorm);
  
  if(out<0)
  {
    out=-(out);
    Serial.print("\t Output (cold)= ");
    Serial.println(out);
    analogWrite(analogOutPin1, 0);
    analogWrite(analogOutPin2, out);
  }    
  else if(out>0)
  {
    Serial.print("\t Output (hot)= ");
    Serial.println(out);
    analogWrite(analogOutPin2, 0);
    analogWrite(analogOutPin1, out);
  }
  else
  {
    Serial.print("\t Output (No Change)= ");
    Serial.println(out);
    analogWrite(analogOutPin2, 0);
    analogWrite(analogOutPin2, 0);
  }
  // wait 200 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(200);                     
}
