/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/


#ifndef _COMPHELPER_CLASSIDS_HXX
#define _COMPHELPER_CLASSIDS_HXX

/*
 * StarWriter
 */

/* 3.0 */

#define SO3_SW_CLASSID_30 \
	0xDC5C7E40L, 0xB35C, 0x101B, 0x99, 0x61, 0x04, \
	0x02, 0x1C, 0x00, 0x70, 0x02

// #110743#
#define BF_SO3_SW_CLASSID_30 \
	0xDC5C7E40L, 0xB35C, 0x101B, 0x99, 0x61, 0x04, \
	0x02, 0x1C, 0x00, 0x70, 0x02
/* 4.0 */

#define SO3_SW_CLASSID_40 \
	0x8b04e9b0,  0x420e, 0x11d0, 0xa4, 0x5e, 0x0, \
	0xa0, 0x24, 0x9d, 0x57, 0xb1

// #110743#
#define BF_SO3_SW_CLASSID_40 \
	0x8b04e9b0,  0x420e, 0x11d0, 0xa4, 0x5e, 0x0, \
	0xa0, 0x24, 0x9d, 0x57, 0xb1
/* 5.0 */

#define SO3_SW_CLASSID_50 \
	0xc20cf9d1, 0x85ae, 0x11d1, 0xaa, 0xb4, 0x0, \
	0x60, 0x97, 0xda, 0x56, 0x1a

// #110743#
#define BF_SO3_SW_CLASSID_50 \
	0xc20cf9d1, 0x85ae, 0x11d1, 0xaa, 0xb4, 0x0, \
	0x60, 0x97, 0xda, 0x56, 0x1a

 /* 6.0, 7, 8 */

#define SO3_SW_CLASSID_60 \
    0x8BC6B165, 0xB1B2, 0x4EDD, 0xAA, 0x47, 0xDA, \
    0xE2, 0xEE, 0x68, 0x9D, 0xD6

// #110743#
#define BF_SO3_SW_CLASSID_60 \
    0x8BC6B165, 0xB1B2, 0x4EDD, 0xAA, 0x47, 0xDA, \
    0xE2, 0xEE, 0x68, 0x9D, 0xD6

 /* ole embed 6.0, 7 */

#define SO3_SW_OLE_EMBED_CLASSID_60 \
	0x30a2652a, 0xddf7, 0x45e7, 0xac, 0xa6, 0x3e, \
	0xab, 0x26, 0xfc, 0x8a, 0x4e

 /* ole embed 8 */

#define SO3_SW_OLE_EMBED_CLASSID_8 \
	0xf616b81f, 0x7bb8, 0x4f22, 0xb8, 0xa5, 0x47, \
	0x42, 0x8d, 0x59, 0xf8, 0xad

/* aktuell */

#define SO3_SW_CLASSID SO3_SW_CLASSID_60

// #110743#
#define BF_SO3_SW_CLASSID BF_SO3_SW_CLASSID_50

/*
 * StarWriter/Web
 */

/* 4.0 */

#define SO3_SWWEB_CLASSID_40 \
	0xf0caa840, 0x7821, 0x11d0, 0xa4, 0xa7, 0x0, \
	0xa0, 0x24, 0x9d, 0x57, 0xb1

// #110743#
#define BF_SO3_SWWEB_CLASSID_40 \
	0xf0caa840, 0x7821, 0x11d0, 0xa4, 0xa7, 0x0, \
	0xa0, 0x24, 0x9d, 0x57, 0xb1
/* 5.0 */

#define SO3_SWWEB_CLASSID_50 \
	0xc20cf9d2, 0x85ae, 0x11d1, 0xaa, 0xb4, 0x0, \
	0x60, 0x97, 0xda, 0x56, 0x1a

// #110743#
#define BF_SO3_SWWEB_CLASSID_50 \
	0xc20cf9d2, 0x85ae, 0x11d1, 0xaa, 0xb4, 0x0, \
	0x60, 0x97, 0xda, 0x56, 0x1a
/* 6.0, 7, 8 */

#define SO3_SWWEB_CLASSID_60 \
    0xA8BBA60C, 0x7C60, 0x4550, 0x91, 0xCE, 0x39, \
    0xC3, 0x90, 0x3F, 0xAC, 0x5E

// #110743#
#define BF_SO3_SWWEB_CLASSID_60 \
    0xA8BBA60C, 0x7C60, 0x4550, 0x91, 0xCE, 0x39, \
    0xC3, 0x90, 0x3F, 0xAC, 0x5E

