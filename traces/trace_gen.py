#!/usr/bin/env python3
import random
import argparse
from pathlib import Path

def generate_trace(file_path: str, num_entries: int = 1000):
    """Generate a branch trace file with various patterns"""
    rng = random.Random(42)  # Fixed seed for reproducibility
    
    with open(file_path, 'w') as f:
        # 1. Simple forward branches (mostly not taken)
        for i in range(100):
            branch_pc = 0x400000 + i * 4
            target_pc = branch_pc + 0x20
            f.write(f"{branch_pc:x} {target_pc:x} 0\n")
        
        # 2. Simple backward branches (loop-like, taken multiple times)
        for i in range(100, 200):
            branch_pc = 0x400000 + i * 4
            target_pc = branch_pc - 0x20
            for _ in range(3):  # Loop 3 times
                f.write(f"{branch_pc:x} {target_pc:x} 1\n")
            f.write(f"{branch_pc:x} {target_pc:x} 0\n")  # Exit loop
        
        # 3. Alternating branches (T/NT/T/NT...)
        for i in range(200, 300):
            branch_pc = 0x400000 + i * 4
            target_pc = branch_pc + (0x20 if i % 2 else -0x20)
            outcome = i % 2
            f.write(f"{branch_pc:x} {target_pc:x} {outcome}\n")
        
        # 4. Nested loop pattern
        outer_loop_pc = 0x500000
        inner_loop_pc = 0x500100
        for _ in range(5):  # Outer loop iterations
            # Outer loop branch (taken 4 times)
            for _ in range(4):
                f.write(f"{outer_loop_pc:x} {outer_loop_pc - 0x40:x} 1\n")
            f.write(f"{outer_loop_pc:x} {outer_loop_pc - 0x40:x} 0\n")
            
            # Inner loop (3 iterations each time)
            for _ in range(3):
                for __ in range(2):
                    f.write(f"{inner_loop_pc:x} {inner_loop_pc - 0x20:x} 1\n")
                f.write(f"{inner_loop_pc:x} {inner_loop_pc - 0x20:x} 0\n")
        
        # 5. Random branches with different taken probabilities
        remaining = num_entries - f.tell()//20  # Approximate count
        for _ in range(remaining):
            branch_pc = rng.randint(0x600000, 0x6FFFFF)
            target_pc = branch_pc + rng.choice([-0x20, 0x20])
            outcome = rng.choices([0, 1], weights=[0.3, 0.7])[0]
            f.write(f"{branch_pc:x} {target_pc:x} {outcome}\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Branch Trace Generator')
    parser.add_argument('-o', '--output', type=Path, default='branch.trace',
                       help='Output trace file path')
    parser.add_argument('-n', '--num-entries', type=int, default=1000,
                       help='Approximate number of branch entries')
    args = parser.parse_args()
    
    generate_trace(args.output, args.num_entries)
    print(f"Generated trace file with {args.num_entries} entries at {args.output}")
