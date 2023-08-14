//Vinicius Araujo 2210392
//Francisco Lou 2211275
#include <stdio.h>
#include <stdlib.h>
#include "gera.h"
#define MAX 366
//MAX = numero maximo de bytes no array de códigos 
//(programa simples com 30 linhas e nossas traducoes tem no maximo 366 bytes)

FILE* openFile(const char *nome);

int main(void) {
  //TESTE 1
  //somente retornar o maior numero inteiro
  unsigned char ret[MAX]; 
  FILE* arq = openFile("programa");
  funcp funcaoSimples = gera(arq, ret);
  int resultado = (*funcaoSimples) ();
  printf("\nresultado esperado Simples 1: %d\n", 2147483647);
  printf("resultado da funcao Simples 1: %d\n\n", resultado);
  fclose(arq);
  
  //TESTE 2
  //somente retornar 0
  unsigned char ret2[MAX];
  arq = openFile("programa2");
  funcaoSimples = gera(arq, ret2);
  resultado = (*funcaoSimples)();
  printf("\nresultado esperado Simples 2: %d\n", 0);
  printf("resultado da funcao Simples 2: %d\n\n", resultado);
  fclose(arq);
  
  //TESTE 3
  //testa atribuição de parâmetro, incremento e retorno de variável
  unsigned char ret3[MAX];
  arq = openFile("programa3");
  funcaoSimples = gera(arq, ret3);
  resultado = (*funcaoSimples)(2); // parametro que so ocupa 1 byte
  printf("\nresultado esperado Simples 3.1: %d\n", 3);
  printf("resultado da funcao Simples 3.1: %d\n\n", resultado);
  resultado = (*funcaoSimples)(5000); //parametro inteiro que ocupa mais de 1 byte
  printf("\nresultado esperado Simples 3.2: %d\n", 5001);
  printf("resultado da funcao Simples 3.2: %d\n\n", resultado); 
  fclose(arq);
  
  //TESTE 4
  //testa se o numero passado no parametro é negativo
  //utiliza desvio condicional 'iflez' para uma linha embaixo
  unsigned char ret4[MAX];
  arq = openFile("programa4");
  funcaoSimples = gera(arq, ret4);
  resultado = (*funcaoSimples) (10); // caso em que o número é positivo
  printf("\nresultado esperado Simples 4.1: %d\n", 0);
  printf("resultado da funcao Simples 4.1: %d\n\n", resultado);
  resultado = (*funcaoSimples) (-1); // caso em que o número é de fato negativo
  printf("\nresultado esperado Simples 4.2: %d\n", 1);
  printf("resultado da funcao Simples 4.2: %d\n\n", resultado);
  resultado = (*funcaoSimples) (0); // caso em que o número é neutro
  printf("\nresultado esperado Simples 4.3: %d\n", 0);
  printf("resultado da funcao Simples 4.3: %d\n\n", resultado);
  fclose(arq);

  //TESTE 5
  //testa a funcao f(x,y) = (x+y) * (x-y), utilizando dois parametros, soma e multiplicação de variáveis
  unsigned char ret5[MAX];
  arq = openFile("programa5");
  funcaoSimples = gera(arq, ret5);
  resultado = (*funcaoSimples) (5, 1000); //testa positivo com positivo
  printf("\nresultado esperado Simples 5.1: %d\n", -999975);
  printf("resultado da funcao Simples 5.1: %d\n\n", resultado);
  resultado = (*funcaoSimples) (5, -1000); //testa positivo com negativo
  printf("\nresultado esperado Simples 5.2: %d\n", -999975);
  printf("resultado da funcao Simples 5.2: %d\n\n", resultado);
  resultado = (*funcaoSimples) (-5, -1000); //testa negativo com negativo
  printf("\nresultado esperado Simples 5.2: %d\n", -999975);
  printf("resultado da funcao Simples 5.3: %d\n\n", resultado);
  fclose(arq);

  //TESTE 6
  //testa a função fatorial, utilizando desvios condicionais, incondicionais e para direções opostas (linha anterior à atual e posterior também)
  unsigned char ret6[MAX];
  arq = openFile("programa6");
  funcaoSimples = gera(arq, ret6);
  resultado = (*funcaoSimples) (5);
  printf("\nresultado esperado Simples 6: %d\n", 120);
  printf("resultado da funcao Simples 6: %d\n\n", resultado);
  fclose(arq);
  //TESTE 7
  //testa uma funcao que use todos os parametros disponiveis e os soma
  unsigned char ret7[MAX];
  arq = openFile("programa7");
  funcaoSimples = gera(arq, ret7);
  resultado = (*funcaoSimples) (5000, 10000, 7);
  printf("\nresultado esperado Simples 7: %d\n", 15007);
  printf("resultado da funcao Simples 7: %d\n\n", resultado);
  fclose(arq);

  //TESTE 8
  //testa todas as ordens de soma 
  //(constante + constante, constante + variavel, variavel + constante, variavel + variavel)
  unsigned char ret8[MAX];
  arq = openFile("programa8");
  funcaoSimples = gera(arq, ret8);
  resultado = (*funcaoSimples) (10);
  printf("\nresultado esperado Simples 8: %d\n", 50);
  printf("resultado da funcao Simples 8: %d\n\n", resultado);
  fclose(arq);

  //TESTE 9
  //testa todas as ordens de multiplicacao
  //(constante * constante, constante * variavel, variavel * constante, variavel * variavel)
  unsigned char ret9[MAX];
  arq = openFile("programa9");
  funcaoSimples = gera(arq, ret9);
  resultado = (*funcaoSimples) (300);
  printf("\nresultado esperado Simples 9: %d\n", 3000000);
  printf("resultado da funcao Simples 9: %d\n\n", resultado);
  fclose(arq);


  //TESTE 10
  //testa todas as ordens de subtracao
  //(constante - constante, constante - variavel, variavel - constante, variavel - variavel)
  unsigned char ret10[MAX];
  arq = openFile("programa10");
  funcaoSimples = gera(arq, ret10);
  resultado = (*funcaoSimples) (1000);
  printf("\nresultado esperado Simples 10: %d\n", -995);
  printf("resultado da funcao Simples 10: %d\n\n", resultado);
  fclose(arq);



  //TESTA 11
  //testa um programa com o numero maximo de linhas(30) e bytes no array de comando
  unsigned char ret11[MAX];
  arq = openFile("programa11");
  funcaoSimples = gera(arq, ret11);
  resultado = (*funcaoSimples) ();
  printf("\nresultado esperado Simples 11: %d\n", 1280);
  printf("resultado da funcao Simples 11: %d\n\n", resultado);
  fclose(arq);


  return 0;
}


FILE* openFile(const char *nome){
  FILE* f;
  if((f = fopen(nome, "r")) == NULL){ //conferir se abre o arquivo
    perror("não conseguiu abrir o arquivo!");
    exit(1);
  }
  return f;
}
