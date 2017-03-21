    // Date and time functions using a DS1307 RTC connected via I2C and Wire lib
    #include <Wire.h>
    #include "RTClib.h"
    RTC_DS1307 RTC;
    double M,Y,D,MN,H,S;
    double A,B,C,E,F;
    float CurrentJDN;
    
    int count = 0;
    int all_vis = 0; //forces stars on even when sun is up
    int test = 0;
    int led_on = 5;
    int led_off = 50000; // time LED are off in milliseconds
    int delay_rep = 3; // number of reps of off time
    int i = 1;
    int led_on_cycle = 1;
    int led_off_cycle = led_off;
    
    int cycles = 1000; // main cycle
    int cycle_1 = 10; //cycle of cycle
    
    int start_up_delay = 200; // the delay on the cycle throught the constellations
    
    //Cape Town
    double latitude = -33.916;
    double longitude = 18.4167;
    
    //AfricaBurn Tankwa Karoo
    //double latitude = -32.3266;
    //double longitude = 19.7481;
    
    
    double obliquity = 23.4373055;
    
    
    
    double ARI_RA = 2.7;
    double TAU_RA = 4.6;
    double GEM_RA = 7.1;
    double CNC_RA = 8.6;
    double LEO_RA = 10.6;
    double VIR_RA = 13.2;
    double LIB_RA = 15.3;
    double SCO_RA = 16.6;
    double OPH_RA = 17.2;
    double SGR_RA = 19.1;
    double CAP_RA = 21.1;
    double AQR_RA = 22.4;
    double PSC_RA = 0.5;
    
    
    double ORI_RA = 5.6;
    
    double ARI_dec = 21.5;
    double TAU_dec = 18.1;
    double GEM_dec = 23.2;
    double CNC_dec = 20.9;
    double LEO_dec = 12.6;
    double VIR_dec = -2.5;
    double LIB_dec = -16.6;
    double SCO_dec = -27.5;
    double OPH_dec = -7.5;
    double SGR_dec = -24.5;
    double CAP_dec = -17.6;
    double AQR_dec = -10.6;
    double PSC_dec = 11.0;
    
    double ORI_dec = 7.78;
    
    double capella_RA = 79.1732/25;
    double capella_dec = 45.9961;
    
    double canopus_RA = 95.9881/15;
    double canopus_dec = -52.6955;
    
    double sirius_RA = 101.2844/15;
    double sirius_dec = -16.7215;
    
    double arcturus_RA = 213.9109/15;
    double arcturus_dec = 19.1731;
    
    double achernar_RA = 24.4291/15;
    double achernar_dec = -57.2370;
    
    double procyon_RA = 144.8224/15;
    double procyon_dec = 5.2206;
    
  
    
    double JD_calc(double(Y),double(M),double(D),double(H),double(MN),double(S)){
      double ja,jb1,jb,jc,jd;
      float JD,MJD,MJD2,JD0,MJD0;
      ja = (int) 367 * Y;

      jb1 = int((M+9)/12);

      jb = int(-(7*(Y+jb1))/4);

      jc = int((275*M)/9);

      jd = (H+(MN+(S/60))/60)/24;

      JD0 = ja+jb+jc+D+1721013.5;

      JD = ja+jb+jc+D+1721013.5+jd;

      MJD0 = ja+jb+jc+D-678987;

      MJD = ja+jb+jc+D-678987+jd;
 
      MJD2 = JD - 2400000.5;

      return JD0;
    }
    
    double LST_calc(float(JD0)){
    
      float TUDI,GST00,GST,LSA,LST;
      float RA;
      int LSH,LSM;

      TUDI = (JD0 - 2451545.0)/36525;

      GST00 = 100.4606184 + (36000.77005361*TUDI) + (0.00038793*TUDI*TUDI);

      while(GST00 > 360){
        GST00 = GST00 - 360;
      }

      GST = GST00 + (0.25068447733746215*((60*(H-2))+MN+(S/60)));

      while(GST > 360){
        GST = GST - 360;
    }

    LSA = GST + longitude;
//    Serial.print(LSA);
//    Serial.println();
    if(LSA < 0){
      LSA = LSA + 360;
    }
    if(LSA > 360){
      LSA = LSA - 360;
    }
//    Serial.print(LSA);
//    Serial.println();
    LST = LSA/360*24;
    LSH = int(LST);
    LSM = int((LST - LSH)*60);
    return LST;
    }
    
    double H_calc(double(latitude),double(dec)){
      //Serial.println(latitude);
      //Serial.println(dec);
      double h;
      double d;
      double t; 
      double t1;
      double t2;
      t1 = tan(radians(latitude));
      t2 = tan(radians(dec));
      t = -1*t1*t2;
      d = degrees(acos(t));
      h = d/15;
      return h;
    }
      
    
    double LST_rise(double(latitude),double(dec),double(RA)){
      double H;
      H = H_calc(latitude,dec);
      double LSTr;
      LSTr = 24.0 + RA - H;
      while(LSTr > 24){
        LSTr = LSTr-24;
      }
      return LSTr;
    }
    
    double LST_set(double(latitude),double(dec),double(RA)){
      double H;
      H = H_calc(latitude,dec);
      double LSTr;
      LSTr = RA + H;
      while(LSTr > 24){
        LSTr = LSTr - 24;
      }
      return LSTr;
    }
    
    double visible(String(Name),int(con),double(RA),double(dec),double(latitude),double(LST)){
       int vis;
       double LST_r;
       double LST_s;
       LST_r = LST_rise(latitude,dec,RA);
       LST_s = LST_set(latitude,dec,RA);
       
       //if(LST_r > LST && RA < 12){
       if(LST >= 0 && LST < 12 && LST_r > 12 && LST_r <= 24){
         //Serial.print(LST_r);
         //Serial.print(" : ");
         LST_r = LST_r - 24;
       }
       //if(LST_s < LST && RA > 12){
       if(LST > 12 && LST <= 24 && LST_s < 8){
       //Serial.print(LST_s);
       //Serial.print(" : ");
         LST_s = LST_s + 24;
       }
       //Serial.print(Name);
       //Serial.print(" : ");
       //Serial.print(LST_r);
       //Serial.print(" : ");
       //Serial.print(RA);
       //Serial.print(" : ");
       //Serial.print(LST_s);
       //Serial.print(" : ");
       if(LST > LST_r && LST < LST_s){
         //Serial.println(" UP");
         digitalWrite(con, HIGH);
         vis = 1;
       }
       else{
         //Serial.println(" DOWN");
         digitalWrite(con, LOW);
         vis = 0;
       }
       return vis;
     }
    
    double visible_simple(String(Name),int(con),double(RA),double(dec),double(latitude),double(LST)){
       int vis;
       double LST_r;
       double LST_s;
       LST_r = LST_rise(latitude,dec,RA);
       LST_s = LST_set(latitude,dec,RA);
       if(LST >= 0 && LST < 12 && LST_r > 12 && LST_r <= 24){
         LST_r = LST_r - 24;
       }
       if(LST > 12 && LST <= 24 && LST_s < 8){
         LST_s = LST_s + 24;
       }
       if(LST > LST_r && LST < LST_s){
         //digitalWrite(con, HIGH);
         vis = 1;
       }
       else{
         //digitalWrite(con, LOW);
         vis = 0;
       }
       return vis;
     }
     
    double circ(double(degr)){
      if(degr < 0){
        while(degr < 360){
          degr = degr + 360;
        }
      }
      if(degr > 360){
        while(degr > 360){
          degr = degr - 360;
        }
      }
      return degr; 
    } 
     
     double calc_sun_y(double(D_y)){
       double N = (360/365.24191)*D_y;
       N = circ(N);
       //Serial.println(N);
       double eg = 279.557208;
       double wg = 283.112438;
       double e = 0.016705;
       double M = N + eg - wg;
       M = circ(M);
       //Serial.println(M);
       double Ec = (360/3.14)*(e*sin(radians(M)));
       Ec = circ(Ec);
       double h = N + Ec + eg;
       h = circ(h);
       return(h);
     }
     
     double calc_sun_dec(double(sun_h),double(obl)){
       float h = radians(sun_h);
       float B = radians(0.0);
       float e = radians(23.438055);
       float sin_h = sin(B)*cos(e) + cos(B)*sin(e)*sin(h);
       //Serial.println(sin_h);
       float d = asin(sin_h);
       //Serial.println(d);
       //Serial.println(degrees(d));
       //Serial.println(degrees(d)/15);
       return(degrees(d));   
     }
     
     double calc_sun_RA(double(sun_h),double(obl)){
         float h = radians(sun_h);
         float B = radians(0.0);
         float e = radians(obl);
         
         float y = sin(h)*cos(e) - tan(B)*sin(e);

         float x = cos(h);

         float a = atan2(y,x);
       return(degrees(a)/15); 
     } 
     
       
    
    
    

    
