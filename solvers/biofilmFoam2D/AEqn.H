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
    pow(1e-017,1)   // nm 
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




volScalarField A_dimless = max(A / tau, 1e-100);
volScalarField hill = pow(A_dimless, n) / (1.0 + pow(A_dimless, n));

volScalarField production =
    gamma * (alpha + beta * hill) * M_inf * M;

fvScalarMatrix AEqn
(
    fvm::ddt(A)
  - fvm::laplacian(Da, A)
  ==
    production

);

solve(AEqn);
