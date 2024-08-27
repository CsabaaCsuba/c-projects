#define MAX_LAYER_SIZE 262144 //512 * 512

typedef struct matrix {
    double *data;
    int cols;
    int rows;
} matrix;

typedef struct {
    double value;
    matrix* WeightsMatrixPtr; //should be the size of the previous layer.
    double bias;
} neuron;

typedef struct {
    int neuron_count;
    neuron* NeuronArrPtr;
} layer;




void InitializeRandomMatrixValues(matrix* InputMatrix){
    //this operation ignorex rows and columns.
    srand(time(NULL)+(unsigned int)InputMatrix->rows);
    double randomNum;
    for (int i=0; i<InputMatrix->cols*InputMatrix->rows; i++){
        randomNum = ((double) rand() / (RAND_MAX));
        InputMatrix->data[i] = randomNum;
    }
}

matrix* CreateMatrix(int rows, int columns){
    matrix* MatrixPtr = malloc(sizeof(matrix));
    if (MatrixPtr == NULL){
        printf("Matrix Allocation failed");
        return NULL;
    }
    double* Array = malloc(rows*columns*sizeof(double));
    if (Array == NULL){
        printf("Array allocation failed");
        return NULL;
    }
    
    for (int i=0; i<rows*columns; i++){
        Array[i] = ((double) rand() / (RAND_MAX));
    }
    MatrixPtr->rows = rows;
    MatrixPtr->cols = columns;
    MatrixPtr->data = Array;
    return MatrixPtr;
}


layer* CreateLayer(int NeuronCount, int PrevLayerRows, int PrevLayerCols){
    if (NeuronCount > MAX_LAYER_SIZE){
        printf("Neuron count exceeds the 512*512 limit.");
        return NULL;
    }
    neuron* NeuronArr = malloc(sizeof(neuron)*NeuronCount);
    for (int i=0; i<NeuronCount; i++){
        NeuronArr[i].value = ((double) rand() / (RAND_MAX));
        NeuronArr[i].bias = ((double) rand() / (RAND_MAX));
        matrix* RandomMatrixPtr = CreateMatrix(PrevLayerRows, PrevLayerCols);
        if (RandomMatrixPtr == NULL){
            printf("Matrix wasn't initialized succesfully. ");
            return NULL;
        }
        NeuronArr[i].WeightsMatrixPtr = RandomMatrixPtr;
    }
    layer* LayerPtr = malloc(sizeof(layer));
    LayerPtr->neuron_count = NeuronCount;
    LayerPtr->NeuronArrPtr = NeuronArr;

    return LayerPtr;
}

void PrintMatrix(matrix* PrintableMatrix){
    for (int i=0; i<PrintableMatrix->rows*PrintableMatrix->cols; i++){
        printf("%.3f", PrintableMatrix->data[i]);
        if ((i+1) % PrintableMatrix->cols == 0)
            printf("\n");
    }
}

void PrintLayerNeurons(layer* PrintableLayer){
    for (int i=0; i<PrintableLayer->neuron_count; i++){
        printf("%.3f", PrintableLayer->NeuronArrPtr[i].value);
    }
    printf("\n(%d neurons)\n\n", PrintableLayer->neuron_count);
}