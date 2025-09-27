dimensionedScalar Da_bulk (
    "Da_bulk",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.000078 / 86400 
);
dimensionedScalar Da_biofilm (
    "Da_biofilm",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.000039 / 86400 
);

dimensionedScalar gamma (
    "gamma",
    dimensionSet(0, 0 , 0, 0, 0, 0, 0),
    5e-09  // nm 
);


tmp<volScalarField> Datmp = Da_bulk*(1.0 - min(E, scalar(1.0))) + Da_biofilm*(min(E, scalar(1.0)));

volScalarField Da
(
    IOobject
    (
        "Da",                    
        runTime.timeName(),     
        mesh,                  
        IOobject::NO_READ,      
        IOobject::AUTO_WRITE
    ),
    Datmp                        
);




volScalarField A_dimless = max(A / tau, SMALL);
volScalarField hill = pow(A, n) / (pow(tau, n) + pow(A, n));

volScalarField production =
    gamma * (alpha + beta * hill) * M_inf * (M+N);

fvScalarMatrix AEqn
(
    fvm::ddt(A)
  - fvm::laplacian(Da, A)
  ==
    production

);

solve(AEqn);
// A = max(A, dimensionedScalar("Amin", A.dimensions(), SMALL));