/* aktuell */

#define SO3_SWWEB_CLASSID SO3_SWWEB_CLASSID_60

// #110743#
#define BF_SO3_SWWEB_CLASSID BF_SO3_SWWEB_CLASSID_50

/*
 * Globaldokument
 */

/* 4.0 */

#define SO3_SWGLOB_CLASSID_40 \
	0x340ac970, 0xe30d, 0x11d0, 0xa5, 0x3f, 0x0, \
	0xa0, 0x24, 0x9d, 0x57, 0xb1

// #110743#
#define BF_SO3_SWGLOB_CLASSID_40 \
	0x340ac970, 0xe30d, 0x11d0, 0xa5, 0x3f, 0x0, \
	0xa0, 0x24, 0x9d, 0x57, 0xb1

/* 5.0 */

#define SO3_SWGLOB_CLASSID_50 \
	0xc20cf9d3, 0x85ae, 0x11d1, 0xaa, 0xb4, 0x0, \
	0x60, 0x97, 0xda, 0x56, 0x1a

// #110743#
#define BF_SO3_SWGLOB_CLASSID_50 \
	0xc20cf9d3, 0x85ae, 0x11d1, 0xaa, 0xb4, 0x0, \
	0x60, 0x97, 0xda, 0x56, 0x1a
/* 6.0, 7, 8 */

#define SO3_SWGLOB_CLASSID_60 \
    0xB21A0A7C, 0xE403, 0x41FE, 0x95, 0x62, 0xBD, \
    0x13, 0xEA, 0x6F, 0x15, 0xA0

// #110743#
#define BF_SO3_SWGLOB_CLASSID_60 \
    0xB21A0A7C, 0xE403, 0x41FE, 0x95, 0x62, 0xBD, \
    0x13, 0xEA, 0x6F, 0x15, 0xA0
/* aktuell */

#define SO3_SWGLOB_CLASSID SO3_SWGLOB_CLASSID_60

// #110743#
#define BF_SO3_SWGLOB_CLASSID BF_SO3_SWGLOB_CLASSID_50

//---------------------------------------------------

/*
 * StarCalc
 */

/* 3.0 */

#define SO3_SC_CLASSID_30 \
	0x3F543FA0L, 0xB6A6, 0x101B, 0x99, 0x61, 0x04, \
	0x02, 0x1C, 0x00, 0x70, 0x02

// #110743#
#define BF_SO3_SC_CLASSID_30 \
	0x3F543FA0L, 0xB6A6, 0x101B, 0x99, 0x61, 0x04, \
	0x02, 0x1C, 0x00, 0x70, 0x02
/* 4.0 */

#define SO3_SC_CLASSID_40 \
	0x6361d441L, 0x4235, 0x11d0, 0x89, 0xcb, 0x00, \
	0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SC_CLASSID_40 \
	0x6361d441L, 0x4235, 0x11d0, 0x89, 0xcb, 0x00, \
	0x80, 0x29, 0xe4, 0xb0, 0xb1
/* 5.0 */

#define SO3_SC_CLASSID_50 \
	0xc6a5b861L, 0x85d6, 0x11d1, 0x89, 0xcb, 0x00, \
	0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SC_CLASSID_50 \
	0xc6a5b861L, 0x85d6, 0x11d1, 0x89, 0xcb, 0x00, \
	0x80, 0x29, 0xe4, 0xb0, 0xb1
/* 6.0 */

#define SO3_SC_CLASSID_60 \
    0x47BBB4CB, 0xCE4C, 0x4E80, 0xA5, 0x91, 0x42, \
    0xD9, 0xAE, 0x74, 0x95, 0x0F

// #110743#
#define BF_SO3_SC_CLASSID_60 \
    0x47BBB4CB, 0xCE4C, 0x4E80, 0xA5, 0x91, 0x42, \
    0xD9, 0xAE, 0x74, 0x95, 0x0F

/* ole embed 6.0, 7 */

#define SO3_SC_OLE_EMBED_CLASSID_60 \
	0x7b342dc4, 0x139a, 0x4a46, 0x8a, 0x93, 0xdb, \
	0x8, 0x27, 0xcc, 0xee, 0x9c

/* ole embed 8 */

#define SO3_SC_OLE_EMBED_CLASSID_8 \
	0x7fa8ae11, 0xb3e3, 0x4d88, 0xaa, 0xbf, 0x25, \
	0x55, 0x26, 0xcd, 0x1c, 0xe8

