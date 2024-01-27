#include "mpg2km.h"
const double MPG2KML_RATE = 0.425143707;
const double MPG2LPHM_RATE = 235.214583;

/**
 * Convert miles per gallon (MPG) to kilometers per liter (Km/L).
 *
 * This function converts a fuel efficiency value from miles per gallon
 * to kilometers per liter. If a negative value is passed, the function
 * returns -1 to indicate an error.
 *
 * @param mpg The fuel efficiency in miles per gallon. Should be a non-negative value.
 * @return The fuel efficiency in kilometers per liter, or -1 if input is negative.
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
 * This function converts a fuel efficiency value from miles per gallon
 * to liters per hundred kilometers. The function returns -1 if the input
 * is zero or negative, as it represents an invalid or undefined condition.
 *
 * @param mpg The fuel efficiency in miles per gallon. Should be greater than zero.
 * @return The fuel efficiency in liters per hundred kilometers, or -1 if input is zero or negative.
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
 * This function converts a fuel consumption rate from liters per hour to
 * miles per gallon. It returns -1 if the input is zero or negative, as
 * these values are invalid for fuel consumption rates.
 *
 * @param lph The fuel consumption in liters per hour. Should be greater than zero.
 * @return The fuel efficiency in miles per gallon, or -1 if input is zero or negative.
 */
double lph2mpg(double lph)
{
    if (lph <= 0)
    {
        return -1;
    }

    return MPG2LPHM_RATE / lph;
}