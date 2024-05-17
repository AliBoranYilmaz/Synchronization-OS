#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int **matrix; // this is the dynamic 2d array for matrix (thus pointer to pointer type)
int *sumOfColumns; // this is the array that is shared by threads, holding the sums of columns
int rowNumber, columnNumber; // global row and column number variables

pthread_t *threads; // this the array of threads
sem_t semaphore; // semaphore

void *calculateColumnSum(void *arg) // arg parameter is used for passing the column index to thread
{
    int columnIndex = *((int *)arg); // first, arg is cast to int* and then it is dereferenced to obtain the actual column index
    int sum = 0;

    for (int i=0; i<rowNumber; ++i) // the thread iterates from top to bottom in the column assigned to itself
    {
        sum += matrix[i][columnIndex]; // and calculates the sum of its column
    }
    // now it is the synchronization part
    sem_wait(&semaphore); // lock the mutex variable to ensure that only one thread at a time can update the sumOfColumns array
    sumOfColumns[columnIndex] = sum; // thread adds the sum that it calculated to sumOfColumns array and finishes its task
    sem_post(&semaphore); // after updating the array, mutex is unlocked, allowing other threads to access the array
    pthread_exit(NULL); // exit the thread
}

void swap(int *a, int *b) // the swap function is used while sorting the sum of columns
{
    int temp = *a; // a's value is in temp variable
    *a = *b; // b's value is now in a
    *b = temp; // b now holds a's value
}

void *lastThread(void *arg)
{
    pthread_join(threads[columnNumber - 1], NULL); // wait for all column sum threads to finish their task

    for (int i=0; i<columnNumber-1; ++i) // iterate through the sumOfColumns array
    {
        for (int j=0; j<columnNumber-i-1; ++j)
        {
            if (sumOfColumns[j] > sumOfColumns[j+1]) // if the elements is not in ascending order
            {
                swap(&sumOfColumns[j], &sumOfColumns[j+1]); // last thread uses bubble sort algorithm to sort the sumOfColumns array
            }
        }
    }

    for (int i=0; i<columnNumber; ++i) // iterate through the sumOfColumns array
    {
        printf("%d ", sumOfColumns[i]); // print the sorted sums
    }

    pthread_exit(NULL); // exit the last thread
}

int main() 
{
    scanf("%d %d", &rowNumber, &columnNumber); // take the row and column numbers from the user

    matrix = (int **)malloc(rowNumber * sizeof(int *)); // allocate memory for the rows of the matrix
    // sizeof(int *) is used because the rows of the matrix is also an integer array
    // thus this line allocates enough memory to store rowNumber pointers to integers

    sumOfColumns = (int *)malloc(columnNumber * sizeof(int)); // allocate memory for the array that holds each column's sum
    threads = (pthread_t *)malloc(columnNumber * sizeof(pthread_t)); // allocate memory for threads that each one will calculate sum of one column

    for (int i=0; i<rowNumber; ++i) // iterate through each row
    {
        matrix[i] = (int *)malloc(columnNumber * sizeof(int)); // allocate memory for columnNumber integers for each row
    }

    for (int i=0; i<rowNumber; ++i) // iterate through each row
    {
        for (int j=0; j<columnNumber; ++j) // iterate through each column
        {
            scanf("%d", &matrix[i][j]); // take the matrix elements from the user
        }
    }
    sem_init(&semaphore, 0, 1); // initialize semaphore

    for (int i=0; i<columnNumber; ++i) // iterate through each column
    {
        int *columnIndex = malloc(sizeof(int)); // create columnIndex pointer to use in pthread_create method as a parameter
        *columnIndex = i; // assign it to the current column number
        pthread_create(&threads[i], NULL, calculateColumnSum, columnIndex); // create thread for each column (&threads[i])
        // use default attributes (NULL)
        // created threads will run calculateColumnSum function (its parameter block is (void *arg))
        // each thread will receive its unique column number through columnIndex argument
    }

    pthread_create(&threads[columnNumber - 1], NULL, lastThread, NULL); // create the last thread that will output the final result
    // it will run the function lastThread, and will take no additional arguments

    for (int i=0; i<columnNumber; ++i) // iterate through each thread created
    {
        pthread_join(threads[i], NULL); // wait for all threads to complete their task
    }

    sem_destroy(&semaphore); 

    // after the last thread finishes its task
    free(sumOfColumns); // release the memory held by sumOfColumns

    free(threads); // release the memory held by threads
    
    for (int i=0; i<rowNumber; ++i) // iterate through each row in the matrix
    {
        free(matrix[i]); // release the memory held by each row
    }
    free(matrix); // release the memory held by matrix itself

    return 0;
}