
//***********************************************************************************************
// **************************************SISTEMA MASSA MOLA**************************************
//****************************************ARDUINO MEGA 2560**************************************
//*******************************************************************VERSÃO FINAL 07/09/2023*****
//***********************************************************************************************

//******DEFINIÇÕES DE HARDWARE
#define LED  13
#define  AO 0        //CONVERSOR ANALOGICO 
#define RELE1 7      //BOMBEIO
#define RELE2 8      //SERVO MOTOR
#define RELE3 5      //IMAGEM DOIS D
#define RELE4 4      //FREIO

#include <Servo.h>
#define SERVO   9         // Porta Digital 9 PWM

//*******DEFINIÇÃO DE VARIAVEIS
char Letra [] = {'A','B','C','D','E','F','G','H','I','J','K','L'};      //CINCO DADOS PODEM SER COLETADOS
int N_Pontos=1000;      //*****
int N_Medias=1;        //NUMERO DE MEDIAS
float Tensao_1;
int Nada;
int velocidade = 0;
int GorS = 0;
int i,ii,j,jj,k,zz,zzz,opcao;
long Ti,Tf,Tempo_1;
//long Tempo;
unsigned long int Tempo[1000];
int Tensao[1000];

int Temp = 100;              
int Teta_i = 5;         //Posição inicial
int Teta_f = 160;        //Posição final
Servo s;                 // Variável Servo
int pos;                 // Posição Servo


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//vetor para armazenar as posições de máximos -- se necessário, aumentar o tamanho
int maximos[500] = {};
//contador para armazenar os valores no vetor de máximos
int contador = 0;
//variável que armazena os valores enquanto a verificação do máximo é feita
float maximo;
//variável booleana que é colocada pra true quando houver duas mudanças de sinal, (+ para -) e (- para +)
bool get_maximo = true;//esta variável que faz o controle para verificar o novo máximo


           void setup() 
        {
              Serial.begin(56000);
              Serial.println("LABEL,Tempo,Distancia,Tempo,Distancia");
              pinMode(LED, OUTPUT); 
              pinMode(RELE1, OUTPUT); 
              pinMode(RELE2, OUTPUT);
              pinMode(RELE3, OUTPUT);
              pinMode(RELE4, OUTPUT);
              digitalWrite(RELE1, LOW);
              digitalWrite(RELE4, LOW);
              analogReference(DEFAULT);
              s.attach(SERVO);
              //s.write(Teta_f);
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
             digitalWrite(RELE2, LOW);  // DESATIVA O RELE DO SERVO MOTOR
             jj = 1;
             Serial.read();     //********Lendo Lixo da Porta Serial
             Serial.println("RESETROW");
             Serial.println("LABEL,Time,Signal,Time,Signal,Time,Signal,Time,ln(Signal),Time,ln(Signal),Time,ln(Signal),Time,Signal");
             digitalWrite(LED, HIGH);

             digitalWrite(RELE1, HIGH);
             delay(6);
             digitalWrite(RELE1, LOW);
             delay(5);  
                 
             // delay(95); //TEMPO MORTO DESATIVADO
             
             Ti = millis();
             for( i=0; i < N_Pontos; i++)
                       {
                         Tensao_1 = 0;
                         Tensao_1 = analogRead (A0);
                         delay(2);     
                         Tensao[i] = (Tensao_1 - 512);    
                         Tf = millis();
                         Tempo[i] = (Tf-Ti);
                         jj=jj+1;
                         Serial.print("CELL,SET,");
                         Serial.print ("M");
                         Serial.print (jj);
                         Serial.print(",");   
                         Serial.println(Tempo[i]);
                        

                         Serial.print("CELL,SET,");
                         Serial.print ("N");
                         Serial.print (jj);
                         Serial.print(",");
                         Serial.println(Tensao[i]);  
                    }
                        digitalWrite(RELE4,HIGH);
                        delay(5000);
                        digitalWrite(RELE4, LOW); 
                        digitalWrite(LED, LOW);
                        opcao=100;       
                        break;

        case 'B':  //****Streched Exponential e Gaussiano*******************
/*
 Nessa parte do programa podemos a partir da interface do excel, escolher o valor 0 ou 1 do segundo parâmetro para decidir se
 escolheremos o decaimento exponencial ou gaussiano. Além disso, o terceiro parâmetro decide a velocidade com que o potenciômetro
 vai girar e, portanto, o formato da curva final.
 */
             digitalWrite(RELE2, HIGH);  // ATIVA O RELE DO SERVO MOTOR
        //********* Caso GorS seja 1, o decaimento é streched****************
             if (GorS == 1) {
               Teta_i=5;       // Declaramos o valor mínimo de theta
               Teta_f = 160;  // Declaramos o valor máximo de theta
               Serial.read();     //********Lendo Lixo da Porta Serial
               contador = 0;
               s.write(Teta_i); // o potenciômetro vai para o valor mínimo de theta
               jj=1;  
               zz=0;  
               zzz=0;
               Serial.println("RESETROW");
               
               digitalWrite(LED, HIGH);
               digitalWrite(RELE1, HIGH);
               delay(50);
               digitalWrite(RELE1, LOW);
               
               delay(100);      //TEMPO MORTO
               
               Ti = millis();
               for( i=0; i < N_Pontos; i++)
                         {
                           
                           Tensao_1 = 0;
                           for( j=0; j < N_Medias; j ++)        //não precisa
                                  {
                                      Tensao_1 = Tensao_1 + analogRead (A0);
                                      delay(40);      //NÃO PODE SER MAIOR QUE QUARENTA
                                  }
                          Tensao_1 = (Tensao_1/N_Medias);
                          Tensao[i] = (Tensao_1 - 512);    
                          Tf = millis();
                          Tempo[i] = (Tf-Ti);
  
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
                           
                          jj=jj+1;
                          Serial.print("CELL,SET,");
                          Serial.print (Letra [ii]);
                          Serial.print (jj);
                          Serial.print(",");   
                          Serial.println(Tempo[i]);
                          
  
                          Serial.print("CELL,SET,");
                          Serial.print (Letra [ii+1]);
                          Serial.print (jj);
                          Serial.print(",");
                          Serial.println(Tensao[i]); 
                  
                      }
             }
              //********* Caso GorS seja 0, o decaimento é gaussiano****************
               if (GorS == 0){
                 Teta_i=5;       // Declaramos o valor mínimo de theta
                 Teta_f = 160;  // Declaramos o valor máximo de theta
                 Serial.read();     //********Lendo Lixo da Porta Serial
                 contador = 0;
                 s.write(Teta_f); // O potenciômetro vai para o máximo.
                 jj=1;  
                 zz=0;  
                 zzz=0;
                 Serial.println("RESETROW");     
                 digitalWrite(LED, HIGH);
                 digitalWrite(RELE1, HIGH);
                 delay(50);
                 digitalWrite(RELE1, LOW);
                 
                 delay(100);      //TEMPO MORTO
                 
                 Ti = millis();
                 for( i=0; i < N_Pontos; i++)
                           {
                             
                             Tensao_1 = 0;
                             for( j=0; j < N_Medias; j ++)        //não precisa
                                    {
                                        Tensao_1 = Tensao_1 + analogRead (A0);
                                        delay(40);      //NÃO PODE SER MAIOR QUE QUARENTA
                                    }
                            Tensao_1 = (Tensao_1/N_Medias);
                            Tensao[i] = (Tensao_1 - 512);    
                            Tf = millis();
                            Tempo[i] = (Tf-Ti);
    
                            zz=zz+1;    //Roda o Potenciometro
                            if(zz>velocidade and Teta_f>=Teta_f) // A cada velocidade iterações o potenciômetro gira.
                         {  
                               if (Teta_i >= Teta_f){
                                 digitalWrite(RELE2, LOW);
                              }
                               Teta_f=Teta_f-3; // Damos um passo de 3 graus.
                               s.write(Teta_f); // O potenciômetro assume o novo valor.
                               zz=0;
                               zzz=zzz+1;
                         }
                             
                            jj=jj+1;
                            Serial.print("CELL,SET,");
                            Serial.print (Letra [ii]);
                            Serial.print (jj);
                            Serial.print(",");   
                            Serial.println(Tempo[i]);
                            
    
                            Serial.print("CELL,SET,");
                            Serial.print (Letra [ii+1]);
                            Serial.print (jj);
                            Serial.print(",");
                            Serial.println(Tensao[i]); 
                    
                        }
              
             }


//&&&&&&&&&&&&&&&&&&&&&Calculando os máximos&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
           for( i=0; i < N_Pontos; i++)
    {
           //já começa como true, mas depois, só entra quando houver as duas mudanças de sinal nos valores
        if(get_maximo == true){
          //atribui o valor atual na variavel maximo
          maximo = Tensao[i];
          //if para verificar se o próximo valor é maior que o atual
          if(Tensao[i+1] > maximo){
            //caso seja, coloca o próximo valor na variável maximo
            maximo = Tensao[i+1];
          }
          //caso o próximo valor não seja maior que o atual, armazena a posição do valor atual
          else{
            maximos[contador] = i;
            //foi necessária a variável contador, para ter acesso ao vetor de máximos
            contador++;
            //desativa a verificação de um novo máximo até que seja encontradas as duas mudanças de sinal
            get_maximo = false;
          }
        }
        //if que faz a verificação das duas mudanças de sinal
        //primeiro verifica se o valor atual é negativo
        if (Tensao[i]<0){
          //se for, faz a verificação se o próximo valor é positivo
          if(Tensao[i+1] > 0){
            //caso tenha as duas mudanças de sinal, ativa a verificação do novo máximo
            get_maximo = true;
          }
        }
      }
                    
                   
      jj=1;      //agora**********************
      for (j = 0; j < contador; j++)     
                {

                        jj=jj+1;          //agora
                        int jjj = maximos[j];
                        Serial.print("CELL,SET,");
                        Serial.print (Letra [ii+6]);
                        Serial.print (jj);
                        Serial.print(",");                           //agora
                        Serial.println(Tempo[jjj]);

                        Serial.print("CELL,SET,");                   //agora
                        Serial.print (Letra [ii+7]);
                        Serial.print (jj);
                        Serial.print(",");
                        Serial.println(Tensao[jjj]);                   //agora   

                }                       
     
                         ii=ii+2;                       //CONTADOR DE COLUNAS
                         if(ii>5)
                        {
                          ii=0;
                        }
                        digitalWrite(RELE4,HIGH);
                        delay(5000);
                        digitalWrite(RELE4, LOW); 
                        digitalWrite(LED, LOW);
                        opcao=100;       
                        break;
                  
         case 'C': // ************* Resistor Fixo ********************
             digitalWrite(RELE2, LOW);  // DESATIVA O RELE DO SERVO MOTOR
             Serial.read();     //********Lendo Lixo da Porta Serial
             contador = 0;
             jj=1;  
             Serial.println("RESETROW");
             
             digitalWrite(LED, HIGH);     //LIGA O LED SINALIZADO.
             digitalWrite(RELE1, HIGH);
             delay(50);                   //BOMBEIO INICIAL
             digitalWrite(RELE1, LOW);
             
             delay(100);                  //TEMPO MORTO
             
             Ti = millis();
             for( i=0; i < N_Pontos; i++)
                {
                        Tensao_1 = analogRead (A0);
                        Tensao[i] = (Tensao_1 - 512);    //FAZ OSCILAR EM TORNO DO ZERO
                        delay(40);
                        Tf = millis();
                        Tempo[i] = (Tf-Ti);
                         
                        jj=jj+1;
                        Serial.print("CELL,SET,");
                        Serial.print (Letra [ii]);
                        Serial.print (jj);
                        Serial.print(",");   
                        Serial.println(Tempo[i]);
                        

                        Serial.print("CELL,SET,");
                        Serial.print (Letra [ii+1]);
                        Serial.print (jj);
                        Serial.print(",");
                        Serial.println(Tensao[i]); 
                
                    }

//***************************DETERMINAÇÃO DOS PONTOS DE MAXIMO**************************
           for( i=0; i < N_Pontos; i++)
    {
           //já começa como true, mas depois, só entra quando houver as duas mudanças de sinal nos valores
        if(get_maximo == true){
          //atribui o valor atual na variavel maximo
          maximo = Tensao[i];
          //if para verificar se o próximo valor é maior que o atual
          if(Tensao[i+1] > maximo){
            //caso seja, coloca o próximo valor na variável maximo
            maximo = Tensao[i+1];
          }
          //caso o próximo valor não seja maior que o atual, armazena a posição do valor atual
          else{
            maximos[contador] = i;
            //foi necessária a variável contador, para ter acesso ao vetor de máximos
            contador++;
            //desativa a verificação de um novo máximo até que seja encontradas as duas mudanças de sinal
            get_maximo = false;
          }
        }
        //if que faz a verificação das duas mudanças de sinal
        //primeiro verifica se o valor atual é negativo
        if (Tensao[i]<0){
          //se for, faz a verificação se o próximo valor é positivo
          if(Tensao[i+1] > 0){
            //caso tenha as duas mudanças de sinal, ativa a verificação do novo máximo
            get_maximo = true;
          }
        }
      }
                    
                   
      jj=1;              //*************ESCREVE NAS COLUNAS OS PONTOS DE MAXIMO
      for (j = 0; j < contador; j++)     
                {

                        jj=jj+1;          //agora
                        int jjj = maximos[j];
                        Serial.print("CELL,SET,");
                        Serial.print (Letra [ii+6]);
                        Serial.print (jj);
                        Serial.print(",");                           
                        Serial.println(Tempo[jjj]);

                        Serial.print("CELL,SET,");                   
                        Serial.print (Letra [ii+7]);
                        Serial.print (jj);
                        Serial.print(",");
                        Serial.println(Tensao[jjj]);                   

                }                       
     
                         ii=ii+2;                       //CONTADOR DE COLUNAS
                         if(ii>5)
                        {
                          ii=0;
                        }
                        digitalWrite(RELE4,HIGH);
                        delay(5000);
                        digitalWrite(RELE4, LOW); 
                        digitalWrite(LED, LOW);
                        opcao=100;       
                        break;
                        
                              
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
                        GorS = Serial.parseInt();  //***Numero de Medias
                        velocidade = Serial.parseInt();  
                        Nada = Serial.parseInt();
                              
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



  
       
