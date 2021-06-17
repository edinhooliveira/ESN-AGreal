#include "defTipo.hpp"
#include "Simulador.hpp"
#include <iostream>
#include <fstream>


using namespace std;

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
	cout <<"SENSORES" <<endl;
	for(int j = 0; j < 4; j ++){
		cout<< "Sensor " << j <<": "<< *(sensores + j) << endl;
	}
}


// modificacao R
int programaUsuario(double *sensores) {
	int acao;
	//sensores[0] direita; sensores[1] frontal; sensores[2]  esquerda; sensores[3] 			; 1 ativado; 0 desativado
	// acao 	0 -- vira 45º; 		1 -- vira -45º ; 		2 -- vira 90º	; 		3 -- anda p/ frente dist cm 
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


/******************************************************************************\
*				  	Impressao na tela							 			 *
\******************************************************************************/
void impressao(populacao *pop , int gen) {
	cout <<"Geracao: "<< gen<<endl;
	cout <<"Individuo com melhor fitness: "<<pop->melhorIndividuo<<endl;
	cout <<"Fitness do melhor Individuo: "<<pop->maxFitness<<endl;
    cout <<"Media do Fitness da geracao: "<<pop->mediaFitness<<endl;
    cout <<"Taxa mutacao: "<< taxaMut <<endl;
    cout <<"Taxa crossover: "<< taxaCross <<endl<<endl<<endl;
}//impressao

/******************************************************************************\
*				  Menu					 			 *
\******************************************************************************/
int menu() {
	int op;
	return 0;
	do 
	{
		cout << "\t1 - Continuar de onde parou" << endl;
		cout << "\t0 - Reiniciar\n" << endl;
		cin >> op;
	}while (op != 0 && op != 1 );
	
	return op;
}
/******************************************************************************\
*				  	Inicializacao da populacao					 			 *
\******************************************************************************/
void inicializacao(int nroExec, int op) {
	switch(op)
	{
		case 0 : //Reiniciar
		{
			apaga_arquivos(nroExec);
			int gene, numIndiv = 0;
		
			while( numIndiv < tamPop) {
			 	for (gene = 0; gene < lcrom; gene++) {
		     		popVelha.indiv[numIndiv].cromossomo[gene] = randon->nextFloat(-1,1) ; 							
				}
		        popVelha.indiv[numIndiv].fitness = calcFitness( popVelha.indiv[numIndiv].cromossomo, 0);	// Armazena Fitness do Individuo
				numIndiv++;
			}
			estatistica( &popVelha,0);
			break;
		}
		case 1 : //Continuar de onde parou
			cout << "Lendo a populacao salva ...\n" << endl;
			ler_pop(nroExec);
			ler_esn(nroExec);
			break;
	}
	impressao(&popVelha,0);

}
/******************************************************************************\
*				  	Execução Algoritimo Genetico							 			 *
\******************************************************************************/
void algGen (int nroExec, int op) {
	int gen = 0; 
	
	inicializacao(nroExec, op);				// procedimento para inicialização das variáveis e da população 
	
	do {
		gen = gen + 1; 				// número de gerações 
		geracao(gen);
		estatistica( &popNova , gen ); 

		individuo *aux;
		aux = popVelha.indiv;
		popVelha = popNova;
		popNova.indiv = aux;
		
		impressao(&popVelha,gen);
	} while ( gen < maxGen );
	//calcTrajeto (popVelha.indiv[popVelha.melhorIndividuo].cromossomo, nroExec, gen);		//Calcula e salva a trajetoria do melhor indiv da ultima geração
	arq_saida( nroExec);					// salva dados
}

/******************************************************************************\
*				  	main							 			 			  *
\******************************************************************************/

int main(void) {
	
	bool dynamicEnvironment = false;
	int maxGen = 1;
	int gen = 0;
	
	
	//Parametros do Treinamento da ESN
	//double** conjunto_stab;
	int nrRodadas = 2;//500  // modificacao R
	int size_stab = 50; //50  // modificacao R
	int nrMov = 500; //10  // modificacao R
	
	//ESN - Treinamento	
	double** inputs = new double*[nrMov * nrRodadas];
	double* outputs = new double[nrMov * nrRodadas];
	
	
	//inicialização do arquivo
	std::ofstream myfile;
	myfile.open ("robot-data.csv"); //arquivo com dados do treinamnento
	
	
	//TREINAMENTO DA ESN
	
	cout<<"***** Treinamento da ESN *****"<<endl;
	for(int rodadaAtual = 0; rodadaAtual < nrRodadas; rodadaAtual++) {
			
		//Simulador::Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen)
		//Simulador *simulador = new Simulador(200, 200, 0, 20, 20, 90, dynamicEnvironment, maxGen); // modificacao R
		Simulador *simulador = new Simulador(200,200, dynamicEnvironment, maxGen, gen); // modificacao R
	
		//Declaração da Rede Neural
		//ESNbp *ESN = new ESNbp(inputSize, repSize, outputSize, nrMov, con_density, spectral_radius_d, size_stab); //size_stab?
	
		cout <<"\t**RODADA**: " << rodadaAtual << endl;
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
		ESNbp *esn = new ESNbp(inputSize, repSize, outputSize, nrMov-size_stab-1, con_density, spectral_radius_d, size_stab);  // modificacao R
		//ESNbp(int n_inp_par, int n_hid_par, int n_out_par, int n_train_par, double con_density, double spectral_radius_d, int size_stab)
	
		/*Movimentação teste, saindo da posição (20,20) e dar uma volta completa na área e voltar para a posição inicial. */
		//double acoes[nrMov] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}; // modificacao R
		
		int acao;
		double acao_v[4];
		for(int mov = 0; mov < nrMov; mov++ ) {
			//acao = random_int(0, 4); //gera uma ação aleatória
			//acao = acoes[mov];	 // modificacao R
			acao = programaUsuario(sensores);	// modificacao R
			// simulador->execute(acao, 10 , 0); // // modificacao R
			
			//Se o robô bateu, gira
			if(!simulador->execute(acao, 10 ,0)){
				break;
			}
			cout << "Movimento: " << mov << endl;
			cout << "Acao: " << acao << endl;
			printRobot(simulador);
			//sensores = simulador->readSensor(10, gen); //distancia medida sensor = 10	// modificacao R
			sensores = simulador->readSensor(10, 0); //distancia medida sensor = 10	// modificacao R
			printSensor(simulador);
									
			//Adiciona ao arquivo .csv os valores de Posição e angulo do robô
			myfile << rodadaAtual <<','; //Rodada Atual
			myfile << mov <<','; //movimento
			myfile << simulador->getPosX() <<','; //posicao X
			myfile << simulador->getPosY() <<','; //posicao Y
			myfile << simulador->getAngle()<<','; //angulo
			
			//Adiciona ao arquivo .csv os valores do sensores
			for(int j = 0; j < 4; j++){
				myfile << sensores[j]<<',';	
			}
			
			//myfile << *(movimentos + mov); //Ação
			myfile << acao; //Ação
			myfile <<"\n";		
			
			//cout << (rodadaAtual+1) * (mov)  <<  endl;
        	//inputs[(rodadaAtual+1) * (mov+1)] = simulador->readSensor(10, gen); // modificacao R
        	inputs[(rodadaAtual+1) * (mov+1)] = sensores; // modificacao R
        	//cout<< inputs[(rodadaAtual+1) * (mov+1)][1] << endl;
			outputs[(rodadaAtual+1) * (mov+1)] = acao; 
			//cout<< outputs[(rodadaAtual+1) * (mov+1)] << endl; 
			
			//if( rodadaAtual <= size_stab) { //definir tamanho de estabilização // modificacao R
			
			if( mov <= size_stab) { //definir tamanho de estabilização // modificacao R
				esn->ESNstab(sensores); //conjunto de estabilização, utilizado para "inicializar"
			}
			else{
			//} else if( rodadaAtual > size_stab){ //size_stab + tam_conj_treinamento // modificacao R
				// modificacao R
				for(int j = 0; j < outputSize; j++)
					acao_v[j]=0.0;
				acao_v[acao]=1.0;
				
				esn->addTrainSet(sensores, acao_v); // modificacao R
			} 		
		}
		cout<< "* * FIM RODADA: " << rodadaAtual << " * *" << endl;   
		cout<<"TREINAMENTO"<<endl;
		esn->ESNTrain();
		
		if(rodadaAtual == nrRodadas-1){
			//esn->printESN()
			cout<< "GRAVANDO NO ARQUIVO"<<endl;
			salv_esn(rodadaAtual);
			
		}
		//ESN->printTrainSet();
		//ESN->printESN();	
	}
	
	salv_esn(1);
	
	
	//ALGORITMO GENÉTICO
		
//	int nroExec, num_ind;
//
//	arq_media_fitness = new double [maxGen+1];
//	arq_melhor_fitness = new double [maxGen+1];
//	popVelha.indiv = new individuo [tamPop];
//	popNova.indiv = new individuo [tamPop];
//	for (num_ind = 0; num_ind < tamPop; num_ind++){
//		popVelha.indiv[num_ind].cromossomo = new double [lcrom];
//		popNova.indiv[num_ind].cromossomo = new double [lcrom];
//	}
//	arq_melhor_individuo = aloc_matrixd(maxGen+1,lcrom);	
//		
//	// Execucao	
//	cout<<"***** Algoritmo Genetico *****"<<endl;
//	int op = menu();
//	
//	for(nroExec = 0; nroExec < nroMaxExec; nroExec++) {	
//		// Visualizacao
//		cout<<"\tExecucao: "<<nroExec<<endl<<endl;
//		randon = new Randon(1,nroExec+1);					// semente para gerar os numeros aleatorios
//		srand(nroExec+1);									// semente para gerar os numeros aleatorios
//		esn = new ESNbp(inputSize, repSize, outputSize, spectral_radius_d, con_density);
//		algGen(nroExec, op);
//		
//		delete esn;
//		delete randon;								// chama a execucao do AG para uma semente aleatoria
//	}//for
//
//	// Desalocacao de Memoria
//	delete [] arq_media_fitness;
//	delete [] arq_melhor_fitness;
//	for (num_ind=0; num_ind<tamPop; num_ind++){	
//		delete [] popVelha.indiv[num_ind].cromossomo;
//		delete [] popNova.indiv[num_ind].cromossomo;
//	}
//	delete [] popVelha.indiv;
//	delete [] popNova.indiv;
//	desaloc_matrixd(arq_melhor_individuo,maxGen+1);
//
//	// Visualizacao 
//	//cout<<"\tFim do programa!"<<endl;
	
	
	cout<< endl;	
	cout<<"\tFim do programa!"<<endl;
 	
	return 0;
}//main




