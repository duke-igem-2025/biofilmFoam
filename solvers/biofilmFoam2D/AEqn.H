dimensionedScalar Da_bulk (
    "Da_bulk",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.000078  
);
dimensionedScalar Da_biofilm (
    "Da_biofilm",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.000039    
);

dimensionedScalar gamma (
    "gamma",
    dimensionSet(0, 0 , 0, 0, 0, 0, 0),
    1e-09  // nm 
);


tmp<volScalarField> Datmp = Da_bulk*(1.0 - M) + Da_biofilm*M;

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
A = max(A, dimensionedScalar("Amin", A.dimensions(), SMALL));
