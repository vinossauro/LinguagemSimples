//Vinicius Araujo 2210392
//Francisco Lou 2211275
#include "gera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union conv { //usado para atribuicao de numeros inteiros
  int val;
  unsigned char d[3];
};
typedef union conv Conv;

struct desvio { //usado para armazenar em que linhas teremos desvios
  int indiceLinha;
  int indiceOff;
  int existe;
  //int linha;
};
typedef struct desvio Desvio;


static void error(const char *msg, int line) {
  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}

funcp gera(FILE *f, unsigned char codigo[]) {

  int indice = 0;
  Desvio vetor[31]; //vetor para armazenar onde teremos que ajeitar o offset para o desvio
  for (int i = 0; i < 31; i++){
    vetor[i].existe = 0;
  }
  codigo[indice++] = 0x55; //prologo "padrao" do assembly
  codigo[indice++] = 0x48;
  codigo[indice++] = 0x89;
  codigo[indice++] = 0xe5;
  codigo[indice++] = 0x48;
  codigo[indice++] = 0x83;
  codigo[indice++] = 0xec;
  codigo[indice++] = 0x20;

  int line = 1; // indicar linha de erro
  int c;        // so receberemos ints

  while ((c = fgetc(f)) != EOF) {
    vetor[line].indiceLinha = indice;
    Conv convert;
    switch (c) {
    case 'r': { // caso encontre ret
      char var0;
      int idx0;
      if (fscanf(f, "et %c%d", &var0, &idx0) != 2)
        error("comando invalido", line);
      if (var0 == '$') {
        convert.val = idx0;
        codigo[indice++] = 0xb8;
        for (int j = 0; j < 4; j++) {
          codigo[indice++] = convert.d[j];
        }

      } else if (var0 == 'v') {
        codigo[indice++] = 0x8b;
        codigo[indice++] = 0x45;
        codigo[indice++] = 0x00 - (idx0 * 4); // rbp menos o sub
      }
      codigo[indice++] = 0xc9;
      codigo[indice++] = 0xc3;
      printf("%d ret %c%d\n", line, var0, idx0);
      break;
    }
    case 'v': { /* atribuiÃ§Ã£o e op. aritmetica */
      int idx0, idx1;
      char var0 = c, c0, var1;
      if (fscanf(f, "%d %c", &idx0, &c0) != 2)
        error("comando invalido", line);

      if (c0 == '<') { /* atribuiÃ§Ã£o */
        if (fscanf(f, " %c%d", &var1, &idx1) != 2)
          error("comando invalido", line);
        printf("%d %c%d < %c%d\n", line, var0, idx0, var1, idx1);
        if (var1 == 'v') {
          codigo[indice++] = 0x8b; //move a var1 para o ebx
          codigo[indice++] = 0x5d;
          codigo[indice++] = 0x00 - (idx1 * 4);
          codigo[indice++] = 0x89; //move o ebx para var0
          codigo[indice++] = 0x5d;
          codigo[indice++] = 0x00 - (idx0 * 4);
        } else if (var1 == 'p') {
          codigo[indice++] = 0x89;
          if (idx1 == 3){
            codigo[indice++] = 0x55;
          }
          else {
            codigo[indice++] = 0x85 - (idx1 * 8);
          }
          codigo[indice++] = 0x00 - (idx0 * 4);

        } else if (var1 == '$') {
          convert.val = idx1;
          codigo[indice++] = 0xc7;
          codigo[indice++] = 0x45;
          codigo[indice++] = 0x00 - (idx0 * 4);
          for (int j = 0; j < 4; j++) {
            codigo[indice++] = convert.d[j];
          }
        }

      } else { /* operaÃ§Ã£o aritmÃ©tica */
        char var2, op;
        int idx2;
        if (c0 != '=')
          error("comando invalido", line);
        if (fscanf(f, " %c%d %c %c%d", &var1, &idx1, &op, &var2, &idx2) != 5)
          error("comando invalido", line);
        printf("%d %c%d = %c%d %c %c%d\n", line, var0, idx0, var1, idx1, op, var2, idx2);
        if (op == '+'){
          if (var1 == '$' && var2 == '$'){ //soma duas constantes
            convert.val = idx1;
            codigo[indice++] = 0xc7; //move a constante na var0
            codigo[indice++] = 0x45;
            codigo[indice++] = 0x00 - (idx0 * 4);
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            convert.val = idx2; //soma a constante no var0
            codigo[indice++] = 0x81;
            codigo[indice++] = 0x45;
            codigo[indice++] = 0x00 - (idx0 * 4);
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
          }
          else if (var1 == '$' && var2 == 'v'){ //soma constante com variavel
            convert.val = idx1;
            codigo[indice++] = 0x81; //soma a constante no var2
            codigo[indice++] = 0x45;
            codigo[indice++] = 0x00 - (idx2 * 4);
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            codigo[indice++] = 0x8b; //move o var2 para o ebx e dps move o ebx para o var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx2 * 4);
            codigo[indice++] = 0x89;
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);         
          }
          else if (var1 == 'v' && var2 == '$'){ //soma variavel com constante
            convert.val = idx2;
            codigo[indice++] = 0x81; //soma a constante no var1
            codigo[indice++] = 0x45;
            codigo[indice++] = 0x00 - (idx1 * 4);
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            codigo[indice++] = 0x8b; //move a variavel do var1 para ebx e do ebx para var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx1 * 4);
            codigo[indice++] = 0x89;
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);
          }
          else if (var1 == 'v' && var2 == 'v'){ //soma variavel com variavel
            codigo[indice++] = 0x8b; //move a var1 para ebx
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx1 * 4);
            codigo[indice++] = 0x03; //soma var2 com o ebx
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx2 * 4);
            codigo[indice++] = 0x89; //move o ebx para var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);            
          }         
        }
        else if (op == '-'){
          if (var1 == '$' && var2 == '$'){ //subtrai constante com constante
            convert.val = idx1;
            codigo[indice++] = 0xc7; //move a constante na var0
            codigo[indice++] = 0x45;
            codigo[indice++] = 0x00 - (idx0 * 4);
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            convert.val = idx2; //subtrai a constante2 no var0
            codigo[indice++] = 0x81;
            codigo[indice++] = 0x6d;
            codigo[indice++] = 0x00 - (idx0 * 4);
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
          }
          else if (var1 == '$' && var2 == 'v'){ //subtrai variavel de constante
            convert.val = idx1;
            codigo[indice++] = 0xbb; //move constante para ebx
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            codigo[indice++] = 0x2b; // subtrai ebx - variavel
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx2 * 4);
            codigo[indice++] = 0x89; //move ebx para var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);
          }
          else if (var1 == 'v' && var2 == '$'){ //subtrai constante de variavel
            convert.val = idx2; //subtrai a constante2 no var1
            codigo[indice++] = 0x81;
            codigo[indice++] = 0x6d;
            codigo[indice++] = 0x00 - (idx1 * 4);
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            codigo[indice++] = 0x8b; //move a variavel do var1 para ebx e do ebx para var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx1 * 4);
            codigo[indice++] = 0x89;
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);
          }
          else if (var1 == 'v' && var2 == 'v'){ //subtrai variavel de variavel
            codigo[indice++] = 0x8b; //move a variavel do var1 para ebx
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx1 * 4);
            codigo[indice++] = 0x2b;
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx2 * 4); //subtrai var2 do ebx
            codigo[indice++] = 0x89; //move ebx para var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);
          }
        }
        else if (op == '*'){
          if (var1 == '$' && var2 == '$') {
            convert.val = idx1; // move a primeira constante para o ebx
            codigo[indice++] = 0xbb;
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            convert.val = idx2;
            codigo[indice++] = 0x69; //multiplica constante * ebx
            codigo[indice++] = 0xdb;
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            codigo[indice++] = 0x89; //move ebx para var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);
          }
          else if (var1 == '$' && var2 == 'v'){ 
            convert.val = idx1;
            codigo[indice++] = 0x8b; //move a variavel do var2 para ebx
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx2 * 4);
            codigo[indice++] = 0x69; //multiplica constante * ebx
            codigo[indice++] = 0xdb;
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            codigo[indice++] = 0x89; //move ebx para var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);
          }
          else if (var1 == 'v' && var2 == '$'){ //multiplica variável com constante
            convert.val = idx2;
            codigo[indice++] = 0x8b; // guarda o valor da variavel1 no ebx
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx1 * 4);
            codigo[indice++] = 0x69; // multiplica constante * ebx
            codigo[indice++] = 0xdb;
            for (int j = 0; j < 4; j++) {
              codigo[indice++] = convert.d[j];
            }
            codigo[indice++] = 0x89; //move ebx para var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);
          }
          else if (var1 == 'v' && var2 == 'v'){ //multiplica variável com variável
            codigo[indice++] = 0x8b; // guarda o valor da variavel1 no ebx
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx1 * 4);
            codigo[indice++] = 0x0f; // multiplica o valor da variavel2 * ebx
            codigo[indice++] = 0xaf;
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx2 * 4);
            codigo[indice++] = 0x89; //move ebx para var0
            codigo[indice++] = 0x5d;
            codigo[indice++] = 0x00 - (idx0 * 4);
          }
        }
      }
      break;
    }
    case 'i': { /* desvio condicional */
      char var0;
      int idx0, n;
      if (fscanf(f, "flez %c%d %d", &var0, &idx0, &n) != 3)
        error("comando invalido", line);
      printf("%d iflez %c%d %d\n", line, var0, idx0, n);
      codigo[indice++] = 0x83; //cmpl de 0 com a var0
      codigo[indice++] = 0x7d;
      codigo[indice++] = 0x00 - (idx0 * 4);
      codigo[indice++] = 0x00; 
      codigo[indice++] = 0x7e; //desvio condicional jle (jump less or equal)
      /*vetor[line].indiceOff = indice;
      vetor[line].linha = n;
      vetor[line].existe = 1;*/
      vetor[n].indiceOff = indice;
      vetor[n].existe = 1;
      codigo[indice++] = 0x00; //coloca lixo no offset
      break;
    }

    default:
      error("comando desconhecido", line);
    }
    line++;
    fscanf(f, " ");
  }
  /*for (int i = 1; i < line; i++) { //coloca os offsets!!
    if (vetor[i].existe) {
      vetor[i].indiceLinha = vetor[vetor[i].linha].indiceLinha;
    }
  }*/
  for (int i = 1; i < line; i++) { //coloca os offsets!!
    if (vetor[i].existe) {
      unsigned char offset = (unsigned char) (vetor[i].indiceLinha - vetor[i].indiceOff - 1);
      codigo[vetor[i].indiceOff] = offset;
    }
  }
  funcp programa = (funcp)codigo;

  return programa;
}
