int greenPin[3] = {8,5,3};
int redPin[3] = {9,6,4};
int echoPin[3] = {12,42,52};
int trigPin[3] = {13,43,53};
float speedOfSound = .03432;
float dist[3] = {0, 0, 0};
float duration[3];
float minDist = 0.;
float maxDist = 50.;
float threatVal[3];
float prevThreatVal[3];
float currentThreatVal[3];
float greenValue[3];
float pwmValG[3];
float redValue[3] = {50,50,50};
float pwmValR[3];
float ledDelay = 100.;
float startTime = 0.;
float currentTime = 0.;
bool isDelayed = false;
const int valueSize = 10;
int count[3] = {0,0,0};
int longRangeCount[3] = {0,0,0};
float valueSet[11][3];
int velCount = 0;
float velSet[4];
float currentTimev = 0.;
float startTimev = 0.;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 3; i++)
  {
    pinMode(redPin[i], OUTPUT);
    pinMode(greenPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
    pinMode(trigPin[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // clear  
  for(int i = 0; i < 3; i++)
  {
    count[i]++;
    digitalWrite(trigPin[i], LOW);
    delayMicroseconds(2);
    // pulse
    digitalWrite(trigPin[i], HIGH);
    delayMicroseconds(10);
    // get data
    digitalWrite(trigPin[i], LOW);
    duration[i] = pulseIn(echoPin[i], HIGH);
    dist[i] = (duration[i] * speedOfSound) /2.; // /2 because there and back

     
  
  // Serial.print("Distance: ");
  // Serial.println(dist);
  // Serial.print("Threat Value: ");
    prevThreatVal[i] = threatVal[i];
    threatVal[i] = (dist[i]-minDist)/((maxDist-minDist)/100);
    startTimev = micros();
    currentTime = millis();
    valueSet[count[i]][i] = threatVal[i];
    if(count[i] >= 10)
    {
      for(int j = 1; j <= count[i]; j++)
      {
        if(valueSet[j] > 125)
        {
          longRangeCount[i]++;
        }
      } 
      count[i] = 0;
  
      if(longRangeCount[i] > 9)
      {
        redValue[i] = 0;
        greenValue[i] = 0;
      }
  
      longRangeCount[i] = 0;
    }

    if(isDelayed && (currentTime - startTime) >= ledDelay)
    {
      isDelayed = !isDelayed;
    }
    if(!isDelayed)
    {
        if(threatVal[i] <= 0.0)
        {
          redValue[i] = 100;
          greenValue[i] = 0;
          startTime = millis();
        }
        else if(threatVal[i] <= 100.0 && threatVal[i] > 0.0)
        {
          greenValue[i] = threatVal[i];
          redValue[i] = 100. - threatVal[i];
          startTime = millis();
        }
        else if (threatVal[i] > 100.0 && threatVal[i] <= 125.0)
        {
          redValue[i] = 0;
          greenValue[i] = 100;
          startTime = millis();
        }
        isDelayed = !isDelayed;
        //Serial.println(threatVal);
    }
    Serial.println(threatVal[1]);
    currentTimev = micros();
    double dx = threatVal[i] - prevThreatVal[i];
    pwmValG[i] = ((100-greenValue[i])*.01)*255;
   // Serial.println(threatVal[2]);
    analogWrite(greenPin[i], pwmValG[i]);
    pwmValR[i] = ((100-redValue[i])*.01)*255;
    analogWrite(redPin[i], pwmValR[i]);
  }
}
