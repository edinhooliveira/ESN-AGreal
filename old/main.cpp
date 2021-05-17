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
	int maxGen = 2;
	int gen = 0;
	
	double** conjunto_stab;
	int nrRodadas = 5;//500
	int size_stab = 5; //50
	int nrMov = 100;  //67-volta completa
	
	std::ofstream myfile;
	myfile.open ("robot-data.csv");
	
	//ESN	
	double** inputs = new double*[nrMov * nrRodadas];
	double* outputs = new double[nrMov * nrRodadas];
	
	double* conjEstab = new double[size_stab];
	
	
	for(int rodadaAtual = 0; rodadaAtual < nrRodadas; rodadaAtual++) {
		
				
		//Simulador::Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen)
		Simulador *simulador = new Simulador(200, 200, 10, 20, 20, 90, dynamicEnvironment, maxGen);
		//Simulador *simulador = new Simulador(200,200, dynamicEnvironment, maxGen, gen);
	
		//Declaração da Rede Neural
		//ESNbp *ESN = new ESNbp(inputSize, repSize, outputSize, nrMov, con_density, spectral_radius_d, size_stab); //size_stab?
	
		
		cout <<"RODADA: " << rodadaAtual << endl;
		cout <<"POSICAO INICIAL" <<endl;
		cout << "Inicial X: " << simulador->getPosX() << endl;
		cout << "Inicial Y: "  << simulador->getPosY() << endl;
		cout << "Angulo: " << simulador->getAngle() << endl;
		cout<< endl;
			
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
		
		
		
		
		//ESN 
		ESNbp *ESN = new ESNbp(inputSize, repSize, outputSize, nrMov, con_density, spectral_radius_d, size_stab); //size_stab?
		//ESNbp(int n_inp_par, int n_hid_par, int n_out_par, int n_train_par, double con_density, double spectral_radius_d, int size_stab)
	
				
		/*Movimentação teste, saindo da posição (20,20) e dar uma volta completa na área e voltar para a posição inicial. */
		//double acoes[nrMov] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
		double acao;
		for(int mov = 0; mov < nrMov; mov++ ) {
			//acao = random_int(0, 4); //gera uma ação aleatória
			//acao = acoes[mov];		
			//simulador->execute(acao, 10 , 0);
						
		//MOVIMENTAÇÃO AUTOMÁTICA UTILIZANDO SENSORES
		
			if(sensores[0] == 0 && sensores[1] == 0 && sensores[2] == 0){
				simulador->execute(4, 10 , 0); // anda pra frente o valor do dist
				printRobot(simulador);
				*(movimentos + mov) = 4;
				cout<<"ACAO: " << *(movimentos + mov)<<endl;
				printSensor(simulador);
			} else if (sensores[0] == 1 && sensores[2] == 0){
				simulador->execute(2, 10 , 0); // anda pra esquerda o valor do dist
				printRobot(simulador);
				*(movimentos + mov) = 2;
				cout<<"ACAO: " << *(movimentos + mov)<<endl;
				printSensor(simulador);
			} else if (sensores[0] == 0 && sensores[2] == 1){
				simulador->execute(3, 10 , 0); // anda pra direita o valor do dist
				printRobot(simulador);
				*(movimentos + mov) = 3;
				cout<<"ACAO: " << *(movimentos + mov)<<endl;
				printSensor(simulador);
			} else if(sensores[1] == 1 ){
				simulador->execute(2, 10 , 0); // anda pra esquerda o valor do dist
				printRobot(simulador);
				*(movimentos + mov) = 2;
				cout<<"ACAO: " << *(movimentos + mov)<<endl;
				printSensor(simulador);
			}
				
			
			cout << "Movimento: " << mov << endl;
			//cout << "Acao: " << acao << endl;
			cout << "Acao: " << *(movimentos + mov) << endl;
			printRobot(simulador);
			sensores = simulador->readSensor(10, gen); //distancia medida sensor = 10	
			printSensor(simulador);
			
						
			//Adiciona ao arquivo .csv os valores de Posição e angulo do robô
			myfile << rodadaAtual <<','; //Rodada Atual
			myfile << mov <<','; //movimento
			//myfile << *posicoesX + mov<<','; //posicao X
			//myfile << *posicoesY + mov<<','; //posicao Y
			myfile << simulador->getPosX() <<','; //posicao X
			myfile << simulador->getPosY() <<','; //posicao Y
			myfile << simulador->getAngle()<<','; //angulo
			
			//Adiciona ao arquivo .csv os valores do sensores
			for(int j = 0; j < 4; j++){
				myfile << sensores[j]<<',';	
			}
			
			//myfile << *(movimentos + mov); //Ação
			//myfile << acao; //Ação
			myfile << *(movimentos + mov); //Ação
			myfile <<"\n";		
			
			
			//cout << (rodadaAtual+1) * (mov)  <<  endl;
        	inputs[(rodadaAtual+1) * (mov+1)] = simulador->readSensor(10, gen);
        	//cout<< inputs[(rodadaAtual+1) * (mov+1)][1] << endl;
			outputs[(rodadaAtual+1) * (mov+1)] = acao; 
			//cout<< outputs[(rodadaAtual+1) * (mov+1)] << endl; 
			
			 		
		}
		cout<< "* * FIM RODADA: " << rodadaAtual << " * *" << endl;   
		
//		cout<< "SEPARANDO CONJUNTO DE ESTABILIZAÇÃO" << endl;
//		if( rodadaAtual <= size_stab) { //definir tamanho de estabilização
//			ESN->ESNstab(sensores); //conjunto de estabilização, utilizado para "inicializar"
//			cout << rodadaAtual << "/t";
//		} else if( rodadaAtual > size_stab){ //size_stab + tam_conj_treinamento
//			ESN->addTrainSet(sensores, movimentos); 
//		}
		
//		cout<<"TREINAMENTO"<<endl;
//		ESN->ESNTrain();
		
			
		//ESN->ESNTrain();		
	}	
	myfile.close();
	

	
	cout<< endl;	
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
		cout<< endl;
}


