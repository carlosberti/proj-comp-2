#include "globals.h"
#include "scan.h"

#define BUFLEN 256

typedef enum
{
  START,
  INDEC,
  INCOMMENT,
  INCOMMENTOUT,
  INNUM,
  INID,
  IN2SS,
  DONE
} StateType;

static struct
{
  char *str;
  TokenType tok;
} reservedWords[MAXRESERVED] = {
    {"else", ELSE},
    {"if", IF},
    {"int", INT},
    {"return", RETURN},
    {"void", VOID},
    {"while", WHILE}};

char tokenString[MAXTOKENLEN + 1];

static char lineBuf[BUFLEN];
static int linepos = 0;
static int bufsize = 0;

static char getNextChar(void)
{
  if (!(linepos < bufsize))
  {
    lineno++;
    if (fgets(lineBuf, BUFLEN - 1, source_file))
    {
      bufsize = strlen(lineBuf);
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
      return EOF;
  }

  return lineBuf[linepos++];
}

static void ungetNextChar(void)
{
  linepos--;
}

static TokenType reservedLookup(char *s)
{
  int i;
  for (i = 0; i < MAXRESERVED; i++)
    if (!strcmp(s, reservedWords[i].str))
      return reservedWords[i].tok;
  return ID;
}

void printToken(TokenType token, const char *tokenString)
{
  switch (token)
  {
  case ELSE:
  case IF:
  case INT:
  case RETURN:
  case VOID:
  case WHILE:
    printf("reserved word: %s\n", tokenString);
    break;
  case PLUS:
    printf("+\n");
    break;
  case MINUS:
    printf("-\n");
    break;
  case TIMES:
    printf("*\n");
    break;
  case OVER:
    printf("/\n");
    break;
  case LT:
    printf("<\n");
    break;
  case LTE:
    printf("<=\n");
    break;
  case GT:
    printf(">\n");
    break;
  case GTE:
    printf(">=\n");
    break;
  case EQ:
    printf("==\n");
    break;
  case NEQ:
    printf("!=\n");
    break;
  case ASSIGN:
    printf("=\n");
    break;
  case SEMI:
    printf(";\n");
    break;
  case COMMA:
    printf(",\n");
    break;
  case LPAREN:
    printf("(\n");
    break;
  case RPAREN:
    printf(")\n");
    break;
  case LBRACKET:
    printf("[\n");
    break;
  case RBRACKET:
    printf("]\n");
    break;
  case LBRACE:
    printf("{\n");
    break;
  case RBRACE:
    printf("}\n");
    break;
  case ENDFILE:
    printf("EOF\n");
    break;
  case NUM:
    printf("NUM, val= %s\n", tokenString);
    break;
  case ID:
    printf("ID, name= %s\n", tokenString);
    break;
  case ERROR:
    printf("ERROR: %s\n", tokenString);
    break;
  default:
    printf("Unknown token: %d\n", token);
  }
}

TokenType getToken(void)
{
  int tokenStringIndex = 0;
  TokenType currentToken;
  StateType state = START;
  int save;

  while (state != DONE)
  {
    char c = getNextChar();
    save = 1;
    switch (state)
    {
    case START:
      if (isdigit(c))
        state = INNUM;
      else if (isalpha(c))
        state = INID;
      else if (c == '<' || c == '>' || c == '!' || c == '=' || c == '/')
        state = IN2SS;
      else if (c == ' ' || c == '\t' || c == '\n')
        save = 0;
      else
      {
        state = DONE;
        switch (c)
        {
        case EOF:
          save = 0;
          currentToken = ENDFILE;
          break;
        case '+':
          currentToken = PLUS;
          break;
        case '-':
          currentToken = MINUS;
          break;
        case '*':
          currentToken = TIMES;
          break;
        case ';':
          currentToken = SEMI;
          break;
        case ',':
          currentToken = COMMA;
          break;
        case '(':
          currentToken = LPAREN;
          break;
        case ')':
          currentToken = RPAREN;
          break;
        case '[':
          currentToken = LBRACKET;
          break;
        case ']':
          currentToken = RBRACKET;
          break;
        case '{':
          currentToken = LBRACE;
          break;
        case '}':
          currentToken = RBRACE;
          break;
        default:
          currentToken = ERROR;
          break;
        }
      }
      break;
    case INNUM:
      if (!isdigit(c))
      {
        ungetNextChar();
        save = 0;
        state = DONE;
        currentToken = NUM;
      }
      break;
    case INID:
      if (!isalpha(c))
      {
        ungetNextChar();
        save = 0;
        state = DONE;
        currentToken = ID;
      }
      break;
    case IN2SS:
      if (tokenString[0] == '/' && c == '*')
      {
        state = INCOMMENT;
        save = 0;
        break;
      }
      if (c != '=')
      {
        ungetNextChar();
        save = 0;
        state = DONE;
        break;
      }
      state = DONE;
      switch (tokenString[0])
      {
      case '<':
        currentToken = LTE;
        break;
      case '>':
        currentToken = GTE;
        break;
      case '=':
        currentToken = EQ;
        break;
      case '!':
        currentToken = NEQ;
        break;
      default:
        currentToken = ERROR;
        break;
      }
    case INCOMMENT:
      save = 0;
      if (c == '*')
      {
        state = INCOMMENTOUT;
      }
      else if (c == EOF)
        state = DONE;
      break;
    case INCOMMENTOUT:
      save = 0;
      if (c == '/')
      {
        tokenString[tokenStringIndex--] = '\0';
        state = START;
        break;
      }
      else if (c == EOF)
        state = DONE;
      state = INCOMMENT;
      break;
    case DONE:
    default:
      state = DONE;
      currentToken = ERROR;
      break;
    }
    if (save && (tokenStringIndex <= MAXTOKENLEN))
      tokenString[tokenStringIndex++] = c;
    if (state == DONE)
    {
      tokenString[tokenStringIndex] = '\0';
      if (currentToken == ID)
        currentToken = reservedLookup(tokenString);
    }
  }

  printToken(currentToken, tokenString);

  return currentToken;
}

FILE *source_file;
int lineno = 0;

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Less arguments than expected\n");
    exit(1);
  }
  if (argc > 2)
  {
    printf("More arguments than expected\n");
    exit(1);
  }

  char *file_name = argv[1];
  source_file = fopen(file_name, "r");

  if (source_file == NULL)
  {
    printf("File not found\n");
    exit(1);
  }

  while (getToken() != ENDFILE)
    ;

  return 0;
}
