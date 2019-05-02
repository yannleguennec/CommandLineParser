#include "pins.h"

#define pinArg (commandArg(1) + 3)
#define pinValue (commandArg(2))

int getPin( char* blocks[] )
{
  if (*pinArg)
  {
    int pinNo = atoi( pinArg );
    int value = digitalRead( pinNo );

    Serial.print( commandArg(1) );
    Serial.print("=");
    Serial.println( value ? "HIGH" : "LOW" );
    return true;
  }

  Serial.println("pin non conforme");
  return false;
}

int setPin( char* blocks[] )
{
  if (*pinArg)
  {
    int pinNo = atoi( pinArg );

    if ( strcmp( commandArg(2), "INPUT") == 0)
    {
      pinMode( pinNo, INPUT );
      Serial.print( "pin" );
      Serial.print( pinNo);
      Serial.println( " is in INPUT mode." );
    }
    else if ( ( strcmp( pinValue, "INPUT_PULLUP") == 0 ) ||
              ( strcmp( pinValue, "PULLUP") == 0 ) )
    {
      pinMode( pinNo, INPUT_PULLUP );
      Serial.print( "pin" );
      Serial.print( pinNo);
      Serial.println( " is in INPUT_PULLUP mode." );
    }
    else if ( strcmp( pinValue, "OUTPUT") == 0)
    {
      pinMode( pinNo, OUTPUT );
      Serial.print( "pin" );
      Serial.print( pinNo);
      Serial.println( " is in OUTPUT mode." );
    }
    else
    {
      int value;
      if ( strcmp( pinValue, "on") == 0)
        value = HIGH;
      else if ( strcmp( commandArg(2), "off") == 0)
        value = LOW;
      else value = atoi( commandArg(2) ) ? HIGH : LOW;

      digitalWrite( pinNo, value );

      Serial.print( commandArg(1) );
      Serial.print( " is " );
      Serial.println( value ? "On" : "Off" );
    }

    return true;
  }
  Serial.println("pin non conforme");
  return false;
}

int togglePin( char *blocks[] )
{
  if (*pinArg)
  {
    int pinNo = atoi( pinArg );
    int value = digitalRead( pinNo ) ? LOW : HIGH;
  
    digitalWrite( pinNo, value);
  
    Serial.print( commandArg(1) );
    Serial.print( " is " );
    Serial.println( value ? "On" : "Off" );
    return true;
  }
  Serial.println("pin non conforme");
  return false;
}
