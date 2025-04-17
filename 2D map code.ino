
//***********************************************************************************************
// **************************************SISTEMA MASSA MOLA**************************************
//****************************************ARDUINO MEGA 2560**************************************
//*******************************************************************VERSÃO FINAL 07/10/2023*****
//***********************************************************************************************

//******DEFINIÇÕES DE HARDWARE
#define LED  13
#define  AO 0        //CONVERSOR ANALOGICO 
#define RELE1 7      //BOMBEIO
#define RELE2 8      //SERVO MOTOR
#define RELE3 5      //IMAGEM DOIS D
#define RELE4 4      // Freio geral

#include <Servo.h>
#define SERVO   9         // Porta Digital 9 PWM

//*******DEFINIÇÃO DE VARIAVEIS
char Letra [] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};      //CINCO DADOS PODEM SER COLETADOS
int N_Pontos=1000;      //*****
float Tensao_1;
int Iteracoes = 40;
int velocidade = 0;
int i,ii,j,jj,k,zz,zzz,opcao;
long Ti,Tf,Tempo_1;
unsigned long int Tempo;
int Tensao;

int Temp = 100;              
int Teta_i = 5;         //Posição inicial
int Teta_f = 160;        //Posição final
Servo s;                 // Variável Servo
int pos;                 // Posição Servo


           void setup() 
        {
              Serial.begin(56000);
              pinMode(LED, OUTPUT); 
              pinMode(RELE1, OUTPUT); 
              pinMode(RELE2, OUTPUT);
              pinMode(RELE3, OUTPUT);
              pinMode(RELE4, OUTPUT);
              digitalWrite(RELE1, LOW); 
              digitalWrite(RELE4, HIGH);
              analogReference(DEFAULT);
              s.attach(SERVO);
              s.write(Teta_i);
              digitalWrite(RELE2, LOW);  // DESATIVA O RELE DO SERVO MOTOR 
              delay (1000);
        }

