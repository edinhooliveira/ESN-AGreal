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
	double *in = new double[inputSize]; // Leituras dos sensores
	double *mov = new double[outputSize];
	//double *in = new double[inputSize];
	
	Simulador *simulador = new Simulador(120,200, dynamicEnvironment, maxGen, gen);
	//ESNbp *esn;
	//cout<<"ESN-CALC-FIT: "<<esn<<endl; 
	
	//Pesos do reposit�rio -> sa�da
	//cout<<"Foi para setResWeight - calcFitnessSimulador" << endl;
	esn->setResWeight(indiv); 
	//cout<<"Saiu do setResWeight - calcFitnessSimulador" << endl;
	cout<< endl;

	for(int i = 0, j = batterry; i < numMov && j > 0; i++, j--){ // Enquanto ainda tenho mov para realizar (i < numMov) e ainda tenho bateria (j > 0)
		in = simulador->readSensor(10, gen);			
		//mov = esn->execute(in);				//Verifica a sa�da da ESN de acordo com a entrada
	
		//cout<<"Executou ESN" << endl;
		esn->ESNoutput(in, mov);
		//cout<<"Finalizou ESN" << endl;
		//cout<<"Battery: " << j << endl;
		//cout<<"numMov: " << i << endl;
		
		
		//Define qual movimento vai ser executado (movimento correspondete ao neuronio de maior ativa��o)
		int aux = 0; // Maior sa�da � a 0
		for(int k = 1; k < outputSize; k++) // Para cada sa�da, se for maior q aux, substitui
			if(mov[k] > mov[aux])
				aux = k;
		//Se o rob� bateu, finaliza a simula��o
		if(!simulador->execute(aux, 10, gen)){
			cout<<"**Robo Bateu**"<<endl;
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
	//double *mov;
	//double *in;
	double *in = new double[inputSize]; // Leituras dos sensores
	double *mov = new double[outputSize];
	Simulador *simulador = new Simulador(120,200, dynamicEnvironment, maxGen, gen);
	//ESNbp *esn;
	cout<<"Entrou no calcTrajeto" <<endl;
	//Pesos do reposit�rio
	cout<<"setResWeight - calcTrajeto" << endl;
	esn->setResWeight(indiv); 
	cout<<"Terminou setResWeight - calcTrajeto" << endl;
	
	int **pos;
	pos = new int*[2];
	pos[0] = new int[numMov+1];
	pos[1] = new int[numMov+1];
	pos[0][0] = simulador->getPosX();
	pos[1][0] = simulador->getPosY();
	
		
	cout<<"getPosX: "<< pos[0][0]<<endl;
	cout<<"getPosY: "<< pos[1][0]<<endl;
	
	cout<<"Antes do For -Calculo do Trajeto" << endl;
	int i, j;
	for(i = 0, j = batterry; i < numMov && j > 0; i++, j--){
		in = simulador->readSensor(10, gen);
		cout<<"Entrou do For - Calculo do Trajeto" << endl;		
		
		//mov = esn->execute(in);	     //Verifica a sa�da da ESN de acordo com a entrada
		//esn->ESNoutput(in, mov);
		cout<<"Executou ESN" << endl;	
		//Define qual movimento vai ser executado (movimento correspondete ao neuronio de maior ativa��o)
		int aux = 0;
		for(int k = 1; k < outputSize; k++)
			if(mov[k] > mov[aux])
				aux = k;
		
		//Se o rob� bateu, finaliza a simula��o
		if(!simulador->execute(aux, 10, gen)){
			cout<<"Robo Bateu"<<endl;
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
	
		cout<<"Terminou For - Calculo do Trajeto" << endl;
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
