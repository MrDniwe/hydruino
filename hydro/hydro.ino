#define LOWLEVEL 7
#define MAXLEVEL 6
#define FLOODLEVEL 4
#define LEDPIN 13
#define PUMPPIN 12
#define POTPIN A3
long previousMs = 0;
unsigned long potpinData = 0;
unsigned long interval; //2 часа 7200000 - 15 мин 900000
bool lowLevelState;
bool maxLevelState;
bool floodLevelState;
long prevTimestamp;
bool isLed;
long prevBlinkTimestamp = 0;
bool needToPump;

void setup() {
  pinMode(LOWLEVEL, INPUT_PULLUP);
  pinMode(MAXLEVEL, INPUT_PULLUP);
  pinMode(FLOODLEVEL, INPUT_PULLUP);
  pinMode(LEDPIN, OUTPUT);
  pinMode(PUMPPIN, OUTPUT);
  prevTimestamp = millis();
  needToPump = 1;
}

void loop() {
  potpinData = analogRead(POTPIN);
  interval = 7200000 - (potpinData * 6158);
  lowLevelState = digitalRead(LOWLEVEL);
  maxLevelState = digitalRead(MAXLEVEL);
  floodLevelState = digitalRead(FLOODLEVEL);
      
  // если на критическом уровне - не стартуем пока не будем чуть выше 
  if (!lowLevelState) {
    needToPump = 0;
    while(!digitalRead(LOWLEVEL)) {
      digitalWrite(LEDPIN, HIGH);
      digitalWrite(PUMPPIN, LOW);
    }
  }
  digitalWrite(LEDPIN, LOW);

  // если время пришло - включаем
  if ((millis() - prevTimestamp) > interval) {
    needToPump = 1;
    prevTimestamp = millis();
  }
  
  // если полный горшок - выключаем
  if (floodLevelState) {
    digitalWrite(PUMPPIN, LOW);
    needToPump = 0;
  }

  if (needToPump) {
    digitalWrite(PUMPPIN, HIGH);
  } else {
    digitalWrite(PUMPPIN, LOW);
  }


  // Мигаем диодом пока не залит на рабочий уровень
  if (!maxLevelState) {
    Alarm();
  }

  delay(100);
}

// Функция мигания светодиодом
void Alarm () {
  if (millis() - prevBlinkTimestamp > 300) {
    prevBlinkTimestamp = millis(); 
    if (!isLed) {
      isLed = 1;
    } else {
      isLed = 0;
    }
    digitalWrite(LEDPIN, isLed);
  }
}
