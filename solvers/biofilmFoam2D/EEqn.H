dimensionedScalar De_bulk (
    "De_bulk",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.0001 / 86400 
);
dimensionedScalar De_biofilm (
    "De_biofilm",
    dimensionSet(0, 2, -1, 0, 0, 0, 0),
    0.000001  / 86400    
);

tmp<volScalarField> Detmp = De_bulk*(1.0 - M) + De_biofilm*M;

dimensionedScalar k_orphan (
    "k_orphan",
    dimensionSet(0, 0, -1, 0, 0, 0, 0),
    10  / 86400    
);


volScalarField De
(
    IOobject
    (
        "De",                    
        runTime.timeName(),     
        mesh,                  
        IOobject::NO_READ,      
        IOobject::AUTO_WRITE
    ),
    Detmp                        
);

volScalarField production_e = gamma_EPS_prod*M;
volScalarField decay_e = eps_disp*D_enzyme*E;

Info << "production range: " << gMin(production_e) << " to " << gMax(production_e) << endl;
Info << "decay range: " << gMin(decay_e) << " to " << gMax(decay_e) << endl;

fvScalarMatrix EEqn
(
    fvm::ddt(E)
    // - fvm::laplacian(De, E, "laplacian(De,E)")
    
    == 
    production_e
    - fvm::Sp(gamma_EPS_deg*(C/(k_1 + C)), E)
    - fvm::Sp(k_orphan*max(scalar(0), E_threshold - M), E)
    - fvm::Sp(eps_disp*D_enzyme, E)
);

solve(EEqn);
