#!/usr/bin/env python3
"""
Test runner for Radix Heap algorithm on SSSP-1, SSSP-5, and P2P-5 scenarios.
Outputs three separate CSV files.
"""
import os
import subprocess
import glob
import csv
import re
from concurrent.futures import ThreadPoolExecutor, as_completed
import argparse

INPUT_DIR = "../inputs"
OUTPUT_DIR = "../results"
GENERATED_DIR = "../generated"
BINARY = "../bin/radixheap.exe"
MAX_WORKERS = 8

def ensure_dirs():
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

def get_graph_metadata(graph_file):
    """Extract node count and max weight from .gr file."""
    node_count = 0
    max_weight = 0
    try:
        with open(graph_file, 'r') as f:
            for line in f:
                if line.startswith('p sp'):
                    parts = line.split()
                    node_count = int(parts[2])
                elif line.startswith('a '):
                    weight = int(line.split()[3])
                    if weight > max_weight:
                        max_weight = weight
    except:
        return 0, 0
    return node_count, max_weight

def get_largest_instance_per_family(graph_metadata):
    """Identify largest instance per graph family."""
    families = {}
    for base_name, meta in graph_metadata.items():
        match = re.match(r'^([A-Za-z\-]+)', base_name)
        if match:
            family = match.group(1)
            if family not in families or meta['nodes'] > families[family]['nodes']:
                families[family] = {'base_name': base_name, 'nodes': meta['nodes']}
    return families

def run_test(graph_file, config_file, res_file, test_type, max_cost):
    """Run a single test (SSSP-1, SSSP-5, or P2P-5)."""
    if test_type in ["SSSP-1", "SSSP-5"]:
        cmd = [BINARY, "-d", graph_file, "-c", str(max_cost), "-ss", config_file, "-oss", res_file]
    elif test_type == "P2P-5":
        cmd = [BINARY, "-d", graph_file, "-c", str(max_cost), "-p2p", config_file, "-op2p", res_file]
    
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=600)
        if result.returncode != 0:
            return None
        
        if os.path.exists(res_file):
            with open(res_file, 'r') as f:
                for line in f:
                    if line.startswith('t '):
                        return float(line.split()[1])
        return None
    except:
        return None

