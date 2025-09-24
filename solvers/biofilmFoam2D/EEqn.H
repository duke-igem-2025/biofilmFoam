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

Info << "De_bulk value: " << De_bulk.value() << endl;
Info << "De_biofilm value: " << De_biofilm.value() << endl;

// Explicit finite difference approximation of growth
// Choose a reference biomass scale, e.g., 1.0 if M is already normalized
scalar M_ref = 1.0;

// Compute dimensionless growth rate
volScalarField dM_dt_dimless = (M - M.oldTime()) / (runTime.deltaT() * M_ref);  
Info << "dM_dt dimensions: " << dM_dt_dimless.dimensions() << endl;
volScalarField production_e = gamma_EPS_prod * M * (C/(k_1 + C));          // gamma_EPS_prod ~ dimensionless

volScalarField decay_e = k_deg * E;

Info << "production range: " << gMin(production_e) << " to " << gMax(production_e) << endl;
Info << "decay range: " << gMin(decay_e) << " to " << gMax(decay_e) << endl;


fvScalarMatrix EEqn
(
    fvm::ddt(E)    
    == 
    gamma_EPS_prod * M * (C/(k_1 + C))
    - k_deg * E
    - fvm::Sp(eps_disp*Be, E)
);

solve(EEqn);

volScalarField ddtE = fvc::ddt(E); // explicit time derivative
Info << "ddtE range: " << gMin(ddtE) << " to " << gMax(ddtE) << endl;
