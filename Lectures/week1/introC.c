#include <stdio.h>


const float TEMPRATIO = (5.0f/9.0f);
const float DISTRATIO = 1.609; // Miles to Kilometers
const float MASSRATIO = 0.454; // Pounds to Kg

int main(int argc, char *argv[]) {
    float quantity;
    double converted;
    char type;
    const char *newType = "";
    const char *typeName = "";
    const char *quantType = "";

    printf("Enter a quantity: ");
    scanf("%f %c", &quantity, &type);

    if (type == 'F' || type == 'C') {
        quantType = "temperature";
    } else if (type == 'M' || type == 'K') {
        quantType = "distance";
    } else {
        quantType = "mass";
    }

    if (type == 'F') {
        converted = TEMPRATIO * (quantity - 32.0f);
        typeName = "F" ;
        newType = "C";
    } 
    else if (type == 'C') {
        converted = quantity / TEMPRATIO + 32.0f;
        typeName = "C";
        newType = "F";
    } 
    else if (type == 'M') {
        converted = quantity * DISTRATIO;
        typeName = " miles";
        newType = " kilometers";
    } 
    else if (type == 'K') {
        converted = quantity / DISTRATIO;
        typeName = " kilometers";
        newType = " miles";
    } 
    else if (type == 'P') {
        converted = quantity * MASSRATIO;
        typeName = " pounds";
        newType = " kilograms";
    } 
    else {
        converted = quantity / MASSRATIO;
        typeName = " kilograms";
        newType = " pounds";
    }

    printf("The %s %.1f%s converts to %.1f%s\n", quantType, quantity, typeName, converted, newType);
}