//			//código estabilização
//			if(mov <= size_stab) { //definir tamanho de estabilização
//				//ESN->ESNstab(sensores); //conjunto de estabilização, utilizado para "inicializar"
//				conjunto_stab[mov] = sensores;
//			
//				
//			} //else if(i <= 100){ //size_stab + tam_conj_treinamento //size_stab + 100
//				//ESN->addTrainSet(sensores, acoes); 
//			//}
//			
//			for(int aux; aux < size_stab; aux++ ){
//				cout<<"Conjunto Estabilização"<<endl;
//				cout<< "Item Estab :" << *(conjunto_stab + aux) << endl;
//			}	
//			for(int aux; aux < 100; aux++ ){
//				cout<<"Conjunto Treinamento"<<endl;
//				cout<< "Item Estab :" << *(sensores + aux) << endl;
//			}




//		cout << "ENTRADADS DA ESN" <<  endl;
//		 
//		for(int x=0;x < (nrMov * nrRodadas) ;x++)  // loop 3 times for three lines
//    	{
//        	for(int y=0;y<4;y++)  // loop for the three elements on the line
//        	{	
//            	cout<<inputs[x][y];  // display the current element out of the array
//       		}
//    		cout<<endl;  // when the inner loop is done, go to a new line
//    	}
//    		
//    	cout << "SAIDAS EXEMPLO" <<  endl;
//		for(int z=0;z < (nrMov * nrRodadas); z++)  // loop 3 times for three lines
//		{
//			cout<<outputs[z] << "\t";
//			
//		}



//
//	//separar conjunto para estabilização
//	for(int i; i < size_stab; i++){
//		conjEstab[i] = inputs[i][0];
//	}
//	
//	//Estabilização
//	ESN->ESNstab(conjEstab); //conjunto de estabilização, utilizado para "inicializar"
//	
//	
//	ESN->addTrainSet(sensores, outputs); 
//	






