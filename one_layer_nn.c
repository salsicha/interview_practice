
// Compile: gcc -o one_layer_nn one_layer_nn.c
// Run: ./one_layer_nn



// #include <iostream>
// #include <list>
// #include <cstdlib>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// Simple network that can learn XOR
// Feartures : sigmoid activation function, stochastic gradient descent, and mean square error fuction

// Potential improvements :
// Different activation functions
// Batch training
// Different error funnctions
// Arbitrary number of hidden layers
// Read training end test data from a file
// Add visualization of training
// Add recurrence? (maybe that should be a separate project)

double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double dSigmoid(double x) { return x * (1 - x); }
double init_weight() { return ((double)rand())/((double)RAND_MAX); }
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int main(int argc, const char * argv[]) {

    static const int numInputs = 2;
    static const int numHiddenNodes = 2;
    static const int numOutputs = 1;
    
    const double lr = 0.1f;
    
    double hiddenLayer[numHiddenNodes];
    double outputLayer[numOutputs];
    
    double hiddenLayerBias[numHiddenNodes];
    double outputLayerBias[numOutputs];

    double hiddenWeights[numInputs][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutputs];
    
    static const int numTrainingSets = 4;
    double training_inputs[numTrainingSets][numInputs] = { {0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f} };
    double training_outputs[numTrainingSets][numOutputs] = { {0.0f},{1.0f},{1.0f},{0.0f} };
    
    for (int i=0; i<numInputs; i++) {
        for (int j=0; j<numHiddenNodes; j++) {
            hiddenWeights[i][j] = init_weight();
        }
    }
    for (int i=0; i<numHiddenNodes; i++) {
        hiddenLayerBias[i] = init_weight();
        for (int j=0; j<numOutputs; j++) {
            outputWeights[i][j] = init_weight();
        }
    }
    for (int i=0; i<numOutputs; i++) {
        outputLayerBias[i] = init_weight();
    }
    
    int trainingSetOrder[] = {0,1,2,3};
    
    for (int n=0; n < 10000; n++) {
        shuffle(trainingSetOrder,numTrainingSets);
        for (int x=0; x<numTrainingSets; x++) {
            
            int i = trainingSetOrder[x];
            
            // Forward pass
            
            for (int j=0; j<numHiddenNodes; j++) {
                double activation=hiddenLayerBias[j];
                 for (int k=0; k<numInputs; k++) {
                    activation+=training_inputs[i][k]*hiddenWeights[k][j];
                }
                hiddenLayer[j] = sigmoid(activation);
            }
            
            for (int j=0; j<numOutputs; j++) {
                double activation=outputLayerBias[j];
                for (int k=0; k<numHiddenNodes; k++) {
                    activation+=hiddenLayer[k]*outputWeights[k][j];
                }
                outputLayer[j] = sigmoid(activation);
            }
            
            printf("input: , %f, %f, output: %f, expected: %f \n", training_inputs[i][0], training_inputs[i][1], outputLayer[0], training_outputs[i][0]);

            // Backprop
            
            double deltaOutput[numOutputs];
            for (int j=0; j<numOutputs; j++) {
                double errorOutput = (training_outputs[i][j]-outputLayer[j]);
                deltaOutput[j] = errorOutput*dSigmoid(outputLayer[j]);
            }
            
            double deltaHidden[numHiddenNodes];
            for (int j=0; j<numHiddenNodes; j++) {
                double errorHidden = 0.0f;
                for(int k=0; k<numOutputs; k++) {
                    errorHidden+=deltaOutput[k]*outputWeights[j][k];
                }
                deltaHidden[j] = errorHidden*dSigmoid(hiddenLayer[j]);
            }
            
            for (int j=0; j<numOutputs; j++) {
                outputLayerBias[j] += deltaOutput[j]*lr;
                for (int k=0; k<numHiddenNodes; k++) {
                    outputWeights[k][j]+=hiddenLayer[k]*deltaOutput[j]*lr;
                }
            }
            
            for (int j=0; j<numHiddenNodes; j++) {
                hiddenLayerBias[j] += deltaHidden[j]*lr;
                for(int k=0; k<numInputs; k++) {
                    hiddenWeights[k][j]+=training_inputs[i][k]*deltaHidden[j]*lr;
                }
            }
        }
    }
    
    // Print weights
    printf("Final Hidden Weights\n[ ");
    for (int j=0; j<numHiddenNodes; j++) {
        printf("[ ");
        for(int k=0; k<numInputs; k++) {
            printf(" %f", hiddenWeights[k][j]);
        }
        printf("] ");
    }
    printf("]\n");

    printf("Final Hidden Biases\n[ ");
    for (int j=0; j<numHiddenNodes; j++) {
        printf("%f ", hiddenLayerBias[j]);
    }
    printf("]\n");
    printf("Final Output Weights");
    for (int j=0; j<numOutputs; j++) {
        printf("[ ");
        for (int k=0; k<numHiddenNodes; k++) {
            printf("%f ", outputWeights[k][j]);
        }
        printf("]\n");
    }
    printf("Final Output Biases\n[ ");
    for (int j=0; j<numOutputs; j++) {
        printf("%f ", outputLayerBias[j]);
    }
    printf("]\n");

    return 0;
}


