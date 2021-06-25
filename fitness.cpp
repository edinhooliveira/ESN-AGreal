/******************************************************************************\
*								 Fitness:									 *
\******************************************************************************/
#include "defTipo.hpp"
#include "Simulador.hpp"
#include <iostream>

using namespace std;
/******************************************************************************\
*								Calculo Fitness Simulador					 *
\******************************************************************************/
double calcFitnessSimulador(alelo *indiv, int gen)
{
	cout<<"Entrou no calc de fitness:" <<endl;
	int Fitness = 0;
	//double *mov; // Acao tomada
	double *mov = new double[outputSize];
	double *in; 
	// Leituras dos sensores
	
	Simulador *simulador = new Simulador(120,200, dynamicEnvironment, maxGen, gen);
	//ESNbp *esn;
	cout<<"ESN-CALC-FIT: "<<esn<<endl; 
	
	//Pesos do reposit�rio -> sa�da
	esn->setResWeight(indiv); 
	cout<<"Saiu do ResWeight" << endl;

	for(int i = 0, j = batterry; i < numMov && j > 0; i++, j--){ // Enquanto ainda tenho mov para realizar (i < numMov) e ainda tenho bateria (j > 0)
		in = simulador->readSensor(10, gen);			
		//mov = esn->Execute(in);				//Verifica a sa�da da ESN de acordo com a entrada
		
		esn->ESNoutput(in, mov); 

		//Define qual movimento vai ser executado (movimento correspondete ao neuronio de maior ativa��o)
		int aux = 0; // Maior sa�da � a 0
		for(int k = 1; k < outputSize; k++) // Para cada sa�da, se for maior q aux, substitui
			if(mov[k] > mov[aux])
				aux = k;
		//Se o rob� bateu, finaliza a simula��o
		if(!simulador->execute(aux, 10, gen)){
			delete mov;
			delete in;
			break;
		}
	
		//Verifica se o rob� est� na base
		if(simulador->isBase())
			j = batterry;						 //Recarrega a bateria
		else
			if(aux == 3) //mov 3 corresponde a andar para frente	
				Fitness++;	
	
		//delete mov;
		delete in;
	}
	
	delete simulador;
		
	return Fitness / (double)numMov;
}

/******************************************************************************\
*								Calculo Trajeto					             *
\******************************************************************************/
double calcTrajeto (alelo *indiv, int nroExec, int gen)
{
	int Fitness = 0;
	double *mov;
	double *in;
	Simulador *simulador = new Simulador(120,200, dynamicEnvironment, maxGen, gen);
	ESNbp *esn;

	//Pesos do reposit�rio
	esn->setResWeight(indiv); 
	
	int **pos;
	pos = new int*[2];
	pos[0] = new int[numMov+1];
	pos[1] = new int[numMov+1];
	pos[0][0] = simulador->getPosX();
	pos[1][0] = simulador->getPosY();
	
	int i, j;
	for(i = 0, j = batterry; i < numMov && j > 0; i++, j--){
		in = simulador->readSensor(10, gen);		
		
		//mov = esn->Execute(in);	     //Verifica a sa�da da ESN de acordo com a entrada
		esn->ESNoutput(in, mov);
		
		//Define qual movimento vai ser executado (movimento correspondete ao neuronio de maior ativa��o)
		int aux = 0;
		for(int k = 1; k < outputSize; k++)
			if(mov[k] > mov[aux])
				aux = k;
		
		//Se o rob� bateu, finaliza a simula��o
		if(!simulador->execute(aux, 10, gen)){
			delete mov;
			delete in;
			break;
		}
		
		pos[0][i+1] = simulador->getPosX();
		pos[1][i+1] = simulador->getPosY();
		
		//Verifica se o rob� est� na base
		if(simulador->isBase())
			j = batterry;						 //Recarrega a bateria
		else
			if(aux == 3) //mov 3 corresponde a andar para frente	
				Fitness++;	
	
		delete mov;
		delete in;
	}

	salv_traj(pos, i, nroExec);


	delete simulador;
	delete pos[0];
	delete pos[1];
	delete pos;
	
	return Fitness / (double)numMov;
}


/******************************************************************************\
*								Calculo Fitness 							 *
\******************************************************************************/
double calcFitness(alelo *indiv, int gen)
{	
	double Fitness = 0;	
	for(int i = 0; i < numSimulacao; i++){
		Fitness += calcFitnessSimulador(indiv, gen);
		cout<<" " << "Fitness: " << Fitness << endl;	
	}
		

	Fitness = Fitness / numSimulacao;
	return Fitness;
}
