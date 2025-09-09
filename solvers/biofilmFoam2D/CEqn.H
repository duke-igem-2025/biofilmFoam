// fvScalarMatrix CEqn
// (
//     fvm::ddt(C)
//     - fvm::laplacian(d1, C)
//     == 
//     - fvm::Sp(M*(mu/Y)*M_inf/(k_1+C), C)
// );

// solve(CEqn);

// ddtC = gMax(mag(fvc::ddt(C)())())/(gMax(C)+SMALL);

dimensionedScalar D_bulk (
    "D_bulk",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.0001 / 86400     // m^2/day in bulk
);
dimensionedScalar D_biofilm (
    "D_biofilm",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.00008 / 86400     // m^2/day in biofilm
);

volScalarField Mtot = M+N+E/2;       // non dimensional

// --- 3) Build a tmp<volScalarField> for the spatial D field ---
tmp<volScalarField> Dtmp = D_bulk*(1.0 - Mtot) + D_biofilm*Mtot;

// --- 4) Construct the actual volScalarField from that tmp ---
volScalarField D
(
    IOobject
    (
        "D",                    // field name
        runTime.timeName(),     // e.g. "0", "1", "2", ...
        mesh,                   // the fvMesh
        IOobject::NO_READ,      
        IOobject::AUTO_WRITE
    ),
    Dtmp                        // the tmp<volScalarField>  
);

// --- 5) Build the consumption rate parameter Rmax ---
dimensionedScalar Rmax = mu*M_inf/Y;  // [g/m^3/day]

// --- 6) Finally, your C‑equation ---
fvScalarMatrix CEqn
(
    fvm::ddt(C)
  - fvm::laplacian(D, C)
  ==
    - fvm::Sp( Rmax * Mtot/(k_1 + C), C )
);

solve(CEqn);
