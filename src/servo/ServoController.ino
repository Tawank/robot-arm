#include <Servo.h>

Servo servo[2];

void setup()
{
	Serial.begin( 9600);
	servo[0].attach( 9);
	servo[1].attach(10);
	pinMode( 8, OUTPUT);
}

void loop()
{
	String inString = "";
	int servoCount = 0;

	while(Serial.available() > 0)
	{
		int inInt = Serial.read();

		if(isDigit(inInt))
		  inString += (char)inInt;
		else if(inInt == '\n')
		{
			servo[servoCount].writeMicroseconds(inString.toInt());

			if(servoCount==0)
				servoCount = 1;
			else
				servoCount = 0;

			inString = "";
			digitalWrite(8, LOW);
		}
		else if(inInt == '\a')
			digitalWrite(8, HIGH);
	}
}
