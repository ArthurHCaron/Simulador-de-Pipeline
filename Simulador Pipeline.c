/*
Nome: Arthur Henrique Caron - RA: 2564297
Atividade 12 - Simulação de Pipeline
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <locale.h>

float acumulador[7][2]; //Simula um registrador da ULA. A etapa de busca de operandos armazena valores nesse array.

int criaInstrucao(void); //Função responsável por adicionar uma instrução na fila. Chamada no início do programa e após cada instrução ser concluída
int gerenciaFila(int* fila, int numProcesso); //Responsável por efetivamente realizar o pipelining.
void ULA(int option, int numProcesso);
void buscaOperando(int numProcesso, int option);

int main(){
     setlocale(LC_ALL, "Portuguese");
     
     int option = 0,
         **fila = (int**) calloc(7, sizeof(int*)), //Ponteiro para linhas (processos) da fila
         flag = 0, //Indica se um dos processos terminou
         processoAtual = 0; //Indica qual processo será executado agora
    
     for(int i = 0; i < 7; i++){
          fila[i] = (int*) calloc(2, sizeof(int)); //Na matriz, há duas colunas: uma para o clock, e outra para o tipo de processo.
          option = criaInstrucao();
          fila[i][1] = option;

          if(option == 0) return 0;
     }

     do{
          srand(time(NULL));
          processoAtual = rand() % 7;
          flag = gerenciaFila(fila[processoAtual], processoAtual + 1); //Passa um processo aleatório para o processador executar.
          fila[processoAtual][0]++;

          if(flag != 0){
               option = criaInstrucao();
               fila[processoAtual][0] = 0;
               fila[processoAtual][1] = option;
          }
     } while(option != 0);

     return 0;
}

int criaInstrucao(void){
     int option = 0;
     
     printf("Digite o número (em decimal) da instrução deseja realizar: \n");
     printf("001 (1) - Soma\n");
     printf("010 (2) - Subtração\n");
     printf("011 (3) - Comparar\n");
     printf("100 (4) - AND\n");
     printf("101 (5) - OR\n");
     printf("000 (0) - Encerrar\n");
     scanf("%d", &option);
     getchar();

     while(option < 0 || option > 5){
          printf("Inválido\n");
          scanf("%d", &option);
          getchar();
     }
     
     system("cls");
     
     return option;
}

int gerenciaFila(int* fila, int numProcesso){
     int flag = 0; //Incrementa se um processo chegou ao fim

     printf("-----------------------------------------------------------------\n");
     printf("Proceso nº %d\n", numProcesso);
     printf("Tipo de processo: %d\n", *(fila + 1));

     switch(*fila){ //Verifica em que ponto da execução está o processo
          case 0:
               printf("Buscando instrução...\n");
               break;
          case 1:
               printf("Decodificando instrução...\n");
               break;
          case 2:
               printf("Buscando operandos...\n");
               buscaOperando(numProcesso, *(fila + 1));
               break;
          case 3:
               printf("Executando...\n");
               ULA(*(fila + 1), numProcesso);
               break;
          case 4:
               printf("Escrevendo na memória...\n");
               flag++;
               break;
     }

     printf("-----------------------------------------------------------------\n");
     printf("Pressione qualquer tecla para tickar o clock");
     getche();
     system("cls");

     if(flag != 0) return 1;
     else return 0;
}

void ULA(int option, int numProcesso){ //O parâmetro que a ULA recebe é o tipo de operação (valor armazenado na segunda coluna da matriz) que deve ser realizado
     float resultado = 0;

     switch(option){
          case 1:
               resultado = acumulador[numProcesso][0] + acumulador[numProcesso][1];
               break;
          case 2:
               resultado = acumulador[numProcesso][0] - acumulador[numProcesso][1];
               break;
          case 3:
               if(acumulador[numProcesso][0] > acumulador[numProcesso][1]) resultado = acumulador[numProcesso][0];
               else resultado = acumulador[numProcesso][1];
               break;
          case 4:
               if(acumulador[numProcesso][0] && acumulador[numProcesso][1]) resultado = 1;
               else resultado = 0;
               break;
          case 5:
               if(acumulador[numProcesso][0] || acumulador[numProcesso][1]) resultado = 1;
               else resultado = 0;
               break;
     }

     printf("Resultado: %.2f\n", resultado);
}

void buscaOperando(int numProcesso, int option){
     printf("Digite o primeiro operando: ");
     scanf("%f", &acumulador[numProcesso][0]);
     getchar();

     if((acumulador[numProcesso][0] != 0 && acumulador[numProcesso][0] != 1) && (option == 4 || option == 5)){
          while(acumulador[numProcesso][0] != 0 && acumulador[numProcesso][0] != 1){
               printf("Apenas valores booleanos para esta operação!\n");
               scanf("%f", &acumulador[numProcesso][0]);
          }
     }

     printf("Digite o segundo operando: ");
     scanf("%f", &acumulador[numProcesso][1]);
     getchar();

     if((acumulador[numProcesso][1] != 0 && acumulador[numProcesso][1] != 1) && (option == 4 || option == 5)){
          while(acumulador[numProcesso][1] != 0 && acumulador[numProcesso][1] != 1){
               printf("Apenas valores booleanos para esta operação!\n");
               scanf("%f", &acumulador[numProcesso][1]);
          }
     }
}