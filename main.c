/* 
 * Author: Stefan Grönroos <dev@stefang.net>
 * An attempt at a simple library for inserting sensor values into                 
 * Smart-M3 using the C KPI.
 *  
 * Example application
 *  
 * Copyright (c) 2016, Stefan Grönroos.			
 * Copyright (c) 2016, Åbo Akademi University.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <time.h>
#include <stdio.h>
#include "kp.h"

// Describes connection to the SS.
#define KP_SS_ADDRESS "130.232.87.18"
#define KP_SS_PORT 10020

int main() {
  // Initialize smart space
  kp *h = kp_join(KP_SS_ADDRESS, KP_SS_PORT);
  if (!h)
    return -1;

  /* Create dummy sensor value and insert it */
  time_t now;
  time(&now);
  kp_insert_float(h, 13.0f, now);

  kp_leave(h);    
  
  printf("\nKP leave SS...\n");

  return 0;

}
