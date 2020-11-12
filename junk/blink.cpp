// see blink.oil head of file for information about the compilation process.

#include "Arduino.h"
#include "tpl_os.h"

// static int SOS[] = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1,
// 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};

static const char *alpha[] = {
    ".-",   // A
    "-...", // B
    "-.-.", // C
    "-..",  // D
    ".",    // E
    "..-.", // F
    "--.",  // G
    "....", // H
    "..",   // I
    ".---", // J
    "-.-",  // K
    ".-..", // L
    "--",   // M
    "-.",   // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.",  // R
    "...",  // S
    "-",    // T
    "..-",  // U
    "...-", // V
    ".--",  // W
    "-..-", // X
    "-.--", // Y
    "--..", // Z
};

static const int alpha_len[] = {2, 4, 4, 3, 1, 4, 3, 4, 2, 4, 3, 4, 2,
                                2, 3, 4, 4, 3, 3, 1, 3, 4, 3, 4, 4, 4};

int CharToSym(char x) { return ((x - 'A')); }

static char output[25]; // limit is 25

void conversion()
{

  int i, j;
  int pos;
  char parola[] = "SOS";

  int index = 0;
  // printf( "Converting %s to Morse code: ", parola );
  for (i = 0; i < 3; i++)
  {
    if ((parola[i] < 'A') || (parola[i] > 'Z'))
    {
      // printf( "ERROR: STRING to be converted shall contain uppcase
      // characters, only\n" );
    }
    else
    {
      pos = CharToSym(parola[i]);

      for (j = 0; j < alpha_len[pos]; j++)
      {
        if (alpha[pos][j] == '.')
        {
          output[index] = '1';
          index++;
        }
        else
        {
          output[index] = '1';
          index++;
          output[index] = '1';
          index++;
          output[index] = '1';
          index++;
        }
        output[index] = '0';
        index++;
      }
      output[index] = '0';
      index++;
      output[index] = '0';
      index++;
    }
  }
  // printf( "%s", output );
}

void setup()
{
  Serial.begin(115200);
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  conversion();
  Serial.print(output);
  Serial.println("");
}

TASK(SosTask)
{
  int i = 0;
  EventMaskType one_second_mask;
  while (1)
  {
    WaitEvent(OneSecond);
    GetEvent(SosTask, &one_second_mask);
    if (one_second_mask)
    {
      ClearEvent(OneSecond);
      if (output[i] == '1')
      {
        digitalWrite(13, HIGH); // switch on led
        Serial.println("1");
      }
      else
      {
        digitalWrite(13, LOW); // switch off led
        Serial.println("0");
      }

      if (i == 33)
        i = 0;
      else
        i++;
    }
  }
}
