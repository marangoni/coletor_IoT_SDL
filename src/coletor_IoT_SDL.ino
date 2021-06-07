/*
 * Project coletor_IoT_SDL
 * Description: Projeto Particle com SDL - testes iniciais
 * Author: Luiz C M Oliveira
 * Date: 31.05.2021
 */

//Processos
void TIMER_10MS();
void TIMER_1S();
void TIMER_5S();
void LED_YELLOW();
void LED_BLUEE();
void BOTAO();
void FF_1S();

//Estados
enum estado_TIMER_10MS {
    ESTADO_T10MS_WAIT = 0,
    ESTADO_T10MS_TIMING
};

enum estado_TIMER_1S {
    ESTADO_T1S_WAIT = 0,
    ESTADO_T1S_TIMING    
};

enum estado_TIMER_5S {
    ESTADO_T5S_WAIT = 0,
    ESTADO_T5S_TIMING
};

enum estado_LED_YELLOW {
    ESTADO_LED_YELLOW_ON = 0,
    ESTADO_LED_YELLOW_OFF
};

enum estado_LED_BLUE {
    ESTADO_LED_BLUE_OFF = 0,
    ESTADO_LED_BLUE_ON
};

enum estado_BOTAO {
    ESTADO_BOTAO_NOT_PRESSED = 0,
    ESTADO_BOTAO_PRESSED
};

enum estado_FF_1S {
    ESTADO_FF_1S_OFF = 0,
    ESTADO_FF_1S_ON
};

int estado_TIMER_1S=0;
int estado_TIMER_5S=0;
int estado_BOTAO=0;
int estado_LED_BLUE=0;
int estado_LED_YELLOW=0;
int estado_TIMER_10MS=0;
int estado_FF_1S=0;


//Sinais
bool sinal_start_T10MS = FALSE;
bool sinal_end_T10MS = FALSE;

bool sinal_start_T1S = FALSE;
bool sinal_end_T1S = FALSE;

bool sinal_start_T5S = FALSE;
bool sinal_end_T5S = FALSE;

bool sinal_liga_LED_Y = FALSE;
bool sinal_desliga_LED_Y = FALSE;

bool sinal_liga_LED_B = FALSE;
bool sinal_desliga_LED_B = FALSE;

bool sinal_botao_ON = FALSE;

bool sinal_FF_1S_START = FALSE;
bool sinal_FF_1S_STOP = FALSE;

//Variáveis globais
unsigned long timer10ms=0;
unsigned long timer1s=0;
bool led5 = FALSE;


void setup() {
  // Put initialization like pinMode and begin functions here.
  pinMode(D5,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D2,INPUT);
}


void loop() {
  
  //Chama os processos
  BOTAO();
  TIMER_10MS();
  TIMER_1S();
  TIMER_5S();
  LED_YELLOW();
  LED_BLUEE();
  FF_1S();
}

//Implementacao dos processos

void BOTAO(){

  switch(estado_BOTAO)
  {
    case ESTADO_BOTAO_NOT_PRESSED:
      //Se botão não pressionado, teste se foi pressionado
      sinal_FF_1S_STOP = TRUE; //envia sinal para que o led pare de piscar
      if(digitalRead(D2)==HIGH){
        estado_BOTAO = ESTADO_BOTAO_PRESSED;  //Se pressionado vai para próximo estado  
      } 
    break;

    case ESTADO_BOTAO_PRESSED:
      //Se botao pressionado, envia sinal para início da temporização de 50MS
      sinal_start_T10MS = TRUE; //envia sinal para inicializar a temporização 50MS
      
       if(sinal_end_T10MS == TRUE){
         sinal_end_T10MS = FALSE; //consome sinal
         sinal_FF_1S_START = TRUE; //envia sinal para que o led pisque
         if(digitalRead(D2)==LOW){                  //verifica se o botao foi liberado
          estado_BOTAO = ESTADO_BOTAO_NOT_PRESSED;  // volta para estado 0
         }
       }
    break;
  } 
}

