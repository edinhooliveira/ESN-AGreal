//Author Rafael Del Lama
//Modificado por Eder Oliveira
 
//-----------------------------------------	TSimulador.hpp -----------------------------------------//
#pragma once
#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP
#include <cmath>

#define PI 3.14159265

class Simulador 
{
	private:
		int sensor (int dist, int ang, int gen);
		int tamX;								// tamanho do tabladoo eixo x
		int tamY;								// tamanho do tabladoo eixo y
		int posX;								// posi��o do centro do rob� no eixo x
		int posY;								// posi��o do centro do rob� no eixo y
		int ang;								// angulo da frente do rob� em rela��o ao eixo x
		int raio;								// raio do rob�
		bool dynamicEnvironment;
		int maxGen;
		
	public:
		Simulador(int tamX, int tamY, bool dynamicEnvironment, int maxGen, int gen);
		Simulador(int tamX, int tamY, int raio, int posX, int posY, int ang, bool dynamicEnvironment, int maxGen);
		~Simulador();
		bool execute(int acao, int dist, int gen);
		double* readSensor(int dist, int gen);
		double* readSensor(int dist, int gen, bool falha); //readSensor com falha
		bool isBase();
		int getPosX();
		int getPosY();
		int getAngle();
		//int sensor (int dist, int ang, int gen);
		
};// class simulador
#endif // SIMULADOR_HPP
