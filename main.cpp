#include <iostream>
#include "defTipo.hpp"
#include "Simulador.hpp"
#include <iostream>
#include <fstream>



using namespace std;

//Prot�tipo das fun��es de print
void printRobot(Simulador *simulador);
void printSensor(Simulador *simulador);


int main(void) {
	bool dynamicEnvironment = false;
	int maxGen = 10;
	int gen = 0;
	
	
	int conjunto_stab[50];
	int nrRodadas = 10;
	int size_stab = 10; //50
	
	std::ofstream myfile;
	myfile.open ("robot-data.csv");
		
	for(int rodadaAtual = 0; rodadaAtual < nrRodadas; rodadaAtual++) {
		int nrMov = 67; //10
		//int movimentos[nrMov];	
		
		//Simulador::Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen)
		Simulador *simulador = new Simulador(200, 200, 10, 20, 20, 90, dynamicEnvironment, maxGen);
		//Simulador *simulador = new Simulador(200,200, dynamicEnvironment, maxGen, gen);
	
		
		//Declara��o da Rede Neural
		//ESNbp *ESN = new ESNbp(inputSize, repSize, outputSize, nrMov, con_density, spectral_radius_d, size_stab); //size_stab?
	
		
		cout <<"RODADA: " << rodadaAtual << endl;
		cout <<"POSICAO INICIAL" <<endl;
		cout << "Inicial X: " << simulador->getPosX() << endl;
		cout << "Inicial Y: "  << simulador->getPosY() << endl;
		cout << "Angulo Inicial: " << simulador->getAngle() << endl;
		
		
		
		//Vetores para movimentos, posi��es
		
		//double movimentos[nrMov];
		int *movimentos = new int [nrMov]; //definir passo a passo
		int *posicoesX = new int [nrMov];
		int *posicoesY = new int [nrMov];
		//int *acoes = new int [nrMov];
		double *sensores = simulador->readSensor(10,0);
		
		
		/*
		//Loop para gerar movimenta��o aleat�ria do rob�
		for(int i = 0; i < nrMov; i++ ) {
			acoes[i] = 0;//random_int(0, 4);
		}*/
		
		
		/*Movimenta��o teste, saindo da posi��o (20,20) e dar uma volta completa na �rea e voltar para a posi��o inicial. */
		int acoes[nrMov] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,};
		int acao;
		for(int i = 0; i < nrMov; i++ ) {
			//acao = random_int(0, 4); //gera uma a��o aleat�ria
			acao = acoes[i];
			cout << "Acao: " << acao << endl;	
			simulador->execute(acao, 10 , 0); //valor do dist n�o est� alterando a posi��o
			
			posicoesX[i] = simulador->getPosX();
			posicoesY[i] = simulador->getPosY();
			
			cout << "movimentos: " << i << endl;
			cout << "posicoesX: " << posicoesX[i] << endl;
			cout << "posicoesY: " << posicoesY[i]<< endl;
			cout << "getAngle: "  << simulador->getAngle() << endl;
			
			sensores = simulador->readSensor(10, gen); //distancia medida sensor = 10
			
			printSensor(simulador);
			/*
			for(int j = 0; j < 6; j ++){	
				// cout<< "Sensor " << j <<": "<< sensores[j] << endl;
				cout<< "Sensor " << j <<": "<< *(sensores + j) << endl;
			} */
			
			//Adiciona ao arquivo .csv os valores de Posi��o e angulo do rob�
			myfile << i<<','; //indice
			myfile << *posicoesX + i<<','; //posicao X
			myfile << *posicoesY + i<<','; //posicao Y
			myfile << simulador->getAngle()<<','; //angulo
			
			//Adiciona ao arquivo .csv os valores do sensores
			for(int j = 0; j < 6; j++){
				myfile << sensores[j]<<',';	
			}
			myfile <<"\n";		
			
			cout<< endl;		
				
		}
		
	
	}

	myfile.close();
	
	cout<<"\tFim do programa!"<<endl;
 	
	return 0;
}//main


//Fun��es de Print

void printRobot(Simulador *simulador) {
		//cout << "A��o: " <<  << endl;
		//cout << "FUN��O PRINT SIMULADOR" <<endl;
		cout <<"POSICAO" <<endl;
		cout << "posicao X: " << simulador->getPosX() << endl;
		cout << "posicao Y: " << simulador->getPosY() << endl;
		cout << "getAngle: "  << simulador->getAngle() << endl;
		cout<< endl;
	
}

void printSensor(Simulador *simulador){
	
	double *sensores = simulador->readSensor(10,0);
	
	//cout << "FUN��O PRINT SENSORES" <<endl;
	cout <<"SENSORES" <<endl;
	for(int j = 0; j < 6; j ++){
		
		cout<< "Sensor " << j <<": "<< *(sensores + j) << endl;
	
	}
}


