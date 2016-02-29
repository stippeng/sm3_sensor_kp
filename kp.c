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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#include <ckpi/ckpi.h>
#include <uuid.h>
#include "kp.h"

// ---Constants---

#define KP_SS_NAME "X"
#define KP_NAME "SENSOR_KP"

// Namespaces and such
#define SSN "http://purl.oclc.org/NET/ssnx/ssn#"
#define NS  "http://dummy.org/sensors#"
#define RDF "http://www.w3.org/1999/02/22-rdf-syntax-ns#"
#define DUL "http://www.loa-cnr.it/ontologies/DUL.owl#"
#define XML_DATE "^^<http://www.w3.org/2001/XMLSchema#dateTime>"

struct kp {
  ss_info_t info;
};

typedef struct {
  uuid_t id;
  char str[37];
} uniq_t;


/****************** Static functions list **********************/
static void generate_uuid(uniq_t *u);
static char *cat(char *dst, char *src0, char *src1, size_t maxlen);
/*******************************************************/

/* 
   Join smart-space at host:port 
   A random name is given
*/
kp *kp_join(char *ip, uint16_t port)
{
  uniq_t ss_id;
  generate_uuid(&ss_id);
  
  kp *h = (kp *)malloc(sizeof(kp));
  printf("UID: %s\n", ss_id.str);
  ss_init_space_info(&h->info, ss_id.str, ip, port);

  if (ss_join(&h->info, KP_NAME) == -1) {
    fprintf(stderr, "Can't join SS\n");
    free(h);
    return NULL;
  }
  
  fprintf(stderr, "KP joined SS\n");

  return h;
}

#define MK_PRED(ns, pval) (cat(p,(ns),(pval),SS_SUBJECT_MAX_LEN))
#define MK_OBJ(ns, oval)  (cat(o,(ns),(oval),SS_OBJECT_MAX_LEN))
int kp_insert_float(kp *h, float val, time_t t)
{
  ss_triple_t *trip = NULL;
  ss_bnode_t *bnode = (ss_bnode_t *)malloc(sizeof(ss_bnode_t *));
  uniq_t obs, s_obs, obs_val, obs_t;
  generate_uuid(&obs);
  generate_uuid(&s_obs);
  generate_uuid(&obs_val);
  generate_uuid(&obs_t);

  fprintf(stderr, "%s, %s, %s, %s\n", obs.str, s_obs.str, obs_val.str, obs_t.str);

  char s_val[16];
  snprintf(s_val, sizeof(s_val), "%8.4f", val);

  char s_time[128];
  strftime(s_time, sizeof(s_time), "%FT%TZ", gmtime(&t));
  strlcat(s_time, XML_DATE, sizeof(s_time));
  fprintf(stderr, "Time: %s\n", s_time);
  
  char p[SS_SUBJECT_MAX_LEN];
  char o[SS_OBJECT_MAX_LEN];

  ss_add_triple(&trip, obs.str, MK_PRED(SSN, "observedBy"), MK_OBJ(NS, "DummySensor_2"),
		SS_RDF_TYPE_URI, SS_RDF_TYPE_URI);
  ss_add_triple(&trip, obs.str, MK_PRED(SSN, "observationResult"), s_obs.str,
		SS_RDF_TYPE_URI, SS_RDF_TYPE_URI);
  ss_add_triple(&trip, s_obs.str, MK_PRED(RDF, "type"), MK_OBJ(SSN, "SensorOutput"),
		SS_RDF_TYPE_URI, SS_RDF_TYPE_URI);
  ss_add_triple(&trip, s_obs.str, MK_PRED(SSN, "hasValue"), obs_val.str,
		SS_RDF_TYPE_URI, SS_RDF_TYPE_URI);
  ss_add_triple(&trip, obs_val.str, MK_PRED(RDF, "type"), MK_OBJ(SSN, "ObservationValue"),
		SS_RDF_TYPE_URI, SS_RDF_TYPE_URI);
  ss_add_triple(&trip, obs_val.str, MK_PRED(DUL, "hasRegionDataValue"), s_val,
		SS_RDF_TYPE_URI, SS_RDF_TYPE_LIT);
  ss_add_triple(&trip, obs.str, MK_PRED(SSN, "observationResultTime"), obs_t.str,
		SS_RDF_TYPE_URI, SS_RDF_TYPE_URI);
  ss_add_triple(&trip, obs_t.str, MK_PRED(RDF,"type"), MK_OBJ(DUL,"TimeInterval"),
		SS_RDF_TYPE_URI, SS_RDF_TYPE_URI);
  ss_add_triple(&trip, obs_t.str, MK_PRED(DUL, "hasRegionDataValue"), s_time,
  		SS_RDF_TYPE_URI, SS_RDF_TYPE_LIT);

  if (trip == NULL) {
    printf("Not enough memory to create a triple.\n");
    return -1;
  }

  int result = ss_insert(&h->info, trip, bnode);

  free(bnode);
  
  if (result != 0) {
    printf("Can't insert triples.\n");
    return -1;
  }

  return 0;
  
}

int kp_leave(kp *h) {
  if (h) {
    ss_leave(&h->info);
    free(h);
    fprintf(stderr, "Left SS\n");
  }

  return 0;
}

/* Combine two strings into target */
static char *cat(char *dst, char *src0, char *src1, size_t maxlen)
{
  if (dst) {
    strlcpy(dst, src0, maxlen);
    strlcat(dst, src1, maxlen);
  }
  
  return dst;
}

static void generate_uuid(uniq_t *u)
{
  uint32_t status;
  char *s;
  uuid_create(&u->id, &status);
  uuid_to_string(&u->id, &s, &status);
  strncpy(u->str, s, 37);
  free(s);

  return;
}