//    int start_pin = 24;
//    int ARI = start_pin + 2;
//    int TAU = start_pin + 4;
//    int GEM = start_pin + 3;
//    int CNC = start_pin + 4;
//    int LEO = start_pin + 5;
//    int VIR = start_pin + 6;
//    int LIB = start_pin + 7;
//    int SCO = start_pin + 8;
//    int OPH = start_pin + 9;
//
//    
//    int SGR = start_pin + 10;
//    int CAP = start_pin + 11;
//    int AQR = start_pin + 12;
//    int PSC = start_pin + 13;
//    
//    int CRU = start_pin + 14;
//    int ORI = start_pin + 15;
    
    int ARI = 23;
    int TAU = 25;
    int GEM = 27;
    int CNC = 29;
    int LEO = 31;
    int VIR = 33;
    int LIB = 35;
    int SCO = 37;
    int OPH = 24;

    
    int SGR = 26;
    int CAP = 41;
    int AQR = 43;
    int PSC = 45;
    
    int CRU = 47;
    int ORI = 49;
    
    int SUN = 51;
    
    int LAMP = 22;
    


    
    
    
    int switchpin = 46;
    int switchstate;
    
    int analogInput = 0;
    float vout = 0.0;
    float vin = 0.0;
    float min_v = 12.26;
    float max_v = 12.62;
    float R1 = 100000;
    float R2 = 9400;
    int value = 0;


