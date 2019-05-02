// Split a line into command + args and execute it according to the command list
#include "parser.h"

#define commandMax  20
#define commandArgs 4
#define blocksMax (commandArgs + 1)

int maxlen = 0;
char *prompt = NULL;

typedef struct
{
  const char *cmd;
  int (*func)(char *[]);
  const char *desc;
} Command;

Command commands[commandMax];
int commandNb = 0;

void parserSetup(const char* prpt)
{
  parserRegister( "help", parserHelp, "Afficher la liste de toutes les commandes" );
  if (prpt) 
  {
    prompt = (char*)prpt;
    Serial.print(prompt);
  }
}

void parserRegister( const char *cmd, int (*func)(char*[]), const char *desc )
{
  if ( commandNb < commandMax )
  {
    int i;
    for (i=0; cmd[i]; i++);
    if (i > maxlen) maxlen = i;
    
    commands[ commandNb ].cmd = cmd;
    commands[ commandNb ].func = func;
    commands[ commandNb ].desc = desc;
    commandNb++;
  }
  else
    Serial.println("Erreur : Trop de commandes.");
}

bool parserInit(char* blocks[])
{
  // Initialize blocks to NULL
  for (int blocksNo = 0; blocksNo < blocksMax; blocksNo++)
    blocks[blocksNo] = NULL;  
  return true;
}

int parserSplit( char* line, char* blocks[])
{
  // Command grammar :
  // -----------------
  //    line  := sep1* command listargs* sep* ;;
  //    listargs := (sep+ arg)* ;;
  //    command := "help"
  //            |  "reboot"
  //            |  "save"
  //            |  ...
  //            ;;
  //
  //    sep1 := space | tab ;;
  //    sep  := space | , | tab | = | + ;;
  //    arg  := quotedstring | integer ;;
  //    quotedstring := string
  //           |  \" string \"
  //           |  \' string \' ;;
  //    integer := [num]+ ;;
  //    string := [char]* ;;
  //
  // TODO : Backslash quote escapement is functionnal but wierd and not tested !!!
  
  // Split line in blocks (ignore extra blocks > blockMax)
  int blocksNb = 0;
  while ( *line && (blocksNb < blocksMax) )
  {
    // Skip space(s) at beginning of line
    while ( *line && (*line == '\t' || *line == ' ') ) line++;

    if (*line)
    {
      // Check for quotes, and store the char used
      char quote = '\0';
      if (*line == '"' || *line == '\'') quote = *line++;
  
      // Start a new block
      blocks[blocksNb++] = line;
  
      if (quote)
      {
        // Search for closing quote ignoring all separators and escaped quotes in between
        while (*line && *line != quote)
        {
          // Handle backslash escaping in strings
          if (*line == '\\')
          {
            // Shift string by 1 char
            char* ptr = line;
            while (*ptr) { *ptr = *(ptr+1); ptr++; }
          };
          if (*line) 
            line++; // Check if case previous loop puts us at the end
        };

        // Whatever it's a quote closing or a \0, we close the block with \0.
        // That means ending quote is seen as a separator.
        // A string "Str"ange will be seen as block(n) as "Str" and block(n+1) as "ange"
        if (*line)
          *line++ = '\0';
      }
      else
      {
        // Search for next separator
        while ( *line && (*line != '\t' && *line != ' ' && *line != '=') ) line++;
  
        // close the string with \0 and go to next char as there may be more after.
        if (*line)
          *line++ = '\0';       
      };
    };
  };
  return blocksNb;
};

int parserCommandSearch( char *cmd )
{
  for( int commandNo=0; commandNo<commandNb; commandNo++)
    if (!strcmp( cmd, commands[commandNo].cmd))
      return commandNo;
  return -1;
}

int parserRun( char* blocks[] )
{
  int commandNo = parserCommandSearch( commandName() );
  if (commandNo >= 0)
    return commands[commandNo].func(blocks);
  return false;    
}

bool parserExecute( char* line )
{
  char *blocks[blocksMax];
  int res = false;
  
  if (parserInit( blocks ))
    if (parserSplit(line, blocks))
    {
      res = parserRun(blocks); 
      if (prompt) Serial.print(prompt);
    }
    return res;
}

void parserCommandDisplay(int commandNo)
{
  int i;
  
  Serial.print( commands[commandNo].cmd );
  for ( i=0; commands[commandNo].cmd[i]; i++);
  for (; i < maxlen; i++) Serial.print(" ");
  Serial.print(" : ");
  Serial.println( commands[commandNo].desc );        
}

int parserHelp( char *blocks[] )
{
  // if an arg is given, print only this command
  if (commandArg(1))
  {
    int commandNo = parserCommandSearch( commandName() );
    parserCommandDisplay( commandNo );
  }
  // if no arg is given, print all commands
  else
  {
    Serial.println("Commandes disponibles :");
    for( int commandNo=0; commandNo<commandNb; commandNo++)
    {
      Serial.print("  ");
      parserCommandDisplay( commandNo );
    }
  }
  return true;
}
