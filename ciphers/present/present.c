/******************************************************************************
  Written and Copyright (C) by Dirk Klose
  and the EmSec Embedded Security group of Ruhr-Universitaet Bochum. 
  All rights reserved.

  Contact lightweight@crypto.rub.de for comments & questions.
  This program is free software; You may use it or parts of it or
  modifiy it under the following terms:

  (1) Usage and/or redistribution and/or modification of the software 
  or parts of the software is permitted for non-commercial use only.

  (2a) If this software or parts are used as part of a new software, you
  must license the entire work, as a whole, under this License to anyone
  who comes into possession of a copy. This License will therefore
  apply, to the whole of the work, and all its parts, regardless of how
  they are packaged.

  (2b) You may expand this license by your own license. In this case this
  license still applies to the software as mentioned in (2a) and must
  not be changed. The expansion must be clearly recognizable as such. In
  any case of collision between the license and the expansion the
  license is superior to the expansion.

  (3) If this software or parts are used as part of a new software, you
  must provide equivalent access to the source code of the entire work,
  as a whole, to anyone who comes into possession of a copy, in the same
  way through the same place at no further charge, as for the binary
  version.

  (4) This program is distributed in the hope that it will be useful,
  but   WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  (5) These notices must be retained in any copies of any part of this
  documentation and/or software.

  (6) If this software is used credit must be given to the
  "Embedded Security Group of Ruhr-Universitaet Bochum, Germany" as
  the authors of the parts of the software used. This can be in the form
  of a textual message at program startup or  at *beginning* of the
  documentation (online or textual) provided with the package.

  If you are interested in a commercial use 
  please contact '''lightweigth@crypto.rub.de'''
 ******************************************************************************/

/*
 * The code is rewritten keeping only the algorithm. Got rid of Visual Studio
 * dependency so now the code is completely in ANSI C. Only encryption for x86
 * 64 bit machines is implemented so far (32 bit machines need to be tested) and
 * safety check needs to be done.
 *
 * Enciphering is now split into separate functions for ease of understanding
 * and customizing. The selftest() function implemented as well to test the code
 * on compliance with provided test vectors.
 *
 * Zoresvit <zoresvit@gmail.com> 9.05.2011
 */

#include "present.h"

void KeySchedule(uint64_t* prevkey, uint64_t* subkeys) {
    int round;
    for (round = 0; round < 32; ++round) {
        uint64_t temp = prevkey[0];
        subkeys[round] = prevkey[0];  /* 61-bit left shift */
        prevkey[0] <<= 61;
        prevkey[0] |= (prevkey[1] << 45);
        prevkey[0] |= (temp >> 19);
        prevkey[1] = (temp >> 3) & 0xFFFF;

        temp = prevkey[0] >> 60;  /* S-Box application */
        prevkey[0] &= 0x0FFFFFFFFFFFFFFF;
        temp = sBox4[temp];
        prevkey[0] |= temp;

        prevkey[1] ^= (((round + 1) & 0x1) << 15);  /*round counter addition */
        prevkey[0] ^= ((round + 1) >> 1);
    }
}


uint64_t encipher(uint64_t state, uint64_t* subkeys) {
    int round;

    for(round = 0; round < 31; ++round)
    {
        state ^= subkeys[round];
        PLayer(&state);
    }
    state ^= subkeys[31];
    return state;
}

