/*---------------------------------------------------------------------------* \


    ]3 | () /= | |_ |\/| /= () /\ |\/|

    Copyright 2023 Toulouse INP
    Romain Guibert

License
    This file is part of biofilmFoam, an extension of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Global
    readTimeControls

\*---------------------------------------------------------------------------*/

const bool adjustTimeStep =
runTime.controlDict().getOrDefault("adjustTimeStep", false);

scalar maxDeltaT =
runTime.controlDict().getOrDefault<scalar>("maxDeltaT", GREAT);

const scalar truncationError_global =
runTime.controlDict().lookupOrDefault<scalar>("truncationError",0.01);

const scalar truncationError_C =
runTime.controlDict().lookupOrDefault<scalar>("truncationError_C",truncationError_global);

const scalar truncationError_M =
runTime.controlDict().lookupOrDefault<scalar>("truncationError_M",truncationError_global);

const scalar truncationError_A =
runTime.controlDict().lookupOrDefault<scalar>("truncationError_A",truncationError_global);

const scalar truncationError_N =
runTime.controlDict().lookupOrDefault<scalar>("truncationError_N",truncationError_global);

const scalar truncationError_E =
runTime.controlDict().lookupOrDefault<scalar>("truncationError_E",truncationError_global);

timestepManager dtManagerC(runTime, C, truncationError_C);
timestepManager dtManagerM(runTime, M, truncationError_M);
timestepManager dtManagerA(runTime, A, truncationError_A);
timestepManager dtManagerN(runTime, N, truncationError_N);
timestepManager dtManagerE(runTime, E, truncationError_E);


// ************************************************************************* //
