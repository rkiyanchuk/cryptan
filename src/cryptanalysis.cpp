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

#include "aux-inl.h"
#include "cryptanalysis.h"

inline void InitLatCol(DataSet& set, int* col, uint mask_y) {
    uint mask_x;
    for(mask_x = 0; mask_x < set.num_inputs_; ++mask_x) {
        col[mask_x] = XorMask(set.ciphertext_space_[mask_x], mask_y);
    }

    for(mask_x = 0; mask_x < set.num_inputs_ - 1; mask_x += 2) {
        tp(&col[mask_x], &col[mask_x + 1]);
    }
}

inline void SubSumLatCol(int* col, size_t len, int i1, int j1, int i2, int j2) {
    static int temp;
    for(int i = 0; i <= (j1 - i1); ++i) {
        temp = col[i1 + i];

        col[i1 + i] = temp + col[i2 + i];
        col[i2 + i] = temp - col[i2 + i];
    }
}

inline void CalcLatCol(int* col, size_t len, int i, int j) {
    int k;
    if(j - i > 2) {
        k = (i + j) / 2;
        CalcLatCol(col, len, i, k);
        CalcLatCol(col, len, k + 1, j);
        SubSumLatCol(col, len, i, k, k + 1, j);
    }
    return;
}

std::map<uint, uint> CalcLat(DataSet& set) {
    int* col = new int [set.num_inputs_];
    std::map<uint, uint> implications;

    for(uint mask_y = 0; mask_y < set.num_outputs_; ++mask_y) {
        InitLatCol(set, col, mask_y);
        CalcLatCol(col, set.num_inputs_, 0, set.num_inputs_-1);

        if (set.print_table_) {
            for (uint i = 0; i < set.num_inputs_; ++i) {
                set.table_[i][mask_y] = col[i];
            }
        }

        for(uint i = 0; i < set.num_inputs_; ++i) {
            ++implications[abs(col[i])];
        }
    }
    delete[] col;
    return implications;
}

void CalcDiffRow(DataSet& set, int* row, uint dx) {
    uint dy;
    memset(row, 0, set.num_inputs_ * sizeof(int));

    for(uint x = 0; x < set.num_inputs_; ++x) {
        dy = set.ciphertext_space_[x] ^ set.ciphertext_space_[x ^ dx];
        ++row[dy];
    }
    if (set.print_table_) {
        memcpy(set.table_[dx], row, sizeof(int) * set.num_outputs_);
    }
}

std::map<uint, uint> CalcDiffTable(DataSet& set) {
    int* row = new int [set.num_outputs_];
    std::map<uint, uint> implications;

    for(uint dx = 0; dx < set.num_inputs_; ++dx) {
        CalcDiffRow(set, row, dx);
        for(uint i = 0; i < set.num_outputs_; ++i) {
            ++implications[abs(row[i])];
        }
    }
    delete[] row;
    return implications;
}
