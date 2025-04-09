#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h" // ReLU funkcia a pomocne funkcie
#include "data.h" // Makra, vahy a bias hodnoty
void rezim_1(){
    int bias_;
    scanf("%d", &bias_);
    for (int i = 0; i < IMAGE_SIZE; i++) {
        printf("%10.2lf", weights[bias_*IMAGE_SIZE+i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
}

void rezim_2() {
    int num;
    scanf("%d",&num);

    double neuron[IMAGE_SIZE];
    for (int i = 0; i < IMAGE_SIZE;i++){
        scanf("%lf", &neuron[i]);
    }

    double sum;
    for (int i = 0; i < IMAGE_SIZE; i++) {
        sum += (neuron[i] * weights[num*IMAGE_SIZE + i]);
    }
    sum+=bias[num];
    printf("%.2lf\n", sum);
}

int findmax(){
    double nums[10];
    double max_num=nums[0];
    int max_index=0;
    for(int i=0; i<10;i++) {
        scanf("%lf", &nums[i]);
        if(max_num<nums[i]){
            max_num=nums[i];
            max_index=i;
        }
    }
    return max_index;
}

void softmax(double z[], double res[]){
    double max_z=z[0];
    double sum=0;
    for (int i=0;i<10;i++){
        if(max_z<z[i]){
            max_z=z[i];
        }
    }
    for(int i=0; i<10; i++){
        double stupen=exp(z[i]-max_z);
        sum+= stupen;
        res[i]=stupen;
    }
    for(int i=0; i<10; i++){
        res[i]/=sum;
        printf("%1.2lf ", res[i]);
    }
}

// Функция softmax для преобразования суммы взвешенных значений в вероятности
void softmax_7(double *inputs, double *outputs, int len) {
    double sum = 0;
    for (int i = 0; i < len; i++) {
        outputs[i] = exp(inputs[i]);
        sum += outputs[i];
    }
    for (int i = 0; i < len; i++) {
        outputs[i] /= sum;
    }
}


int classify_digit(double *pixels, double *weights, double *bias) {
    double scores[NUMBER_OF_NEURONS];
    for (int i = 0; i < NUMBER_OF_NEURONS; i++) {
        double sum = bias[i];
        for (int j = 0; j < IMAGE_SIZE; j++) {
            sum += pixels[j] * weights[i * IMAGE_SIZE + j];
        }
        scores[i] = sum;
    }


    double probabilities[NUMBER_OF_NEURONS];
    softmax_7(scores, probabilities, NUMBER_OF_NEURONS);

    int max_index = 0;
    double max_probability = probabilities[0];
    for (int i = 1; i < NUMBER_OF_NEURONS; i++) {
        if (probabilities[i] > max_probability) {
            max_probability = probabilities[i];
            max_index = i;
        }
    }

    return max_index;
}

void rezim_7(){
    double obrazok[IMAGE_SIZE];
    for (int i = 0; i < IMAGE_SIZE; i++) {
        scanf("%lf", &obrazok[i]);
    }
    int digit = classify_digit(obrazok, weights, bias);
    printf("%d\n", digit);
}

void rezimy(){
    int num;
    scanf(" %d ", &num);
    if(num==1){
        rezim_1();
    }
    if(num==2){
        rezim_2();
    }
    if(num==3){
        double x;
        scanf("%lf",&x);
        double res=relu(x);
        printf("%.2lf", res);
    }
    if(num==4){
        double pole[10];
        double res[10];
        for (int i=0; i<10; i++){
            scanf("%lf", &pole[i]);
        }
        softmax(pole,res);
    }
    if(num==5){
        int res=findmax();
        printf("%d", res);
    }
    if(num==6){
        double obrazok[IMAGE_SIZE];
        for(int i=0; i<IMAGE_SIZE;i++){
            scanf("%lf", &obrazok[i]);
        }
        print_image(obrazok, IMAGE_WIDTH,IMAGE_HEIGHT);
    }
    if(num==7){
        rezim_7();
    }
}

int main(void) {
    rezimy();
    return 0;
}
