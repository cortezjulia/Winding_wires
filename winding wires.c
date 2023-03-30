void verifica_fios(); void verifica_carretel();
volatile int LED_A = 12;
volatile int LED_V = 13;
volatile int i = 0;
volatile int j = 0;
volatile int a = 0;
const unsigned int pin_CARR = 3;
const unsigned int pin_FIO = 2;
int rele = 8;
int tempo_ini = 0;
int START = 0;
int trava = 0;
int para = 0;
int resetar = 0;
int start = 5;
volatile int ciclo = 0;
unsigned long v1, v2, v3;
unsigned long i_ini;
unsigned long trava_start;
#define pin_reset 6
unsigned long au = 0;
//const long intervalo = 25200000;

void setup()
{
  pinMode(start, INPUT_PULLUP);
  pinMode(rele, OUTPUT);
  digitalWrite(rele, 0);
  pinMode(LED_A, OUTPUT);
  pinMode(LED_V, OUTPUT);
  pinMode(pin_CARR, INPUT);
  pinMode(pin_FIO, INPUT);
  digitalWrite(LED_A, LOW); //LED INICIA DESLIGADO
  digitalWrite(LED_V, 1);
  attachInterrupt(digitalPinToInterrupt(pin_FIO), verifica_fios, RISING);
  attachInterrupt(digitalPinToInterrupt(pin_CARR), verifica_carretel, FALLING);
  noInterrupts();

  while (au < 15000000)au++;
}
boolean lastButton = HIGH;     // variavel que guarda o ultimo estado do botao
boolean currentButton = HIGH;    // variavel contendo o atual estado do botao

boolean debounce(boolean last)
{
  boolean current = digitalRead(start);      // le o estado atual do botao
  if (last != current)
  {
    delay(3);     // espera 3ms --> AQUI VAI O "SEGREDO" DO "EFETIVO E SEGURO, ALÉM DE RÁPIDO DEBOUNCING"
    current = digitalRead(start);      // le NOVAMENTE o estado do botao apos o "debounce"
  }
  return current;     // retorna o valor atualizado do botao (button)
}


void loop()
{
  i = 0; j = 0; a = 0; START = 0; trava = 0; para = 0; resetar = 0; ciclo = 0; tempo_ini = 0;

  digitalWrite(LED_A, LOW);
  digitalWrite(LED_V, 1);
  noInterrupts();
  digitalWrite(rele, 1);

  while (trava == 0)
  {
    currentButton = debounce(lastButton);     // Lê o estado do botao após o debounce

    if (lastButton == HIGH && currentButton == LOW)     // se o botao foi PRESSIONADO
    {
      trava = 1;
    }
  }

  START = 1;
  digitalWrite(LED_A, HIGH);
  digitalWrite(LED_V, 0);

  v1 = millis();
  v2 = millis();
  v3 = millis();
  i_ini = millis();
  interrupts();

  while (START == 1)
  {
    v1 = millis();
    v2 = (millis() - v3);

    if ((v2 >= 10000) && (resetar == 1))
    {
      digitalWrite(rele, 0);
      break;
    }
    if ((millis() - i_ini) >= 14400000)
    {
      digitalWrite(rele, 0);
      break;
    }
    if (ciclo == 1)
    {
      para = 1;
      ciclo = 0;
      v3 = v1;
      digitalWrite(LED_A, HIGH);
      if (i == 0)
      {
        j++;
        digitalWrite(LED_A, LOW);
        // digitalWrite(LED_V, 1);
      }
      i = 0;
      a++;
    }

    if ((a == 9) && (j >= 3))
    {
      noInterrupts();
      digitalWrite(LED_A, HIGH);
      digitalWrite(LED_V, 1);
      digitalWrite(rele, 0);
      break;
    }
    else if ((a == 9) && (j < 3))
    {
      noInterrupts();
      resetar = 1;
      j = 0;
      i = 0;
      a = 0;
      ciclo = 0;
      interrupts();
    }

  }
  pinMode(pin_reset, OUTPUT);
  digitalWrite(pin_reset, 0);
}
void verifica_fios()
{
  i = 1;
}
void verifica_carretel()
{
  ciclo = 1;
}