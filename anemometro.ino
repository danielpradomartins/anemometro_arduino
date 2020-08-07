#include <SPI.h>
#include <Ethernet2.h>

//------------------------------------------
const int buttonPin = 2;

const float Raio = 0.147; // Raio em metros
float v[60];
float v5;
float v3;
float v2;
float soma;

int delayTime = 50;
unsigned long periodo = 1.0*60.0;
unsigned long start;


int pushButtonCounter = 0;
int buttonState = 0;
int lastButtonState = 0;
//-----------------------------------------

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 15); // Endereço IP que a Ethernet Shield terá. Deve ser alterado para um endereço livre da sua rede.
EthernetServer server(80);     // Cria um servidor WEB

void setup() {
//------------------------------------------------
  for(int i = 0; i < 60; i++){
       v[i] = 0;
   }
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
//--------------------------------------------------  
  
    Ethernet.begin(mac, ip);  // Inicializa a Ethernet Shield
    server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
} // fim do setup

void loop() {
// ================================================================
          start = millis();
          while( (unsigned long)(millis()/1000.0) <= (unsigned long)(start/1000LL + periodo) ){






  // ===============Client===============================

    EthernetClient client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)

    if (client) {  // Existe um cliente em conexão ?
      
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // os dados do cliente estão disponiveis para serem lidos
                char c = client.read(); // lê 1 byte (character) do cliente
                
                // a ultima linha da requisição do cliente é branca e termina com o caractere \n
                // responde para o cliente apenas após a última linha recebida
                if (c == '\n' && currentLineIsBlank) {
                  
                    // envia o cabeçalho de uma resposta http padrão
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    
                    // ENVIA A PÁGINA WEB
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Vento Praia do Flamengo(SSA)</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h2>Velocidade m&eacute;dia do vento na praia do Flamengo (SSA) nas &uacute;ltimas horas</h2>");
 

              client.print("Velocidade m&eacute;dia dos &uacuteltimos 2 minutos ");            
              client.print(v2);
              client.println(" n&oacutes");
              client.print("<br/>");

                    
              client.print("Velocidade m&eacute;dia dos &uacuteltimos 3 minutos ");            
              client.print(v3);
              client.println(" n&oacutes");
              client.print("<br/>");


              client.print("Velocidade m&eacute;dia dos &uacuteltimos 5 minutos ");            
              client.print(v5);
              client.println(" n&oacutes");
              client.print("<br/>");

// =====================================================
                   
                    client.println("</body>");
                    client.println("</html>");
                    break;
                }
                // toda linha de texto recebida do cliente termina com os caracteres \r\n
                if (c == '\n') {
                    // ultimo caractere da linha do texto recebido
                    // iniciando nova linha com o novo caractere lido
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // um caractere de texto foi recebido do cliente
                    currentLineIsBlank = false;
                }
            } // fim do if (client.available())
        } // fim do while (client.connected())
        
        delay(1);      // da um tempo para o WEB Browser receber o texto
        client.stop(); // termina a conexão
        
    } // fim do if (client)
//=============fim Client===================

            
            
              
              buttonState = digitalRead(buttonPin);
            
            
              if(buttonState != lastButtonState){
                if(buttonState == HIGH){
                  pushButtonCounter++;
                  Serial.print("Número de pulsos");
                  Serial.println(pushButtonCounter);
                  delay(delayTime);
            
                }
              }
            
              lastButtonState = buttonState;
              
        
          }
          
          for(int i = 0; i < (60-1); i++){
            v[i] = v[i+1];
          }
//          velocidade(pushButtonCounter, periodo); 
            v[59] = 2*PI*pushButtonCounter*Raio/periodo;
            v[59] = v[59]*1.94384; // Converter para nós
            
            Serial.print("Velocidade média do último ");
            Serial.print(60-59);
            Serial.print(" minuto: ");
            Serial.print(v[59]);
            Serial.println(" nós");

          //Para 2 minutos
          soma = 0;
          for(int i = 60-2; i < 60; i++){
              soma = soma + v[i];
          }
              v2 = soma/2;
              Serial.print("Velocidade média dos últimos 2 minutos");            
              Serial.print(v2);
              Serial.println(" nós");

          //Para 3 minutos
          soma = 0;
          for(int i = 60-3; i < 60; i++){
              soma = soma + v[i];
          }
              v3 = soma/3;
              Serial.print("Velocidade média dos últimos 3 minutos");            
              Serial.print(v3);
              Serial.println(" nós");

           //Para 5 minutos
          soma = 0;
          for(int i = 60-5; i < 60; i++){
              soma = soma + v[i];
          }
              v5 = soma/5;              
              Serial.print("Velocidade média dos últimos 5 minutos");            
              Serial.print(v5);
              Serial.println(" nós");

          pushButtonCounter = 0;

  // ==================================================== 

//======================================
} // fim do loop