/* aktuell */

#define SO3_SC_CLASSID SO3_SC_CLASSID_60

// #110743#
#define BF_SO3_SC_CLASSID BF_SO3_SC_CLASSID_50

/****************************************************
* StarImpress
****************************************************/

/* 3.0 */

#define SO3_SIMPRESS_CLASSID_30 \
    0xAF10AAE0L, 0xB36D, 0x101B, 0x99, 0x61, 0x04, \
    0x02, 0x1C, 0x00, 0x70, 0x02

// #110743#
#define BF_SO3_SIMPRESS_CLASSID_30 \
    0xAF10AAE0L, 0xB36D, 0x101B, 0x99, 0x61, 0x04, \
    0x02, 0x1C, 0x00, 0x70, 0x02
/* 4.0 */

#define SO3_SIMPRESS_CLASSID_40 \
    0x12d3cc0L, 0x4216, 0x11d0, 0x89, 0xcb, 0x0, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SIMPRESS_CLASSID_40 \
    0x12d3cc0L, 0x4216, 0x11d0, 0x89, 0xcb, 0x0, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1
/* 5.0 */

#define SO3_SIMPRESS_CLASSID_50 \
    0x565c7221L, 0x85bc, 0x11d1, 0x89, 0xd0, 0x0, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SIMPRESS_CLASSID_50 \
    0x565c7221L, 0x85bc, 0x11d1, 0x89, 0xd0, 0x0, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1
/* 6.0 */

#define SO3_SIMPRESS_CLASSID_60 \
    0x9176E48A, 0x637A, 0x4D1F, 0x80, 0x3B, 0x99, \
    0xD9, 0xBF, 0xAC, 0x10, 0x47

// #110743#
#define BF_SO3_SIMPRESS_CLASSID_60 \
    0x9176E48A, 0x637A, 0x4D1F, 0x80, 0x3B, 0x99, \
    0xD9, 0xBF, 0xAC, 0x10, 0x47

/* ole embed 6.0, 7 */

#define SO3_SIMPRESS_OLE_EMBED_CLASSID_60 \
	0xe5a0b632, 0xdfba, 0x4549, 0x93, 0x46, 0xe4, \
	0x14, 0xda, 0x6, 0xe6, 0xf8

/* ole embed 8 */

#define SO3_SIMPRESS_OLE_EMBED_CLASSID_8 \
	0xee5d1ea4, 0xd445, 0x4289, 0xb2, 0xfc, 0x55, \
	0xfc, 0x93, 0x69, 0x39, 0x17

/* aktuell */

#define SO3_SIMPRESS_CLASSID  SO3_SIMPRESS_CLASSID_60

// #110743#
#define BF_SO3_SIMPRESS_CLASSID  BF_SO3_SIMPRESS_CLASSID_50

/****************************************************
* StarDraw
****************************************************/

/* 5.0 */

#define SO3_SDRAW_CLASSID_50 \
    0x2e8905a0L, 0x85bd, 0x11d1, 0x89, 0xd0, 0x0, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SDRAW_CLASSID_50 \
    0x2e8905a0L, 0x85bd, 0x11d1, 0x89, 0xd0, 0x0, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1
/* 6.0 */
#define SO3_SDRAW_CLASSID_60 \
    0x4BAB8970, 0x8A3B, 0x45B3, 0x99, 0x1C, 0xCB, \
    0xEE, 0xAC, 0x6B, 0xD5, 0xE3

// #110743#
#define BF_SO3_SDRAW_CLASSID_60 \
    0x4BAB8970, 0x8A3B, 0x45B3, 0x99, 0x1C, 0xCB, \
    0xEE, 0xAC, 0x6B, 0xD5, 0xE3

/* ole embed 6.0, 7 */

#define SO3_SDRAW_OLE_EMBED_CLASSID_60 \
	0x41662fc2, 0xd57, 0x4aff, 0xab, 0x27, 0xad, \
	0x2e, 0x12, 0xe7, 0xc2, 0x73

/* ole embed 8 */

#define SO3_SDRAW_OLE_EMBED_CLASSID_8 \
	0x448bb771, 0xcfe2, 0x47c4, 0xbc, 0xdf, 0x1f, \
	0xbf, 0x37, 0x8e, 0x20, 0x2c

/* aktuell */

#define SO3_SDRAW_CLASSID  SO3_SDRAW_CLASSID_60

// #110743#
#define BF_SO3_SDRAW_CLASSID  BF_SO3_SDRAW_CLASSID_50

