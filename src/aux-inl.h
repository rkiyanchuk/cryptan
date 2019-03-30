/*
 * Copyright 2012 Zoresvit (c) <zoresvit@gmail.com>
 *
 * This file is part of Cryptalize.
 *
 * Cryptalize is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cryptalize is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cryptalize.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_AUX_INL_H_
#define SRC_AUX_INL_H_

#include <iostream>

typedef unsigned long int ulong;
typedef unsigned int uint;

/*!
 * Applies mask for equation.
 * Apply mask to the value via '&' and find parity of the result. The parity 
 * is computed by xoring half-words.
 * @param value input number
 * @param mask  mask to apply
 * @return true or false of xoring the result bits after applying the mask
 */
inline uint XorMask(uint value, uint mask) {
    uint xorbit = value & mask;
    uint i = sizeof(uint) * 8;
    while (i != 0) {
        xorbit = (xorbit & (((uint)1 << i) - 1)) ^ (xorbit >> i);
        i >>= 1;
    }
    return xorbit;
}

inline void tp(int* v0, int* v1)
{
    if(*v0 == 0 && *v1 == 0)
    {
        *v0 = 1; *v1 = 0;
        return;
    }
    if(*v0 == 0 && *v1 == 1)
    {
        return;
    }
    if(*v0 == 1 && *v1 == 0)
    {
        *v0 = 0; *v1 = -1;
        return;
    }
    if(*v0 == 1 && *v1 == 1)
    {
        *v0 = -1; *v1 = 0;
        return;
    }
    std::cerr << "ERROR in tp()" << std::endl;
}

#endif  // SRC_AUX_INL_H_
