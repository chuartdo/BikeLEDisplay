/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

/*
   Calulate LED lighting effect based on acceleration parameters from
   Intel(R) Curie(TM) module.
*/

#include "CurieIMU.h"
#define MIN_MOVEMENT  7

void OrientationSetup() {
  
  CurieIMU.begin();

  // Set the accelerometer range to degrees/second
  CurieIMU.setGyroRange(125);
}

int ToColor(float val) {
  int color = 0;
  if (abs(val) > MIN_MOVEMENT) {
    color = abs(val);
    if (val > 0)
     ;
    else {
      color += abs(val) + 128;
    }

    if (DEBUG) { 
      Serial.print("\t Color:");
      Serial.print(BaseLedColor);
      Serial.println();
    }
  }
  return color;
}

void MotionSensingLoop() {
  float gx, gy, gz; //scaled Gyro values
  // Map color to orientation

  // read gyro measurements from device, scaled to the configured range
  CurieIMU.readGyroScaled(gx, gy, gz);

  BaseLedColor = leds.Color(ToColor(gx),ToColor(gy),ToColor(gz));

  /*
  // display tab-separated gyro x/y/z values
  if (DEBUG) {
  Serial.print("g:\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.print(gz);
  }
  */
}

/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/