/****************************************************
* StarChart
****************************************************/

/* 3.0 */

#define SO3_SCH_CLASSID_30 \
    0xFB9C99E0L, 0x2C6D, 0x101C, 0x8E, 0x2C, 0x00, \
    0x00, 0x1B, 0x4C, 0xC7, 0x11

// #110743#
#define BF_SO3_SCH_CLASSID_30 \
    0xFB9C99E0L, 0x2C6D, 0x101C, 0x8E, 0x2C, 0x00, \
    0x00, 0x1B, 0x4C, 0xC7, 0x11
/* 4.0 */

#define SO3_SCH_CLASSID_40 \
    0x2b3b7e0L, 0x4225, 0x11d0, 0x89, 0xca, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SCH_CLASSID_40 \
    0x2b3b7e0L, 0x4225, 0x11d0, 0x89, 0xca, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1
/* 5.0 */

#define SO3_SCH_CLASSID_50 \
    0xbf884321L, 0x85dd, 0x11d1, 0x89, 0xd0, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SCH_CLASSID_50 \
    0xbf884321L, 0x85dd, 0x11d1, 0x89, 0xd0, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1
/* 6.0 */

#define SO3_SCH_CLASSID_60 \
    0x12DCAE26, 0x281F, 0x416F, 0xA2, 0x34, 0xC3, \
    0x08, 0x61, 0x27, 0x38, 0x2E

// #110743#
#define BF_SO3_SCH_CLASSID_60 \
    0x12DCAE26, 0x281F, 0x416F, 0xA2, 0x34, 0xC3, \
    0x08, 0x61, 0x27, 0x38, 0x2E

/* ole embed 6.0, 7 */

#define SO3_SCH_OLE_EMBED_CLASSID_60 \
	0xd415cd93, 0x35c4, 0x4c6f, 0x81, 0x9d, 0xa6, \
	0x64, 0xa1, 0xc8, 0x13, 0xae

/* ole embed 8 */

#define SO3_SCH_OLE_EMBED_CLASSID_8 \
	0xdd0a57f, 0xcf3b, 0x4fd2, 0xbd, 0xa4, 0x94, \
	0x42, 0x71, 0x9b, 0x2a, 0x73

/* aktuell */

#define SO3_SCH_CLASSID  SO3_SCH_CLASSID_60

// #110743#
#define BF_SO3_SCH_CLASSID  BF_SO3_SCH_CLASSID_50

/****************************************************
* StarImage
****************************************************/

/* 3.0 */

#define SO3_SIM_CLASSID_30 \
    0xEA60C941L, 0x2C6C, 0x101C, 0x8E, 0x2C, 0x00, \
    0x00, 0x1B, 0x4C, 0xC7, 0x11

// #110743#
#define BF_SO3_SIM_CLASSID_30 \
    0xEA60C941L, 0x2C6C, 0x101C, 0x8E, 0x2C, 0x00, \
    0x00, 0x1B, 0x4C, 0xC7, 0x11
/* 4.0 */

#define SO3_SIM_CLASSID_40 \
    0x447BB8A0L, 0x41FB, 0x11D0, 0x89, 0xCA, 0x00, \
    0x80, 0x29, 0xE4, 0xB0, 0xB1

// #110743#
#define BF_SO3_SIM_CLASSID_40 \
    0x447BB8A0L, 0x41FB, 0x11D0, 0x89, 0xCA, 0x00, \
    0x80, 0x29, 0xE4, 0xB0, 0xB1
/* 5.0 */

#define SO3_SIM_CLASSID_50 \
    0x65c68d00L, 0x85de, 0x11d1, 0x89, 0xd0, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SIM_CLASSID_50 \
    0x65c68d00L, 0x85de, 0x11d1, 0x89, 0xd0, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1

/* aktuell */

#define SO3_SIM_CLASSID  SO3_SIM_CLASSID_50

// #110743#
#define BF_SO3_SIM_CLASSID  BF_SO3_SIM_CLASSID_50

/***************************************************
* StarMath
***************************************************/

/* 3.0 */

#define SO3_SM_CLASSID_30 \
    0xD4590460L, 0x35FD, 0x101C, 0xB1, 0x2A, 0x04, \
    0x02, 0x1C, 0x00, 0x70, 0x02

// #110743#
#define BF_SO3_SM_CLASSID_30 \
    0xD4590460L, 0x35FD, 0x101C, 0xB1, 0x2A, 0x04, \
    0x02, 0x1C, 0x00, 0x70, 0x02
