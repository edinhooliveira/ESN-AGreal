/*******************************************************************************************************\
*					Class: Echo State Network (ESN) 	  			 		  			                *
.    																	                             	*
*   for ESN, see:																					 	*
*		Jaeger, H. (2001). The �echo state� approach to analysing and training 							*
*			recurrent neural networks-with an erratum note. Bonn, Germany: German National 				*
*			Research Center for Information Technology GMD Technical Report, 148(34), 1					*
\*******************************************************************************************************/

//-----------------------------------------	ESN.cpp -----------------------------------------//
#include <typeinfo>
#include "ESN.hpp"
#include "defTipo.hpp"
#include <iostream>
#include <cmath>

using namespace std; 

/******************************************************************************\
*								Default Constructor							   *
\******************************************************************************/

ESNbp::ESNbp(){
	//cout<<"Criou ESN vazia" << endl;
}

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
	W_in=aloc_matrixd (n_inp+1,n_hid);		
	W=aloc_matrixd (n_hid,n_hid);
	W_out=aloc_matrixd (n_hid+1,n_out);
	D=aloc_matrixd(n_train,n_out);
	hid_neurons= new neuron_rec [n_hid]; 

	// Neurons - Hidden Layer (Reservoir)
	for (int j=0;j<n_hid;j++){
		// Input weights
		for (int i=0;i<n_inp+1;i++)
			W_in[i][j]=(max_W-min_W)*random_dou () + min_W;						// random number between min_W and max_W					
		// Initializing Recurrences
		hid_neurons[j].rec=aloc_vectori(n_hid);
		hid_neurons[j].n_rec=0;	
		hid_neurons[j].h=0.0;		
	}
	// Recurrences
	for (int i=0;i<n_hid;i++){
		for (int j=0;j<n_hid;j++){
			if (random_dou()<con_density){
				hid_neurons[i].rec[ hid_neurons[i].n_rec ]=j;
				hid_neurons[i].n_rec=hid_neurons[i].n_rec+1;
				W[i][j]=(max_W-min_W)*random_dou () + min_W;					// random number between min_W and max_W	
			}
			else{
				W[i][j]=0.0;
			}				
		}	
	}
	// Scaling W to spectral_radius_d W	
	spectral_radius=largEig(W, n_hid, n_hid);									// Computing the spectral radius of W_temp
	if (!isnan(spectral_radius))
		for (int i=0;i<n_hid;i++)
			for (int j=0;j<n_hid;j++)
				W[i][j]=spectral_radius_d * W[i][j] / spectral_radius;			// Normalizing W to desired spectral radius (Scaling W to spectral_radius_d (1/spectral_radius) W)

	 	
}


/******************************************************************************\
*								 Destructor									   *
\******************************************************************************/
ESNbp::~ESNbp(void){

	// Memory Desallocation
	desaloc_matrixi(X,n_train);
	desaloc_matrixd(D,n_train);
	desaloc_matrixd (W_in, n_inp+1);
	desaloc_matrixd (W, n_hid);
	desaloc_matrixd (W_out,n_hid+1);
	for (int i=0;i<n_hid;i++)
		delete [] hid_neurons[i].rec;
	delete [] hid_neurons;

}  


///******************************************************************************\
//*								 Constructor2									   *
//\******************************************************************************/
ESNbp::ESNbp(int n_inp_par, int n_hid_par, int n_out_par, double spectral_radius_d, double con_density){
	
	double spectral_radius, min_W=-0.6, max_W=0.6;

	// Parameters of the ESN
	n_inp=n_inp_par;
	n_hid=n_hid_par;						// number of hidden neurons
	n_out=n_out_par;
	//n_stab=size_stab;
		
	// Parameters of the datasets
	//n_train=n_train_par;
	//i_train=0;
		
	// Memory Allocation
	//X=aloc_matrixi(n_train,n_inp);	
	W_in=aloc_matrixd (n_inp+1,n_hid);	
	W=aloc_matrixd (n_hid,n_hid);
	W_out=aloc_matrixd (n_hid+1,n_out);
	//D=aloc_matrixd(n_train,n_out);
	hid_neurons= new neuron_rec [n_hid];
	

	// Neurons - Hidden Layer (Reservoir)
	for (int j=0;j<n_hid;j++){
		// Input weights
		for (int i=0;i<n_inp+1;i++)
			W_in[i][j]=(max_W-min_W)*random_dou () + min_W;						// random number between min_W and max_W					
		// Initializing Recurrences
		hid_neurons[j].rec=aloc_vectori(n_hid);
		hid_neurons[j].n_rec=0;	
		hid_neurons[j].h=0.0;		
	}
	// Recurrences
	for (int i=0;i<n_hid;i++){
		for (int j=0;j<n_hid;j++){
			if (random_dou()<con_density){
				hid_neurons[i].rec[ hid_neurons[i].n_rec ]=j;
				hid_neurons[i].n_rec=hid_neurons[i].n_rec+1;
				W[i][j]=(max_W-min_W)*random_dou () + min_W;					// random number between min_W and max_W	
			}
			else{
				W[i][j]=0.0;
			}				
		}	
	}
	// Scaling W to spectral_radius_d W	
	spectral_radius=largEig(W, n_hid, n_hid);									// Computing the spectral radius of W_temp
	if (!isnan(spectral_radius))
		for (int i=0;i<n_hid;i++)
			for (int j=0;j<n_hid;j++)
				W[i][j]=spectral_radius_d * W[i][j] / spectral_radius;				// Normalizing W to desired spectral radius (Scaling W to spectral_radius_d (1/spectral_radius) W)


}


