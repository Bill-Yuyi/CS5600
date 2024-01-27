#include "mpg2km.h"
const double MPG2KML_RATE = 0.425143707;
const double MPG2LPHM_RATE = 235.214583;

/**
 * Convert miles per gallon (MPG) to kilometers per liter (Km/L).
 *
 * This function converts  miles per gallon
 * to kilometers per liter. If a negative value is passed, the function
 * returns -1 to indicate an error.
 *
 * @param mpg  miles per gallon. Should be a non-negative value.
 * @return   kilometers per liter, or -1 if input is negative.
 */
double mpg2kml(double mpg)
{
    if (mpg < 0)
    {
        return -1;
    }

    return mpg * MPG2KML_RATE;
}

/**
 * Convert miles per gallon (MPG) to liters per hundred kilometers (L/100km).
 *
 * This function converts miles per gallon to liters per hundred kilometers.
 * the function returns -1 to indicate an error.
 *
 * @param mpg miles per gallon. Should be greater than zero.
 * @return liters per hundred kilometers, or -1 if input is zero or negative.
 */
double mpg2lphm(double mpg)
{
    if (mpg <= 0)
    {
        return -1;
    }

    return MPG2LPHM_RATE / mpg;
}

/**
 * Convert liters per hour (L/h) to miles per gallon (MPG).
 *
 * This function converts liters per hour to
 * miles per gallon. the function returns -1 to indicate an error.
 *
 * @param lph The fuel consumption in liters per hour. Should be greater than zero.
 * @return miles per gallon, or -1 if input is zero or negative.
 */
double lph2mpg(double lph)
{
    if (lph <= 0)
    {
        return -1;
    }

    return MPG2LPHM_RATE / lph;
}