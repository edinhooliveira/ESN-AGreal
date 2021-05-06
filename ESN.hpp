/*******************************************************************************************************\
*					Class: Echo State Network (ESN) 	  			 		  			*
.    																		*
*   for ESN, see:																					 	*
*		Jaeger, H. (2001). The “echo state” approach to analysing and training 							*
*			recurrent neural networks-with an erratum note. Bonn, Germany: German National 				*
*			Research Center for Information Technology GMD Technical Report, 148(34), 1					*

\*******************************************************************************************************/

#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include "defTipo.hpp"
#include "util_funcions.cpp"


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
	public:		
		int i_train;														// index of the example in the training set	
		int n_stab;										    				// number of interations for the stabilizing period 
		int n_train;														// size of the current training set  
		ESNbp(int n_inp_par, int n_hid_par, int n_out_par, int n_train_par, double con_density, double spectral_radius_d, int size_stab);
	    ~ESNbp(void);
		void ESNstab(double *sol);												// ESN Stabilization Period	
		void ESNTrain(void); 												// ESN Training 	
		//void addTrainSet(int *sol, double *d); // Add example to the training dataset
		void addTrainSet(double *sol, double *d); 
};


/******************************************************************************\
*								Constructor									   *
\******************************************************************************/
ESNbp::ESNbp(int n_inp_par, int n_hid_par, int n_out_par, int n_train_par, double con_density, double spectral_radius_d, int size_stab){
	double spectral_radius, min_W=-0.6, max_W=0.6;

	// Parameters of the ESN
	n_inp=n_inp_par;
	n_hid=n_hid_par;						// number of hidden neurons
	n_out=n_out_par;
	n_stab=size_stab;
		
	// Parameters of the datasets
	n_train=n_train_par;
	i_train=0;
	
 
		
	// Memory Allocation
	X=aloc_matrixi(n_train,n_inp);	
	W_in=aloc_matrixd (n_hid+1,n_inp);		
	W=aloc_matrixd (n_hid,n_hid);
	W_out=aloc_matrixd (n_out,n_hid+1);
	D=aloc_matrixd(n_train,n_out);
	hid_neurons= new neuron_rec [n_hid]; 

	// Neurons - Hidden Layer (Reservoir)
	for (int i=0;i<n_hid;i++){
		// Input weights
		for (int j=0;j<n_inp+1;j++)
			W_in[i][j]=(max_W-min_W)*random_dou () + min_W;						// random number between min_W and max_W					
		// Initializing Recurrences
		hid_neurons[i].rec=aloc_vectori (n_hid);
		hid_neurons[i].n_rec=0;
		for (int j=0;j<n_hid;j++)
			W[i][j]=0.0;	
	}
	// Recurrences
	for (int i=0;i<n_hid;i++){
		for (int j=0;j<n_inp;j++){
			if (random_dou()<con_density){
				hid_neurons[i].rec[ hid_neurons[i].n_rec ]=j;
				hid_neurons[i].n_rec=hid_neurons[i].n_rec+1;
				W[i][j]=(max_W-min_W)*random_dou () + min_W;					// random number between min_W and max_W	
			}				
		}	
	}
	// Scaling W to spectral_radius_d W	
	spectral_radius=largEig(W, n_hid, n_hid);									// Computing the spectral radius of W_temp
	for (int i=0;i<n_hid;i++)
		for (int j=0;j<n_hid;j++)
			W[i][j]=spectral_radius_d * W[i][j] / spectral_radius;				// Normalizing W to desired spectral radius (Scaling W to spectral_radius_d (1/spectral_radius) W)



	 	
}


/******************************************************************************\
*								 Destructor									   *
\******************************************************************************/
ESNbp::~ESNbp(void){	

	// Memory Desallocation
	desaloc_matrixi(X,n_train);
	desaloc_matrixd(D,n_train);
	desaloc_matrixd (W_in, n_hid);
	desaloc_matrixd (W, n_hid);
	desaloc_matrixd (W_out,n_out);
	for (int i=0;i<n_hid;i++)
		delete [] hid_neurons[i].rec;
	delete [] hid_neurons;

}  


