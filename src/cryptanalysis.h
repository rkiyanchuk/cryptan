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

#ifndef SRC_CRYPTANALYSIS_H_
#define SRC_CRYPTANALYSIS_H_

#include <cstdlib>
#include "aux-inl.h"
#include "dataset.h"

void InitLatCol(DataSet& set, int* col, uint mask_y);
void SubSumLatCol(int* col, size_t len, int i1, int j1, int i2, int j2);
void CalcLatCol(int* col, size_t len, int i, int j);
std::map<uint, uint> CalcLat(DataSet& set);
void CalcDiffRow(DataSet& set, int* row, uint dx);
std::map<uint, uint> CalcDiffTable(DataSet& set);

#endif  // SRC_CRYPTANALYSIS_H_
