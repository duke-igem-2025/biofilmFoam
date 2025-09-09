// --- reading

Info << "Reading field C\n" << endl;

volScalarField C
(
    IOobject
    (
	"C",
	runTime.timeName(),
	mesh,
	IOobject::MUST_READ,
	IOobject::AUTO_WRITE
    ),
    mesh
);

Info << "Reading field M\n" << endl;

dimensionedScalar ddtC("ddtC", dimensionSet(0,0,0,0,0), 0.);

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

dimensionedScalar ddtM("ddtM", dimensionSet(0,0,0,0,0), 0.);

Info << "Reading field A\n" << endl;

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

dimensionedScalar ddtA("ddtA", dimensionSet(0,0,0,0,0), 0.);

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

dimensionedScalar ddtN("ddtN", dimensionSet(0,0,0,0,0), 0.);

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

dimensionedScalar ddtE("ddtE", dimensionSet(0,0,0,0,0), 0.);

volScalarField B
(
    IOobject
    (
	"B",
	runTime.timeName(),
	mesh,
	IOobject::MUST_READ,
	IOobject::AUTO_WRITE
    ),
    mesh
);

dimensionedScalar ddtB("ddtB", dimensionSet(0,0,0,0,0), 0.);

// --- transportProperties

Info << "Reading transportProperties\n" << endl;

IOdictionary transportProperties
(
    IOobject
    (
	"transportProperties",
	runTime.constant(),
	mesh,
	IOobject::MUST_READ_IF_MODIFIED,
	IOobject::NO_WRITE
    )
);

// dimemsions: kg | m | s | K | mol | A | cd

Info << "Reading parameter d1 \n" << endl;
dimensionedScalar d1 ("d1", dimensionSet(0, 2, -1, 0, 0, 0, 0), transportProperties);

Info << "Reading parameter mmax \n" << endl;
dimensionedScalar mmax ("mmax", dimensionSet(1, 3, 0, 0, 0, 0, 0), transportProperties);

Info << "Reading parameter c0 \n" << endl;
dimensionedScalar c0 ("c0", dimensionSet(1, 3, 0, 0, 0, 0, 0), transportProperties);

Info << "Reading parameter mum \n" << endl;
dimensionedScalar mum ("mum", dimensionSet(0, 0, -1, 0, 0, 0, 0), transportProperties);

Info << "Reading parameter Yxs \n" << endl;
dimensionedScalar Yxs ("Yxs", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);

Info << "Reading parameter ms \n" << endl;
dimensionedScalar ms ("ms", dimensionSet(0, 0, -1, 0, 0, 0, 0), transportProperties);

Info << "Reading parameter Ks \n" << endl;
dimensionedScalar Ks ("Ks", dimensionSet(1, 3, 0, 0, 0, 0, 0), transportProperties);

Info << "Reading parameter eps \n" << endl;
dimensionedScalar eps ("eps", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);

Info << "Reading parameter a \n" << endl;
dimensionedScalar a ("a", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);

Info << "Reading parameter b \n" << endl;
dimensionedScalar b ("b", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);


dimensionedScalar mu ("mu", dimensionSet(0, 0, -1, 0, 0, 0, 0), transportProperties);

dimensionedScalar Y ("Y", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);

dimensionedScalar k_1 ("k_1", dimensionSet(1, -3, 0, 0, 0, 0, 0), transportProperties);

dimensionedScalar k_4 ("k_4", dimensionSet(0, 0, -1, 0, 0, 0, 0), transportProperties);
dimensionedScalar M_inf ("M_inf", dimensionSet(1, -3, 0, 0, 0, 0, 0), transportProperties);


dimensionedScalar alpha ("alpha", dimensionSet(-1, 0, -1, 0, 1, 0, 0), transportProperties);
dimensionedScalar beta ("beta", dimensionSet(-1, 0, -1, 0, 1, 0, 0), transportProperties);

dimensionedScalar n ("n", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);

dimensionedScalar tau ("tau", dimensionSet(0, -3, 0, 0, 1, 0, 0), transportProperties);

dimensionedScalar eta1 ("eta1", dimensionSet(0, 0, -1, 0, 0, 0, 0), transportProperties);
dimensionedScalar eta2 ("eta2", dimensionSet(0, 0, -1, 0, 0, 0, 0), transportProperties);

dimensionedScalar k5 ("k5", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);

dimensionedScalar gamma_EPS_prod(
    "gamma_EPS_prod",
    dimensionSet(0, 0, -1, 0, 0, 0, 0),  // 1/time (adjust if E has other base dims)
    1e4  / 86400                                  // value in 1/s (or 1/your_time_unit)
);

