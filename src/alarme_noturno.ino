#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(32, 16, 2);
int interruptor;
bool mensagem1 = false;
bool mensagem2 = false;


// Controle do alarme atual
int alarmeAtual = 0; // 0: nenhum, 1: Janela, 2: Porta, 3: Garagem, 4: Sensor de Luz

void setup() {
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  
  pinMode(13, OUTPUT); // ALARME
  pinMode(9, OUTPUT);  // LED VERDE
  pinMode(8, OUTPUT);  // LED VERMELHO
  pinMode(2, INPUT);   // INTERRUPTOR
  pinMode(5, INPUT);   // JANELA
  pinMode(4, INPUT);   // PORTA
  pinMode(3, INPUT);   // GARAGEM
  pinMode(A0, INPUT);  // SENSOR DE LUZ
}

void loop() {
  interruptor = digitalRead(2);

  if (interruptor == LOW) {
    digitalWrite(9, HIGH); // Liga o LED verde
    if(!mensagem1){
      lcd.setBacklight(HIGH);
      lcd.print("Sistema Ativo");
      delay(3000);
      mensagem1 = true;
       mensagem2 = false;
      lcd.clear();
   
    }
    
    int novoAlarme = 0; // 0: nenhum

    // Verifica qual botão foi acionado
    if (digitalRead(5) == HIGH) {
      novoAlarme = 1;
      lcd.clear();
      lcd.print("Janela Aberta");
    } else if (digitalRead(4) == HIGH) {
      novoAlarme = 2;
      lcd.clear();
      lcd.print("Porta Aberta");
    } else if (digitalRead(3) == HIGH) {
      novoAlarme = 3;
      lcd.clear();
      lcd.print("Garagem Aberta");
    } else if (analogRead(A0) >= 500) {
      novoAlarme = 4;
      lcd.clear();
      lcd.print("Sensor de Luz"); //Alarme para quando o sistema noturno detecta uma luz proximo ao hardware
    }

    // Verifica quando um novo botão é acionado, assim o anterior não fica travado
    if (novoAlarme != 0 && novoAlarme != alarmeAtual) {
      pararAlarme(); // Para o alarme anterior
      alarmeAtual = novoAlarme; // Atualiza o alarme atual
    }

    // Toca o alarme correspondente
    if (alarmeAtual == 1) {
      alarmeJanela();
    } else if (alarmeAtual == 2) {
      alarmePorta();
    } else if (alarmeAtual == 3) {
      alarmeGaragem();
    } else if (alarmeAtual == 4) {
      alarmeSensorLuz();
    }

  } else {
    // Desliga tudo ao desligar o sistema
    if(!mensagem2){
      lcd.setBacklight(HIGH);
      lcd.print("Desativando");
      delay(3000);
      mensagem2 = true;
      mensagem1 = false;
      lcd.clear();
   
    }
    digitalWrite(9, LOW);
    noTone(13);
    digitalWrite(8, LOW);
    lcd.setBacklight(LOW);
    lcd.clear();
    pararAlarme(); // Para qualquer alarme ativo
    alarmeAtual = 0; // Libera para novo disparo
  }
}

// Função para parar o alarme atual
void pararAlarme() {
  noTone(13);
  digitalWrite(8, LOW);
}

// Alarmes contínuos
void alarmeJanela() {
  tone(13, 1000);
  digitalWrite(8, HIGH);
  delay(200);
  noTone(13);
  digitalWrite(8, LOW);
  delay(200);
}

void alarmePorta() {
  tone(13, 800);
  digitalWrite(8, HIGH);
  delay(400);
  noTone(13);
  digitalWrite(8, LOW);
  delay(200);
}

void alarmeGaragem() {
  tone(13, 600);
  digitalWrite(8, HIGH);
  delay(150);
  noTone(13);
  digitalWrite(8, LOW);
  delay(150);
}

void alarmeSensorLuz() {
  tone(13, 900);
  digitalWrite(8, HIGH);
  delay(1000);
  noTone(13);
  digitalWrite(8, LOW);
  delay(500);
}