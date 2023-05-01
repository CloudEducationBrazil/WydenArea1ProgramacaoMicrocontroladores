//-------- RASTREAMENTO DE EQUIPAMENTOS -----------//
//------- DE PROTEÇÃO INDIVIDUAL POR RFID ---------//
//=================================================//
//=================================================//




#include <SPI.h>
#include <MFRC522.h>          // biblioteca sensor RFID//
#include <Servo.h>            // biblioteca servo motor//
#include <LiquidCrystal.h>    // biblioteca Displey LCD//

Servo myservo;  // create servo object to control a servo

const int rs = A0, en = A1, d4 =A2 , d5 = A3, d6 = A4, d7 = A5; //Pinos do Displey//
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

//Pinos
#define LED_VERDE 6
#define LED_VERMELHO 7
#define BUZZER 8
#define SS_PIN 10
#define RST_PIN 9


int PIR = 3;    // sensor de presnça

String IDtag = ""; //Variável que armazenará o ID da Tag
bool Permitido = false; //Variável que verifica a permissão 

//Vetor responsável por armazenar os ID's das Tag's cadastradas
String TagsCadastradas[] = {"35b5fbc2", 
                            "35b1f745",
                            "ID_3"};

MFRC522 LeitorRFID(SS_PIN, RST_PIN);    // Cria uma nova instância para o leitor e passa os pinos como parâmetro


void setup() {
        Serial.begin(9600);             // Inicializa a comunicação Serial
        SPI.begin();                    // Inicializa comunicacao SPI 
        LeitorRFID.PCD_Init();          // Inicializa o leitor RFID
        pinMode(LED_VERDE, OUTPUT);     // Declara o pino do led verde como saída
        pinMode(LED_VERMELHO, OUTPUT);  // Declara o pino do led vermelho como saída
        pinMode(BUZZER, OUTPUT);        // Declara o pino do buzzer como saída
       pinMode(PIR, INPUT);             // Declara o pino do Sensor de presença como entrada
        myservo.attach(2);              // attaches the servo on pin 9 to the servo object
        myservo.write(0);               //Servo motor na posição inicial

       lcd.begin(16, 2);                //Define o número de colunas e linhas do LCD
       
}

void loop() {           //Escreve no LCD
  lcd.clear();
   lcd.print(" Aproxima-se do");  
   lcd.setCursor(0, 3);
  lcd.print("  leitor RFID");
  lcd.setCursor(0, 0);
  lcd.noDisplay();
  delay(500);
  lcd.display();
  delay(500);
  
  
   if (digitalRead (PIR)==1 ){                     //Comando para fechar o portão, após ter permição de entrada
lcd.clear();
lcd.print("Fechando portao!");  myservo.write(0);   // Escreve no LCD
 lcd.setCursor(5, 1);
 lcd.noDisplay();  digitalWrite(LED_VERMELHO, HIGH);    
  delay(900);
  lcd.display();  digitalWrite(LED_VERMELHO, LOW);
  delay(900);
    digitalWrite(LED_VERMELHO, HIGH);
    delay(200);
    digitalWrite(LED_VERMELHO,LOW);
    
    
  }  
  Leitura();  //Chama a função responsável por fazer a leitura das Tag's
}

void Leitura(){

        IDtag = ""; //Inicialmente IDtag deve estar vazia.
        
        // Verifica se existe uma Tag presente
        if ( !LeitorRFID.PICC_IsNewCardPresent() || !LeitorRFID.PICC_ReadCardSerial() ) {
            delay(50);
            return;
        }
        
        // Pega o ID da Tag através da função LeitorRFID.uid e Armazena o ID na variável IDtag        
        for (byte i = 0; i < LeitorRFID.uid.size; i++) {        
            IDtag.concat(String(LeitorRFID.uid.uidByte[i], HEX));
        }        
        
        //Compara o valor do ID lido com os IDs armazenados no vetor TagsCadastradas[]
        for (int i = 0; i < (sizeof(TagsCadastradas)/sizeof(String)); i++) {
          if(  IDtag.equalsIgnoreCase(TagsCadastradas[i])  ){
              Permitido = true; //Variável Permitido assume valor verdadeiro caso o ID Lido esteja cadastrado
          }
        }       

        if(Permitido == true) acessoLiberado(); //Se a variável Permitido for verdadeira será chamada a função acessoLiberado()        
        else acessoNegado(); //Se não será chamada a função acessoNegado()

        delay(2000); //aguarda 2 segundos para efetuar uma nova leitura
}

void acessoLiberado(){
  Serial.println("Tag Cadastrada: " + IDtag); //Exibe a mensagem "Tag Cadastrada" e o ID da tag não cadastrada
    efeitoPermitido();  //Chama a função efeitoPermitido()
    Permitido = false;  //Seta a variável Permitido como false novamente
}

void acessoNegado(){
  Serial.println("Tag NAO Cadastrada: " + IDtag); //Exibe a mensagem "Tag NAO Cadastrada" e o ID da tag cadastrada
  efeitoNegado(); //Chama a função efeitoNegado()
}

void efeitoPermitido(){    

lcd.clear();
lcd.print("    Acesso");         //Escreve a permissão de entrada
lcd.setCursor(0,3);
lcd.print("   Permitido!");
lcd.setCursor(0,0);
  lcd.display();
  delay(900);
      
  int qtd_bips = 2;              //definindo a quantidade de bips
  for(int j=0; j<qtd_bips; j++){    
    //Ligando o buzzer com uma frequência de 1500 hz e ligando o led verde.
    tone(BUZZER,3000);             myservo.write(85);
    digitalWrite(LED_VERDE, HIGH);   
    delay(90);

                  // tell servo to go to position in variable 'pos'
   
    lcd.clear();
    //Desligando o buzzer e led verde.      
    noTone(BUZZER);                      
    digitalWrite(LED_VERDE, LOW);        
    delay(100);         lcd.print("    Acesso");
 
 lcd.setCursor(0,3);
lcd.print("   Permitido!");
lcd.setCursor(0,0);

  }
   
}
                                                                                                //Não utilizado//
void efeitoNegado(){                                                
  int qtd_bips = 1;  //definindo a quantidade de bips
  for(int j=0; j<qtd_bips; j++){   
    //Ligando o buzzer com uma frequência de 500 hz e ligando o led vermelho.
    tone(BUZZER,100);
    digitalWrite(LED_VERMELHO, HIGH);   
    delay(500); 
    myservo.write(85);              // tell servo to go to position in variable 'pos'
    delay(15); 
    //Desligando o buzzer e o led vermelho.
    noTone(BUZZER);
    digitalWrite(LED_VERMELHO, LOW);
    delay(500);
  } 
  
  }
