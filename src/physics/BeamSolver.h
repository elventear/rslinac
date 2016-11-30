//---------------------------------------------------------------------------

#ifndef BeamSolverH
#define BeamSolverH

// #include "Types.h"       (DLB, 20150819;  creates link conflict)
// #include "Spectrum.h"
// #include "Spline.h"
// #include "Matrix.h"

#include "Beam.h"
#include "Matrix.h"
#include "Spline.h"
#include "Spectrum.h"

#ifndef RSLINAC
#include "SmartProgressBar.h"
#else
#include "TStringList.hpp"
#endif

//---------------------------------------------------------------------------
class TBeamSolver
{
private:
    //FLAGS
    bool DataReady;
	AnsiString UserIniPath;

	//BEAM
	TBeamInput BeamPar;

	//These should be removed

	double Kernel,Smooth;
	double AngErr;
	double dh;

	int Np_beam,Nstat,Ngraph,Nbars,Nav,Nliv;
	double I;

	TSplineType SplineType;
	//STRUCTURE
	TStringList *InputStrings,*ParsedStrings;
	TStructureInput StructPar;
	int ChangeCells(int N);
	void ResetStructure();

	int Mode_N,Mode_M,MaxCells,Nmesh,Npoints;//,Ncells,Nlim;
	double Rb,Lb,phi0,dphi,w,betta0; //create new structure TStructure

	int GetSolenoidPoints();
	bool ReadSolenoid(int Nz,double *Z,double* B);

	//OTHER
    #ifndef RSLINAC 
    TSmartProgress *SmartProgress;
	#endif

	//INITIALIZATION & PARSING
    void Initialize();
	void LoadIniConstants();

	AnsiString AddLines(TInputLine *Lines,int N1, int N2);

	TInputLine *ParseFile(int& N);
	TError ParseLines(TInputLine *Lines,int N,bool OnlyParameters=false);

	TError ParseOptions (TInputLine *Line);
	TError ParseSpaceCharge (TInputLine *Line);
	TError ParseSolenoid (TInputLine *Line);
	TError ParseBeam (TInputLine *Line);
	TError ParseCurrent (TInputLine *Line);

	TError ParsePID (TInputLine *Line, AnsiString &F);
	TError ParsePIT (TInputLine *Line, AnsiString &F);
	TError ParseFile2R (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseFile1Z (TInputLine *Line, AnsiString &F, int Nz,int Zpos);
	TError ParseFile2Z (TInputLine *Line, AnsiString &F, int Nz,int Zpos);
	TError ParseTwiss2D (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseTwiss4D (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseSPH (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseELL (TInputLine *Line, AnsiString &F, int Nr);
	TError ParseNorm (TInputLine *Line, AnsiString &F, int Nz,int Zpos);

	//INTERPOLATION
	double *LinearInterpolation(double *x,double *X,double *Y,int Nbase,int Nint);
    double *SplineInterpolation(double *x,double *X,double *Y,int Nbase,int Nint);
    double *SmoothInterpolation(double *x,double *X,double *Y,int Nbase,int Nint,double p0,double *W=NULL);
    void GetDimensions(TCell& Cell);

	//INTEGRATION
    void Step(int Si);
    void Integrate(int Si, int Sj);
    void CountLiving(int Si);
	TIntegration **K;
	TIntParameters *Par;

	//TYPE CHECKS
	bool IsKeyWord(AnsiString &S);
	TInputParameter Parse(AnsiString &S);

	TBeamType ParseDistribution(AnsiString &S);
	TSpaceChargeType ParseSpchType(AnsiString &S);
	bool IsFullFileKeyWord(TBeamType D);
	bool IsTransverseKeyWord(TBeamType D);
	bool IsLongitudinalKeyWord(TBeamType D);
	bool IsFileKeyWord(TBeamType D);

	void ShowError(AnsiString &S);
protected:

public:
    //INITIALIZATION
    __fastcall TBeamSolver(AnsiString _Path);
    __fastcall TBeamSolver();
    __fastcall ~TBeamSolver();

    #ifndef RSLINAC
    void AssignSolverPanel(TObject *SolverPanel);
    #endif

    void Abort();
	bool Stop;
    
    TBeam **Beam;
    TStructure *Structure;
    AnsiString InputFile;

    TError LoadData(int Nlim=-1);
    TError MakeBuncher(TCell& iCell);

    void AppendCells(TCell& iCell,int N=1);
    void AddCells(int N=1);
    double GaussIntegration(double r,double z,double Rb,double Lb,int component);
    TCell LastCell();
    TCell GetCell(int j);

    void SaveToFile(AnsiString& Fname);
    bool LoadFromFile(AnsiString& Fname);
   // bool LoadEnergyFromFile(AnsiString& Fname, int NpEnergy);     move to beam.h

    TError CreateBeam();
    int CreateGeometry();

    void ChangeInputCurrent(double Ib);

    //GET INITIAL PARAMETERS
    int GetNumberOfPoints();
    int GetMeshPoints();
	int GetNumberOfParticles();
   //   double GetWaveLength();
    int GetNumberOfChartPoints();
    int GetNumberOfBars();
    int GetNumberOfCells();
    double GetFrequency();
    double GetPower();
    double GetInputCurrent();
   //   double GetMode(int *N=NULL,int *M=NULL);

	TGauss GetInputEnergy();
	TGauss GetInputPhase();

    double GetInputAlpha();
    double GetInputBetta();
	double GetInputEpsilon();

	bool CheckMagnetization();
	bool CheckReverse();
	TSpaceCharge GetSpaceChargeInfo();
	TMagnetParameters GetSolenoidInfo();

	//GET OUTPUT PARAMETERS
	void GetCourantSneider(int Nknot, double& alpha,double& betta, double& epsilon);
	void GetEllipticParameters(int Nknot, double& x0,double& y0, double& a,double& b,double& phi,double &Rx,double& Ry);
	TSpectrumBar *GetEnergySpectrum(int Nknot,double& Wav,double& dW);
	TSpectrumBar *GetPhaseSpectrum(int Nknot,double& Fav,double& dF);
	TSpectrumBar *GetEnergySpectrum(int Nknot,bool Env,double& Wav,double& dW);
	TSpectrumBar *GetPhaseSpectrum(int Nknot,bool Env,double& Fav,double& dF);
	void GetBeamParameters(int Nknot,double *X,TBeamParameter Par);
	void GetStructureParameters(double *X,TStructureParameter Par);
	double GetKernel();

	void Solve();
	#ifndef RSLINAC
	TResult Output(AnsiString& FileName,TMemo *Memo=NULL);
    #else
    TResult Output(AnsiString& FileName);
    #endif
};

//---------------------------------------------------------------------------
#endif
