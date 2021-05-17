#include <iostream>
#include "defTipo.hpp"
#include "Simulador.hpp"
#include <fstream>
#include "ESN.hpp"

using namespace std;

//Prot�tipo das fun��es de print
void printRobot(Simulador *simulador);
void printSensor(Simulador *simulador);

 // modificacao R
int programaUsuario(double *sensores) {
	int acao;
	//sensores[0] direita; sensores[1] frontal; sensores[2]  esquerda; sensores[3] 			; 1 ativado; 0 desativado
	// acao 	0 -- vira 45�; 		1 -- vira -45� ; 		2 -- vira 90�	; 		3 -- anda p/ frente dist cm 
	if (sensores[1]<0.01)
		acao=3;
	else if (sensores[0]<0.01)
		acao=1;
	else if (sensores[2]<0.01)
		acao=0;
	else
		acao=2;				
	return acao;
}

int main(void) {
	bool dynamicEnvironment = false;
	int maxGen = 1;
	int gen = 0;
	
	//double** conjunto_stab;
	int nrRodadas = 2;//500  // modificacao R
	int size_stab = 50; //50  // modificacao R
	int nrMov = 500; //10  // modificacao R
	
	std::ofstream myfile;
	myfile.open ("robot-data.csv");
	
	//ESN	
	double** inputs = new double*[nrMov * nrRodadas];
	double* outputs = new double[nrMov * nrRodadas];
	
	//double* conjEstab = new double[size_stab];
	for(int rodadaAtual = 0; rodadaAtual < nrRodadas; rodadaAtual++) {
			
		//Simulador::Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen)
		//Simulador *simulador = new Simulador(200, 200, 0, 20, 20, 90, dynamicEnvironment, maxGen); // modificacao R
		Simulador *simulador = new Simulador(200,200, dynamicEnvironment, maxGen, gen); // modificacao R
	
		//Declara��o da Rede Neural
		//ESNbp *ESN = new ESNbp(inputSize, repSize, outputSize, nrMov, con_density, spectral_radius_d, size_stab); //size_stab?
	
		cout <<"\t**RODADA**: " << rodadaAtual << endl;
		cout <<"POSICAO INICIAL" <<endl;
		cout << "Inicial X: " << simulador->getPosX() << endl;
		cout << "Inicial Y: "  << simulador->getPosY() << endl;
		cout << "Angulo: " << simulador->getAngle() << endl;
		cout<< endl;
			
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
		
		//ESN 
		ESNbp *ESN = new ESNbp(inputSize, repSize, outputSize, nrMov-size_stab-1, con_density, spectral_radius_d, size_stab); //size_stab?  // modificacao R
		//ESNbp(int n_inp_par, int n_hid_par, int n_out_par, int n_train_par, double con_density, double spectral_radius_d, int size_stab)
	
		/*Movimenta��o teste, saindo da posi��o (20,20) e dar uma volta completa na �rea e voltar para a posi��o inicial. */
		//double acoes[nrMov] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}; // modificacao R
		int acao;
		double acao_v[4];
		for(int mov = 0; mov < nrMov; mov++ ) {
			//acao = random_int(0, 4); //gera uma a��o aleat�ria
			//acao = acoes[mov];	 // modificacao R
			acao = programaUsuario(sensores);	// modificacao R
			// simulador->execute(acao, 10 , 0); // // modificacao R
			
			//Se o rob� bateu, gira
			if(!simulador->execute(acao, 10 ,0)){
				break;
			}
			cout << "Movimento: " << mov << endl;
			cout << "Acao: " << acao << endl;
			printRobot(simulador);
			//sensores = simulador->readSensor(10, gen); //distancia medida sensor = 10	// modificacao R
			sensores = simulador->readSensor(10, 0); //distancia medida sensor = 10	// modificacao R
			printSensor(simulador);
									
			//Adiciona ao arquivo .csv os valores de Posi��o e angulo do rob�
			myfile << rodadaAtual <<','; //Rodada Atual
			myfile << mov <<','; //movimento
			myfile << simulador->getPosX() <<','; //posicao X
			myfile << simulador->getPosY() <<','; //posicao Y
			myfile << simulador->getAngle()<<','; //angulo
			
			//Adiciona ao arquivo .csv os valores do sensores
			for(int j = 0; j < 4; j++){
				myfile << sensores[j]<<',';	
			}
			
			//myfile << *(movimentos + mov); //A��o
			myfile << acao; //A��o
			myfile <<"\n";		
			
			//cout << (rodadaAtual+1) * (mov)  <<  endl;
        	//inputs[(rodadaAtual+1) * (mov+1)] = simulador->readSensor(10, gen); // modificacao R
        	inputs[(rodadaAtual+1) * (mov+1)] = sensores; // modificacao R
        	//cout<< inputs[(rodadaAtual+1) * (mov+1)][1] << endl;
			outputs[(rodadaAtual+1) * (mov+1)] = acao; 
			//cout<< outputs[(rodadaAtual+1) * (mov+1)] << endl; 
			
			//if( rodadaAtual <= size_stab) { //definir tamanho de estabiliza��o // modificacao R
			
			if( mov <= size_stab) { //definir tamanho de estabiliza��o // modificacao R
				ESN->ESNstab(sensores); //conjunto de estabiliza��o, utilizado para "inicializar"
			}
			else{
			//} else if( rodadaAtual > size_stab){ //size_stab + tam_conj_treinamento // modificacao R
				// modificacao R
				for(int j = 0; j < outputSize; j++)
					acao_v[j]=0.0;
				acao_v[acao]=1.0;
				
				ESN->addTrainSet(sensores, acao_v); // modificacao R
			} 		
		}
		cout<< "* * FIM RODADA: " << rodadaAtual << " * *" << endl;   
		cout<<"TREINAMENTO"<<endl;
		ESN->ESNTrain();
		//ESN->printTrainSet();
		//ESN->printESN();	
	}	
	myfile.close();
	cout<< endl;	
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
		cout << "Angulo: "  << simulador->getAngle() << endl;
		cout<< endl;	
}

void printSensor(Simulador *simulador){
	
	double *sensores = simulador->readSensor(10,0);
	cout <<"SENSORES" <<endl;
	for(int j = 0; j < 4; j ++){
		cout<< "Sensor " << j <<": "<< *(sensores + j) << endl;
	}
}

