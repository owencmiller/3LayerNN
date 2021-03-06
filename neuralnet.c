//3 layer deep learning network
//random initial weights
//backpropagation
//
//Spec: 2 synapses
//      3 layers
//      Sigmoid linearity
//
//
//usage: neuralnet [iterations of learning]

#include "neuralnet.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void generate_synapse1(double synapse[]){
    srand(time(NULL));
    for(int col=0;col<NUM_DATA_SETS;col++){
        synapse[col] = (double)rand()/RAND_MAX*2.0-1.0;
    }
}

void generate_synapse0(int size, double synapse[][size]){
    srand(time(NULL));
    for(int row=0; row<NUM_DATA_SETS; row++){
        for(int col=0; col<LEN_DATA; col++){
            synapse[row][col] = (double)rand()/RAND_MAX*2.0-1.0; //rand float between -1 and 1
        }
    }
}

void analyze(int iterations, int size, int data[][size], int solution[]){
    //instantiate matrices and vectors
    double synapse0[LEN_DATA][NUM_DATA_SETS]={0};//(lenght of train data, num of train data)
    double synapse1[NUM_DATA_SETS]={0};//vector size of the num of train data
    int layer0[NUM_DATA_SETS][LEN_DATA]={0};//will be copy of train data
    double layer1[NUM_DATA_SETS][NUM_DATA_SETS]={0};//hidden weights
    double layer2[NUM_DATA_SETS]={0};//final layer to be error checked
    double layer2error[NUM_DATA_SETS]={0};//vector to hold error
    double layer2delta[NUM_DATA_SETS]={0};//hold altered answers
    double layer1error[NUM_DATA_SETS][NUM_DATA_SETS]={0};//vector for layer1 error
    double layer1delta[NUM_DATA_SETS][NUM_DATA_SETS]={0};//hold altered answers for layer1

    double results[NUM_DATA_SETS]={0}; //array to hold the nn results

    //GENERATE SYNAPSES
    generate_synapse0(LEN_DATA, synapse0);//fill both with random data -1,1
    generate_synapse1(synapse1);

#ifdef DEBUG
    printf("INITIAL DATA\n");
    for(int row=0; row<NUM_DATA_SETS; row++){//print the data
        for(int col=0; col<LEN_DATA; col++){
            printf("%d ",data[row][col]);
        }
        printf("\n");
    }
    printf("SOLUTION DATA\n");
    for(int i=0;i<4;i++){
        printf("%d\n", solution[i]);
    }
#endif

    //BEGIN TRAINING LOOP
    for(int train = 0; train<iterations; train++){
#ifdef DEBUG
        printf("=============================================\n");
#endif
        //prepare layer 0
        deepcopy(NUM_DATA_SETS,LEN_DATA,data,layer0);//fill up layer 0
        //LAYER 0 COMPLETE
        //prepare layer 1
        //perform matrix multiplication on layer 0 and synapse0 and store the
        //results in layer 1
        matrix_mult(LEN_DATA, layer0, NUM_DATA_SETS, synapse0, layer1);
#ifdef DEBUG
        printf("SYNAPSE0\n");
        for(int row=0;row<3;row++){
            for(int col=0;col<4;col++){
                printf("%f ",synapse0[row][col]);
            }
            printf("\n");
        }
        printf("SYNAPSE1\n");
        for(int col=0;col<NUM_DATA_SETS;col++){//print synapse1
            printf("%f \n",synapse1[col]);
        }
        printf("LAYER1: MATRIX AFTER MULTIPLICATION, pre sigmoid\n");
        for(int row=0;row<4;row++){
            for(int col=0;col<4;col++){
                printf("%f ",layer1[row][col]);
            }
            printf("\n");
        }
#endif  //push layer one through the nunlinearitly function(sigmoid)
        nonlinearity(NUM_DATA_SETS, layer1);//modify in place
#ifdef DEBUG
        printf("LAYER1: MATRIX AFTER NONLINEARITY, post sigmoid\n");
        for(int row=0;row<4;row++){
            for(int col=0;col<4;col++){
                printf("%f ",layer1[row][col]);
            }
            printf("\n");
        }
#endif
        //LAYER ONE COMPLETED
        //prepare layer 2
        //perform vertor matrix multiplication on layer1 and synapse 1
        //store the results in layer2 matrix
        vector_matrix(NUM_DATA_SETS, layer1, synapse1, layer2);
#ifdef DEBUG
        printf("LAYER2: VECTOR AFTER MULTIPLICATION, pre sigmoid\n");
        for(int i=0;i<NUM_DATA_SETS;i++){
            printf("%f\n", layer2[i]);
        }
#endif
        //run layer2 though the sigmoid function
        nonlinearityVector(layer2);
        for(int ans=0;ans<NUM_DATA_SETS;ans++){
            results[ans] = layer2[ans];
        }
#ifdef DEBUG
        printf("LAYER2: VECTOR AFTER NONLINEARITY, post sigmoid\n");
        for(int i=0;i<NUM_DATA_SETS;i++){
            printf("%f\n", layer2[i]);
        }
#endif
        //LAYER TWO COMPLETED
        //check error and create the arror vector
        checkerror(layer2,solution,layer2error);
#ifdef DEBUG
        printf("LAYER2 ERROR\n");
        for(int i=0;i<NUM_DATA_SETS;i++){
            printf("%f\n",layer2error[i]);
        }
#endif
        if(train%1000==0){
            printf("Percent complete: %f%%",((double)train/(double)iterations)*100);
            printf("%d:!!!!Synapse Error: %f!!!!\n",train,meanabs(layer2error));
        }
        //run through nonlinearity derivative
        //slightly alter confident results
        //greatly alter unconfident results
        nonlinearityprimeVector(layer2);
#ifdef DEBUG
        printf("LAYER2 AFTER NONLIN PRIME\n");
        for(int col = 0;col<NUM_DATA_SETS;col++){
            printf("%f\n",layer2[col]);
        }
#endif
        elementwiseVector(layer2error,layer2,layer2delta);
#ifdef DEBUG
        printf("LAYER2 DELTA\n");
        for(int i = 0;i<NUM_DATA_SETS;i++){
            printf("%f\n",layer2delta[i]);
        }
#endif
        //ammount layer1 contributed to layer2 error
        //perform matrix mult on layer2delta and synapse1
        //which is (A,1) * (1,A) where A>1
        vectorvectordot(layer2delta, synapse1, NUM_DATA_SETS, layer1error);
#ifdef DEBUG
        printf("LAYER1 ERROR\n");
        for(int row=0;row<NUM_DATA_SETS;row++){
            for(int col=0;col<NUM_DATA_SETS;col++){
                printf("%f ",layer1error[row][col]);
            }
            printf("\n");
        }
#endif
        //calculate the layer1delta
        //this is just the layer1error ajusted by the neural nets confidence
        //first run through derivative nonlinearity
        double templayer1[NUM_DATA_SETS][NUM_DATA_SETS]={0};//layer 1 copy
        doubledeepcopy(NUM_DATA_SETS,NUM_DATA_SETS,layer1,templayer1);
        nonlinearityprime(NUM_DATA_SETS, templayer1);
#ifdef DEBUG
        printf("LAYER1 AFTER NONLIN PRIME\n");
        for(int row=0;row<NUM_DATA_SETS;row++){
            for(int col=0;col<NUM_DATA_SETS;col++){
                printf("%f ",templayer1[row][col]);
            }
            printf("\n");
        }
#endif
        elementwiseMatrix(NUM_DATA_SETS, layer1error, templayer1, layer1delta);
#ifdef DEBUG
        printf("LAYER1 DELTA\n");
        for(int row=0;row<NUM_DATA_SETS;row++){
            for(int col=0;col<NUM_DATA_SETS;col++){
                printf("%f ",layer1delta[row][col]);
            }
            printf("\n");
        }
#endif
        //PREPARE TO UPDATE WEIGHTS
        //weight_change = initial_data*layer_delta
        updatesynapse1(synapse1, layer1, layer2delta);
#ifdef DEBUG
        printf("UPDATED SYNAPSE1\n");
        for(int weight=0;weight<NUM_DATA_SETS;weight++){
            printf("%f\n",synapse1[weight]);
        }
#endif
        updatesynapse0(synapse0, layer0, layer1delta);
#ifdef DEBUG
        printf("UPDATED SYNAPSE0\n");
        for(int row=0;row<LEN_DATA;row++){
            for(int col=0;col<NUM_DATA_SETS;col++){
                printf("%f ",synapse0[row][col]);
            }
            printf("\n");
        }
#endif
    }
#ifdef DEBUG
    printf("!!!!Results!!!!\n");
    for(int row=0;row<NUM_DATA_SETS;row++){
        printf("Data Set: ");
        for(int col=0;col<LEN_DATA;col++){
            printf("%d ",data[row][col]);
        }
        printf("- NN Guess: %f%% sure this is correct",(results[row])*100);
        if((results[row]*100)>90){
            printf(" -> 1\n");
        }
        else{
            printf(" -> 0\n");
        }
    }
#endif
    int test_data[NUM_DATA_SETS][LEN_DATA] = {
        {0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,0,0,0,0,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0}
    };
    validate(synapse0,synapse1,test_data);
}

int main(int argc, char * argv[]){
    //NUM_DATA_SETS: 4
    //LEN_DATA: 3
    //data to train
    int data[NUM_DATA_SETS][LEN_DATA] = {
        {1,1,0,0,1,0,0,1,1,0,0,1},
        {0,1,0,0,1,1,1,0,0,0,1,0},
        {1,1,1,1,0,0,1,0,1,0,0,1},
        {1,0,0,1,1,1,0,0,0,1,1,1},
        {1,0,1,1,0,0,0,1,0,1,1,0}
    };

    //the pattern is the 2nd col must contain a 1 and either the
    //0th or 1st col must also be 1, but not both
    int solution[NUM_DATA_SETS] = {0,1,1,0,1};

    if (argc==1){//use default iterations
        analyze(0,LEN_DATA,data,solution);
    }
    else if(argc==2){//use user specified inputs
        analyze(atoi(argv[1]),LEN_DATA,data,solution);
    }
    else{
        printf("Usage: neuralnet [optional iter count]");
    }
    return 0;
}
