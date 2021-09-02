/******************************************************************************\
*						 Definicao Valor(inicializacao)						 *
\******************************************************************************/

#include "defTipo.hpp"

/* Definicao da variaveis globais */
populacao popVelha , popNova;		
double *arq_media_fitness, *arq_melhor_fitness; 	// dados para serem gravados
double **arq_melhor_individuo;						// dados para serem gravados
Randon *randon;


// Parametros ESN
ESNbp *esn;
int inputSize = 4; //mais sensores
int repSize = 50;
int outputSize = 4; //mais ações
double spectral_radius_d = 0.95;
double con_density = 0.15;
int size_stab = 50; //modificação Eder


// Parametros da simulacao
int nroMaxExec = 25; // 25								// numero de repeticoes do AG
int maxGen = 2200;//10 //2200							// numero maximo de geracoes 

// Parametros do AG
int tamPop = 100;//100								// tamanho da pop. 
int lcrom = (repSize + 1) * outputSize;				// tamanho do cromossomo 

double taxaCross = 0.6;								// taxa de crossover
double taxaMut = 1.0/lcrom;							// taxa de mutacao

bool elitismo = true;								// true: elitismo ; false: sem elitismo
int tamTorneio = 3;									// tamanho do torneio (usado na selecao por torneio)

//Hipermutação
bool hipermutacao = false;							// true: hipermutacao ; false: sem hipermutacao
int tamCiclo = 10; //10   //maxGen/100;				// tamanho do ciclo de hipermutação
double taxaR = 3;									//porcentagem de acrescimo na taxa de mutação (1 + taxaR)*tavaMut
bool cicloHipermutacao = false;						//determina se acabou de ocorrer um ciclo de hipermutação - NÃO ALTERAR

//Imigração
bool imigracao = false;								// true: imigracao ; false: sem imigracao
double taxaMigracao = 0.05;							// taxa de imigrantes


//Parametros Simulação
int numMov = 300; //300
int batterry = 80; //80
bool dynamicEnvironment = true;
int numSimulacao = 10; //10

//Parametros Treinamento ESN
int nrRodadas = 500; //500  // modificacao R		//rodadas de Treinamento da ESN

//Inserir Parametros de Falha
bool falha = false;
double chanceFalha = 0.1;
double percentualFalha = 0.1;


