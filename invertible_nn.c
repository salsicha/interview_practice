
// Compile: gcc -o invertible_nn invertible_nn.c
// Run: ./invertible_nn


#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double sigmoid(double x) { 
    return 1 / (1 + exp(-x));
}

double dSigmoid(double x) { 
    return x * (1 - x); 
}

double init_weight() { 
    return ((double)rand()) / ((double)RAND_MAX);
}

void shuffle(int *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int main(int argc, const char * argv[]) {

    static const int num_inputs = 2;
    static const int num_hidden_nodes = 2;
    static const int num_outputs = 4;

    const double lr = 0.1f;
    
    double hidden_layer[num_hidden_nodes];
    double output_layer[num_outputs];
    
    double hidden_layer_bias[num_hidden_nodes];
    double output_layer_bias[num_outputs];

    double hidden_weights[num_inputs][num_hidden_nodes];
    double output_weights[num_hidden_nodes][num_outputs];
    
    static const int num_training_sets = 4;
    double training_inputs[num_training_sets][num_inputs] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
    double training_outputs[num_training_sets][num_outputs] = { {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} };
    
    for (int i = 0; i < num_inputs; i++) {
        for (int j = 0; j < num_hidden_nodes; j++) {
            hidden_weights[i][j] = init_weight();
        }
    }
    for (int i = 0; i < num_hidden_nodes; i++) {
        hidden_layer_bias[i] = init_weight();
        for (int j = 0; j < num_outputs; j++) {
            output_weights[i][j] = init_weight();
        }
    }
    for (int i = 0; i < num_outputs; i++) {
        output_layer_bias[i] = init_weight();
    }
    
    int trainingSetOrder[] = {0,1,2,3};
    
    for (int n = 0; n < 10000; n++) {
        shuffle(trainingSetOrder, num_training_sets);
        for (int x = 0; x < num_training_sets; x++) {
            
            int i = trainingSetOrder[x];
            
            // Forward pass
            
            for (int j = 0; j < num_hidden_nodes; j++) {
                double activation = hidden_layer_bias[j];
                 for (int k = 0; k < num_inputs; k++) {
                    activation += training_inputs[i][k] * hidden_weights[k][j];
                }
                hidden_layer[j] = sigmoid(activation);
            }
            
            for (int j = 0; j < num_outputs; j++) {
                double activation = output_layer_bias[j];
                for (int k = 0; k < num_hidden_nodes; k++) {
                    activation += hidden_layer[k] * output_weights[k][j];
                }
                output_layer[j] = sigmoid(activation);
            }

            printf("input: ");
            for (int j = 0; j < num_inputs; j++) {
                printf("%f ", training_inputs[i][j]);
            }
            printf("\n");
            printf("expected: ");
            for (int j = 0; j < num_outputs; j++) {
                printf("%f ", training_outputs[i][j]);
            }
            printf("\n");
            printf("output: ");
            for (int j = 0; j < num_outputs; j++) {
                printf("%f ", output_layer[j]);
            }
            printf("\n\n");


            // Backprop
            
            double deltaOutput[num_outputs];
            for (int j = 0; j < num_outputs; j++) {
                double errorOutput = (training_outputs[i][j] - output_layer[j]);
                deltaOutput[j] = errorOutput*dSigmoid(output_layer[j]);
            }
            
            double deltaHidden[num_hidden_nodes];
            for (int j = 0; j < num_hidden_nodes; j++) {
                double errorHidden = 0.0f;
                for(int k = 0; k < num_outputs; k++) {
                    errorHidden+=deltaOutput[k] * output_weights[j][k];
                }
                deltaHidden[j] = errorHidden*dSigmoid(hidden_layer[j]);
            }
            
            for (int j = 0; j < num_outputs; j++) {
                output_layer_bias[j] += deltaOutput[j]*lr;
                for (int k = 0; k < num_hidden_nodes; k++) {
                    output_weights[k][j] += hidden_layer[k] * deltaOutput[j] * lr;
                }
            }
            
            for (int j = 0; j < num_hidden_nodes; j++) {
                hidden_layer_bias[j] += deltaHidden[j]*lr;
                for(int k = 0; k < num_inputs; k++) {
                    hidden_weights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr;
                }
            }
        }
    }
    
    // Print weights
    printf("Final Hidden Weights\n[ ");
    for (int j = 0; j < num_hidden_nodes; j++) {
        printf("[ ");
        for(int k = 0; k < num_inputs; k++) {
            printf(" %f", hidden_weights[k][j]);
        }
        printf("] ");
    }
    printf("]\n");

    printf("Final Hidden Biases\n[ ");
    for (int j = 0; j < num_hidden_nodes; j++) {
        printf("%f ", hidden_layer_bias[j]);
    }
    printf("]\n");
    printf("Final Output Weights");
    for (int j = 0; j < num_outputs; j++) {
        printf("[ ");
        for (int k = 0; k < num_hidden_nodes; k++) {
            printf("%f ", output_weights[k][j]);
        }
        printf("]\n");
    }
    printf("Final Output Biases\n[ ");
    for (int j = 0; j < num_outputs; j++) {
        printf("%f ", output_layer_bias[j]);
    }
    printf("]\n");


    // Inverse pass

    double inverse_outputs[num_outputs] = { 0.0f, 0.0f, 1.0f, 1.0f };
    double inverse_inputs[num_inputs];

    // Invert output
    for (int k = 0; k < num_hidden_nodes; k++) {
        double activation = hidden_layer_bias[k];
        for (int j = 0; j < num_outputs; j++) {
            activation += inverse_outputs[j] * output_weights[k][j];
        }
        hidden_layer[k] = sigmoid(activation);
    }
    
    // Invert input
    for (int k = 0; k < num_inputs; k++) {
        double activation = 0.0;
        for (int j = 0; j < num_hidden_nodes; j++) {
            activation += hidden_weights[k][j] * hidden_layer[j];
        }
        inverse_inputs[k] = sigmoid(activation);
    }

    // Inversion
    printf("Inversion: \n[ ");
    for (int j = 0; j < num_inputs; j++) {
        printf("%f ", inverse_inputs[j]);
    }
    printf("]\n");
    printf("Input: \n[ ");
    for (int j = 0; j < num_outputs; j++) {
        printf("%f ", inverse_outputs[j]);
    }
    printf("]\n");

    // end inverse

    return 0;
}

