/**
 * Copyright (C) 2012 51Degrees.mobi Limited
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef DEGREESMOBI_H
#define DEGREESMOBI_H
#include <stdlib.h>

/*Compiler specific settings*/
#ifdef _WIN32
#pragma warning (disable: 4820)
#include <sys\types.h>
#endif

/*Must be defined before including pcreposix.h*/
#define PCRE_STATIC STATIC

/*Include the PCRE regex library.*/
#include "pcre/pcreposix.h"

/*The maxium size of the input buffer in workset and the vectors*/
#define MAXBUFFER 1024

/*The maximum number of devices contained in the data set*/
#define MAXDEVICES 101187

/*The maxium number of segments that will be used with a handler*/
#define MAXSEGMENTS 25

/*The maximum number of regex matches per segment*/
#define MAXSEGMENTCHECK 34

/*The number of profiles for each device*/
#define PROFILES 4

/*The character used to seperate profile ids in the device Id*/
#define PROFILE_SEPERATOR '-'

/*The character used to seperate profile ids in the device Id*/
#define VALUE_SEPERATOR '|'


/*Structure for containing segments of a string*/
typedef struct segment_t {
	const char * const start;
	int length;
} Segment;

/*Structure for property values*/
typedef struct value_t {
	const char * const valueString;
	const struct value_t *next; /*Next node in list*/
} Value;

/*Structure for device properties*/
typedef struct property_t {
	const char * const id;
	const int hasProperty;
	const Value *values; /*Linked List*/
	const struct property_t *next; /*Next node in list*/
} Property;

/*Structure for device profiles*/
typedef struct profile_t {
	const char * const id;
	const Property *properties; /*Linked list*/
} Profile;

/*Structure for devices*/
typedef struct device_t {
	const char * const userAgent;
	const int length;
	const Profile * const profiles[PROFILES]; /*Array of profiles*/
} Device;

/*Structure for holding references to devices in a list*/
typedef struct workset_t {
	char input[MAXBUFFER + 1];
	char *userAgent;
	const Device *devices[MAXDEVICES];
	regmatch_t regMatch[MAXSEGMENTCHECK][MAXSEGMENTS];
	int length;
	int count;
	int score;
	int lowScore;
	int *vector1;
	int *vector2;
} Workset;

/*Structure for holding string segment data*/
typedef struct segmentdata_t {
	const Segment * const * segments;
	const Device * const devices;
} Segmentdata;

/*Prototypes*/

#ifdef __cplusplus
#define EXTERNAL extern "C"
#else
#define EXTERNAL
#endif

EXTERNAL int init(char *properties);
EXTERNAL void destroy(void);
EXTERNAL Workset *createWorkset(void);
EXTERNAL void freeWorkset(Workset *ws);
EXTERNAL int processDeviceCSV(const Device *subject, char* result, int resultLength);
EXTERNAL int processDeviceXML(const Device *subject, char* result, int resultLength);
EXTERNAL int processDeviceJSON(const Device *subject, char* result, int resultLength);
EXTERNAL const Device *getDevice(Workset *ws);

#endif