void TIMER_10MS(){
  switch(estado_TIMER_10MS)
  {
    case ESTADO_T10MS_WAIT:
      if(sinal_start_T10MS==TRUE){
        sinal_start_T10MS = FALSE;      // Consome sinal
        timer10ms = millis();           //inicializa temporizador
        estado_TIMER_10MS = ESTADO_T10MS_TIMING;
      }
    break;

     case ESTADO_T10MS_TIMING:
      if(millis() - timer10ms >10){             //Testa se já passaram 50ms
        sinal_end_T10MS = TRUE;                 //envia sinal fim de temporizacao 50ms
        estado_TIMER_10MS = ESTADO_T10MS_WAIT;  //volta para estado inicial
        timer10ms = 0;                          //zera timer50ms
      }
     break;       
  }
  
    
}

void TIMER_1S(){
   switch(estado_TIMER_1S)
  {
    case ESTADO_T1S_WAIT:
      if(sinal_start_T1S==TRUE){
        sinal_start_T1S = FALSE;              // Consome sinal
        timer1s = millis();                   //inicializa temporizador
        estado_TIMER_1S = ESTADO_T1S_TIMING;  //muda de estado
      }
    break;

     case ESTADO_T1S_TIMING:
      if(millis() - timer1s >1000){             //Testa se já passaram 1s
        sinal_end_T1S = TRUE;                 //envia sinal fim de temporizacao 1s
        estado_TIMER_1S = ESTADO_T1S_WAIT;    //volta para estado inicial
        timer1s = 0;                          //zera timer50ms
      }
     break;       
  }

}

void TIMER_5S(){
    
}

void LED_YELLOW(){
    
}

void LED_BLUEE(){
  switch(estado_LED_BLUE){
    
    case ESTADO_LED_BLUE_OFF:
      digitalWrite(D5,LOW);                   //apaga led
      if(sinal_liga_LED_B==TRUE){             //verifica se recebeu sinal para acender
        sinal_liga_LED_B = FALSE;             //consome sinal
        estado_LED_BLUE = ESTADO_LED_BLUE_ON;  //muda de estado
      }
    break;
    case ESTADO_LED_BLUE_ON:
      digitalWrite(D5,HIGH);                    //acende led
      if(sinal_desliga_LED_B==TRUE){            //verifica se recebeu sinal para apagar
        sinal_desliga_LED_B = FALSE;            //consome sinal
        estado_LED_BLUE = ESTADO_LED_BLUE_OFF;  //muda de estado
      }    
    break;
  }  
}

void FF_1S(){
  
   switch(estado_FF_1S){
    
    case ESTADO_FF_1S_ON:
     
     if(sinal_liga_LED_B == TRUE){
       sinal_liga_LED_B = FALSE;
       sinal_desliga_LED_B = TRUE;
     }                                  //inverte status led
     sinal_start_T1S = TRUE;            //inicializa timer 1S

     if(sinal_end_T1S==TRUE){           //quando terminar a temporizacao
       sinal_end_T1S = FALSE;           //consome sinal
       
       if(sinal_liga_LED_B==FALSE){     //inverte status led
         sinal_liga_LED_B = TRUE;
         sinal_desliga_LED_B = FALSE;
       }
      }

     if(sinal_FF_1S_STOP==TRUE){        //recebeu sinal de stop?
       sinal_FF_1S_STOP = FALSE;        //consome sinal
       estado_FF_1S = ESTADO_FF_1S_OFF; //muda de estado
     }
  
    break;

    case ESTADO_FF_1S_OFF:
        //nesse estado led não pisca. Fica desligado
        sinal_desliga_LED_B = TRUE;

        if(sinal_FF_1S_START == TRUE){    //recebeu sinal de start?
          sinal_FF_1S_START = FALSE;      //consome o sinal
          estado_FF_1S = ESTADO_FF_1S_ON; //muda de estado
        }
    break;
  }
}
