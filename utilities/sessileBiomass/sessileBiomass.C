#include "fvCFD.H"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::ofstream bfile;
    bfile.open("Mtot_fraction.dat");

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

        const scalarField& Mfield = M.primitiveField();
        const scalarField& V = mesh.V();

        scalar biomass = 0.0;
        scalar totalVolume = 0.0;

        forAll(Mfield, celli)
        {
            scalar mval = Mfield[celli];
            scalar vol = V[celli];

            totalVolume += vol;

            if (mval > SMALL)
            {
                biomass += mval * vol;
            }
        }

        reduce(biomass, sumOp<scalar>());
        reduce(totalVolume, sumOp<scalar>());

        scalar MtotFraction = (totalVolume > VSMALL) ? (biomass / totalVolume) : 0.0;

        Info << "Mtot (sessile biomass / domain volume) = " << MtotFraction << endl;
        bfile << runTime.timeName() << " " << MtotFraction << "\n";
    }

    Info << nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
         << "  ClockTime = " << runTime.elapsedClockTime() << " s"
         << nl << endl;

    Info << "End\n" << endl;
    return 0;
}
