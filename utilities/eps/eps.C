#include "fvCFD.H"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::ofstream bfile;
    bfile.open("EPS_fraction.dat");

    Foam::timeSelector::addOptions();
    Foam::argList args(argc, argv);

    #include "createTime.H"
    Foam::instantList timeDirs = Foam::timeSelector::select0(runTime, args);

    forAll(timeDirs, timei)
    {
        runTime.setTime(timeDirs[timei], timei);
        #include "createMesh.H"

        Info << "Reading E" << endl;
        volScalarField E
        (
            IOobject
            (
                "E",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh
        );

        const scalarField& Efield = E.primitiveField();
        const scalarField& V = mesh.V();

        scalar eps = 0.0;
        scalar totalVolume = 0.0;

        forAll(Efield, celli)
        {
            scalar eval = Efield[celli];
            scalar vol = V[celli];

            totalVolume += vol;

            if (eval > SMALL)
            {
                eps += eval * vol;
            }
        }

        reduce(eps, sumOp<scalar>());
        reduce(totalVolume, sumOp<scalar>());

        scalar EPSFraction = (totalVolume > VSMALL) ? (eps / totalVolume) : 0.0;

        Info << "EPS_fraction (eps / domain volume) = " << EPSFraction << endl;
        bfile << runTime.timeName() << " " << EPSFraction << "\n";
    }

    Info << nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
         << "  ClockTime = " << runTime.elapsedClockTime() << " s"
         << nl << endl;

    Info << "End\n" << endl;
    return 0;
}
