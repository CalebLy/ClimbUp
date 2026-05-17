#pragma once

// Provide default values in case user does not provide them
// or if we want to use them for tests.
struct UserProportions
{
    // Height in meters
    double height = 1.75;

    // ape Index in meters
    double apeIndex = 0.0;

    // weight in kg
    double weight = 70.0;
};
