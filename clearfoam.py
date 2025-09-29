import tkinter as tk
from tkinter import messagebox
import subprocess
import os

TRANSPORT_PROPERTIES_PATH = "dev/biofilmFoam/user_base/transportProperties"

def read_transport_properties():
    props = {}
    if os.path.exists(TRANSPORT_PROPERTIES_PATH):
        with open(TRANSPORT_PROPERTIES_PATH, "r") as f:
            for line in f:
                if "=" in line and not line.strip().startswith("//"):
                    key, value = line.split("=", 1)
                    props[key.strip()] = value.strip().rstrip(";")
    return props

def write_transport_properties(props):
    lines = []
    if os.path.exists(TRANSPORT_PROPERTIES_PATH):
        with open(TRANSPORT_PROPERTIES_PATH, "r") as f:
            for line in f:
                if "=" in line and not line.strip().startswith("//"):
                    key = line.split("=", 1)[0].strip()
                    if key in props:
                        lines.append(f"{key}    {props[key]};\n")
                        continue
                lines.append(line)
    else:
        for k, v in props.items():
            lines.append(f"{k}    {v};\n")
    with open(TRANSPORT_PROPERTIES_PATH, "w") as f:
        f.writelines(lines)

def run_commands():
    try:
        subprocess.run(["./clean"], check=True)
        subprocess.run(["./run"], check=True)
        messagebox.showinfo("Success", "Commands executed successfully.")
    except Exception as e:
        messagebox.showerror("Error", f"Failed to run commands:\n{e}")

def save_and_run():
    for key, entry in entries.items():
        props[key] = entry.get()
    write_transport_properties(props)
    run_commands()

props = read_transport_properties()

root = tk.Tk()
root.title("Transport Properties Editor")

entries = {}
row = 0
for key, value in props.items():
    tk.Label(root, text=key).grid(row=row, column=0, sticky="e")
    entry = tk.Entry(root)
    entry.insert(0, value)
    entry.grid(row=row, column=1)
    entries[key] = entry
    row += 1

tk.Button(root, text="Run", command=save_and_run).grid(row=row, column=0, columnspan=2, pady=10)

root.mainloop()