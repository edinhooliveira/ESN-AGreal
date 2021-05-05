//#include <iostream>
//#include "defTipo.hpp"
//#include "Simulador.hpp"
//#include <iostream>
//#include <fstream>
//
//#include "ESN.h"
//
//using namespace std;
//void printRobot(Simulador *simulador) {
//		//cout << "Ação: " <<  << endl;
//		cout <<"POSICAO" <<endl;
//		cout << "posicao X: " << simulador->getPosX() << endl;
//		cout << "posicao Y: " << simulador->getPosY() << endl;
//		cout << "getAngle: "  << simulador->getAngle() << endl;
//		cout<< endl;
//	
//}
//
//void printSensor(Simulador *simulador){
//	
//	double *sensores = simulador->readSensor(10,0);
//	
//	cout <<"SENSORES" <<endl;
//	for(int j = 0; j < 6; j ++){
//		
//		cout<< "Sensor " << j <<": "<< *(sensores + j) << endl;
//	
//	}
//}
//	
//int main(void) {
//	bool dynamicEnvironment = true;
//	int maxGen = 1000;
//	int gen = 450;
//	int nrMov  = 10; //color os primeiros movimentos para estabilização
//	double movimentos[nrMov];
//	int conjunto_stab[50];
//	int nrRodadas = 10;
//	int size_stab = 50;
//	for(int k = 0; k < nrRodadas; k++ ) {
//		
//	//int nrMov  = 10;
//	//int movimentos[nrMov];
//	
//	//Simulador::Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen)
//	//Simulador *simulador = new Simulador(200, 200, 10, 100, 100, 90 , dynamicEnvironment, maxGen);
//	Simulador *simulador = new Simulador(200,100, dynamicEnvironment, maxGen, gen);
//	ESNbp *ESN = new ESNbp(inputSize, repSize, outputSize, nrMov, con_density, spectral_radius_d, size_stab); //size_stab?
//	//ESNbp(int n_inp_par, int n_hid_par, int n_out_par, int n_train_par, double con_density, double spectral_radius_d, int size_stab)
//	
//	std::ofstream myfile;
//	myfile.open ("example.csv");	
//	
//	//int nrMov  = 10;
//	//int nrRodadas = 10;
//	//int movimentos[nrMov];
//	
//		
//		
//	cout <<"RODADA: " << k << endl;
//	cout <<"POSICAO INICIAL" <<endl;
//	cout << "inicial X: " << simulador->getPosX() << endl;
//	cout << "inicial Y: "  << simulador->getPosY() << endl;
//	cout << "inicial Angle: "  << simulador->getAngle() << endl;
//	cout<< endl;
//	
//	//int nrMov  = 100;
//	double *sensores;
//	//int movimentos[nrMov];
//	//double *in, out;
//	
//	//delete esn;
//		
//	//executa até a quantidade de movimentos determinada
//	for(int i = 0; i < nrMov; i++ ) {
//		
//		sensores = simulador->readSensor(10, 0); //distancia medida sensor = 10
//		double *in = sensores;
//		double *out;
//		
//		/*	
//			*Ação
//		0 -- vira 45º
//		1 -- vira -45º	
//		2 -- vira 90º
//		3 -- vira -90º // 270	
//		4 -- anda p/ frente dist cm 
//		*/	
//		
//		/*
//			*Sensores
//		sensores[0] //direita -45
//		sensores[1] //frontal
//		sensores[2] //esquerda +45
//		sensores[3] //superior
//		sensores[4] //direita -90
//		sensores[5] //esquerda +90	
//		*/
//		
//		//cout<<"\t******"<<endl;
//		cout<<"MOVIMENTO: " << i <<endl;
//		
//		if(sensores[0] == 0 && sensores[1] == 0 && sensores[2] == 0){
//			simulador->execute(4, 10 , 0); // anda pra frente o valor do dist
//			printRobot(simulador);
//			*(movimentos + i) = 4;
//			cout<<"ACAO: " << *(movimentos + i)<<endl;
//			printSensor(simulador);
//		} else if (sensores[0] == 1 && sensores[2] == 0){
//			simulador->execute(0, 10 , 0); // anda pra esquerda o valor do dist
//			printRobot(simulador);
//			*(movimentos + i) = 0;
//			cout<<"ACAO: " << *(movimentos + i)<<endl;
//			printSensor(simulador);
//			*(movimentos + i) = 0;
//		} else if (sensores[0] == 0 && sensores[2] == 1){
//			simulador->execute(1, 10 , 0); // anda pra esquerda o valor do dist
//			printRobot(simulador);
//			*(movimentos + i) = 1;
//			cout<<"ACAO: " << *(movimentos + i)<<endl;
//			printSensor(simulador);
//			*(movimentos + i) = 1;
//		} else if(sensores[1] == 1 ){
//			simulador->execute(0, 10 , 0); // anda pra esquerda o valor do dist
//			printRobot(simulador);
//			*(movimentos + i) = 0;
//			cout<<"ACAO: " << *(movimentos + i)<<endl;
//			printSensor(simulador);
//			*(movimentos + i) = 0;
//		}
//		
//		//esn = new ESN(inputSize, repSize, outputSize, spectral_radius_d, con_density);
//		
//		//n, posX, posY, angle, sens0,sens1,sens2,sens3,sens4,sens5,acao
//		//Valores a serem salvos em arquivo
//		
//		myfile << k <<',',
//		myfile << i <<',';
//		myfile << simulador->getPosX()<<',';
//		myfile << simulador->getPosY()<<',';
//		myfile << simulador->getAngle()<<',';
//		myfile << sensores[0]<<',';
//		myfile << sensores[1]<<',';
//		myfile << sensores[2]<<',';
//		myfile << sensores[3]<<',';
//		myfile << sensores[4]<<',';
//		myfile << sensores[5]<<',';
//		myfile << *(movimentos + i);
//		myfile <<"\n";
//	
//	
//		
//	if( i <= size_stab) { //definir tamanho de estabilização
//		ESN->ESNstab(sensores); //conjunto de estabilização, utilizado para "inicializar"
//	} else if( i <= size_stab + 100){ //size_stab + tam_conj_treinamento
//		ESN->addTrainSet(sensores, movimentos); 
//	}
//	
//	}
//	
//	ESN->ESNTrain();
//	
//	myfile.close();
//	
//}
//	
//	cout<< endl;
//		
//	//Salvar valores de todos os sensores
//	//Salvar valores de todas as movimentações
//	
//	cout<<"LISTA DE MOVIMENTOS: "<<endl;
//	for(int i = 0; i < nrMov; i++){
//		cout<<"Movimento "<<i<<" : "<<movimentos[i]<<endl;
//	}
//	
//		cout<< endl;				
//	
//	cout<<"\tFim do programa!"<<endl;
//	
//	//Simulador *simulador = new Simulador(200,100, dynamicEnvironment, maxGen, gen);
//	//ESNbp *ESN = new ESNbp(inputSize, repSize, outputSize, nrMov, con_density, spectral_radius_d, stab_size);
//	//ESNbp(int n_inp_par, int n_hid_par, int n_out_par, int n_train_par, double con_density, double spectral_radius_d, int size_stab)
// 	
//	return 0;
//}//main

