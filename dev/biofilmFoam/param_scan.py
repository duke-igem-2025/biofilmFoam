import subprocess
import shutil
from pathlib import Path
import re
import multiprocessing as mp
import sys
import numpy as np

# --- CONFIG ---
container = "/hpc/group/bassigem/openfoam.sif"
workdir = "/home/openfoam/biofilmFoam/dev/biofilmFoam"
base_dir = Path("base")
results_dir = Path("scan_results")
results_dir.mkdir(exist_ok=True)

# Parameter scan
params = {
    "E_crit": sorted(list(np.round(np.linspace(0.0, 0.8, 9), 1)) + [0.15, 0.05]),
    "eps_disp": list(np.round(np.linspace(0.0, 0.00006944444444*2, 12), 14)),
    "gamma_eps_prod": list(np.round(np.linspace(0.0, 0.00006944444444*2, 12), 14)),
    "M_b": [0, 1.25, 2.5, 5, 7.5, 10],
    "eps_disp_death": list(np.round(np.linspace(0.0, 0.00006944444444*2, 12), 14)),
    "mu": list(np.round(np.linspace(0.0, 0.00006944444444*2, 12), 14)),
    "tau": list(np.round(np.linspace(10e-09, 70e-09, 7), 14)),
}

# params = {
#     "tau": list(np.round(np.linspace(10e-09, 70e-09, 7), 14)),
# }

# --- HELPERS ---
import re

def modify_transport_file(case_dir, param, value):
    transport_file = case_dir / "constant" / "transportProperties"
    lines = transport_file.read_text().splitlines()
    new_lines = []

    # Regex: matches start of line (ignoring spaces), then exact param, then whitespace
    pattern = re.compile(rf"^\s*{re.escape(param)}\b")

    for line in lines:
        if pattern.match(line):
            # Preserve indentation
            indent = re.match(r"^\s*", line).group(0)
            new_lines.append(f"{indent}{param}\t{value};")
        else:
            new_lines.append(line)

    transport_file.write_text("\n".join(new_lines) + "\n")


def modify_setfields_file(case_dir, value):
    setfields_file = case_dir / "system" / "setFieldsDict"
    text = setfields_file.read_text()
    new_text = re.sub(
        r"(fieldValues\s*\(.*volScalarFieldValue\s+M\s+[0-9.eE+-]+\s+volScalarFieldValue\s+E\s+)[0-9.eE+-]+",
        lambda m: m.group(1) + str(value),
        text,
        flags=re.DOTALL
    )
    setfields_file.write_text(new_text)

def run_in_container(cmd, case_name):
    full_cmd = f"cd {workdir}/{case_name} && {cmd}"
    subprocess.run(
        [
            "singularity", "exec",
            "--bind", "/hpc/group/bassigem/rs670/biofilmFoam:/home/openfoam/biofilmFoam",
            container,
            "bash", "--login", "-c", full_cmd
        ],
        check=True
    )

# --- MAIN CASE FUNCTION ---
def run_case(args):
    param, v = args
    case_name = f"{param}_{v}"
    case_dir = Path(case_name)

    print(f"\n=== Running case: {case_name} ===")

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
    for cmd in ["biomass", "sessileBiomass", "autoinducer", "eps", "eps_max", "biomass_max", "enzyme"]:
        run_in_container(cmd, case_name)

    # Save results
    run_dir = results_dir / case_name
    run_dir.mkdir(exist_ok=True)
    for fname in Path(case_dir).glob("*.dat"):
        shutil.move(str(fname), run_dir / fname.name)

    shutil.rmtree(case_dir)
    return case_name

# --- ENTRY POINT ---
if __name__ == "__main__":
    # Get number of processes from command line (default=1)
    nproc = int(sys.argv[1]) if len(sys.argv) > 1 else 1
    print(f"\n[INFO] Using {nproc} parallel workers\n")

    # Build parameter list
    tasks = [(param, v) for param, values in params.items() for v in values]

    if nproc == 1:
        for t in tasks:
            run_case(t)
    else:
        with mp.Pool(processes=nproc) as pool:
            pool.map(run_case, tasks)

    print("\n✅ All scans completed!")
