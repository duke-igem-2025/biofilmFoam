import subprocess
import shutil
from pathlib import Path

# Base case directory
base_dir = Path("base")
results_dir = Path("scan_results")
results_dir.mkdir(exist_ok=True)

# Example scan: E_crit and E
param = "E_crit"
values = [0.1, 0.2, 0.3]

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

    # Replace `volScalarFieldValue E <number>` with the new value
    import re
    new_text = re.sub(
        r"(volScalarFieldValue\s+E\s+)[0-9.eE+-]+",
        rf"\g<1>{value}",
        text
    )

    setfields_file.write_text(new_text)

for v in values:
    case_name = f"{param}_{v}"
    case_dir = Path(case_name)

    print(f"\n=== Running case: {case_name} ===")

    # Copy base directory → case directory
    if case_dir.exists():
        shutil.rmtree(case_dir)
    shutil.copytree(base_dir, case_dir)

    # Modify transportProperties
    modify_transport_file(case_dir, param, v)

    # If scanning E_crit, also update E in setFieldsDict
    if param == "E_crit":
        modify_setfields_file(case_dir, param, v)

    # Run simulation in the case directory
    subprocess.run(["./clean"], cwd=case_dir, check=True)
    subprocess.run(["./run"], cwd=case_dir, check=True)

    # Run data extraction commands
    for cmd in ["biomass", "sessileBiomass", "autoinducer", "eps", "eps_max", "biomass_max"]:
        subprocess.run([f"./{cmd}"], cwd=case_dir, check=True)

    # Save results
    run_dir = results_dir / case_name
    run_dir.mkdir(exist_ok=True)

    for fname in ["biomass.dat", "sessileBiomass.dat", "autoinducer.dat", "eps.dat"]:
        f = case_dir / fname
        if f.exists():
            shutil.move(str(f), run_dir / f.name)

    # Delete case directory to save space
    shutil.rmtree(case_dir)

print("\n✅ All scans completed!")