/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void shuffle(int *array, size_t n) {
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
        }
    }
}


// Activation function and its derivative
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}
double dSigmoid(double x) {
    return x * (1 - x);
}
// Init all weights and biases between 0.0 and 1.0
double init_weight() {
    return ((double)rand()) / ((double)RAND_MAX);
}


int main() {
    // printf() displays the string inside quotation
    printf("NN in C");

    static const int epochs = 1000;

    static const float lr = 0.1;

    static const int numInputs = 2;
    static const int numHiddenNodes = 2;
    static const int numOutputs = 1;
    double hiddenLayer[numHiddenNodes];
    double outputLayer[numOutputs];
    double hiddenLayerBias[numHiddenNodes];
    double outputLayerBias[numOutputs];
    double hiddenWeights[numInputs][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutputs];


    static const int numTrainingSets = 4;
    double training_inputs[numTrainingSets][numInputs] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
    double training_outputs[numTrainingSets][numOutputs] = { {0.0f}, {1.0f}, {1.0f}, {0.0f} };


    // Iterate through the entire training for a number of epochs
    for (int n = 0; n < epochs; n++) {
        // As per SGD, shuffle the order of the training set
        int trainingSetOrder[] = {0,1,2,3};

        shuffle(trainingSetOrder, numTrainingSets);

        // Cycle through each of the training set elements
        for (int x = 0; x < numTrainingSets; x++) {
            int i = trainingSetOrder[x];

            // Compute hidden layer activation
            for (int j = 0; j < numHiddenNodes; j++) {
                double activation = hiddenLayerBias[j];
                for (int k = 0; k < numInputs; k++) {
                    activation += training_inputs[i][k] * hiddenWeights[k][j];
                }
                hiddenLayer[j] = sigmoid(activation);
            }

            // Compute output layer activation
            for (int j = 0; j < numOutputs; j++) {
                double activation = outputLayerBias[j];
                for (int k = 0; k < numHiddenNodes; k++) {
                    activation += hiddenLayer[k] * outputWeights[k][j];
                }
                outputLayer[j] = sigmoid(activation);
            }

            // The next step involves calculating a small incremental change in the network weights 
            // that will move the network towards minimizing the error of the output that the network just computed. 
            // This step starts at the output nodes and works its way backwards. 
            // We calculate the change in weights (deltaOutput) by calculating the 
            // derivative of the error and multiplying it by the derivative of the output at that node. 
            // Here we are using the mean squared error (MSE) function to compute the error. 
            // However notice that for the back-propagation itself, we only need to compute the 
            // derivative of the error which for MSE is just the difference between expected and calculated output. 
            // For the output layer, we calculate this delta per output node:

            // Compute change in output weights
            double deltaOutput[numOutputs];
            for (int j = 0; j < numOutputs; j++) {
                double dError = (training_outputs[i][j] - outputLayer[j]);
                deltaOutput[j] = dError * dSigmoid(outputLayer[j]);
            }

            // For the hidden layer it is a similar process, 
            // with the exception that the error calculation for a given hidden node is the 
            // sum of the error across all output nodes (with the appropriate weight applied to it):

            // Compute change in hidden weights
            double deltaHidden[numHiddenNodes];
            for (int j = 0; j < numHiddenNodes; j++) {
                double dError = 0.0f;
                for (int k = 0; k < numOutputs; k++) {
                    dError += deltaOutput[k] * outputWeights[j][k];
                }
                deltaHidden[j] = dError * dSigmoid(hiddenLayer[j]);
            }

            // Now that we have deltas for each output and hidden node, 
            // the final step is applying them to their respective weight matrices and bias units:

            // Apply change in output weights
            for (int j = 0; j < numOutputs; j++) {
                outputLayerBias[j] += deltaOutput[j] * lr;
                for (int k = 0; k < numHiddenNodes; k++) {
                    outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;

                    printf("new output: %d, %.06f \n", k, outputWeights[k][j]);
                }
            }

            // Apply change in hidden weights
            for (int j=0; j<numHiddenNodes; j++) {
                hiddenLayerBias[j] += deltaHidden[j] * lr;
                for(int k = 0; k < numInputs; k++) {
                    hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr;
                }
            }

        } // training set


        // print data for epoch
        printf("Epoch number: %d \n", n);

        int i = 0;

        // Compute hidden layer activation
        for (int j = 0; j < numHiddenNodes; j++) {
            double activation = hiddenLayerBias[j];
            for (int k = 0; k < numInputs; k++) {
                printf("testing input: %.06f \n", training_inputs[i][k]);
                activation += training_inputs[i][k] * hiddenWeights[k][j];
            }
            hiddenLayer[j] = sigmoid(activation);
        }
        // Compute output layer activation
        for (int j = 0; j < numOutputs; j++) {
            double activation = outputLayerBias[j];
            for (int k = 0; k < numHiddenNodes; k++) {
                activation += hiddenLayer[k] * outputWeights[k][j];
            }
            outputLayer[j] = sigmoid(activation);

            printf("output expected: %.6f, returned: %.6f \n", training_outputs[i][j], outputLayer[j]);
        }


    } // epochs

   return 0;
}
*/


