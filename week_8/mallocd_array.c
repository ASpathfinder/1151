// <PROGRAM-DESCRIPTION>
// Written by <YOUR-NAME-HERE> (<YOUR-ZID>) 
// on <DATE>

#include <stdio.h>
#include <stdlib.h>
//TODO add correct #include for malloc

//////////////// DO NOT CHANGE ANY OF THE CODE BELOW HERE //////////////////

int *scan_array(int size);
double calculate_average(int *array, int size);

int main (void) {

    int size;
    printf("Enter size: ");
    scanf(" %d", &size);
    if (size <= 0) {
        printf("Invalid Size\n");
        return 1;
    }
    printf("Enter %d integers:\n", size);
    int *array = scan_array(size);

    printf("The average of all values in the array is: %.2lf\n", 
           calculate_average(array, size));

    return 0;
}
//////////////// DO NOT CHANGE ANY OF THE CODE ABOVE HERE //////////////////

////////////////////////////////////////////////////////////////////////////
///////////////////// ONLY WRITE CODE BELOW HERE ///////////////////////////
////////////////////////////////////////////////////////////////////////////

// A function which scans in `size` integers and stores them into a 
// malloc'd array.
// returns: a pointer to the malloc'd array
int *scan_array(int size) {
    // TODO: 1. create a MALLOC'd array of size `size`
    int *array;
    array = (int *) malloc(sizeof(signed int)*size);
    int *array_head = array;

    // TODO: 2. Loop through and scan values into the array.
    int number;
    int i = 0;
    while(i<size) {
        scanf(" %d", &number);
        array[i++] = number;
    }

    return array_head;
}

// Given an integer array and it's size, 
// returns the sum of all elements inside the array, divided by the size of the
// array.
double calculate_average(int *array, int size) {
    
    // TODO calculate the sum of the array.
    float sum = 0;
    float average = 0.0;
    for(int i=0;i<size;i++) {
        printf("%d\n", array[i]);
        sum += array[i];
    }
    average = sum / size;
    return average;
}

////////////////////////////////////////////////////////////////////////////
///////////////////// ONLY WRITE CODE ABOVE HERE ///////////////////////////
////////////////////////////////////////////////////////////////////////////