/////////////////////////////////////////////////////////////////////////    
void setup () {
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
    //if (! RTC.isrunning()) {
    //Serial.println("RTC is NOT running!");
    
    
    
    
    
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
    
  


    //}
    //else{
    //  Serial.println("RTC Running");
    //}
    pinMode(switchpin, INPUT);

   
    
    pinMode(ARI, OUTPUT);
    pinMode(TAU, OUTPUT);
    pinMode(GEM, OUTPUT);
    pinMode(CNC, OUTPUT);
    pinMode(LEO, OUTPUT);
    pinMode(VIR, OUTPUT);
    pinMode(LIB, OUTPUT);
    pinMode(SCO, OUTPUT);
    pinMode(OPH, OUTPUT);
    pinMode(CRU, OUTPUT);
    pinMode(ORI, OUTPUT);
    pinMode(SGR, OUTPUT);
    pinMode(CAP, OUTPUT);
    pinMode(AQR, OUTPUT);
    pinMode(PSC, OUTPUT);
    pinMode(SUN, OUTPUT);
    pinMode(LAMP, OUTPUT);
    
    //initial state all off
      
      
    digitalWrite(ARI, HIGH);
    digitalWrite(TAU, HIGH);
    digitalWrite(GEM, HIGH);
    digitalWrite(CNC, HIGH);
    digitalWrite(LEO, HIGH);
    digitalWrite(VIR, HIGH);
    digitalWrite(LIB, HIGH);
    digitalWrite(SCO, HIGH);
    digitalWrite(OPH, HIGH);
    digitalWrite(CRU, HIGH);
    digitalWrite(ORI, HIGH);
    digitalWrite(SGR, HIGH);
    digitalWrite(CAP, HIGH);
    digitalWrite(AQR, HIGH);
    digitalWrite(PSC, HIGH);
    digitalWrite(SUN, HIGH);
    digitalWrite(LAMP, HIGH);
    //RTC.adjust(DateTime(__DATE__, __TIME__));
    
    pinMode(analogInput, INPUT);
//////// ADJUST THE TIME TO NOW
     //RTC.adjust(DateTime(__DATE__, __TIME__))
  }






