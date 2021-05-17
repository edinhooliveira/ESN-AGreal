//#include <iostream>
//#include "defTipo.hpp"
//#include "Simulador.hpp"
//#include <iostream>
//#include <fstream>
//
//using namespace std;
//
//int main(void) {
//	bool dynamicEnvironment = false;
//	int maxGen = 10;
//	int gen = 0;
//	//Simulador::Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen)
//	Simulador *simulador = new Simulador(200, 200, 10, 20, 20, 90, dynamicEnvironment, maxGen);
//	//Simulador *simulador = new Simulador(200,200, dynamicEnvironment, maxGen, gen);
//	
//	std::ofstream myfile;
//	myfile.open ("example.csv");
//	
//	cout <<"POSICAO INICIAL" <<endl;
//	cout << "inicial X: " << simulador->getPosX() << endl;
//	cout << "inicial Y: "  << simulador->getPosY() << endl;
//	cout << "inicial Angle: "  << simulador->getAngle() << endl;
//	
//	int nrMov  = 67;
//	
//		
//	int *movimentos = new int [nrMov]; //definir passo a passo
//	int *posicoesX = new int [nrMov];
//	int *posicoesY = new int [nrMov];
//	//int *acoes = new int [nrMov];
//	double *sensores;
//		
//	/*
//	for(int i = 0; i < nrMov; i++ ) {
//		acoes[i] = 0;//random_int(0, 4);
//	} */
//	
//	/*Movimentação teste, saindo da posição (20,20) e dar uma volta completa na área e voltar para a posição inicial. */
//	int acoes[nrMov] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,};
//	int acao;
//	for(int i = 0; i < nrMov; i++ ) {
//		//acao = random_int(0, 4); //gera uma ação aleatória
//		acao = acoes[i];
//		cout << "Acao: " << acao << endl;	
//		simulador->execute(acao, 10 , 0); //valor do dist não está alterando a posição
//		
//		posicoesX[i] = simulador->getPosX();
//		posicoesY[i] = simulador->getPosY();
//		
//		cout << "movimentos: " << i << endl;
//		cout << "posicoesX: " << posicoesX[i] << endl;
//		cout << "posicoesY: " << posicoesY[i]<< endl;
//		cout << "getAngle: "  << simulador->getAngle() << endl;
//		
//		sensores = simulador->readSensor(10, gen); //distancia medida sensor = 10
//		
//		for(int j = 0; j < 6; j ++){
//			
//			// cout<< "Sensor " << j <<": "<< sensores[j] << endl;
//			cout<< "Sensor " << j <<": "<< *(sensores + j) << endl;
//			
//
//		}
//		//Valores a serem salvos em arquivo
//		myfile << i<<',';
//		myfile << *posicoesX + i<<',';
//		myfile << *posicoesY + i<<',';
//		myfile << simulador->getAngle()<<',';
//		myfile << sensores[0]<<',';
//		myfile << sensores[1]<<',';
//		myfile << sensores[2]<<',';
//		myfile << sensores[3]<<',';
//		myfile << sensores[4]<<',';
//		myfile << sensores[5]<<',';
//		myfile <<"\n";
//		
//		
//		
//		cout<< endl;		
//			
//	}
//	myfile.close();
//	
//	
//	cout<<"\tFim do programa!"<<endl;
// 	
//	return 0;
//}//main

