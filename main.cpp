#include "defTipo.hpp"
#include "Simulador.hpp"
#include <cstdlib>
#include <iostream>
//#include <fstream>

using namespace std;


//Funções de Print
void printRobot(Simulador *simulador) {
		
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
	cout<<"Entrou em inicialização..." <<endl;
	switch(op)
	{
		case 0 : //Reiniciar
		{
			apaga_arquivos(nroExec);
			int gene, numIndiv = 0;
		
			while(numIndiv < tamPop) {
			 	for (gene = 0; gene < lcrom; gene++) {
		     		popVelha.indiv[numIndiv].cromossomo[gene] = randon->nextFloat(-1,1) ; 							
				}
		        popVelha.indiv[numIndiv].fitness = calcFitness( popVelha.indiv[numIndiv].cromossomo, 0);	// Armazena Fitness do Individuo
				cout<<"Saiu do calcFitness"<<endl;
				numIndiv++;
				//system("pause");
			}
			//cout<<"Entrando em estatistica..."<<endl;
			estatistica( &popVelha,0);
			break;
		}
		case 1 : //Continuar de onde parou
			cout << "Lendo a populacao salva ...\n" << endl;
			ler_pop(nroExec);
			ler_esn(nroExec);
			break;
	}
	cout<<"Entrando em Impressão da pop..." <<endl;
	impressao(&popVelha,0);
	cout<<"Saiu inicialização..." <<endl;

}
/******************************************************************************\
*				  	Execução Algoritimo Genetico							 			 *
\******************************************************************************/
void algGen(int nroExec, int op) {
	int gen = 1; 
	inicializacao(nroExec, op);				// procedimento para inicialização das variáveis e da população 
	
	do {	
		
		//cout<<"Do - Geracao: " << gen << " Do - maxGen: "<< maxGen<<endl;
		gen = gen + 1; 				// número de gerações 
		//cout<<"Entrou em Geracao" <<endl;
		geracao(gen);
		//cout<<"Saiu de Geracao" <<endl; 
		//cout<<"Entrou em Estatistica" <<endl;
		estatistica( &popNova , gen );
		//cout<<"Saiu de Estatistica" <<endl; 

		individuo *aux;
		aux = popVelha.indiv;
		popVelha = popNova;
		popNova.indiv = aux;
		cout<<endl;
		cout<<"Impressao do Resultados da Exec: "<<nroExec << endl;
		impressao(&popVelha,gen);
		arq_saida(nroExec);
	} while ( gen < maxGen );
	//cout<<"While - Geracao: " << gen << " While - maxGen: "<< maxGen<<endl;
	//cout<<"Entrou no while do calcTrajeto" << endl;
	//cout<<"Geracao: " << gen << " maxGen: "<< maxGen<<endl;
	calcTrajeto (popVelha.indiv[popVelha.melhorIndividuo].cromossomo, nroExec, gen);		//Calcula e salva a trajetoria do melhor indiv da ultima geração
	//cout<<"********Saiu do calcTrajeto" << endl;
	cout<<"Salvando Arquivos do AlgGen..." <<endl;
	arq_saida(nroExec);					// salva dados
}


/******************************************************************************\
*				  	main							 			 			  *
\******************************************************************************/

