# Name: Gonul AYCI
# Date: Bogazici University, Feb. 2017
# Description: Implementation of several Sorting Algorithms


# insertion sort
def insertionSort(arr):
	for i in range(1, len(arr)):

		currValue = arr[i]
		position = i

		while position > 0 and arr[position - 1] > currValue:
			arr[position] = arr[position - 1]
			position = position - 1

		arr[position] = currValue
	return arr

print insertionSort([5, 5, 1, 27, 91, 3, 38, 9])

# selection sort
def selectionSort(arr):
	for i in range(len(arr)):
		min = i
		for j in range(i+1, len(arr)):
			if arr[j] < arr[min]:
				min = j
		temp = arr[min]
		arr[min] = arr[i]
		arr[i] = temp
	return arr		

print selectionSort([5, 5, 1, 27, 91, 3, 38, 9])

# bubble sort
def bubbleSort(arr):
    for i in range(len(arr)):
        for j in range(len(arr)-1, i, -1):
            if (arr[j] < arr[j-1]):
                temp = arr[j]
                arr[j] = arr[j-1]
                arr[j-1] = temp
    return arr

print bubbleSort([5, 5, 1, 27, 91, 3, 38, 9])

# quick sort 
def quickSort(arr):

    if len(arr) <= 1:
        return arr

    pivot = arr[len(arr) / 2]
    left = [x for x in arr if x < pivot]
    middle = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    return quickSort(left) + middle + quickSort(right)
    
print quickSort([5, 5, 1, 27, 91, 3, 38, 9])