dimensionedScalar gamma_EPS_deg(
    "gamma_EPS_prod",
    dimensionSet(0, 0, -1, 0, 0, 0, 0),  // 1/time (adjust if E has other base dims)
    0.00003472222222 / 3                               // value in 1/s (or 1/your_time_unit)
);

dimensionedScalar eps_disp(
    "eps_disp",
    dimensionSet(0, 3, -1, 0, -1, 0, 0),  // 1/time (adjust if E has other base dims)
   0.000001736111111   * 5000                             // value in 1/s (or 1/your_time_unit)
);

dimensionedScalar M_deg_eps(
    "M_deg_eps",
    dimensionSet(0, 0, -1, 0, 0, 0, 0),  // 1/time (adjust if E has other base dims)
    1                           // value in 1/s (or 1/your_time_unit)
);

dimensionedScalar E_threshold ("E_threshold", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);
dimensionedScalar E_crit ("E_crit", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);

dimensionedScalar eps_disp_death(
    "eps_disp_death",
    dimensionSet(0, 0, -1, 0, 0, 0, 0),  // 1/time (adjust if E has other base dims)
    transportProperties                                 // value in 1/s (or 1/your_time_unit)
);

dimensionedScalar D_enzyme ("D_enzyme", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);


volScalarField Mdim = M / M_inf;


// --- calculating

Info << "Calculating parameters ki \n" << endl;
dimensionedScalar k1 = mum/Yxs+ms;
dimensionedScalar k2 = Ks;
dimensionedScalar k3 = Yxs/mmax;
dimensionedScalar k4 = ms*mmax;

Info << "Calculating parameters Ki \n" << endl;
dimensionedScalar K1 = mmax*k1/c0;
dimensionedScalar K2 = k2/c0;
dimensionedScalar K3 = mmax*k3*k1;
dimensionedScalar K4 = k3*k4;

// --- G

// Info << "G = l * l * G0" << endl;
// Info << "G0 = mum * mmax / (d1 * c0)" << endl;
// Info << "G0 = " << mum*mmax/(d1*c0) << endl;

// --- biofilm phase

volScalarField biofilmPhase
(
    IOobject
    (
	"biofilmPhase",
	runTime.timeName(),
	mesh,
	IOobject::NO_READ,
	IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar ("biofilmPhase", dimensionSet(0,0,0,0,0), 0.),
    "zeroGradient"
);


// For B
// New mask field for production restriction
volScalarField regionMask
(
    IOobject
    (
        "regionMask",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("zero", dimless, 0.0) // initialize all to 0
);

// =============================================================
// Randomly assign ~10 patches of 2x2 cells in the upper half
// =============================================================

#include "Random.H"

// Set seed for reproducibility
unsigned int seed = 12345;
srand(seed);

int numBlocks = 10;

const vectorField& C_ = mesh.C(); // cell centers


scalar zMid = 0.3 * (gMax(C_).z() + gMin(C_).z()); // midpoint in z-direction

scalar dx = (gMax(C_).x() - gMin(C_).x()) / std::cbrt(mesh.nCells());
scalar dz = (gMax(C_).z() - gMin(C_).z()) / std::cbrt(mesh.nCells());

for (int b = 0; b < numBlocks; b++)
{
    int randCell = rand() % mesh.nCells();

    // ensure it’s in the upper half along z
    if (C_[randCell].z() < zMid)
    {
        b--; // retry if not in upper half
        continue;
    }

    // mark 2x2 block: all cells within dx, dz of the chosen anchor
    forAll(C_, cellI)
    {
        if (mag(C_[cellI].x() - C_[randCell].x()) <= dx
         && mag(C_[cellI].z() - C_[randCell].z()) <= dz)
        {
            regionMask[cellI] = 1.0;
        }
    }
}

regionMask.write();


// End B

// --- file biofilmVolume.dat

// std::ofstream bfile;
// bfile.open ("biofilmVolume.dat", std::fstream::app);

// ---

Info << "Reading biofilm permeability\n" << endl;
// dimensionedScalar biofilmPermeability ("biofilmPermeability", dimensionSet(0, 2, 0, 0, 0, 0, 0), transportProperties);
dimensionedScalar biofilmPermeability(transportProperties.lookupOrDefault<dimensionedScalar>("biofilmPermeability", dimensionedScalar("biofilmPermeability", dimLength*dimLength, 1e-12)));

Info << "Reading biofilm porosity\n" << endl;
// dimensionedScalar biofilmPorosity ("biofilmPorosity", dimensionSet(0, 0, 0, 0, 0, 0, 0), transportProperties);
dimensionedScalar biofilmPorosity(transportProperties.lookupOrDefault<dimensionedScalar>("biofilmPorosity", dimensionedScalar("biofilmPorosity", dimless, 1)));
