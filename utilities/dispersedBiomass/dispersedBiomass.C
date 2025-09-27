#include "fvCFD.H"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::ofstream bfile;
    bfile.open("Ntot_fraction.dat");

    Foam::timeSelector::addOptions();
    Foam::argList args(argc, argv);

    #include "createTime.H"
    Foam::instantList timeDirs = Foam::timeSelector::select0(runTime, args);

    forAll(timeDirs, timei)
    {
        runTime.setTime(timeDirs[timei], timei);
        #include "createMesh.H"

        Info << "Reading N" << endl;
        volScalarField N
        (
            IOobject
            (
                "N",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh
        );

        const scalarField& Nfield = N.primitiveField();
        const scalarField& V = mesh.V();

        scalar biomass = 0.0;
        scalar totalVolume = 0.0;

        forAll(Nfield, celli)
        {
            scalar nval = Nfield[celli];
            scalar vol = V[celli];

            totalVolume += vol;

            if (nval > SMALL)
            {
                biomass += nval * vol;
            }
        }

        reduce(biomass, sumOp<scalar>());
        reduce(totalVolume, sumOp<scalar>());

        scalar NtotFraction = (totalVolume > VSMALL) ? (biomass / totalVolume) : 0.0;

        Info << "Ntot (dispersed biomass / domain volume) = " << NtotFraction << endl;
        bfile << runTime.timeName() << " " << NtotFraction << "\n";
    }

    Info << nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
         << "  ClockTime = " << runTime.elapsedClockTime() << " s"
         << nl << endl;

    Info << "End\n" << endl;
    return 0;
}
