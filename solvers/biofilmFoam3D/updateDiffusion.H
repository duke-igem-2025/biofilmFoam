//volScalarField d2 = (Foam::pow(eps/(1-M),a)*Foam::pow(M,b))();
volScalarField d2 = eps * Foam::pow(M, a) / Foam::pow(1 - M, b);
d2.dimensions().reset(dimensionSet(0,2,-1,0,0)); // m²/s


surfaceScalarField d2f (fvc::interpolate(d2,"d2"));
