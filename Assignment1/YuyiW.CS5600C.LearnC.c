/*
 * YuyiW.c / Assignment1
 *
 * Yuyi Wang / CS5600 / Northeastern University
 * Spring 2024 / Jan 26, 2024
 *
 */
#include <stdio.h>
#include "mpg2km.h"

int main(int argc, char const *argv[])
{
    /* code */
    // Test for mpg2kml
    double test_mpg = 30.0;
    double kml = mpg2kml(test_mpg);
    printf("mpg2kml: %.2f MPG = %.2f Km/L\n", test_mpg, kml);

    // Test for mpg2lphm
    double lphm = mpg2lphm(test_mpg);
    printf("mpg2lphm: %.2f MPG = %.2f L/100km\n", test_mpg, lphm);

    // Test for lph2mpg
    double test_lph = 8.0;
    double mpg = lph2mpg(test_lph);
    printf("lph2mpg: %.2f L/100km = %.2f MPG\n", test_lph, mpg);

    // bad input test cases
    double test_mpg1 = -1;
    double kml1 = mpg2kml(test_mpg1);
    printf("mpg2kml: %.2f MPG = %.2f Km/L\n", test_mpg1, kml1);

    double lphm1 = mpg2lphm(test_mpg1);
    printf("mpg2lphm: %.2f MPG = %.2f L/100km\n", test_mpg1, lphm1);

    double test_lph1 = 0;
    double mpg1 = lph2mpg(test_lph1);
    printf("lph2mpg: %.2f L/100km = %.2f MPG\n", test_lph1, mpg1);

    return 0;
}
