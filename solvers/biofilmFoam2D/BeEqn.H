dimensionedScalar Dbe_bulk (
    "Dbe_bulk",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.0000078 / 86400 
); // 10x slower than autoinducer

dimensionedScalar Dbe_biofilm (
    "Dbe_biofilm",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.0000000039 / 86400 
);

tmp<volScalarField> Dbetmp = Dbe_bulk*(1.0 - min(E, scalar(1.0))) + Dbe_biofilm*(min(E, scalar(1.0)));

volScalarField Dbe
(
    IOobject
    (
        "Dbe",                    
        runTime.timeName(),     
        mesh,                  
        IOobject::NO_READ,      
        IOobject::AUTO_WRITE
    ),
    Dbetmp                        
);



fvScalarMatrix BeEqn
(
    fvm::ddt(Be)
  - fvm::laplacian(Dbe, Be)
  ==
    M_b * K_I * B
    - fvm::Sp( gamma_Ie, Be)
);

solve(BeEqn);
