import subprocess
# Define the path to your Python file
python_file = "hw5.py"
# Number of times to run the script
num_runs = 5
# List to store the runtimes
runtimes = []
# Run the script multiple times
for _ in range(num_runs):
    # Run the Python file and capture the output
    result = subprocess.run(["python", python_file], capture_output=True, text=True)
    output = result.stdout.strip()
    # Extract the runtime from the output
    runtime = float(output.split()[-2])  # Assuming the runtime is always the second last element
    runtimes.append(runtime)
# Calculate the average runtime
average_runtime = sum(runtimes) / num_runs
print(runtimes)
# Print the average runtime
print("Average runtime:", average_runtime, "milliseconds")
