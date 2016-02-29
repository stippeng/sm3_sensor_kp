/* 
 * Author: Stefan Grönroos <dev@stefang.net>
 * An attempt at a simple library for inserting sensor values into                 
 * Smart-M3 using the C KPI. 
 * Based on HelloWorld example by Aleksandr A. Lomov 
 * in the Smart-M3 C KPI Library.
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

#ifndef KP_H
#define KP_H

#include <sys/types.h>

typedef struct kp kp;

kp *kp_join(char *host, uint16_t port);
int kp_leave(kp *h);
int kp_insert_float(kp *h, float val, time_t t);


#endif
