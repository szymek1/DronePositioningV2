/**
 * @file CoordinatesUtils.h
 * @brief Definition of class aggregating utilities for corrdinate systems
 *		  computations.
 *
 * @details This file contains the definition for class which performs coordinate system computations
 *			in order to recompute different coorindate systems.
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

#pragma once

#include <cmath>
#include <tuple>
#include <utility>
#define M_PI 3.14159265358979323846


/**
 * @class CoordinatesUtils
 * @brief Class responsible for coordinate system computations. It uses algorithm based on 
 *        the WGS84 ellipsoid model.
 *        Source: https://github.com/MichaelTaylor3D/UnityGPSConverter/blob/master/GPSEncoder.cs
 */
class CoordinatesUtils {
public:
  CoordinatesUtils() = default;
  ~CoordinatesUtils() = default;

  /**
   * @brief Recompute GPS coordinates to Unity Coordinates System (UCS).
   * @param lat: latitude in degrees.
   * @param lon: longitude in degrees.
   * @param latOrg: latitude of origin in degrees.
   * @param lonOrg: longitude of origin in degrees.
   * @param meterPerLatitude: pair container holding:
   *						  - meters per latitude  , index [0]
   *						  - meters per longtitude, index [1]
   * 
   * @note altitude remains unchanged here
   */
  static void getGPS2UCS(
      float &lat, float &lon, float &latOrg, float &lonOrg,
      const std::pair<float, float> &meterPerLatitude);

private:

  /**
   * @brief Compute how many meters are in one degree of longitude at given
   *        latitude.
   * @param latitude: latitude in degrees of home position.
   * @return pair of two floats: meters per degree of longitude and meters
   *		 per degree of latitude.
   */
  static const std::pair<float, float> getMetersPerLatitudeDegree_(float& latitude);
};
