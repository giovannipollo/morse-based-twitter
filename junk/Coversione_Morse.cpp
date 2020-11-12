#include <stdio.h>
#include <string.h>

static const char *alpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};

static const int alpha_len[] = {2, 4, 4, 3, 1, 4, 3, 4, 2, 4, 3, 4, 2, 2, 3, 4, 4, 3, 3, 1, 3, 4, 3, 4, 4, 4};

int CharToSym(char x) {
   return ((x - 'A'));
}

int main() {
   int i, j;
   int pos;
   char parola[] = "SOS";
   char output[40]; // limit is 25
   int index = 0;
   printf("Converting %s to Morse code: ", parola);
   for (i = 0; i < strlen(parola); i++) {
      if ((parola[i] < 'A') || (parola[i] > 'Z')) {
         printf("ERROR: STRING to be converted shall contain uppcase characters, only\n");
         return 1;
      }
      pos = CharToSym(parola[i]);

      for (j = 0; j < alpha_len[pos]; j++) {
         if (alpha[pos][j] == '.') {
            output[index] = '1';
            index++;
         } else {
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
   printf("%s", output);

   return 0;
}