/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void loop () {
      DateTime now = RTC.now();
      //double JD0;
      //double sun_H;
      //double sun_RA;
      //double sun_h;
      //double sun_dec;
      //float LST;
       
      
       
        M = (int) now.month();
        Y = (int) now.year();
        D = (int) now.day();
        MN = (int) now.minute();
        H = (int) now.hour();
        S = (int) now.second();
       
        double JD0 = JD_calc(Y,M,D,H,MN,S);
        double JD_y = JD_calc(2010,1,0,0,0,0);
        double D_y = JD0 - JD_y;
        double sun_h = calc_sun_y(D_y);
        double sun_RA = calc_sun_RA(sun_h,obliquity);
    
        
        double sun_dec = calc_sun_dec(sun_h,obliquity);
       
        //Serial.print("Sun ecliptic RA : ");
        //Serial.println(sun_h);
        //Serial.print("Sun RA : ");
        //Serial.println(sun_RA);
        //Serial.print("Sun dec : ");
        //Serial.println(sun_dec);
        double LST_sun_rise = LST_rise(latitude,sun_dec,sun_RA);
        double LST_sun_set = LST_set(latitude,sun_dec,sun_RA);
        float LST = LST_calc(JD0);
        //delay(3000);
        
        value = analogRead(analogInput);
        //Serial.println(value);
        vout = (value * 5) / 1024.0;
        vin = vout / (R2/(R1+R2));
        if(vin < 0.09){
            vin=0;
          }
       if(vin < min_v){
         min_v = vin;
       }
       if(vin > max_v){
         max_v = vin;
       }
       
       Serial.println();
       Serial.println();
       Serial.println(count);
       Serial.print("Battery Voltage : ");
       Serial.println(vin);
       
        float RA;

        Serial.print(now.year(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print(' ');
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();
        
        Serial.print("longitude : ");
        Serial.println(longitude);
        Serial.print("latitude : ");
        Serial.println(latitude);
        //Serial.println();
        
        Serial.print("Julian Day: ");
        Serial.println(JD0);
        Serial.print("LST : ");
        Serial.println(LST);
        Serial.print("Sunrise : ");
        Serial.println(LST_sun_rise);
        Serial.println();
        Serial.print("Sunset : ");
        Serial.println(LST_sun_set);
        Serial.println();
        int sun_vis = visible("SUN",SUN,sun_RA,sun_dec,latitude,LST);
        Serial.print('Sun Visible : ');
        Serial.println(sun_vis);
        
       //Serial.println("START");
       
       
   switchstate = digitalRead(switchpin);
   Serial.print("Switchstate : ");
   Serial.println(switchstate);
   if(switchstate == HIGH){
     for(int j = 0; j <= cycles; j++){
     //Serial.println("Switch : ON : ");
      digitalWrite(ARI, HIGH);
      digitalWrite(TAU, HIGH);
      digitalWrite(GEM, HIGH);
      digitalWrite(CNC, HIGH);
      digitalWrite(LEO, HIGH);
      digitalWrite(VIR, HIGH);
      digitalWrite(LIB, HIGH);
      digitalWrite(SCO, HIGH);
      digitalWrite(OPH, HIGH);
      digitalWrite(SGR, HIGH);
      digitalWrite(CAP, HIGH);
      digitalWrite(AQR, HIGH);
      digitalWrite(PSC, HIGH);
  
      digitalWrite(CRU, HIGH);
      digitalWrite(ORI, HIGH);
      digitalWrite(SUN, HIGH);
      digitalWrite(LAMP, HIGH);
      
      delayMicroseconds(led_on_cycle);
      digitalWrite(ARI, LOW);
      digitalWrite(TAU, LOW);
      digitalWrite(GEM, LOW);
      digitalWrite(CNC, LOW);
      digitalWrite(LEO, LOW);
      digitalWrite(VIR, LOW);
      digitalWrite(LIB, LOW);
      digitalWrite(SCO, LOW);
      digitalWrite(OPH, LOW);
      digitalWrite(SGR, LOW);
      digitalWrite(CAP, LOW);
      digitalWrite(AQR, LOW);
      digitalWrite(PSC, LOW);
  
      digitalWrite(CRU, LOW);
      digitalWrite(ORI, LOW);
      digitalWrite(SUN, LOW);
      digitalWrite(LAMP, LOW);
      delayMicroseconds(led_off_cycle);
      count = 0;
     }
   }

   
   if(switchstate == LOW){
     int sun_vis = visible("SUN",SUN,sun_RA,sun_dec,latitude,LST);
     Serial.print('Sun Visible : ');
     Serial.println(sun_vis);
     if(count <= 1){
         int start_LST;
         for(int revs = 0; revs < 3; revs++){
           if(revs == 0){
             start_LST = int(LST);
           }else{
             start_LST = 0;
           }
           for(int LSTc = start_LST; LSTc < 24;LSTc++){
               sun_vis = visible("SUN",SUN,sun_RA,sun_dec,latitude,LSTc);
               visible("ARI",ARI,ARI_RA,ARI_dec,latitude,LSTc);
               visible("TAU",TAU,TAU_RA,TAU_dec,latitude,LSTc);
               visible("GEM",GEM,GEM_RA,GEM_dec,latitude,LSTc);
               visible("CNC",CNC,CNC_RA,CNC_dec,latitude,LSTc);
               visible("LEO",LEO,LEO_RA,LEO_dec,latitude,LSTc);
               visible("VIR",VIR,VIR_RA,VIR_dec,latitude,LSTc);
               visible("LIB",LIB,LIB_RA,LIB_dec,latitude,LSTc);
               visible("SCO",SCO,SCO_RA,SCO_dec,latitude,LSTc);
               visible("OPH",OPH,OPH_RA,OPH_dec,latitude,LSTc);
               visible("SGR",SGR,SGR_RA,SGR_dec,latitude,LSTc);
               visible("CAP",CAP,CAP_RA,CAP_dec,latitude,LSTc);
               visible("AQR",AQR,AQR_RA,AQR_dec,latitude,LSTc);
               visible("PSC",PSC,PSC_RA,PSC_dec,latitude,LSTc);
               visible("ORI",ORI,ORI_RA,ORI_dec,latitude,LSTc);
               digitalWrite(CRU, HIGH);
               delay(start_up_delay);
           }
         }
         Serial.println("ON");
      digitalWrite(ARI, HIGH);
      delay(start_up_delay);
      digitalWrite(TAU, HIGH);
      delay(start_up_delay);
      digitalWrite(GEM, HIGH);
      delay(start_up_delay);
      digitalWrite(CNC, HIGH);
      delay(start_up_delay);
      digitalWrite(LEO, HIGH);
      delay(start_up_delay);
      digitalWrite(VIR, HIGH);
      delay(start_up_delay);
      digitalWrite(LIB, HIGH);
      delay(start_up_delay);
      digitalWrite(SCO, HIGH);
      delay(start_up_delay);
      digitalWrite(OPH, HIGH);
      delay(start_up_delay);
      digitalWrite(SGR, HIGH);
      delay(start_up_delay);
      digitalWrite(CAP, HIGH);
      delay(start_up_delay);
      digitalWrite(AQR, HIGH);
      delay(start_up_delay);
      digitalWrite(PSC, HIGH);
      delay(start_up_delay);
      digitalWrite(CRU, HIGH);
      delay(start_up_delay);
      digitalWrite(ORI, HIGH);
      delay(start_up_delay);
      digitalWrite(SUN, HIGH);
      delay(start_up_delay);
       }
     
     
     
     //Serial.println("Switch : OFF : ");


     //int sun_vis;
     //if(count == 1){
     
     //}
     sun_vis = visible("SUN",SUN,sun_RA,sun_dec,latitude,LST);
     if(sun_vis == 0 || all_vis == 1){
       if(all_vis == 1){
         //Serial.println("Force Stars ON");
       }
       int ARI_vis = visible_simple("ARI",ARI,ARI_RA,ARI_dec,latitude,LST);
       int TAU_vis = visible_simple("TAU",TAU,TAU_RA,TAU_dec,latitude,LST);
       int GEM_vis = visible_simple("GEM",GEM,GEM_RA,GEM_dec,latitude,LST);
       int CNC_vis = visible_simple("CNC",CNC,CNC_RA,CNC_dec,latitude,LST);
       int LEO_vis = visible_simple("LEO",LEO,LEO_RA,LEO_dec,latitude,LST);
       int VIR_vis = visible_simple("VIR",VIR,VIR_RA,VIR_dec,latitude,LST);
       int LIB_vis = visible_simple("LIB",LIB,LIB_RA,LIB_dec,latitude,LST);
       int SCO_vis = visible_simple("SCO",SCO,SCO_RA,SCO_dec,latitude,LST);
       int OPH_vis = visible_simple("OPH",OPH,OPH_RA,OPH_dec,latitude,LST);
       int SGR_vis = visible_simple("SGR",SGR,SGR_RA,SGR_dec,latitude,LST);
       int CAP_vis = visible_simple("CAP",CAP,CAP_RA,CAP_dec,latitude,LST);
       int AQR_vis = visible_simple("AQR",AQR,AQR_RA,AQR_dec,latitude,LST);
       int PSC_vis = visible_simple("PSC",PSC,PSC_RA,PSC_dec,latitude,LST);
       int ORI_vis = visible_simple("ORI",ORI,ORI_RA,ORI_dec,latitude,LST);
       for(int l = 0; l <= cycle_1; l++){
         for(int k = 0; k <= cycles; k++){
//       visible("ARI",ARI,ARI_RA,ARI_dec,latitude,LST);
//       visible("TAU",TAU,TAU_RA,TAU_dec,latitude,LST);
//       visible("GEM",GEM,GEM_RA,GEM_dec,latitude,LST);
//       visible("CNC",CNC,CNC_RA,CNC_dec,latitude,LST);
//       visible("LEO",LEO,LEO_RA,LEO_dec,latitude,LST);
//       visible("VIR",VIR,VIR_RA,VIR_dec,latitude,LST);
//       visible("LIB",LIB,LIB_RA,LIB_dec,latitude,LST);
//       visible("SCO",SCO,SCO_RA,SCO_dec,latitude,LST);
//       visible("OPH",OPH,OPH_RA,OPH_dec,latitude,LST);
//       visible("SGR",SGR,SGR_RA,SGR_dec,latitude,LST);
//       visible("CAP",CAP,CAP_RA,CAP_dec,latitude,LST);
//       visible("AQR",AQR,AQR_RA,AQR_dec,latitude,LST);
//       visible("PSC",PSC,PSC_RA,PSC_dec,latitude,LST);
//       visible("ORI",ORI,ORI_RA,ORI_dec,latitude,LST);
       if(ARI_vis == 1){digitalWrite(ARI, HIGH);}       
       if(TAU_vis == 1){digitalWrite(TAU, HIGH);}
       if(GEM_vis == 1){digitalWrite(GEM, HIGH);}
       if(CNC_vis == 1){digitalWrite(CNC, HIGH);}
       if(LEO_vis == 1){digitalWrite(LEO, HIGH);}
       if(VIR_vis == 1){digitalWrite(VIR, HIGH);}
       if(LIB_vis == 1){digitalWrite(LIB, HIGH);}
       if(SCO_vis == 1){digitalWrite(SCO, HIGH);}
       if(OPH_vis == 1){digitalWrite(OPH, HIGH);}
       if(SGR_vis == 1){digitalWrite(SGR, HIGH);}
       if(CAP_vis == 1){digitalWrite(CAP, HIGH);}
       if(AQR_vis == 1){digitalWrite(AQR, HIGH);}
       if(PSC_vis == 1){digitalWrite(PSC, HIGH);}
       if(ORI_vis == 1){digitalWrite(ORI, HIGH);}
       //Serial.println("CRUX always UP");
       digitalWrite(CRU, HIGH);
       digitalWrite(SUN, HIGH);
       
      delayMicroseconds(led_on);
      digitalWrite(ARI, LOW);
      digitalWrite(TAU, LOW);
      digitalWrite(GEM, LOW);
      digitalWrite(CNC, LOW);
      digitalWrite(LEO, LOW);
      digitalWrite(VIR, LOW);
      digitalWrite(LIB, LOW);
      digitalWrite(SCO, LOW);
      digitalWrite(OPH, LOW);
      digitalWrite(SGR, LOW);
      digitalWrite(CAP, LOW);
      digitalWrite(AQR, LOW);
      digitalWrite(PSC, LOW);
  
      digitalWrite(CRU, LOW);
      digitalWrite(ORI, LOW);
      digitalWrite(SUN, LOW);
      digitalWrite(LAMP, LOW);
      for(int d = 0; d <= delay_rep; d++){
        delayMicroseconds(led_off);
      }

      
       }}
       
       
 
       //Serial.println(count);

         
           
           
     }else{
      Serial.println("Stars not visible");
      digitalWrite(ARI, LOW);
      digitalWrite(TAU, LOW);
      digitalWrite(GEM, LOW);
      digitalWrite(CNC, LOW);
      digitalWrite(LEO, LOW);
      digitalWrite(VIR, LOW);
      digitalWrite(LIB, LOW);
      digitalWrite(SCO, LOW);
      digitalWrite(OPH, LOW);
      digitalWrite(CRU, LOW);
      digitalWrite(ORI, LOW);
      digitalWrite(SGR, LOW);
      digitalWrite(CAP, LOW);
      digitalWrite(AQR, LOW);
      digitalWrite(PSC, LOW);
      
      digitalWrite(CRU, LOW);
      digitalWrite(ORI, LOW);
      digitalWrite(SUN, HIGH);
      digitalWrite(LAMP, LOW);
      delay(10000);
     }
     
   }
    
  count = count + 1;
    }
