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

#ifndef SRC_DATASET_H_
#define SRC_DATASET_H_

#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <stdexcept>
#include "aux-inl.h"

const size_t kMAX_CIPHER_BIT_SIZE = 24;  // motivated by the required RAM size

/*!
 * Input data processing class.
 *
 * DataSet class is responsible for reading input data (plaintexts and 
 * ciphertexts) and process it regardless the cryptanalysis method or 
 * permutation type (i.e. read hex or dec format, check for correct and 
 * complete data).
 */
class DataSet {
 public:

    DataSet(size_t input_size, size_t output_size, bool in_dec, bool print_table);
    ~DataSet();

    /*! if true, assume the input data is in hex */
    bool in_dec_;
    /*! if true, print the results table */
    bool print_table_;
    int** table_;
    /*! number of possible permutation inputs */
    size_t num_inputs_;
    /*! number of possible permutation outputs */
    size_t num_outputs_;
    /*! array of all plain texts (inputs of permutation) */
    uint* plaintext_space_;
    /*! array of all cipher texts (outputs of permutation) */
    uint* ciphertext_space_;

    void ReadData(std::istream& input);
    void PrintTable();
};

#endif  // SRC_DATASET_H_
