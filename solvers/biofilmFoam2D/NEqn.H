volScalarField hill_N = pow(A, n) / (pow(tau, n) + pow(A, n));

dimensionedScalar Dn_bulk (
    "Dn_bulk",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.0001 / 86400 
);
dimensionedScalar Dn_biofilm (
    "Dn_biofilm",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.00002  / 86400    
);

Info << "min/max hill_N: " << gMin(hill_N) << " / " << gMax(hill_N) << endl;
Info << "min/max M: " << gMin(M) << " / " << gMax(M) << endl;

tmp<volScalarField> Dntmp = Dn_bulk*(1.0 - M) + Dn_biofilm*M;

volScalarField Dn
(
    IOobject
    (
        "Dn",                    
        runTime.timeName(),     
        mesh,                  
        IOobject::NO_READ,      
        IOobject::AUTO_WRITE
    ),
    Dntmp                        
);

volScalarField production_n = eta1 * hill_N * M;

fvScalarMatrix NEqn
(
    fvm::ddt(N)
    - fvm::laplacian(Dn, N, "laplacian(Dn,N)")
    
    == 
    production_n
    + fvm::Sp(mu*C/(k_1+C), N)
    - fvm::Sp(k_4, N)
    - fvm::Sp(eta2*M/(k5+M), N)
    
);

solve(NEqn);