/**********************************************************************************************\
*		Generate the activations of the neurons in the hidden layer (reservoir) 			   *
\**********************************************************************************************/
void ESNbp::ESNActivationHid(double *x, double *h_new){
		int k;
		double u;	      
        
		for (int i=0;i<n_hid;i++){
			u=0.0;
			for (int j=0;j<n_inp+1;j++)
				u += x[j] * W_in[i][j];
			for (int j=0 ; j<hid_neurons[i].n_rec ; j++){
				k=hid_neurons[i].rec[j]; 
				u += hid_neurons[k].h * W[i][k];
			}
			h_new[i]=tanh(1.0*u);							// Tangent hiperbolic with half-slope a=2						          			
		}
		h_new[n_hid]=1.0;									// bias
		
}


/*****************************************************************************************************************************\
*	ESN Stabilization Period			          																			  *
\*****************************************************************************************************************************/
void ESNbp::ESNstab(double *x){   
	double *h_new;
	   
   
    
	h_new=aloc_vectord(n_hid+1);

	// Activation of the neurons in hidden layer 
	ESNActivationHid(x, h_new);			// compute the hidden layer (reservoir) activations
	for (int j=0;j<n_hid;j++)
		hid_neurons[j].h=h_new[j];
		 

	delete [] h_new; 
      
}


/*****************************************************************************************************************************\
*	ESN Pre-trainnning: generate the activations of the neurons of hidden layer for all training examples			          *
\*****************************************************************************************************************************/
void ESNbp::ESNpreTrain(double **H){   
	double *x, *h_new;   
   
    x=aloc_vectord(n_inp+1);     
	h_new=aloc_vectord(n_hid+1);

	// Activation of the neurons in hidden layer for all examples of the training set
	for (int n=0;n<n_train;n++) {
		for (int i=0;i<n_inp;i++)	
			x[i]=X[n][i];				

		x[n_inp + 1] = 1;
		
		ESNActivationHid(x, h_new);			// compute the hidden layer (reservoir) activations
		for (int j=0;j<n_hid;j++){	
			hid_neurons[j].h=h_new[j];
			H[n][j]=h_new[j];
		}
		H[n][n_hid]=1.0;					// bias
	}
	
	 
	delete [] x;  
	delete [] h_new; 
      
}


/*****************************************************************************************************************************\
*	ESN Training 			   																							  	  *
\*****************************************************************************************************************************/
void ESNbp::ESNTrain(void){
	int n_hid1;
	double **H, **Ht, **HtH, **HtH_i, **H_pinv;
  	     	
  	n_hid1=n_hid+1;			// number of hidden neurons + 1  	
  
  	// Memory Allocation  	   
  	H=aloc_matrixd(n_train,n_hid1);				// matrix with the activations of the n_hid neurons of hidden layer during the trainning
	Ht=aloc_matrixd(n_hid1,n_train); 
	HtH=aloc_matrixd(n_hid1,n_hid1); 
    HtH_i=aloc_matrixd(n_hid1,n_hid1); 
    H_pinv=aloc_matrixd(n_hid1,n_train); 
		
	// Computing outputs of the hidden layer (reservoir) for all inputs of the training set 
	ESNpreTrain(H);	
	//printTrainSet(H);	    		 		 		 	   
    // Regularized pseudoinverse of H    
	transpose(Ht, H, n_train, n_hid1);   								// Ht: transpose of H
	multMatrix(HtH, Ht, n_hid1, n_train, H, n_train, n_hid1);			// HtH: Ht*H
    inverse(HtH_i, n_hid1, HtH);										// HtH_i: inverse of HtH 
    multMatrix(H_pinv, HtH_i, n_hid1, n_hid1, Ht, n_hid1, n_train);		// H_pinv: Pseudo-inverse    
	// Computing W_out 
    multMatrix(W_out, H_pinv, n_hid1, n_train, D, n_train, n_out);
	//printESN();
	    
	// Memory Desallocation
    desaloc_matrixd(Ht,n_hid1); 
    desaloc_matrixd(HtH,n_hid1); 
    desaloc_matrixd(HtH_i,n_hid1);
    desaloc_matrixd(H_pinv,n_hid1);
	desaloc_matrixd (H,n_train);
	   
}


/*****************************************************************************\
*		Add example to the training dataset 			                	  *	
\*****************************************************************************/
// void ESNbp::addTrainSet(int *x, double *d){
void ESNbp::addTrainSet(double *x, double *d){
    
	// Inputs: store solution in X
	for (int i=0;i<n_inp;i++)
		X[i_train][i]=x[i];
			  
	// Desired output (mudar para colocar acoes 
	for (int j=0;j<n_out;j++){
	  	D[i_train][j]=d[j];				// perturbation resulted in success
	}
			  	
	i_train++;
	
}




