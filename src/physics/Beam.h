//---------------------------------------------------------------------------

#ifndef BeamH
#define BeamH

#include "Spectrum.h"
#include "SpectrumPhase.h"
#include <stdlib.h>

const int Ncoef=4;
//---------------------------------------------------------------------------
class TBeam
{
private:
    int Nliv,Nbars;
    double Kernel;
    void CountLiving();
    //void TwoRandomGauss(double& x1,double& x2); //Moved to types.h
    void SetParameters(double *X,TBeamParameter Par);
    TSpectrumBar *GetSpectrum(bool Smooth,double *X,double& Xav,double& dX,bool width=false);
    TSpectrumBar *GetPhaseSpectrum(bool Smooth,double *Radius,double *Phase,double& FavPhase,double& dPhase, int Nslices, bool width=false);

	//DISTRIBUTIONS
	double *MakeRayleighDistribution(TGauss G);
	double *MakeRayleighDistribution(double Xav,double sX);
	double *MakeRayleighDistribution(double Xav,double sX,double Xlim);
	double *MakeGaussDistribution(TGauss G);
	double *MakeGaussDistribution(double Xav,double sX);
	double *MakeGaussDistribution(double Xav,double sX,double Xlim);
	void MakeGaussDistribution(TGauss G,TBeamParameter Par);  //to private
	void MakeGaussDistribution(double Xav,double sX,TBeamParameter Par,double Xlim=0); //move to private!!

	double *MakeEquiprobableDistribution(double Xav, double dX);
	void MakeEquiprobableDistribution(double Xav, double dX,TBeamParameter Par); //to private

	TPhaseSpace *MakeTwissDistribution(TTwiss T);

	//IMPORT
	double **ImportFromFile(TBeamType BeamType,TBeamInput *BeamPar,bool T=true);

	FILE *logFile;

    double BesselSum(TIntParameters& Par,TIntegration *I,TTrig Trig);
public:
    __fastcall TBeam(int N);
    __fastcall ~TBeam();
    TParticle *Particle;
    double lmb,h,Cmag;
    double Ib,I0; //beam current
    bool Reverse;
	int Np;
    
    void SetKernel(double Ker);
    void SetBarsNumber(int N);

	// BEAM CREATION
	bool BeamFromCST(TBeamInput *BeamPar);
	bool BeamFromTwiss(TBeamInput *BeamPar);
	bool BeamFromFile(TBeamInput *BeamPar);
	bool BeamFromSphere(TBeamInput *BeamPar);
	bool BeamFromEllipse(TBeamInput *BeamPar);
	void GenerateEnergy(TGauss G);
	void GeneratePhase(TGauss G);
	void GenerateAzimuth(TGauss G);
	bool ImportEnergy(TBeamInput *BeamPar);

	//int CountCSTParticles(AnsiString &F);        //remove
	////int CountCSTParticles(TBeamType bType);
   //	void MakeGaussEmittance(double alpha, double betta, double eps); //remove
   // bool ReadCSTEmittance(AnsiString &F, int Nmax);     //remove

		//GET OUTPUT PARAMETERS
    void GetCourantSneider(double& alpha,double& betta, double& epsilon);
    void GetEllipticParameters(double& x0,double& y0, double& a,double& b,double& phi, double& Rx, double& Ry);
	TSpectrumBar *GetEnergySpectrum(bool Smooth,double& Wav,double& dW);
	TSpectrumBar *GetPhaseSpectrum(bool Smooth,double& Fav,double& dF);
	void GetParameters(double *X,TBeamParameter Par);

	TGauss GetEnergyDistribution(TDeviation D);
	TGauss GetPhaseDistribution(TDeviation D);

    double GetBeamRadius();
    double GetPhaseLength();
    double GetAveragePhase();
    double GetAverageEnergy();
	double GetMaxEnergy();
	double GetMinPhase();
	double GetMaxPhase();

    double iGetAverageEnergy(TIntParameters& Par,TIntegration *I);
    double iGetBeamLength(TIntParameters& Par,TIntegration *I, int Nslices, bool SpectrumOutput=false);
    double iGetBeamRadius(TIntParameters& Par,TIntegration *I, bool SpectrumOutput=false);
    double iGetAveragePhase(TIntParameters& Par,TIntegration *I);

    double SinSum(TIntParameters& Par,TIntegration *I);
    double CosSum(TIntParameters& Par,TIntegration *I);

    int GetLivingNumber();
    double FindEmittanceAngle(double& a,double& b);

    void Integrate(TIntParameters& Par,TIntegration **I,int Si);
    void Next(TBeam *nBeam,TIntParameters& Par,TIntegration **I);
    void Next(TBeam *nBeam);

    void Solve();

};
//---------------------------------------------------------------------------
#endif