int main(void) {
	
	bool dynamicEnvironment = false;
	//int maxGen = 10; //1
	int gen = 0;
	
	//Parametros do Treinamento da ESN
	//double** conjunto_stab;
	
	//int nrRodadas = 2;//500  // modificacao R -mover para defValor
	//int size_stab = 50; //50  // modificacao R -mover para defValor
	//int nrMov = 500;//500 //10  // modificacao R -mover para defValor
	
	//ESN - Treinamento	
	double** inputs = new double*[numMov * nrRodadas]; //nrMov
	double* outputs = new double[numMov * nrRodadas]; //nrMov
	
	//TREINAMENTO DA ESN	
	cout<<"***** Treinamento da ESN *****"<<endl;
	for(int rodadaAtual = 0; rodadaAtual < nrRodadas; rodadaAtual++) {
			
		//Simulador::Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen)
		//Simulador *simulador = new Simulador(200, 200, 0, 20, 20, 90, dynamicEnvironment, maxGen); // modificacao R
		Simulador *simulador = new Simulador(120,200, dynamicEnvironment, maxGen, gen); // modificacao R
	
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
		int *movimentos = new int [numMov]; //definir passo a passo //nrMov
		int *posicoesX = new int [numMov]; //nrMov
		int *posicoesY = new int [numMov]; //nrMov
		//int *acoes = new int [nrMov];
		double *sensores = simulador->readSensor(10,0);
		
		//Declaração da Rede Neural
		/*ESNbp * */
		esn = new ESNbp(inputSize, repSize, outputSize, numMov-size_stab-1, con_density, spectral_radius_d, size_stab);  // modificacao R
		//ESNbp(int n_inp_par, int n_hid_par, int n_out_par, int n_train_par, double con_density, double spectral_radius_d, int size_stab)
		//esn = new ESN(inputSize, repSize, outputSize, spectral_radius_d, con_density);
		int acao;
		double acao_v[4];
		for(int mov = 0; mov < numMov; mov++ ) { //nrMov
			//acao = acoes[mov];	 // modificacao R
			acao = programaUsuario(sensores);	// modificacao R
			// simulador->execute(acao, 10 , 0); // // modificacao R
			
			//Se o robô bateu, gira
			if(!simulador->execute(acao, 10 ,0)){
				break;
			}
			//cout << "Movimento: " << mov << endl;
			//cout << "Acao: " << acao << endl;
			//printRobot(simulador);
			//sensores = simulador->readSensor(10, gen); //distancia medida sensor = 10	// modificacao R
			sensores = simulador->readSensor(10, 0); //distancia medida sensor = 10	// modificacao R
			//printSensor(simulador);
									
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
		//cout<<"ESN-TREINAMENTO: "<<esn << endl;
		cout<<endl;
		esn->ESNTrain();
					
		if(rodadaAtual == nrRodadas - 1){
			cout<<"Fim ESN"<<endl;
			//esn->printESN();
			salv_esn_sup(rodadaAtual);		
		}	
	} //FIM TREINAMENTO ESN
	
	
	
	//ALGORITMO GENÉTICO
		
	int nroExec, num_ind;

	arq_media_fitness = new double [maxGen+1];
	arq_melhor_fitness = new double [maxGen+1];
	popVelha.indiv = new individuo [tamPop];
	popNova.indiv = new individuo [tamPop];
	for (num_ind = 0; num_ind < tamPop; num_ind++){
		popVelha.indiv[num_ind].cromossomo = new double [lcrom];
		popNova.indiv[num_ind].cromossomo = new double [lcrom];
	}
	arq_melhor_individuo = aloc_matrixd(maxGen+1,lcrom);	
	
	// Execucao	
	cout<<"***** Algoritmo Genetico *****"<<endl;
	//int op = menu();	
	int op = 0;
	
	for(nroExec = 0; nroExec < nroMaxExec; nroExec++) {	
		// Visualizacao
		cout<<"\tExecucao: "<<nroExec<<endl<<endl;
		randon = new Randon(1,nroExec+1);					// semente para gerar os numeros aleatorios
		srand(nroExec+1); 	// semente para gerar os numeros aleatorios
		//cout<<"ESN-AG: "<<esn << endl;								
		//esn = new ESNbp(inputSize, repSize, outputSize, spectral_radius_d, con_density);
		//cout<<"Entra no AlgGen:" << endl;	
		algGen(nroExec, op);
		//cout<<"Saiu do AlgGen:" << endl;
		
		delete esn;
		delete randon;								// chama a execucao do AG para uma semente aleatoria
	}//for
	
	cout<<"Desalocando Memoria do AG..."<<endl;
	// Desalocacao de Memoria
	delete [] arq_media_fitness;
	delete [] arq_melhor_fitness;
	for (num_ind=0; num_ind<tamPop; num_ind++){	
		delete [] popVelha.indiv[num_ind].cromossomo;
		delete [] popNova.indiv[num_ind].cromossomo;
	}
	delete [] popVelha.indiv;
	delete [] popNova.indiv;
	desaloc_matrixd(arq_melhor_individuo,maxGen+1);

	// Visualizacao 
	cout<<"\tFim do programa!"<<endl;
 	
	system("pause");
	return 0;
}//main




