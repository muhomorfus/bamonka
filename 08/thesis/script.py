data = [3642, 3604, 3647, 3645, 3643, 3592, 3621, 3599, 3588, 3573]

import statistics

def r(arr):
	return max(arr) - min(arr)

def mean(arr): 
	return sum(arr) / len(arr)

def variance(arr):
	return statistics.variance(arr, xbar=None)

print("R =", r(data))
print("Mean =", mean(data))
print("Variance =", variance(data))
