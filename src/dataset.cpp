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

#include <iomanip>
#include "dataset.h"

/*!
 * @param input_size   number of permutation input bits.
 * @param output_size  number of permutation output bits (not required and
 * default is set to equal input_size).
 *
 * input_size is required and must be greater or equal 2.
 * output_size is optional. If output_size isn't specified, it is set to equal
 * input_size.
 */
DataSet::DataSet(size_t input_size, size_t output_size, bool in_dec, bool print_table) {
    if (input_size > kMAX_CIPHER_BIT_SIZE ||
            output_size > kMAX_CIPHER_BIT_SIZE) {
        throw std::out_of_range("Bit size of the cipher is too big.");
    }

    // all possible inputs equal 2^input_size
    num_inputs_ = pow(2, input_size);
    if (output_size == 0)  // assume outputs number = inputs number
        num_outputs_ = num_inputs_;
    else
        num_outputs_ = pow(2, output_size);

    in_dec_ = in_dec;
    plaintext_space_ = new uint[num_inputs_];
    ciphertext_space_ = new uint[num_inputs_];
    memset(plaintext_space_, 0, sizeof(uint) * num_inputs_);
    memset(ciphertext_space_, 0, sizeof(uint) * num_inputs_);

    print_table_ = print_table;
    if (print_table_) {
        if (input_size > 14 || output_size > 14) {
            print_table_ = false;
            std::cout << "WARNING: Table size is too big for printing."
                << "Countinue without storing table" << std::endl;
        } else {
            table_ = new int*[num_inputs_];
            for (uint i = 0; i < num_inputs_; ++i) {
                table_[i] = new int[num_outputs_];
                memset(table_[i], 0, sizeof(int) * num_outputs_);
            }
        }
    }
}

DataSet::~DataSet() {
    delete[] plaintext_space_;
    delete[] ciphertext_space_;
}

/*!
 * Read plaintexts and ciphertexts from input stream.
 *
 * Get formated input data. If hex flag is true, data is expected to be in hex.
 * Otherwise decimal format is expected.
 */
void DataSet::ReadData(std::istream& input) {
    if (!input.good()) {
        // TODO(zoresvit): implement right exception here
        throw std::out_of_range("Bad input file.");
    }

    size_t index = 0;
    std::string temp;
    do {
        if (DataSet::in_dec_) {
            input >> plaintext_space_[index];
            input >> ciphertext_space_[index];
        } else {
            input >> std::hex >> plaintext_space_[index];
            input >> std::hex >> ciphertext_space_[index];
        }
        ++index;
    } while (input.good() && index < num_inputs_);

    if ((index < num_inputs_)) {
        std::cout << "WARNING:\tNot all plaintext-ciphertext sets are provided!"
            << std::endl
            << index << " sets have been read out of "
            << num_inputs_ << " needed." << std::endl;
    }
}

void DataSet::PrintTable() {
    std::cout << std::endl;
    // header
    std::cout.setf(std::ios::right);
    std::cout << "       ";
    for (uint i = 0; i < num_outputs_; ++i) {
        std::cout << std::hex << std::setw(6) << i;
    }
    std::cout << std::endl << "       ";
    for (uint i = 0; i < num_outputs_; ++i) {
        std::cout << std::setfill('-') << std::setw(6) << '-';
    }
    std::cout << std::endl << std::setfill(' ');

    // main table
    for (uint i = 0; i < num_inputs_; ++i, std::cout << std::endl) {
        std::cout << std::hex << std::setw(6) << i << '|';
        for (uint j = 0; j < num_outputs_; ++j) {
            std::cout << std::dec << std::setw(6) << table_[i][j];
        }
    }
}
