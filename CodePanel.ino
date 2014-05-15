/**
*  Russell Stephens
*  Senior Design
*  HAUS - Locking Mechanism v1.1
*  February 14, 2014
*/
#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"
#include <Keypad.h>

//init RestClient with HAUS domain
RestClient client = RestClient("www.yoursite.com/haus");

// 4x4 Keypad from Amazon - http://www.amazon.com/Universial-Switch-Keypad-Keyboard-Arduino/dp/B008A30NW4/ref=cm_cd_ql_qh_dp_t
const byte ROWS = 4;
const byte COLS = 4;
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 5, 4, 3, 2 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 9, 8, 7, 6 }; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int ACCESS_CODE_LENGTH = 8;
char accessCode[ACCESS_CODE_LENGTH] = { 0, 0, 0, 0, 0, 0, 0, 0};
int accessCodeIndex = 0;

//Setup
void setup() {
  
  Serial.begin(9600);
  // Connect via DHCP
  Serial.println("connect to network");
  client.dhcp();
  Serial.println("Awaiting keypad interaction..");
  
}

//Main Loop
void loop()
{
  
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    Serial.println(key);
    
    if (key == '*' ) {
      Serial.println("Clearing Input");
      accessCodeIndex=0;
    }else
    {
        
      accessCode[accessCodeIndex] = key;
      accessCodeIndex++;
      if (accessCodeIndex == ACCESS_CODE_LENGTH ) {
             
        accessCodeIndex=0;
        Serial.print("Posting Device Update With access code: ");
        Serial.println(accessCode);
                
        String query = "/postdeviceupdates.php?passcode=1234567890123456&access_code=";
        String queryWithAccessCode = query + accessCode;
        
        char queryArr[queryWithAccessCode.length()+1];
        queryWithAccessCode.toCharArray(queryArr, queryWithAccessCode.length()+1);
        
        Serial.print("Query Str: ");
        Serial.println(queryArr);
        client.dhcp();
        String response = "";
        int statusCode = client.get( queryArr , &response);
           
        Serial.print("Status Code: ");
        Serial.println(statusCode);
                
        Serial.println(response);
        if (statusCode > 0) {
           Serial.println("Waiting 5 of your earth seconds.");
           delay(5000);
           Serial.println("Done Waiting.");
        }
            
            
      }//if 8
      
    }//else
   
  
  } //if key
  
 
}//done
