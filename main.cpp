#include <iostream>
#include "defTipo.hpp"
#include "Simulador.hpp"
#include <fstream>
#include "ESN.hpp"




using namespace std;

//Protótipo das funções de print
void printRobot(Simulador *simulador);
void printSensor(Simulador *simulador);


int main(void) {
	bool dynamicEnvironment = false;
	int maxGen = 10;
	int gen = 0;
	
	
	int conjunto_stab[10];
	int nrRodadas = 100;//500
	int size_stab = 10; //50
	
	std::ofstream myfile;
	myfile.open ("robot-data.csv");
		
	for(int rodadaAtual = 0; rodadaAtual < nrRodadas; rodadaAtual++) {
		int nrMov = 67; //10
		//int movimentos[nrMov];	
		
		//Simulador::Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen)
		Simulador *simulador = new Simulador(200, 200, 10, 20, 20, 90, dynamicEnvironment, maxGen);
		//Simulador *simulador = new Simulador(200,200, dynamicEnvironment, maxGen, gen);
	
		
		//Declaração da Rede Neural
		ESNbp *ESN = new ESNbp(inputSize, repSize, outputSize, nrMov, con_density, spectral_radius_d, size_stab); //size_stab?
	
		
		cout <<"RODADA: " << rodadaAtual << endl;
		cout <<"POSICAO INICIAL" <<endl;
		cout << "Inicial X: " << simulador->getPosX() << endl;
		cout << "Inicial Y: "  << simulador->getPosY() << endl;
		cout << "Angulo Inicial: " << simulador->getAngle() << endl;
		
		
		
		//Vetores para movimentos, posições
		
		//double movimentos[nrMov];
		int *movimentos = new int [nrMov]; //definir passo a passo
		int *posicoesX = new int [nrMov];
		int *posicoesY = new int [nrMov];
		//int *acoes = new int [nrMov];
		double *sensores = simulador->readSensor(10,0);
		
		
		/*
		//Loop para gerar movimentação aleatória do robô
		for(int i = 0; i < nrMov; i++ ) {
			acoes[i] = 0;//random_int(0, 4);
		}*/
		
		
		/*Movimentação teste, saindo da posição (20,20) e dar uma volta completa na área e voltar para a posição inicial. */
		double acoes[nrMov] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,};
		double acao;
		for(int i = 0; i < nrMov; i++ ) {
			//acao = random_int(0, 4); //gera uma ação aleatória
			acao = acoes[i];		
			simulador->execute(acao, 10 , 0); //valor do dist não está alterando a posição
			
			cout << "Movimento: " << i << endl;
			cout << "Acao: " << acao << endl;
			printRobot(simulador);
			sensores = simulador->readSensor(10, gen); //distancia medida sensor = 10	
			printSensor(simulador);
			
						
			//Adiciona ao arquivo .csv os valores de Posição e angulo do robô
			myfile << i<<','; //indice
			myfile << *posicoesX + i<<','; //posicao X
			myfile << *posicoesY + i<<','; //posicao Y
			myfile << simulador->getAngle()<<','; //angulo
			
			//Adiciona ao arquivo .csv os valores do sensores
			for(int j = 0; j < 4; j++){
				myfile << sensores[j]<<',';	
			}
			myfile << *(movimentos + i);
			myfile <<"\n";		
			
			cout<< endl;		
			
			if( i <= size_stab) { //definir tamanho de estabilização
				ESN->ESNstab(sensores); //conjunto de estabilização, utilizado para "inicializar"
			} else if( i <= size_stab + 100){ //size_stab + tam_conj_treinamento
				ESN->addTrainSet(sensores, acoes); 
			}	
	
		}
		ESN->ESNTrain();
			
	}	
		
		

	myfile.close();
	
	cout<<"\tFim do programa!"<<endl;
 	
	return 0;
}//main


//Funções de Print

void printRobot(Simulador *simulador) {
		//cout << "Ação: " <<  << endl;
		//cout << "FUNÇÃO PRINT SIMULADOR" <<endl;
		cout <<"POSICAO" <<endl;
		cout << "posicao X: " << simulador->getPosX() << endl;
		cout << "posicao Y: " << simulador->getPosY() << endl;
		cout << "Angulo: "  << simulador->getAngle() << endl;
		cout<< endl;
	
}

void printSensor(Simulador *simulador){
	
	double *sensores = simulador->readSensor(10,0);
	
	//cout << "FUNÇÃO PRINT SENSORES" <<endl;
	cout <<"SENSORES" <<endl;
	for(int j = 0; j < 4; j ++){
		
		cout<< "Sensor " << j <<": "<< *(sensores + j) << endl;
	
	}
}


