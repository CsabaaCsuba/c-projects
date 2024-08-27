#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
// #include "parsefile.h"
#include "matrix.h"

// #define RAND_MAX 1000;




int main(){
    const int DESIRED_COLUMNS = 28;
    const int DESIRED_ROWS = 28;
    const int NEURON_COUNT = 16;
    srand(time(NULL));

    matrix* SecondMatrix = CreateMatrix(DESIRED_ROWS, DESIRED_COLUMNS);

    free(SecondMatrix->data);
    free(SecondMatrix);

    layer* InputLayer = CreateLayer(28*28, 0,0); //there is no previous layer, so the previous matrix can't have a dimension


    layer* Hidden1 = CreateLayer(NEURON_COUNT, 28, 28);
    PrintMatrix(Hidden1->NeuronArrPtr[0].WeightsMatrixPtr);

    layer* Hidden2 = CreateLayer(NEURON_COUNT, NEURON_COUNT, NEURON_COUNT);
    PrintMatrix(Hidden2->NeuronArrPtr[0].WeightsMatrixPtr);

    layer* OutputLayer = CreateLayer(10, NEURON_COUNT, NEURON_COUNT);
    PrintMatrix(OutputLayer->NeuronArrPtr[0].WeightsMatrixPtr);


    printf("First layer's neurons: \n");
    PrintLayerNeurons(InputLayer);

    printf("Second layer's neurons: \n");
    PrintLayerNeurons(Hidden1);

    printf("THird layer's neurons: \n");
    PrintLayerNeurons(Hidden2);

    printf("Output layer's neurons: \n");
    PrintLayerNeurons(OutputLayer);


    //TODO:
    //This returns an array of arrays.
    matrix* TrainValuesPtr = malloc(sizeof(matrix));
    if (TrainValuesPtr == NULL){
        printf("pointer is null.");
        return -1;
    }
    
    // TrainValuesPtr->cols=60000;
    // TrainValuesPtr->rows=DESIRED_COLUMNS*DESIRED_ROWS;
    // double* MatrixValues = malloc(TrainValuesPtr->cols*TrainValuesPtr->rows*sizeof(double)+sizeof(double));
    // TrainValuesPtr->data = MatrixValues;
    // ParseFileIntoMatrix((char *)&"fashionmnist/fashion-mnist_train.csv", TrainValuesPtr, 28*28, 10);

    // PrintMatrix(TrainValuesPtr);
    
    // free(InputLayer->NeuronArrPtr->WeightsMatrixPtr);
    // free(InputLayer->NeuronArrPtr);
    // free(InputLayer);
    //28*28 input layer -> 128 neurons -> 128 neurons -> 10 neurons output layer.

    return 0;
}