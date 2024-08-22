/**
 * @file CoordinatesUtils.cpp
 * @brief Code implementing cooridnates computations utilities.
 *
 * @details This file contains the definiton of coordinates computations utilities. It uses algorithm based on the WGS84 ellipsoid model. 
 *          It mainly recalculates GPS coordinates to Unity Coordinates System (UCS).
 *
 * @author Szymon Bogus
 * @date 2024-08-22
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 *
 * @note This is still a work in progress.
 */

#include "../include/CoordinatesUtils.h"

void CoordinatesUtils::getGPS2UCS(
    float &lat, float &lon, float &latOrg, float &lonOrg,
    const std::pair<float, float> &meterPerLatitude) const {

    lat = std::get<0>(meterPerLatitude) * (lat - latOrg); // z-axis in UCS
    lon = std::get<1>(meterPerLatitude) * (lon - lonOrg); // x-axis in UCS
    // alt = alt                                             y-axis in UCS, altitude remains unchanged
}

const std::pair<float, float>
CoordinatesUtils::getMetersPerLatitudeDegree_(float& latitude) const {

  constexpr float m1 = 111132.92; // latitude calculation term 1
  constexpr float m2 = -559.82;   // latitude calculation term 2
  constexpr float m3 = 1.175;     // latitude calculation term 3
  constexpr float m4 = -0.0023;   // latitude calculation term 4
  constexpr float p1 = 111412.84; // longitude calculation term 1
  constexpr float p2 = -93.5;     // longitude calculation term 2
  constexpr float p3 = 0.118;     // longitude calculation term 3

  latitude = latitude * (M_PI / 180.0);

  const float metersPerLat = m1 + (m2 * cos(2 * latitude)) +
                             (m3 * cos(4 * latitude)) +
                             (m4 * cos(6 * latitude));

  const float metersPerLon = (p1 * cos(latitude)) + (p2 * cos(3 * latitude)) +
                             (p3 * cos(5 * latitude));

  return std::make_pair(metersPerLat, metersPerLon);
}
