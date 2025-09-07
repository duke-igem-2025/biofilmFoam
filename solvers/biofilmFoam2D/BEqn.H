dimensionedScalar Db_bulk (
    "Db_bulk",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.0000078 / 86400 
); // 10x slower than autoinducer

dimensionedScalar Db_biofilm (
    "Db_biofilm",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.0000039 / 86400 
);

dimensionedScalar prod_enzyme (
    "prod_enzyme",
    dimensionSet(0, 0, -1, 0, 0, 0, 0),
    100
);


tmp<volScalarField> Dbtmp = Db_bulk*(1.0 - M) + Db_biofilm*M;

volScalarField Db
(
    IOobject
    (
        "Db",                    
        runTime.timeName(),     
        mesh,                  
        IOobject::NO_READ,      
        IOobject::AUTO_WRITE
    ),
    Dbtmp                        
);


// create a dimensionless switch field (1 if E is decreasing, else 0)
volScalarField switchField_B
(
    IOobject
    (
        "switchField-B",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("zero", dimless, 1.0) // initialize to 0
);

// fill cell by cell
forAll(M, cellI)
{
    if (M[cellI] > SMALL)
        switchField_B[cellI] = 0.0; // dimensionless
}



fvScalarMatrix BEqn
(
    fvm::ddt(B)
  - fvm::laplacian(Db, B)
  ==
    prod_enzyme * A * switchField_B * regionMask * D_enzyme
    // 0
);

solve(BEqn);
