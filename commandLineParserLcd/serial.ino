// Get a string from Serial and launch execution at end of line.
#include "parser.h"

void serialSetup(void)
{
  Serial.begin(115200);
}

#define lineMax 128
char line[lineMax + 1];
int  lineNo = 0;

void serialEvent(void)
{
  while (Serial.available())
  {
    byte charIn = Serial.read();

    if (isprint(charIn))
    {
      if (lineNo < lineMax) // add char to string if it still fits
        line[lineNo++] = charIn;
      else Serial.println("Serial buffer overflow !!!");
    }
    
    // If a newline is found
    if ((charIn == '\r') || (charIn == '\n'))
    {
      // If buffer is not empty
      if (lineNo)
      {
        // End buffer properly
        line[lineNo] = '\0';

        // Execute command
        //if (prompt) Serial.println( line );
        if (parserExecute( line ))
          Serial.println("Ok.");
        else
          Serial.println("Erreur.");
        if (prompt)
          Serial.print(prompt);

        // Clear buffer
        lineNo = 0;
      }
    }
  }
}