/**********************************************************************************************\
*		Generate the activations of the neurons in the hidden layer (reservoir) 			   *
\**********************************************************************************************/
void ESNbp::ESNActivationHid(double *x, double *h_new){
		int k;
		double u;	      

		for (int j=0;j<n_hid;j++){
			u=0.0;
			for (int i=0;i<n_inp+1;i++)
				u += x[i] * W_in[i][j];	
			for (int i=0 ; i<hid_neurons[j].n_rec ; i++){
				k=hid_neurons[j].rec[i]; 
				u += hid_neurons[k].h * W[k][i];
			}
			h_new[j]=tanh(1.0*u);							// Tangent hiperbolic with half-slope a=2						          			
		}
		h_new[n_hid]=1.0;									// bias
}

/**********************************************************************************************\
*		Generate the activations of the neurons in the output layer 	   					   *
\**********************************************************************************************/
void ESNbp::ESNActivationOut(double *x, double *h, double *y){
	double u;	      

	for (int j=0;j<n_out;j++){
			u=0.0;
			for (int i=0;i<n_hid+1;i++){
				u += h[i] * W_out[i][j];
		}	
			y[j]=u;							// linear
					
	}		
}


/******************************************************************************\
*					 Output (y) of the neural netwok for input x 			   *
\******************************************************************************/
void ESNbp::ESNoutput(double *x, double *y){
	double *h_new;
	h_new=aloc_vectord(n_hid+1);
	ESNActivationHid(x, h_new);				// compute the hidden layer (reservoir) activations
	ESNActivationOut(x, h_new,y);					// compute the output layer activations
	//printESNOperation(x,h_new,y);
	//printESN();
	for (int j=0;j<n_hid;j++)
		hid_neurons[j].h=h_new[j];
	delete [] h_new; 
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
		x[n_inp] = 1;
		
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
	int n_hidn;
	double **H, **Ht, **HtH, **HtH_i, **H_pinv;
  	     	
   	n_hidn=n_hid+1;			// number of hidden neurons + 1  	
  
  	// Memory Allocation  	   
  	H=aloc_matrixd(n_train,n_hidn);				// matrix with the activations of the n_hid neurons of hidden layer during the trainning
	Ht=aloc_matrixd(n_hidn,n_train); 
	HtH=aloc_matrixd(n_hidn,n_hidn); 
    HtH_i=aloc_matrixd(n_hidn,n_hidn); 
    H_pinv=aloc_matrixd(n_hidn,n_train); 
		
	// Computing outputs of the hidden layer (reservoir) for all inputs of the training set 
	ESNpreTrain(H);	
	//printTrainSet(H);	    		 		 		 	   
    // Regularized pseudoinverse of H    
	transpose(Ht, H, n_train, n_hidn);   								// Ht: transpose of H
	multMatrix(HtH, Ht, n_hidn, n_train, H, n_train, n_hidn);			// HtH: Ht*H
	inverse(HtH_i, n_hidn, HtH);										// HtH_i: inverse of HtH 
    multMatrix(H_pinv, HtH_i, n_hidn, n_hidn, Ht, n_hidn, n_train);		// H_pinv: Pseudo-inverse 
	// Computing W_out 
    multMatrix(W_out, H_pinv, n_hidn, n_train, D, n_train, n_out);
	//printESN();
	    
	// Memory Desallocation
	desaloc_matrixd (H,n_train);
    desaloc_matrixd(Ht,n_hidn); 
    desaloc_matrixd(HtH,n_hidn); 
    desaloc_matrixd(HtH_i,n_hidn);
    desaloc_matrixd(H_pinv,n_hidn);
	   
}


/*****************************************************************************\
*		Add example to the training dataset 			                	  *	
\*****************************************************************************/
//void ESNbp::addTrainSet(int *x, double *d){
void ESNbp::addTrainSet(double *x, double *d){
    
	// Inputs: store solution in X
	for (int i=0;i<n_inp;i++)
		X[i_train][i]=x[i];
			  
	// Desired output (mudar para colocar acoes) 
	for (int j=0;j<n_out;j++){
	  	D[i_train][j]=d[j];				// perturbation resulted in success
	}
			  	
	i_train++;
	
}


