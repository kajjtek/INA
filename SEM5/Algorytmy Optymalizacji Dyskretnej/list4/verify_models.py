#!/usr/bin/env python3
"""
verify_models.py

Run each C++ binary with --glpk to produce a MathProg model, solve it with glpsol,
and compare the objective returned by glpsol with the program's own reported value.

Usage examples:
  python3 verify_models.py --task 1 --k 5 --runs 3
  python3 verify_models.py --all --runs 1

Requirements:
  - glpsol (GLPK) available on PATH. On macOS: brew install glpk
  - compiled binaries: ./ex1/ex1, ./ex2/ex2, ./ex4/ex4

Logs and mismatches are saved to ./verify_results/
"""

import argparse
import subprocess
import re
import os
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent
RESULT_DIR = ROOT / 'verify_results'
RESULT_DIR.mkdir(exist_ok=True)

# Binary mapping: task id -> executable path and default ranges
BIN_INFO = {
    '1': {'path': ROOT / 'ex1' / 'ex1', 'k_range': range(1, 17)},
    '2': {'path': ROOT / 'ex2' / 'ex2', 'k_range': range(3, 11)},
    '4': {'path': ROOT / 'ex4' / 'ex4', 'k_range': range(1, 17)},
}

# Regex patterns to parse program outputs
PROGRAM_OUT_PATTERNS = [
    re.compile(r"Maksymalny przeplyw(?: \(.*\))?:\s*([0-9]+)"),
    re.compile(r"Rozmiar maksymalnego skojarzenia:\s*([0-9]+)"),
]

# Regex to parse display name from model file ("display MaxFlow;" or "display TotalMatchingSize;")
DISPLAY_RE = re.compile(r"display\s+([A-Za-z0-9_]+)\s*;", re.IGNORECASE)

# Regex to parse glpsol output for the displayed variable
GLPSOL_VALUE_RE = re.compile(r"([A-Za-z0-9_]+)\s*[:=]?\s*([0-9]+)")


def parse_program_output(stdout: str) -> (str, int):
    """Return (name, value) parsed from C++ program stdout. Name is guessed from matched label."""
    for pat in PROGRAM_OUT_PATTERNS:
        m = pat.search(stdout)
        if m:
            # label name guessed from pattern's first matching group context
            label = None
            text = m.group(0)
            if 'Maksymalny' in text:
                label = 'MaxFlow'
            elif 'Rozmiar maksymalnego skojarzenia' in text:
                label = 'TotalMatchingSize'
            else:
                label = 'Value'
            return label, int(m.group(1))
    # fallback: find last integer in stdout
    nums = re.findall(r"([0-9]+)", stdout)
    if nums:
        return 'Value', int(nums[-1])
    return 'Value', None


def parse_model_display_name(model_path: Path) -> str:
    txt = model_path.read_text(encoding='utf-8')
    m = DISPLAY_RE.search(txt)
    if m:
        return m.group(1)
    return None


def parse_glpsol_output(stdout: str, display_name: str = None) -> int:
    # If display_name provided, try to find its printed value
    if display_name:
        pat = re.compile(rf"{re.escape(display_name)}\s*[:=]?\s*([0-9]+)")
        m = pat.search(stdout)
        if m:
            return int(m.group(1))
    # Fallback: find any lines like NAME = value or NAME: value
    for m in GLPSOL_VALUE_RE.finditer(stdout):
        name, val = m.group(1), m.group(2)
        # prefer obvious labels
        if display_name and name.lower() == display_name.lower():
            return int(val)
    # Fallback: last integer
    nums = re.findall(r"([0-9]+)", stdout)
    if nums:
        return int(nums[-1])
    return None


def compute_max_flow_from_caps(caps):
    """Compute max flow from list of (u,v,capacity) using Edmonds-Karp (integer capacities).
    Returns integer max flow from source 0 to sink = max node id.
    """
    # Build adjacency and capacity maps
    nodes = set()
    for u, v, c in caps:
        nodes.add(u); nodes.add(v)
    if not nodes:
        return 0
    source = min(nodes)
    sink = max(nodes)
    # build graph
    from collections import deque, defaultdict
    adj = defaultdict(list)
    cap = {}
    for u, v, c in caps:
        adj[u].append(v)
        adj[v].append(u)
        cap[(u, v)] = cap.get((u, v), 0) + c
        cap.setdefault((v, u), 0)

    def bfs(parent):
        q = deque([source])
        parent.clear()
        parent[source] = None
        while q:
            u = q.popleft()
            for v in adj[u]:
                if v not in parent and cap.get((u, v), 0) > 0:
                    parent[v] = u
                    if v == sink:
                        return True
                    q.append(v)
        return False

    flow = 0
    parent = {}
    while bfs(parent):
        # find bottleneck
        v = sink
        bottleneck = float('inf')
        while v != source:
            u = parent[v]
            bottleneck = min(bottleneck, cap.get((u, v), 0))
            v = u
        if bottleneck == float('inf') or bottleneck == 0:
            break
        # augment
        v = sink
        while v != source:
            u = parent[v]
            cap[(u, v)] = cap.get((u, v), 0) - bottleneck
            cap[(v, u)] = cap.get((v, u), 0) + bottleneck
            v = u
        flow += bottleneck

    return int(flow)