def main(input_dir, output_dir, workers):
    global INPUT_DIR, OUTPUT_DIR, MAX_WORKERS
    INPUT_DIR = input_dir
    OUTPUT_DIR = output_dir
    MAX_WORKERS = workers
    
    if not os.path.exists(BINARY):
        print(f"ERROR: Binary not found: {BINARY}")
        return
    
    ensure_dirs()
    
    graph_files = sorted(glob.glob(os.path.join(INPUT_DIR, '**', '*.gr'), recursive=True))
    if not graph_files:
        print(f"No .gr files found in {INPUT_DIR}")
        return
    
    graph_metadata = {}
    for graph_path in graph_files:
        base_name = os.path.relpath(graph_path, INPUT_DIR).replace(os.sep, '_').replace('.gr', '')
        node_count, max_cost = get_graph_metadata(graph_path)
        graph_metadata[base_name] = {
            'path': graph_path,
            'nodes': node_count,
            'max_cost': max_cost
        }
    
    print(f"[Radix] Found {len(graph_metadata)} graphs\n")
    
    largest_per_family = get_largest_instance_per_family(graph_metadata)
    print(f"[Radix] Largest instances per family: {list(largest_per_family.keys())}\n")
    
    # === SSSP-1 Tests ===
    print("="*60)
    print("[Radix] Running SSSP-1 tests...")
    print("="*60)
    sssp1_results = []
    tasks = []
    task_info = {}
    
    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as ex:
        for base_name, meta in graph_metadata.items():
            ss1_file = os.path.join(GENERATED_DIR, base_name + ".ss1")
            if not os.path.exists(ss1_file):
                continue
            
            res_file = os.path.join(OUTPUT_DIR, f"{base_name}_radix_sssp1.res")
            task = ex.submit(run_test, meta['path'], ss1_file, res_file, "SSSP-1", meta['max_cost'])
            tasks.append(task)
            task_info[task] = base_name
        
        for i, future in enumerate(as_completed(tasks)):
            base_name = task_info[future]
            exec_time = future.result()
            if exec_time is not None:
                sssp1_results.append((base_name, exec_time))
            print(f"[{i+1}/{len(tasks)}] SSSP-1: {base_name:40} -> {exec_time if exec_time else 'ERROR'}")
    
    # === SSSP-5 Tests ===
    print("\n" + "="*60)
    print("[Radix] Running SSSP-5 tests...")
    print("="*60)
    sssp5_results = []
    tasks = []
    task_info = {}
    
    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as ex:
        for base_name, meta in graph_metadata.items():
            ss5_file = os.path.join(GENERATED_DIR, base_name + ".ss5")
            if not os.path.exists(ss5_file):
                continue
            
            res_file = os.path.join(OUTPUT_DIR, f"{base_name}_radix_sssp5.res")
            task = ex.submit(run_test, meta['path'], ss5_file, res_file, "SSSP-5", meta['max_cost'])
            tasks.append(task)
            task_info[task] = base_name
        
        for i, future in enumerate(as_completed(tasks)):
            base_name = task_info[future]
            exec_time = future.result()
            if exec_time is not None:
                sssp5_results.append((base_name, exec_time))
            print(f"[{i+1}/{len(tasks)}] SSSP-5: {base_name:40} -> {exec_time if exec_time else 'ERROR'}")
    
    # === P2P-5 Tests (Largest Instances Only) ===
    print("\n" + "="*60)
    print("[Radix] Running P2P-5 tests (largest instances)...")
    print("="*60)
    p2p5_results = {}
    tasks = []
    task_info = {}
    
    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as ex:
        for family, largest in largest_per_family.items():
            base_name = largest['base_name']
            meta = graph_metadata[base_name]
            p2p5_file = os.path.join(GENERATED_DIR, base_name + ".p2p5")
            
            if not os.path.exists(p2p5_file):
                print(f"Skipping P2P-5 for {base_name}: missing {p2p5_file}")
                continue
            
            res_file = os.path.join(OUTPUT_DIR, f"{base_name}_radix_p2p5.res")
            task = ex.submit(run_test, meta['path'], p2p5_file, res_file, "P2P-5", meta['max_cost'])
            tasks.append(task)
            task_info[task] = (base_name, family)
        
        for i, future in enumerate(as_completed(tasks)):
            base_name, family = task_info[future]
            exec_time = future.result()
            if exec_time is not None:
                p2p5_results[family] = exec_time
            print(f"[{i+1}/{len(tasks)}] P2P-5: {base_name:40} ({family}) -> {exec_time if exec_time else 'ERROR'}")
    
    # === Write CSV files ===
    print("\n" + "="*60)
    print("[Radix] Writing CSV files...")
    print("="*60)
    
    with open(os.path.join(OUTPUT_DIR, "radix_sssp1.csv"), 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Graf", "Time (s)", "Status"])
        for graph, time_val in sorted(sssp1_results):
            writer.writerow([graph, f"{time_val:.6f}", "OK"])
    print("✓ radix_sssp1.csv")
    
    with open(os.path.join(OUTPUT_DIR, "radix_sssp5.csv"), 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Graf", "Avg Time (s)", "Status"])
        for graph, time_val in sorted(sssp5_results):
            writer.writerow([graph, f"{time_val:.6f}", "OK"])
    print("✓ radix_sssp5.csv")
    
    with open(os.path.join(OUTPUT_DIR, "radix_p2p5.csv"), 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Family", "Time (s)", "Status"])
        for family in sorted(p2p5_results.keys()):
            time_val = p2p5_results[family]
            writer.writerow([family, f"{time_val:.6f}", "OK"])
    print("✓ radix_p2p5.csv")
    
    print("\n" + "="*60)
    print("[Radix] All tests completed!")
    print("="*60)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Run Radix tests on SSSP-1, SSSP-5, P2P-5.')
    parser.add_argument('--input', '-i', default=INPUT_DIR)
    parser.add_argument('--output', '-o', default=OUTPUT_DIR)
    parser.add_argument('--workers', '-w', type=int, default=MAX_WORKERS)
    args = parser.parse_args()
    
    main(args.input, args.output, args.workers)
