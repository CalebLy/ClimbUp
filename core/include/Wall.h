#pragma once


// TODO: When we eventually implement angled wall flattening from a photo,
// we will also need "ImagePoint" that will be converted to WallPoints
// with a function like convertImagePointToWallPoint(ImagePoint point, WallCalibration calibration);
// For now, focus on getting things to work on a flat vertical wall.
struct WallPoint
{
    double x;
    double y;
};

struct Hold
{
    WallPoint position;
    double radius;
};

struct Wall
{
    double width;
    double height;
    double angleDegrees;
};