void loop() 

  {
                  if (Serial.available()>0)  
                              
               {
                     opcao = Serial.read();    
               }
                 
         switch(opcao)
         
   {

         case 'A': // ********** Tempo Morto ******************
             digitalWrite(RELE2, HIGH);  // DESATIVA O RELE DO SERVO MOTOR
             digitalWrite(RELE4, LOW);  // DESATIVA O RELE DO FREIO
             Serial.println("RESETROW");     
             for( k=0; k <= Iteracoes; k++)
                        {
                          digitalWrite(RELE2, HIGH);  // DESATIVA O RELE DO SERVO MOTOR
                          zz=0;  
                          zzz=0;
                          Teta_i=5;       // Declaramos o valor mínimo de theta
                          Teta_f = 160;  // Declaramos o valor máximo de theta
                          digitalWrite(LED, HIGH);              
                          s.write(Teta_i); // o potenciômetro vai para o valor mínimo de theta
                          delay(100);                           
                          jj=1;              
                          digitalWrite(RELE3, HIGH);  // ATIVA O RELE DO MAPA 2D
                          delay(100);
                          digitalWrite(RELE1, HIGH);
                          delay(50);
                          digitalWrite(RELE1, LOW);  
                          delay(100);      //TEMPO MORTO
                            
                          Ti = millis();
                            for( i=0; i < N_Pontos; i++)
                                      {       
                                      Tensao = (analogRead (A0) - 512);    
                                      Tf = millis();
                                      Tempo = (Tf-Ti);
                                       
                                      if (Tempo > 500*k)
                                      {
                                      digitalWrite(RELE3, LOW);
                                      zz=zz+1;    //Roda o Potenciometro
                                      if(zz>velocidade and Teta_i<=Teta_f) // a cada velocidade iterações, rodamos o potenciômetro.
                                    {  
                                          if (Teta_i >= Teta_f){
                                            digitalWrite(RELE2, LOW);
                                          }
                                          Teta_i=Teta_i+1;
                                          s.write(Teta_i); // o potenciômetro assume a nova posição
                                          zz=0;
                                          zzz=zzz+1;
                                      }
                                    }
                                      delay(40); 
                                      jj=jj+1;
                                      Serial.print("CELL,SET,");
                                      Serial.print (Letra [ii]);
                                      Serial.print (jj);
                                      Serial.print(",");   
                                      Serial.println(Tempo);
                                      

                                      Serial.print("CELL,SET,");
                                      Serial.print (Letra [ii+1]);
                                      Serial.print (jj);
                                      Serial.print(",");
                                      Serial.println(Tensao);
                                  }
                                  ii += 2;
                                  digitalWrite(RELE4, HIGH);  // ATIVA O RELE DO FREIO
                                  delay(5000);
                                  digitalWrite(RELE4, LOW);  // ATIVA O RELE DO FREIO
                                  digitalWrite(LED, LOW);
                        }

         case 'B':  //****Streched Exponential e Gaussiano*******************
                digitalWrite(RELE2, HIGH);  // DESATIVA O RELE DO SERVO MOTOR
             digitalWrite(RELE4, LOW);  // DESATIVA O RELE DO FREIO
             Serial.println("RESETROW");     
             for( k=0; k <= Iteracoes; k++)
                        {  
                          digitalWrite(LED, HIGH);              
                          delay(100);                           
                          jj=1;              
                          digitalWrite(RELE3, HIGH);  // ATIVA O RELE DO MAPA 2D
                          delay(100);
                          digitalWrite(RELE1, HIGH);
                          delay(50);
                          digitalWrite(RELE1, LOW);  
                          delay(100);      //TEMPO MORTO
                            
                          Ti = millis();
                            for( i=0; i < N_Pontos; i++)
                                      {       
                                      Tensao = (analogRead (A0) - 512);    
                                      Tf = millis();
                                      Tempo = (Tf-Ti);
                                       
                                      if (Tempo > 500*k)
                                      {
                                      digitalWrite(RELE3, LOW);
                                    }
                                      delay(40); 
                                      jj=jj+1;
                                      Serial.print("CELL,SET,");
                                      Serial.print (Letra [ii]);
                                      Serial.print (jj);
                                      Serial.print(",");   
                                      Serial.println(Tempo);
                                      

                                      Serial.print("CELL,SET,");
                                      Serial.print (Letra [ii+1]);
                                      Serial.print (jj);
                                      Serial.print(",");
                                      Serial.println(Tensao);
                                  }
                                  ii += 2;
                                  digitalWrite(RELE4, HIGH);  // ATIVA O RELE DO FREIO
                                  delay(5000);
                                  digitalWrite(RELE4, LOW);  // ATIVA O RELE DO FREIO
                                  digitalWrite(LED, LOW);
                        }
                  
         case 'C': // ************* Resistor Fixo ********************


                              
         case 'D':    
                        //************ZERANDO A PLANILHA******************
                        
                        Serial.read();               //********Lendo Lixo
                        digitalWrite(LED, HIGH);
                        Serial.println("RESETROW");  
                        for( i=0; i < N_Pontos; i++)
                       {
                        Serial.print("DATA,");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0);
                        Serial.print(",");
                        Serial.print(0); 
                        Serial.print(",");
                        Serial.println(0);      
                       }
                        Serial.println("RESETROW");
                    
                        jj=1;      //CONTADOR DE LINHAS
                        ii=0;      //CONTADOR DE COLUNAS
                        
                        digitalWrite(LED, LOW);
                       
                        
                        opcao=100;
                        break;
              

      case 'E':
                        //************RECEBENDO PARAMETROS******************
                        Serial.read();     //********Lendo Lixo
                        
                        digitalWrite(LED, HIGH);
                        
                        N_Pontos = Serial.parseInt();  //***Numero de Pontos
                        velocidade = Serial.parseInt();  
                        Iteracoes = Serial.parseInt();
                              
                        digitalWrite(LED, LOW);
                                   
                        for( i=0; i< 5; i++ )    
                            {
                                digitalWrite(LED, HIGH);  
                                delay(200);
                                digitalWrite(LED, LOW);  
                                delay(200);
                            }
                        
                        opcao=100;
                        break;
                      
                   default:
                   delay(1);                
     }

}          



  
       
