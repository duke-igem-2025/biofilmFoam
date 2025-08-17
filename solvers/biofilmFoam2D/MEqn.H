volScalarField A_dimless_ = max(A / tau, SMALL);
volScalarField hill_ = eta1*pow(A, n) / (pow(tau, n) + pow(A, n));

volScalarField decayCoeff = eta1 * hill_;
volScalarField growthCoeff = mu * C / (k_1 + C);

Info << "Decay range: " << gMin(decayCoeff) << " to " << gMax(decayCoeff) << endl;
Info << "Growth range: " << gMin(growthCoeff) << " to " << gMax(growthCoeff) << endl;


fvScalarMatrix MEqn
(
    fvm::ddt(M)
    - fvm::laplacian(d2, M, "laplacian(d2,M)")
    == 
    fvm::Sp(mu*C/(k_1+C), M)
    - fvm::Sp(k_4, M)
    - fvm::Sp(hill_, M)
    + fvm::Sp(eta2*N/(k5+M), M)
    // - fvm::Sp(gamma_EPS_prod, M)
    - fvm::Sp(eps_disp_death*max(scalar(0), (E_crit - E)/E_crit), M)
);

solve(MEqn);

ddtM = gMax(mag(fvc::ddt(M)())())/(gMax(M)+SMALL);
