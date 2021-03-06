#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printgemtext(FILE *fp)
{
  char c; // character to check/print
  char t; // tmp character
  char listbuf[4];
  int firstchar = 1;
  int preformatted = 0;
  int hideln = 0;
  int listi = 0; // used for numbers on lists
  int linki = 0; // used for number on links

  while ((c = getc(fp)) != EOF)
  {
    if (!preformatted)
    {
      if (c == '#' && firstchar) // checks for header
      {
        t = getc(fp); // reads next char
        if (t == '#') // checks if sub-header
        { 
          t = getc(fp); // reads next char
          if (t == '#') // checks if sub-sub-header
            printf("\033[33m##");     // goes yellow if sub-sub-header
          else
            printf("\033[34m#%c", c); // goes blue if sub-header
        }
        else
          printf("\033[35m%c", c);    // goes purple if header

        c = t; // sets the next character to be printed to t(mp) so that the file is continued
      }
  
      if (c == '*' && firstchar)
      {
        ++listi;

        sprintf(listbuf, "%i", listi);
        printf("\033[37m%s\033[0m", listbuf);

        c = 0x0;
      }
      else if (firstchar)
        listi = 0;

      if (c == '=' && firstchar) // checks for start of link
      {
        t = getc(fp); // reads next char
        if (t == '>') // checks for end of link
        {
          printf("\033[31m[%i]", linki); // if link then index it
          
          while ((c = getc(fp)) == ' ')
          while ((c = getc(fp)) != ' ')
            ;

          ++linki; 
          printf(" ");
        }
      }

      if (c == '>' && firstchar)
        printf("\033[32m"); // goes green if quote
    }

    if (c == '`' && firstchar)
      if (getc(fp) == '`')
        if (getc(fp) == '`')
        {
          hideln = 1; 

          if (preformatted)
            preformatted = 0;
          else 
            preformatted = 1;
        }

    firstchar = 0;

    if (c == '\n')
    {
      firstchar = 1;
       
      if (hideln)
      {
        hideln = 0;
        printf("\033[1A"); // removes newline
      }

      printf("\033[0m"); // clears colour if newline
    } 
    
    if (!hideln)
      printf("%c", c);
  }
}

FILE * openurl(char * url)
{
  FILE *fp = NULL;

  fp = fopen(url, "r");

  return fp;
}

int main(int argc, char ** argv)
{
  char url[100];
  FILE *fp = NULL;
  
  if (argc < 2)
    fp = stdin;
  else
  {
    strcpy(url, argv[1]);
    fp = openurl(url);
  }

  printgemtext(fp);

  fclose(fp);
}
