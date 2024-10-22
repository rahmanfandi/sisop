# Define the problem from the image
processes = ["P0", "P1", "P2", "P3", "P4"]
available = [3, 3, 2]  # Resources A, B, C
max_need = [
    [7, 5, 3],  # Max need for P0
    [3, 2, 2],  # Max need for P1
    [9, 0, 2],  # Max need for P2
    [2, 2, 2],  # Max need for P3
    [4, 3, 3],  # Max need for P4
]
allocation = [
    [0, 1, 0],  # Allocated resources for P0
    [2, 0, 0],  # Allocated resources for P1
    [3, 0, 2],  # Allocated resources for P2
    [2, 1, 1],  # Allocated resources for P3
    [0, 0, 2],  # Allocated resources for P4
]

# Calculate need matrix
n_processes = len(processes)
n_resources = len(available)
need = [
    [max_need[i][j] - allocation[i][j] for j in range(n_resources)]
    for i in range(n_processes)
]


def is_safe_with_skip(processes, available, need, allocation):
    n_processes = len(processes)
    n_resources = len(available)

    # Mark all processes as unfinished
    finished = [False] * n_processes

    # To store the safe sequence
    safe_sequence = []

    # Make a copy of available resources to track changes
    work = available[:]

    # Store step-by-step information
    steps = []

    while len(safe_sequence) < n_processes:
        found = False
        for i in range(n_processes):
            if not finished[i]:
                # Check if the process's needs can be satisfied with available resources
                if all(need[i][j] <= work[j] for j in range(n_resources)):
                    # Log the step before allocation
                    step_info = f"Process {processes[i]} can be executed, need {need[i]} <= available {work}"
                    steps.append(step_info)

                    # Allocate resources to the process and add it to the safe sequence
                    for j in range(n_resources):
                        work[j] += allocation[i][j]

                    # Log resource update
                    steps.append(
                        f"After process {processes[i]} execution, available: {work}"
                    )

                    safe_sequence.append(processes[i])
                    finished[i] = True
                    found = True

        # If no process was found that can run
        if not found:
            break  # Exit loop if no more processes can be executed

    final_result = f"Final available resources: {work}"
    return True, safe_sequence, steps, final_result


# Run the algorithm with step-by-step output
is_safe_state, safe_sequence, steps, final_result = is_safe_with_skip(
    processes, available, need, allocation
)

# Print each step
print("Step by step execution:")
for step in steps:
    print(step)

# Print the final result
print("\nSafe sequence: ", safe_sequence)
print(final_result)
