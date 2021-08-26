//Author Rafael Del Lama
//Modificado por Eder Oliveira
 
//-----------------------------------------	TSimulador.cpp -----------------------------------------//
#include "Simulador.hpp"
#include "defTipo.hpp"
#include <cstdlib>
#include <iostream>


double arred(double x){
	x = x * 100;
	int aux = round(x);
	
	return ((double)aux/100.0);
}


//-----------------	simulador -----------------//
Simulador::Simulador(int tamX, int tamY, bool dynamicEnvironment, int maxGen, int gen){
	this->tamX = tamX; // Tamanho tablado eixo X
	this->tamY = tamY; // Tamanho tablado eixo y
	this->raio = 10; // Raio do Rob�
	this->ang = 45*random_int(0,7); // Angulo inicial do rob� em rela��o ao eixo X
	this->dynamicEnvironment = dynamicEnvironment; // Se obst�culo for dinamico, altera a posi��o dos obst�culos no tablado
	this->maxGen = maxGen; // Nro m�ximo de gera��es para definir o ambiente.
	
	//Sortear uma posi��o inicial sem estar no obst�culo...
	int count = 0;
	do{
		if(count < 5) {
			posX = random_int(raio + 5,tamX- (raio + 5)); // X do centro do rob�
			posY = random_int(raio + 5,tamY-(raio + 5)); // Y do centro do rob�
			count++;
		}
		else {
			posX = raio + 5;
			posY = raio + 5;
		}
	} while(sensor (0, ang, gen) != 0);
}//construtor

//-----------------	simulador -----------------//
Simulador::Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen){
	this->tamX = tamX; // Tamanho tablado eixo X
	this->tamY = tamY; // Tamanho tablado eixo y
	this->raio = raio; // Raio do Rob�
	this->posX = posX; // X do centro do rob�
	this->posY = posY; // Y do centro do rob�
	this->ang = ang; // Angulo inicial do rob� em rela��o ao eixo X
	this->dynamicEnvironment = dynamicEnvironment; // Se obst�culo for dinamico, altera a posi��o dos obst�culos no tablado
	this->maxGen = maxGen; // Nro m�ximo de gera��es para definir o ambiente.
}//construtor


//-----------------	~simulador -----------------//
Simulador::~Simulador(){
}//destrutor


//-----------------	execute -----------------//
/*
* return true: mov executado com sucesso, false caso contr�rio
*/
 // modificacao R
bool Simulador::execute(int acao, int dist, int gen){
		/*	
			*A��o
		1 -- vira 45�
		2 -- vira -45�	
		3 -- vira 90�	
		4 -- anda p/ frente dist cm 
		*/			
		switch (acao) 
		{
			case 0 : 
				/** Bloco Rotacionar 45� **/
				ang = ((int)ang + 45) % 360;
				break;
		
			case 1 :
				/** Bloco Rotacionar -45� **/
				ang = ((int)ang + 315) % 360;
				break;

			case 2 :
				/** Bloco Rotacionar 90� **/
				ang = ((int)ang + 90) % 360;
				break;

			case 3 :
				if(sensor(dist, ang, gen) == 0){
					/** Mover para frente dist cm	 M = 1	**/
					posX = posX + dist * cos(ang * PI / 180.0); // (ang / 360) * 2PI
					posY = posY + dist * sin(ang * PI / 180.0);
				}
				else
					return false;
				break;
		}//switch
		
		return true;			
}// execute


//-----------------	readSensor -----------------//
double* Simulador::readSensor(int dist, int gen){
	double *sensores = new double[4]; // ALTERAR CASO TIVER MAIS SENSORES!!
	sensores[0] = sensor(dist, ang - 45, gen); 		// direita
	sensores[1] = sensor(dist, ang, gen);			// frontal
	sensores[2] = sensor(dist, ang + 45, gen); 		// esquerda
	if(isBase())
		sensores[3] = 1;						// cima
	else
		sensores[3] = 0;						// cima
	
	//sensores[4] = sensor(dist, ang - 90, gen); //novos 2 sensores
	//sensores[5] = sensor(dist, ang + 90, gen); //sensor esquerda 
	return sensores;
}//readSensor