def run_one_check(bin_path: Path, args: list, model_path: Path, runs_dir: Path) -> dict:
    """Run the program with given args (should include --glpk model_path), then run glpsol on model_path and compare."""
    res = {'binary_cmd': [str(bin_path)] + args, 'model': str(model_path)}

    # Run program
    try:
        p = subprocess.run([str(bin_path)] + args, capture_output=True, text=True, check=True)
    except FileNotFoundError:
        res['error'] = f"Binary not found: {bin_path}"
        return res
    except subprocess.CalledProcessError as e:
        res['error'] = f"Program failed: {e}"
        res['stdout'] = e.stdout
        res['stderr'] = e.stderr
        return res

    res['stdout'] = p.stdout
    res['stderr'] = p.stderr

    # Parse program's reported objective
    prog_name, prog_val = parse_program_output(p.stdout)
    res['prog_name'] = prog_name
    res['prog_value'] = prog_val

    # Ensure model file exists
    if not model_path.exists():
        res['error'] = f"Model file not created: {model_path}"
        return res

    # Detect display name from model
    display_name = parse_model_display_name(model_path)
    res['model_display'] = display_name

    # Run glpsol
    # Run glpsol and ask it to write display output to a separate file so we can parse it reliably
    display_file = model_path.with_suffix('.display')
    try:
        g = subprocess.run(['glpsol', '--model', str(model_path), '--display', str(display_file)], capture_output=True, text=True, check=True)
    except FileNotFoundError:
        res['glpsol_error'] = 'glpsol (GLPK) not found on PATH; install it (brew install glpk)'
        return res
    except subprocess.CalledProcessError as e:
        res['glpsol_stdout'] = e.stdout
        res['glpsol_stderr'] = e.stderr
        res['glpsol_error'] = f'glpsol failed: {e}'
        return res

    res['glpsol_stdout'] = g.stdout
    res['glpsol_stderr'] = g.stderr

    # Read display file if present and parse value
    glp_val = None
    if display_file.exists():
        try:
            disp_txt = display_file.read_text(encoding='utf-8')
            # Try to parse integer value from display output
            glp_val = parse_glpsol_output(disp_txt, display_name)
            res['glpsol_display'] = disp_txt
        except Exception as ex:
            res['glpsol_display_error'] = str(ex)
    else:
        # Fallback: try to parse stdout
        glp_val = parse_glpsol_output(g.stdout, display_name)
    res['glpsol_value'] = glp_val
    # --- As a fallback/independent check: parse the generated model's 'param cap' table
    # and compute max-flow in Python (Edmonds-Karp) to verify the program's result.
    try:
        caps = []
        txt = model_path.read_text(encoding='utf-8')
        m = re.search(r"param\s+cap\s*:=\s*(.*?)\s*;", txt, re.S)
        if m:
            body = m.group(1)
            for line in body.splitlines():
                line = line.strip()
                if not line:
                    continue
                parts = line.split()
                if len(parts) >= 3:
                    u = int(parts[0]); v = int(parts[1]); c = int(parts[2])
                    caps.append((u, v, c))
        if caps:
            py_val = compute_max_flow_from_caps(caps)
            res['py_maxflow'] = py_val
        else:
            res['py_maxflow'] = None
    except Exception as ex:
        res['py_maxflow_error'] = str(ex)

    # Determine match: prefer glpsol value if available, otherwise use python computed maxflow
    res['match'] = False
    if res.get('prog_value') is not None:
        if res.get('glpsol_value') is not None:
            try:
                res['match'] = int(res['prog_value']) == int(res['glpsol_value'])
            except Exception:
                res['match'] = False
        elif res.get('py_maxflow') is not None:
            try:
                res['match'] = int(res['prog_value']) == int(res['py_maxflow'])
            except Exception:
                res['match'] = False

    # Save run logs
    run_id = len(list(runs_dir.glob('run-*.json'))) + 1
    import json
    out_file = runs_dir / f'run-{run_id}.json'
    out_file.write_text(json.dumps(res, indent=2), encoding='utf-8')

    return res


