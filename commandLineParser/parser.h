#ifndef __Parser__
#define __Parser__

// Get command name and arguments from blocks
#define commandName()   commandArg(0)
#define commandArg(x)   blocks[x]

// Parser initialisation, must be run before anything else.
void parserSetup(char *prompt = NULL);
void parserRegister( const char *cmd, void (*func)(char*[]), const char *desc );
bool parserExecute( char* line );

#endif
