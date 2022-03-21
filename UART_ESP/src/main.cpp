#include <Arduino.h>

String myString = "";

void setup()
{
	// put your setup code here, to run once
	Serial.begin(9600);
}
void readUart();

void loop()
{
	// put your main code here, to run repeatedly:
	readUart(); //
}

// --------------------------------------------- //

void readUart()
{
	
	char myChar;
	while(Serial.available() > 0)
	{
		myChar = (char)Serial.read(); 
		if(myChar != '\n')
		{
			myString += myChar; 
		}
		else
		{
			int TimA, TimB, TimC, TimD = -1; 
			TimA = myString.indexOf("A"); 
			TimB = myString.indexOf("B"); 
			TimC = myString.indexOf("C"); 
			TimD = myString.indexOf("D"); 	
			
			//Serial.printf("TimA: %d\nTimB: %d\nTimC: %d\nTimD: %d\n", TimA, TimB, TimC,TimD); 
			//A123B234C345\n
			if(TimA >= 0 && TimB >= 0 && TimC >= 0 && TimD >= 0)
			{
				String str1  = myString.substring(TimA + 1, TimB); 
				String str2  = myString.substring(TimB + 1, TimC); 
				String str3  = myString.substring(TimC + 1, TimD); 

				Serial.println(str1); 
				Serial.println(str2); 
				Serial.println(str3); 

				int n1 = str1.toInt(); // Convert stirng to int A1B A0B
				int n2 = str2.toInt();
				int n3 = str3.toInt(); 
			}
			if(myString == "GT")
			{
				int temp = random(30);
				int humi = random(100);
				// AtempBhumiC\n
				String sendString = "A" + String(temp) + "B" + String(humi) + "C"; 
				Serial.println(sendString);
				Serial.flush();

			}
			myString = ""; 
		}
	}
}