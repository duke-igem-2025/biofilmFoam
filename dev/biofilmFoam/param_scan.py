import subprocess
import shutil
from pathlib import Path
import re

# --- CONFIG ---
# Singularity container path
container = "../openfoam.sif"

# Directory inside the container where code runs
workdir = "/home/openfoam/biofilmFoam/dev/biofilmFoam"

# Base case directory (host side!)
base_dir = Path("base")
results_dir = Path("scan_results")
results_dir.mkdir(exist_ok=True)

# Parameter scan example: E_crit and E
param = "E_crit"
values = [0.1, 0.2, 0.3]

# --- HELPERS ---
def modify_transport_file(case_dir, param, value):
    """Edit transportProperties inside the given case directory."""
    transport_file = case_dir / "constant" / "transportProperties"
    lines = transport_file.read_text().splitlines()
    new_lines = []
    for line in lines:
        if line.strip().startswith(param):
            new_lines.append(f"{param}\t{value};")
        else:
            new_lines.append(line)
    transport_file.write_text("\n".join(new_lines) + "\n")

def modify_setfields_file(case_dir, value):
    """Edit setFieldsDict to update the initial value of E."""
    setfields_file = case_dir / "system" / "setFieldsDict"
    text = setfields_file.read_text()
    new_text = re.sub(
        r"(volScalarFieldValue\s+E\s+)[0-9.eE+-]+",
        rf"\1{value}",
        text
    )
    setfields_file.write_text(new_text)

def run_in_container(cmd, cwd):
    """Run a command inside the singularity container at given host cwd."""
    subprocess.run(
        [
            "singularity", "exec",
            "--bind", f"{Path.cwd()}:{workdir}",
            container,
            "bash", "-c", f"cd {workdir}/{cwd} && {cmd}"
        ],
        check=True
    )

# --- MAIN LOOP ---
for v in values:
    case_name = f"{param}_{v}"
    case_dir = Path(case_name)

    print(f"\n=== Running case: {case_name} ===")

    # Copy base directory → case directory
    if case_dir.exists():
        shutil.rmtree(case_dir)
    shutil.copytree(base_dir, case_dir)

    # Modify input files
    modify_transport_file(case_dir, param, v)
    if param == "E_crit":
        modify_setfields_file(case_dir, v)

    # Run simulation inside container
    run_in_container("./clean", case_name)
    run_in_container("./run", case_name)

    # Run postprocessing inside container
    for cmd in ["biomass", "sessileBiomass", "autoinducer", "eps", "eps_max", "biomass_max"]:
        run_in_container(f"./{cmd}", case_name)

    # Save results
    run_dir = results_dir / case_name
    run_dir.mkdir(exist_ok=True)
    for fname in ["biomass.dat", "sessileBiomass.dat", "autoinducer.dat", "eps.dat"]:
        f = case_dir / fname
        if f.exists():
            shutil.move(str(f), run_dir / f.name)

    # Clean up
    shutil.rmtree(case_dir)

print("\n✅ All scans completed!")