def ensure_binary(bin_path: Path) -> bool:
    return bin_path.exists() and os.access(bin_path, os.X_OK)


def main():
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--task', choices=['1','2','4'], help='Task id to verify')
    group.add_argument('--all', action='store_true', help='Verify all tasks')
    parser.add_argument('--k', type=int, help='Value of k to test (default: iterate typical range)')
    parser.add_argument('--max-k', type=int, help='If provided, run k from default min up to this max (inclusive)')
    parser.add_argument('--degree', type=int, help='For task 2: degree i (optional)')
    parser.add_argument('--runs', type=int, default=1, help='Number of independent runs per k')
    parser.add_argument('--out', type=str, default=str(RESULT_DIR), help='Directory to store results')

    args = parser.parse_args()

    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)

    tasks = ['1','2','4'] if args.all else [args.task]

    summary = []

    for task in tasks:
        info = BIN_INFO[task]
        bin_path = info['path']
        if not ensure_binary(bin_path):
            print(f"Binary for task {task} not found or not executable at {bin_path}. Skipping.")
            summary.append({'task': task, 'error': 'binary_missing'})
            continue

        # Determine k values: single --k, or range up to --max-k, or default range
        if args.k:
            k_values = [args.k]
        elif args.max_k:
            start = info['k_range'].start
            end = min(args.max_k, info['k_range'].stop - 1)
            k_values = list(range(start, end + 1))
        else:
            k_values = list(info['k_range'])
        task_dir = out_dir / f'task-{task}'
        task_dir.mkdir(exist_ok=True)

        for k in k_values:
            for r in range(args.runs):
                model_path = task_dir / f'model_task{task}_k{k}_run{r+1}.mod'
                runs_dir = task_dir / 'runs'
                runs_dir.mkdir(exist_ok=True)

                # Build binary args
                bin_args = ['--size', str(k), '--glpk', str(model_path)]
                if task == '2':
                    if args.degree:
                        bin_args += ['--degree', str(args.degree)]
                    else:
                        # pick a small default degree if not provided
                        bin_args += ['--degree', '1']

                print(f"Running task {task}, k={k}, run={r+1}...")
                res = run_one_check(bin_path, bin_args, model_path, runs_dir)

                ok = res.get('match', False)
                if 'error' in res:
                    print(f"  ERROR: {res['error']}")
                elif 'glpsol_error' in res:
                    print(f"  GLPSOL ERROR: {res['glpsol_error']}")
                else:
                    print(f"  prog={res.get('prog_value')} glpsol={res.get('glpsol_value')} match={ok}")

                summary.append({'task': task, 'k': k, 'run': r+1, 'result': res})
                # Append result to CSV for this task
                try:
                    import csv
                    csv_path = task_dir / 'comparison.csv'
                    write_header = not csv_path.exists()
                    # parse program stderr for time and augmenting paths
                    prog_time = None
                    aug_paths = None
                    if res.get('stderr'):
                        m_time = re.search(r"Czas wykonania:\s*([0-9]+)\s*us", res['stderr'])
                        if m_time:
                            prog_time = int(m_time.group(1))
                        m_paths = re.search(r"Liczba sciezek powiekszajacych(?: \(.*\))?:\s*([0-9]+)", res['stderr'])
                        if m_paths:
                            aug_paths = int(m_paths.group(1))

                    with open(csv_path, 'a', newline='') as f:
                        writer = csv.writer(f)
                        if write_header:
                            writer.writerow(['task','k','run','prog_value','glpsol_value','py_maxflow','match','prog_time_us','augmenting_paths','model'])
                        writer.writerow([
                            task,
                            k,
                            r+1,
                            res.get('prog_value'),
                            res.get('glpsol_value'),
                            res.get('py_maxflow'),
                            res.get('match'),
                            prog_time,
                            aug_paths,
                            res.get('model')
                        ])
                except Exception as ex:
                    print(f"  Warning: failed to write CSV row: {ex}")

    # Print summary
    total = len(summary)
    matches = sum(1 for s in summary if isinstance(s.get('result'), dict) and s['result'].get('match'))
    print('\nVerification finished. Summary:')
    print(f'  total checks: {total}, matches: {matches}')
    print(f'  results and logs saved under: {out_dir}')


if __name__ == '__main__':
    main()
