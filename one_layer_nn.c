
// Compile: gcc -o one_layer_nn one_layer_nn.c
// Run: ./one_layer_nn

#include <stdio.h>
int main() {
   // printf() displays the string inside quotation
   printf("Hello, World!");
   return 0;
}



// Activation function and its derivative
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}
double dSigmoid(double x) {
    return x * (1 — x);
}
// Init all weights and biases between 0.0 and 1.0
double init_weight() {
    return ((double)rand())/((double)RAND_MAX);
}