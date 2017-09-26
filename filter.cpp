// filter.cpp - Filtering measured freqency.
// ENEL200 Design Project, building a theremin.
// 25-09-17, Group 13
//
// Convolute a gausian filter over distance data

#define KERNAL_SIZE 13

double distanceBuffer[KERNAL_SIZE] = {0.0};
double filterKernal[KERNAL_SIZE] = {0.000244140625, 0.0029296875, 0.01611328125,
    0.0537109375, 0.120849609375, 0.193359375, 0.2255859375, 0.193359375,
    0.120849609375, 0.0537109375, 0.01611328125, 0.0029296875, 0.000244140625};

// True modulus, result is allways a positive int
int modp(int a, int b) {
    return (a + b) % b;
}

// Apply filter kernal to distance data (i.e. smooth the data)
double apply_filter(float nextDistance) {
    static int nextPos = 3;

    // Update newest value
    distanceBuffer[nextPos] = nextDistance;

    double sum = 0.0;
    for (int i = 0; i < KERNAL_SIZE; i++) {
        int disti = modp(nextPos + i + 1, KERNAL_SIZE);
        sum += distanceBuffer[disti] * filterKernal[i];
    }

    // Increment for next time
    nextPos = modp(nextPos + 1, KERNAL_SIZE);
    return sum;
}


// So that there is no transient sound on startup, initalise to max value
void init_filter_with(double initValue)
{
    for (int i = 0; i < KERNAL_SIZE; distanceBuffer[i++] = initValue);
}