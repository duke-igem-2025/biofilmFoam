volScalarField A_dimless_ = max(A / tau, SMALL);
volScalarField hill_ = pow(A, n) / (pow(tau, n) + pow(A, n));

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
    - fvm::Sp(eta1 * hill_, M)
);

solve(MEqn);

ddtM = gMax(mag(fvc::ddt(M)())())/(gMax(M)+SMALL);
