#include "serial.h"
#include "parser.h"
#include "pins.h"

char nom[20] = "Arduino";

void setName(char *name)
{
  int i;
  for (i = 0; name[i]; i++)
    nom[i] = name[i];
  nom[i] = '\0';
  Serial.print("D'accord, mon nom est ");
  Serial.println( nom );
}

int hello( char *blocks[] )
{
  Serial.print("Salut, je suis ");
  if (commandArg(1))
    Serial.println( commandArg(1));
  else
    Serial.println( nom );

  return true;
}

int set( char *blocks[] )
{
  if (strcmp( commandArg(1), "message") == 0)
  {
    
  }
  else if (strcmp( commandArg(1), "name") == 0)
  {
    setName( commandArg(2) );
    return true;
  }
  else if (strncmp( commandArg(1), "pin", 3) == 0)
  {
    if (setPin(blocks))
      return true;
  }
  return false;
}

int get( char *blocks[] )
{
  if (strncmp( commandArg(1), "pin", 3) == 0)
  {
    if (getPin(blocks))
      return true;
  }
  return false;
}

int toggle( char *blocks[] )
{
  if (strncmp( commandArg(1), "pin", 3) == 0)
  {
    if (togglePin(blocks))
      return true;
  }
  return false;
}

void setup()
{
  serialSetup();
  Serial.println("parser - Execution d'une ligne de commande.");

  parserSetup(">");
  parserRegister( "salut",  hello,  "Dire bonjour à tout le monde."   );
  parserRegister( "get",    get,    "Récupérer la valeur d'une pin."   );
  parserRegister( "set",    set,    "Changer la valeur d'une pin."   );
  parserRegister( "toggle", toggle, "Basculer la valeur d'une pin.");
}

void loop()
{
  // Utiliser un mode de programmation non bloquant.
}