/* 4.0 */

#define SO3_SM_CLASSID_40 \
    0x2b3b7e1L, 0x4225, 0x11d0, 0x89, 0xca, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SM_CLASSID_40 \
    0x2b3b7e1L, 0x4225, 0x11d0, 0x89, 0xca, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1
/* 5.0 */

#define SO3_SM_CLASSID_50 \
    0xffb5e640L, 0x85de, 0x11d1, 0x89, 0xd0, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_SM_CLASSID_50 \
    0xffb5e640L, 0x85de, 0x11d1, 0x89, 0xd0, 0x00, \
    0x80, 0x29, 0xe4, 0xb0, 0xb1
/* 6.0, 7, 8 */

#define SO3_SM_CLASSID_60 \
    0x078B7ABA, 0x54FC, 0x457F, 0x85, 0x51, 0x61, \
    0x47, 0xE7, 0x76, 0xA9, 0x97

// #110743#
#define BF_SO3_SM_CLASSID_60 \
    0x078B7ABA, 0x54FC, 0x457F, 0x85, 0x51, 0x61, \
    0x47, 0xE7, 0x76, 0xA9, 0x97

/* ole embed 6.0, 7 */

#define SO3_SM_OLE_EMBED_CLASSID_60 \
	0xd0484de6, 0xaaee, 0x468a, 0x99, 0x1f, 0x8d, \
	0x4b, 0x7, 0x37, 0xb5, 0x7a

/* ole embed 8 */

#define SO3_SM_OLE_EMBED_CLASSID_8 \
	0xd2d59cd1, 0xa6a, 0x4d36, 0xae, 0x20, 0x47, \
	0x81, 0x70, 0x77, 0xd5, 0x7c

/* aktuell */

#define SO3_SM_CLASSID  SO3_SM_CLASSID_60

// #110743#
#define BF_SO3_SM_CLASSID  BF_SO3_SM_CLASSID_50

#define SO3_OUT_CLASSID \
	0x970b1e82, 0xcf2d, 0x11cf, 0x89, 0xca, 0x00, \
	0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_OUT_CLASSID \
	0x970b1e83, 0xcf2d, 0x11cf, 0x89, 0xca, 0x00, \
	0x80, 0x29, 0xe4, 0xb0, 0xb1

#define SO3_DUMMY_CLASSID \
    0x970b1fff, 0xcf2d, 0x11cf, \
	0x89,0xca,0x00,0x80,0x29,0xe4,0xb0,0xb1

#define SO3_APPLET_CLASSID \
	0x970b1e81, 0xcf2d, 0x11cf, \
	0x89,0xca,0x00,0x80,0x29,0xe4,0xb0,0xb1

// #110743#
#define BF_SO3_APPLET_CLASSID \
	0x970b1e82, 0xcf2d, 0x11cf, \
	0x89,0xca,0x00,0x80,0x29,0xe4,0xb0,0xb1
#define SO3_PLUGIN_CLASSID \
	0x4caa7761, 0x6b8b, 0x11cf, \
	0x89,0xca,0x0,0x80,0x29,0xe4,0xb0,0xb1

// #110743#
#define BF_SO3_PLUGIN_CLASSID \
	0x4caa7762, 0x6b8b, 0x11cf, \
	0x89,0xca,0x0,0x80,0x29,0xe4,0xb0,0xb1

#define SO3_IFRAME_CLASSID \
	0x1a8a6701, 0xde58, 0x11cf, \
	0x89, 0xca, 0x0, 0x80, 0x29, 0xe4, 0xb0, 0xb1

// #110743#
#define BF_SO3_IFRAME_CLASSID \
	0x1a8a6702, 0xde58, 0x11cf, \
	0x89, 0xca, 0x0, 0x80, 0x29, 0xe4, 0xb0, 0xb2

#define SO3_GLOBAL_CLASSID \
 	0x475198a8, 0x694c, 0x4bd8, \
	0xb0, 0x2f, 0xd9, 0xb7, 0x6b, 0xcf, 0x31, 0x28

#define SO3_RPT_CLASSID_90 \
	0xd7896d52, 0xb7af, 0x4820, \
	0x9d, 0xfe, 0xd4, 0x04, 0xd0, 0x15, 0x96, 0x0f

#define SO3_RPT_SCH_CLASSID_90 \
	0x80243d39, 0x6741, 0x46c5, \
	0x92, 0x6e, 0x06, 0x91, 0x64, 0xff, 0x87, 0xbb


#endif
