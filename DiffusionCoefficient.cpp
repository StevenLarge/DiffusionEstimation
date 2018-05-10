/* This C++ instance calculates the numerical diffusion coefficient from a simulation trajectory */

#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include <string>
#include <ctime>

using namespace std;


/* Global declaration of random device */

std::random_device rd;
std::mt19937 gen(rd());
std::normal_distribution<> d(0,1);
double GaussRandom;

double mass = 1;
double beta = 1;
double dt = 0.1;
double DampingVal = 0.25;

void Langevin(double * time, double * position, double * velocity);
double DiffusionCoeff(double * Trajectory, int Length);
void WriteDiffusionData(string WriteName, double * Diffusion, int * SampleLength, int DataPoints);

int main(){
	
	int TrajLength = 100000000;

	double * Trajectory;
	Trajectory = new double [TrajLength];

	double time;
	double position;
	double velocity;

	double * positionPointer = &position;
	double * timePointer = &time;
	double * velocityPointer = &velocity;

	time = 0;
	velocity = 0;
	position = 0;

	for(int k = 0 ; k < 100 ; k++){
		Langevin(timePointer,positionPointer,velocityPointer);
	}

	time = 0;
	position = 0;

	for(int k = 0 ; k < TrajLength ; k++){
		Langevin(timePointer,positionPointer,velocityPointer);
		Trajectory[k] = position;
	}	

	double * Diffusion;
	int * SampleLength;
	int DataPoints = 20;
	Diffusion = new double [DataPoints];
	SampleLength = new int [DataPoints];

	for(int k = 0 ; k < DataPoints ; k++){
		Diffusion[k] = DiffusionCoeff(Trajectory,int(TrajLength/((k+1))));
		SampleLength[k] = int(TrajLength/(k+1));
	}

	WriteDiffusionData("Diff_Coeff.dat",Diffusion,SampleLength,DataPoints);

}

void Langevin(double * time, double * position, double * velocity){

	GaussRandom = d(gen);

	*velocity = sqrt(DampingVal)*(*velocity) + sqrt((1-DampingVal)/(beta*mass))*GaussRandom;
	*position = *position + 0.5*dt*(*velocity);

	GaussRandom = d(gen);

	*position = *position + 0.5*dt*(*velocity);
	*velocity = sqrt(DampingVal)*(*velocity) + sqrt((1-DampingVal)/(beta*mass))*GaussRandom;

}

double DiffusionCoeff(double * Trajectory, int Length){

	/* Vestergaard Diffusion coefficient estimation method */

	double Diff;
	double VarAcc = 0;
	double CovAcc = 0;

	for(int k = 0 ; k < Length-2 ; k++){
		VarAcc += (Trajectory[k+1]-Trajectory[k])*(Trajectory[k+1]-Trajectory[k])/double(2*dt);
		CovAcc += (Trajectory[k+2]-Trajectory[k+1])*(Trajectory[k+1]-Trajectory[k])/dt;
		//Diff += ((Trajectory[k+1]-Trajectory[k])*(Trajectory[k+1]-Trajectory[k])/double(2*dt)) + (((Trajectory[k+2]-Trajectory[k+1])*(Trajectory[k+1]-Trajectory[k]))/(dt));
	}

	Diff = (VarAcc + CovAcc)/double(Length);

	double Fric = double(1)/Diff;

	return Fric;
}


void WriteDiffusionData(string WriteName, double * Diffusion, int * SampleLength, int DataPoints){

	std::ofstream WriteFile;

	WriteFile.open(WriteName);

	for(int k = 0 ; k < DataPoints ; k++){
		WriteFile << std::to_string(SampleLength[k]) << "\t" << std::to_string(Diffusion[k]) << "\t" << "13.86" << "\n";
	}

	WriteFile.close();

}

