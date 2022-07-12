#include <MyStepper.h>
#include <EEPROM.h>
#include <Arduino_JSON.h>

#define EEPROM_SIZE 128
#define X_SENSOR_INICIAL 14
#define X_SENSOR_FINAL 12
#define MAX_DISTANCE 10000

// Variáveis
String msg = "";

//Prototipos
void callback(char *topic, byte * payload, unsigned int length);
int read_distance();

struct Configuration {
  int initialStep = 1;
  int maxX = 0;
  int microStep = 1;
  int stepRevolution = 200;
  float distance = 5;
  int speed = 25;
  int acceleration = 150;
};

Configuration confValues;
boolean infoCompleta = false;
char fimInformacao = '*';
boolean calibracao = false;
boolean configuracao = false;
int deslocamentoX = 0;
int maxStep = 0;
bool movendo = false;

MyStepper stepperX(AccelStepper::DRIVER, 2, 0); // Defaults to AccelStepper::DRIVER (2 pins) on 2, 3

void setup() {
  Serial.begin(9600);

  pinMode(X_SENSOR_INICIAL, INPUT);
  pinMode(X_SENSOR_FINAL, INPUT);

  EEPROM.begin(EEPROM_SIZE);//Inicia a EEPROM com o tamanho da informação.
  //gravaEEPROM();
  getEEPROM();

  stepperX.setMicroStep(confValues.microStep);
  stepperX.setStepsPerRevolution(confValues.stepRevolution);
  stepperX.setAccelerationDistance(confValues.acceleration);
  stepperX.setMaxSpeedDistance(confValues.speed);
}

void loop() {

  ESPserialEvent();

  if (calibracao) {
    if (digitalRead(X_SENSOR_FINAL) == HIGH) {
      stepperX.setCurrentPosition(0);
      stepperX.moveToDistance(-MAX_DISTANCE);
    }
    if (digitalRead(X_SENSOR_INICIAL) == HIGH) {
      Serial.println("Motor parado");
      stepperX.stop();
      maxStep = stepperX.getCurrentPositionDistance() * -1;
      Serial.print("Total percurso: ");
      Serial.println(maxStep);
      stepperX.setCurrentPosition(0);
      stepperX.runToNewDistance(confValues.initialStep + 20);
      stepperX.setCurrentPosition(0);
      calibracao = false;
    }
  }
  if (infoCompleta) {
    if ((deslocamentoX >= 0 || calibracao) && !configuracao) {
      if (calibracao) {
        Serial.println("Calibrando");
        stepperX.moveToDistance(MAX_DISTANCE);
      }
      else
      {
        if (deslocamentoX > maxStep) {
          deslocamentoX = maxStep;
        }
        movendo = true;
        stepperX.moveToDistance(deslocamentoX);
      }
    }
    infoCompleta = false;
    configuracao = false;
  }
  if (stepperX.distanceToGo() != 0) {
    stepperX.run();
  }

  if (stepperX.distanceToGo() == 0 && movendo) {
    Serial.println("ok");
    movendo = false;
  }
}

void ESPserialEvent() {
  int value;
  while (Serial.available()) {
    char c = Serial.read();
    Serial.flush();
    switch (c) {
      case 'm':
        stepperX.setMicroStep(Serial.parseInt());
        break;
      case 'd':
        stepperX.setDistancePerRevolution(Serial.parseFloat());
        break;
      case 'r':
        stepperX.setStepsPerRevolution(Serial.parseInt());
        break;
      case 'a':
        confValues.acceleration = Serial.parseInt();
        stepperX.setAccelerationDistance(confValues.acceleration);
        break;
      case 's':
        confValues.speed = Serial.parseFloat();
        stepperX.setMaxSpeedDistance(confValues.speed);
        stepperX.setSpeedDistance(confValues.speed);
        break;
      case 'x':
        value = Serial.parseInt();
        Serial.println(value);
        deslocamentoX += value;
        break;
      case 'c':
        calibracao = true;
        break;
      case 'h':
        deslocamentoX = 0;
        break;
      case 'l':
        configuracao = true;
        JSONVar info;
        info["initialStep"] = confValues.initialStep;
        info["maxX"] = confValues.maxX;
        info["microStep"] = confValues.microStep;
        info["stepRevolution"] = confValues.stepRevolution;
        info["distance"] = confValues.distance;
        info["speed"] = confValues.speed;
        info["acceletation"] = confValues.acceleration;
        Serial.println("Configurar");
        Serial.println(info);
        break;
    }
    infoCompleta = (c == fimInformacao) || (c == '\n');
  }
}

void getEEPROM() {
  EEPROM.get(0, confValues);
  if (confValues.initialStep == -1) {
    confValues.initialStep = 1;
    confValues.maxX = 0;
    confValues.microStep = 16;
    confValues.stepRevolution = 200;
    confValues.distance = 8;
    confValues.speed = 150;
    confValues.acceleration = 750;
    gravaEEPROM();
  }
  mostrarEEPROM("Lendo EEPROM");
}

void gravaEEPROM() {
  EEPROM.put(0, confValues);
  EEPROM.commit();//Salva o dado na EEPROM.
  //mostrarEEPROM("Gravando EEPROM");
}

void mostrarEEPROM (String titulo) {
  Serial.println(titulo);
  Serial.print("initialStep = ");
  Serial.print(confValues.initialStep);
  Serial.print(" maxX = ");
  Serial.print(confValues.maxX);
  Serial.print(" (microStep= ");
  Serial.print(confValues.microStep);
  Serial.print(", stepRevolution = ");
  Serial.print(confValues.stepRevolution);
  Serial.print(", distance= ");
  Serial.print(confValues.distance);
  Serial.print("), speed = ");
  Serial.print(confValues.speed);
  Serial.print(", acceleration = ");
  Serial.println(confValues.acceleration);
}

void callback(char *topic, byte * payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  msg = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
    char c = (char)payload[i];
    msg += c;
  }
  Serial.println();
}

int read_distance() {
  String distance = "";
  for (int i = 1; i < msg.length(); i++) {
    distance += msg[i];
  }
  return distance.toInt();
}