//-----------------	readSensor + geraFalha -----------------//

//Adicionada fun��o geraFalha - Definir valores para - chanceFalha e percentualFalha
double* Simulador::readSensor(int dist, int gen, bool falha){
	double *sensores = new double[6]; // ALTERAR CASO TIVER MAIS SENSORES!!
	sensores[0] = sensor(geraFalha(dist, 0.5, 0.5), ang - 45, gen); 	//direita
	sensores[1] = sensor(geraFalha(dist, 0.5, 0.5), ang, gen);			// frontal
	sensores[2] = sensor(geraFalha(dist, 0.5, 0.5), ang + 45, gen); 	//esquerda
	//novos sensores
	//sensores[3] = sensor(geraFalha(dist, 0.5, 0.5), ang + 270, gen); 	// direita 
	//sensores[4] = sensor(geraFalha(dist, 0.5, 0.5), ang + 90, gen); 		// esquerda
	
	if(isBase())
		sensores[3] = 1;						// cima
	else
		sensores[3] = 0;						// cima

	
	return sensores;
}//readSensor



//-----------------	sensor -----------------//
int Simulador::sensor (int dist, int ang, int gen){
	double anguloRad = ang * PI / 180.0;
	int distancia = dist + raio; // Somar o raio pq posX e posY sao o centro do rob�
	int x = posX + distancia*arred(cos(anguloRad));
	int y = posY + distancia*arred(sin(anguloRad));
		 
	if( x >= tamX || x <= 0 || y >= tamY || y <= 0 ) // Verifica se a posicao est� dentro do tablado
		return 1;	
	
	if(dynamicEnvironment == true && gen > 0) { 
		int ambiente = (gen - 1) / ((maxGen -200)/ 10);
		//std::cout<<"Ambiente: " << ambiente << std::endl;

		switch (ambiente){
			case 1 : // Ambiente 1
				if(0 <= x && x <= 55 && 52 <= y && y <= 86)
					return 1;
				if(0 <= x && x <= 55 && 126 <= y && y <= 160)
					return 1;
				break;
				
			case 2 : // Ambiente 2
				if(166 <= y && y <= 200)
					return 1;
				break;
				
			case 3 : // Ambiente 3
				if(65 <= x && x <= 120 && 122 <= y && y <= 156)
					return 1;
				break;
				
			case 4 : // Ambiente 4
				if(86 <= x && x <= 120 && 45 <= y && y <= 155)
					return 1;
				break;
				
			case 5 : // Ambiente 5
				if(86 <= x && x <= 120 && 0 <= y && y <= 55)
					return 1;
				if(0 <= x && x <= 55 && 166 <= y && y <= 200)
					return 1;
				break;
				
			case 6 : // Ambiente 6
				if(65 <= x && x <= 120 && 52 <= y && y <= 86)
					return 1;
				if(65 <= x && x <= 120 && 126 <= y && y <= 160)
					return 1;		
				break;
				
			case 7 : // Ambiente 7
				if(43 <= x && x <= 77 && 45 <= y && y <= 155)
					return 1;
				break;
				
			case 8 : // Ambiente 8
				if(65 <= x && x <= 120 && 44 <= y && y <= 78)
					return 1;
				if(0 <= x && x <= 55 && 122 <= y && y <= 156)
					return 1;
				break;
				
			case 9 : // Ambiente 9
				if(43 <= x && x <= 77 && 0 <= y && y <= 55)
					return 1;
				if(43 <= x && x <= 77 && 145 <= y && y <= 200)
					return 1;		
				break;				
		}
	}	
	return 0;
}//sensor


//-----------------	isBase -----------------//
bool Simulador::isBase(){
	 if(posX < 30 && posY < 30)
		return true;
	 return false;
}//isBase

//-----------------	getPosX -----------------//
int Simulador::getPosX(){
	//std::cout<<"SENO:"<<sin(ang * PI / 180.0) * dist<<std::endl; */
	return posX;
}//getPosX

//-----------------	getPosY -----------------//
int Simulador::getPosY(){
	return posY;
}//getPosY

//-----------------	getAngle -----------------//
int Simulador::getAngle(){
	return ang;
}//getAngle

