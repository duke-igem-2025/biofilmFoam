volScalarField A_dimless_ = max(A / tau, SMALL);
volScalarField hill_ = pow(A_dimless_, n) / (1.0 + pow(A_dimless_, n));

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
