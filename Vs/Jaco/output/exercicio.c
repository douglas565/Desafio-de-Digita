#include <math.h>
#include <stdio.h>

typedef struct {
    float x;
    float y;
} Ponto;


float distancia(Ponto p1, Ponto p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrt(dx*dx + dy*dy);
}

int main(){
    Ponto a;
    Ponto b;
    
    a.x = 5;
    a.y = 7;
    b.x = 9;
    b.y = 12;


    printf("Distancia entre os pontos: %f\n", distancia(a, b));
    return 0;
}