/******************************************************************************\
*								Print ESN information														   *
\******************************************************************************/
void ESNbp::printESN(void){
	
	cout<< "Echo State Network: "<<endl;
	cout<<" Number of Inputs: "<<n_inp<<endl;
	cout<<" Number of Neurons in Hidden Layer (reservoir): "<<n_hid<<endl;
	cout<<" Number of Outputs: "<<n_out<<endl;
	
	cout<<" Hidden Layer: "<<endl;
	for (int j=0;j<n_hid;j++){
			cout<<" Neuron: " <<j<<endl;
			cout<<"  W_in: ";	
			for (int i=0;i<n_inp+1;i++)
					cout<< W_in[i][j] << ", ";
			cout<<endl;
			cout<<"  W: ";	
			for (int i=0;i<n_hid;i++)
					cout<< W[i][j] << ", ";
			cout<<endl;
	}

	cout<<" Output Layer: "<<endl;
	for (int j=0;j<n_out;j++){
			cout<<" Neuron: " <<j<<endl;
			cout<<"  W_out: ";	
			for (int i=0;i<n_hid+1;i++)
					cout<< W_out[i][j] << ", ";
			cout<<endl;
	}
	//system("pause"); //comentado para teste
}


/******************************************************************************\
*								Print ESN operation information			   	   *
\******************************************************************************/
void ESNbp::printESNOperation(double *x, double *h, double *y){
	
	cout<< "Echo State Network: "<<endl;
	cout<<" Inputs: "<<endl;
	for (int i=0;i<n_inp;i++)		
		cout<< x[i] << ", ";
	cout<<endl;
	cout<<" Activation of the Hidden Neurons: "<<endl;
	for (int j=0;j<n_hid+1;j++)
		cout<< h[j] << ", ";
	cout<<endl;
	cout<<" Outputs: "<<endl;
	for (int i=0;i<n_out;i++)
		cout<< y[i] << ", ";
	cout<<endl;
	
}


/******************************************************************************\
*								Print Training Set						   		*
\******************************************************************************/
void ESNbp::printTrainSet(double **H){
	
	cout<< "Training Set: "<<endl;
	for (int n=0;n<n_train;n++){
			cout<<" Example: " <<n<<endl;

			cout<<"  Input: ";	
			for (int i=0;i<n_inp;i++)	
				cout<<" "<< X[n][i];						
			cout<<endl;
			
			cout<<"  Activation of the hidden neurons: ";	
			for (int j=0;j<n_hid+1;j++)
				cout<<" "<< H[n][j];
			cout<<endl;
			
			cout<<"  Desired Output: ";	
			for (int j=0;j<n_out;j++)
					cout<<" "<< D[n][j];
			cout<<endl;
	}
	system("pause");
	
}

//-----------------	setResWeight -----------------//
/* 
* W_out[][] � a matriz de pesos do reservat�rio para a camada de sa�da
*/
void ESNbp::setResWeight (double *weight) //modifica��o Eder
{	

	for (int j=0;j<n_out;j++){
		for (int i=0;i<n_hid+1;i++){ 
     		W_out[i][j] = weight[j*n_hid + i];
		}
	}

						
	// n�o faz sentido alterar o valor das recorr�ncias obtidas no treinamento
	//for(int i = 0; i < n_hid; i++) //altera��o de repSize para n_hid	
	//	neuron_rec->rec[i] = 0; //recorrence substituido por hid_neurons
			
}//setResWeight

//-----------------	getWout -----------------//

double** ESNbp::getWout (){
	
	return W_out;
}

//criar metodo para pegar dados da camada de sa�da

//-----------------	getWin -----------------//
/*
* retorna a matriz Win, contendo os pesos da camada de entrada
*/
double** ESNbp::getWin ()
{
	return W_in; //altera��o de Win para W_in	
}//getWin

//-----------------	setWin -----------------//
/*
* define os pesos de Win, pesos da camada de entrada
*/
void ESNbp::setWin (double **weight)
{
	//for(int i = 0; i < n_hid; i++)
	for(int i = 0; i < n_inp+1; i++){ //altera��o de repSize para n_hid	
		delete W_in[i]; //altera��o de Win para W_in
	}
	delete W_in; //altera��o de Win para W_in
	W_in = weight;
}//setWin

//-----------------	getW -----------------//
/*
* retorna a matriz W, contendo os pesos do reposit�rio
*/
double** ESNbp::getW ()
{
	return W;
}//getW

//-----------------	setW -----------------//
/*
* define os pesos de W, pesos do reposit�rio
*/
void ESNbp::setW (double **weight)
{
	for(int i = 0; i < n_hid; i++) //altera��o de repSize para n_hid	
		delete W[i];
	delete W;
	
	W = weight;
}//setW

