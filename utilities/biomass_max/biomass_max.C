#include "fvCFD.H"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::ofstream bfile;
    bfile.open("biomass_max.dat");

    Foam::timeSelector::addOptions();
    Foam::argList args(argc, argv);

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

        // Use built-in OpenFOAM max operator (parallel safe)
        scalar globalMaxM = gMax(M);

        Info << "M_max = " << globalMaxM << endl;
        bfile << runTime.timeName() << " " << globalMaxM << "\n";
    }

    Info << nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
         << "  ClockTime = " << runTime.elapsedClockTime() << " s"
         << nl << endl;

    Info << "End\n" << endl;
    return 0;
}
