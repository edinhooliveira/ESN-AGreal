/*******************************************************************************************************\
*					Class: Echo State Network (ESN) 	  			 		  			                *
.    																	                             	*
*   for ESN, see:																					 	*
*		Jaeger, H. (2001). The “echo state” approach to analysing and training 							*
*			recurrent neural networks-with an erratum note. Bonn, Germany: German National 				*
*			Research Center for Information Technology GMD Technical Report, 148(34), 1					*

\*******************************************************************************************************/

//-----------------------------------------	ESN.hpp -----------------------------------------//

#ifndef ESN_HPP
#define ESN_HPP

class ESNbp {
	private:
		int n_inp;															// number of units in the input layer
		int n_hid;															// number of neurons in the hidden layer
		int n_out;															// number of neurons in the output layer
	    int xmin_sc;														// parameter for scaling the inputs				
		int x_aux;															// parameter for scaling the inputs	
		int **X;															//  training set: inputs
		double **D;															// desired outputs for the examples in the training set
		double **W_in;														// weight matrix between input and hidden layer 1 (reservoir)
		double **W;															// weight matrix for the recurrent connections in the reservoir (hidden layer)
		double **W_out;														// weight matrix between hidden layer (reservoir) and output layers
		typedef struct{	
			int n_rec;														// number the recurrences in the same layer
			int *rec;														// vector indicating the recurrences in the same layer
			double h;														// activation
		} neuron_rec;														// recurrent neurons        
		neuron_rec *hid_neurons;        									// neurons in hidden layer (reservoir)
		
		void ESNActivationHid(double *x, double *h_new); 					// Generate the activations of the neurons in the hidden layer (reservoir) 	
		void ESNpreTrain(double **H); 										// ESN Pre-trainnning: generate the activations of the neurons of hidden layer for all training examples
		void ESNActivationOut(double *x, double *h, double *y); 			// Generate the activations of the neurons in the output layer 
	public:		
		int i_train;														// index of the example in the training set	
		int n_stab;										    				// number of interations for the stabilizing period 
		int n_train;														// size of the current training set  
		
		ESNbp(); //construtor vazio
		ESNbp(int n_inp_par, int n_hid_par, int n_out_par, int n_train_par, double con_density, double spectral_radius_d, int size_stab);
		//NOVO CONSTRUTOR
		ESNbp(int n_inp_par, int n_hid_par, int n_out_par, double spectral_radius_d, double con_density); //novo construtor
	    ~ESNbp(void); //destrutor
		void ESNstab(double *sol);												// ESN Stabilization Period	
		void ESNTrain(void); 												// ESN Training 	
		//void addTrainSet(int *sol, double *d); // Add example to the training dataset
		void addTrainSet(double *sol, double *d); 
		void printESN(void);												// Print ESN
		void printESNOperation(double *x, double *h, double *y);			// Print ESN operation information	
		void printTrainSet(double **H);										// Print Training Set	
		void ESNoutput(double *x, double *y); 								// Output (y) of the neural netwok for input x 
		
		//modificação Eder
		double** getWin(); //* retorna a matriz Win, contendo os pesos da camada de entrada
		void setWin(double **weight); //* define os pesos de Win, pesos da camada de entrada
		double** getW(); //* retorna a matriz W, contendo os pesos do repositório
		void setW(double **weight); // define os pesos de W, pesos do repositório
		void setResWeight(double *weight); //* weight[] é o novo vetor de pesos do reservatório para a camada de saída atualizado pelo AG
		double** getWout();  //* retorna a matriz Win, contendo os pesos da camada de saída
		
}; // class ESNbp

#endif // ESN_HPP
