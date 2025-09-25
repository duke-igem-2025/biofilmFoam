#include "fvCFD.H"
#include <iostream>
#include <fstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

    std::ofstream bfile;
    bfile.open ("enzyme.dat");
    
    Foam::timeSelector::addOptions();  
    Foam::argList args(argc,argv); 

    #include "createTime.H"
    Foam::instantList timeDirs = Foam::timeSelector::select0(runTime, args);

    forAll(timeDirs, timei)
    {
        runTime.setTime(timeDirs[timei], timei);
    
        #include "createMesh.H"

        Info << "Reading Be" << endl;
        volScalarField Be
        (
            IOobject
            (
                "Be",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh
        );

        scalar tau = 1;

        volScalarField Be_dimless
        (
            IOobject
            (
                "Be_dimless",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            Be / tau
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

        // Compute volume-weighted average of Be/τ
        dimensionedScalar Beavg_dimless =
            dimensionedScalar("Beavg_dimless", Be.dimensions() / dimTime, gSum(Be_dimless.primitiveField() * mesh.V()) / domainVolume.value());

        Info << "Beavg_dimless = " << Beavg_dimless.value() << endl;
        bfile << runTime.timeName() << " " << Beavg_dimless.value() << "\n";

        Info << endl;
    }
	
    Info << nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
         << "  ClockTime = " << runTime.elapsedClockTime() << " s"
         << nl << endl;

    Info << "End\n" << endl;

    return 0;
}

// ************************************************************************* //
