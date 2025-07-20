#include "fvCFD.H"
#include <iostream>
#include <fstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

    std::ofstream bfile;
    bfile.open ("autoinducer.dat");
    
    Foam::timeSelector::addOptions();  
    Foam::argList args(argc,argv); 

    #include "createTime.H"
    Foam::instantList timeDirs = Foam::timeSelector::select0(runTime, args);

    // int last = timeDirs.size()-1;
    // runTime.setTime(timeDirs[last], last);
    
    forAll(timeDirs, timei)
    {
	
	runTime.setTime(timeDirs[timei], timei);
    
	#include "createMesh.H"

	    Info << "Reading A" << endl;
    volScalarField A
    (
        IOobject
        (
            "A",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    scalar tau = 10e-9; // You can also read this from a dictionary if needed

    volScalarField A_dimless
    (
        IOobject
        (
            "A_dimless",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        A / tau
    );

    // Cell volumes
    volScalarField cellVolume
    (
        IOobject
        (
            "cellVolume",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("zero",dimVolume,0.0)
    );
    cellVolume.ref() = mesh.V();

    dimensionedScalar domainVolume = gSum(cellVolume);

    // Compute volume-weighted average of A/τ
    dimensionedScalar Aavg_dimless =
        dimensionedScalar("Aavg_dimless", A.dimensions() / dimTime, gSum(A_dimless.primitiveField() * mesh.V()) / domainVolume.value());

    Info << "Aavg_dimless = " << Aavg_dimless.value() << endl;
    bfile << runTime.timeName() << " " << Aavg_dimless.value() << "\n";


	Info << endl;
	
    }
	
    Info << nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
	<< "  ClockTime = " << runTime.elapsedClockTime() << " s"
	<< nl << endl;

    Info << "End\n" << endl;

    return 0;
    
}


// ************************************************************************* //
