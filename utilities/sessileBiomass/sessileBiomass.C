#include "fvCFD.H"
#include <iostream>
#include <fstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

    std::ofstream bfile;
    bfile.open ("sessileBiomass.dat");
    
    Foam::timeSelector::addOptions();  
    Foam::argList args(argc,argv); 

    #include "createTime.H"
    Foam::instantList timeDirs = Foam::timeSelector::select0(runTime, args);
    
    forAll(timeDirs, timei)
    {
	
	runTime.setTime(timeDirs[timei], timei);
    
	#include "createMesh.H"

	Info << "Reading M" << endl;
	volScalarField M
	(
	    IOobject
	    (
		"M",
		runTime.timeName(),
		mesh,
		IOobject::MUST_READ,
		IOobject::AUTO_WRITE
	    ),
	    mesh
	);

	// volume

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

	// Mtot(t) = integral of M over the domain (sessile biomass)

	dimensionedScalar Mtot = dimensionedScalar("Mtot", M.dimensions()*dimVolume, gSum(M.primitiveField() * mesh.V()));

	// dimensionedScalar Mtot = gSum(M.internalField() * mesh.V());

	Info << "Mtot = " << Mtot.value() << endl;
	bfile << runTime.timeName() << " " << Mtot.value() << "\n";

	Info << endl;
	
    }
	
    Info << nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
	<< "  ClockTime = " << runTime.elapsedClockTime() << " s"
	<< nl << endl;

    Info << "End\n" << endl;

    return 0;
    
}


// ************************************************************************* //
