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

#include <fstream>
#include <iomanip>
#include "getoptpp/getopt_pp.h"
#include "cryptanalysis.h"
#include "aux-inl.h"
#include "dataset.h"

const std::string VERSION = "cryptalize 0.9";

void usage();

int main(int argc, char **argv) {
    if (argc == 1) {
        usage();
        return 0;
    }

    // vars for storing options
    size_t input_bitsize;
    size_t output_bitsize;
    bool in_dec = false;
    bool print_table = false;
    std::string ifname;

    GetOpt::GetOpt_pp opts(argc, argv);
    opts.exceptions(std::ios::failbit | std::ios::eofbit);

    try {
        if (opts >> GetOpt::OptionPresent('h', "help")) {
            usage();
            return 0;
        } else if (opts >> GetOpt::OptionPresent('v', "version")) {
            std::cout << VERSION << std::endl;
            return 0;
        }
        if (opts >> GetOpt::OptionPresent('c', "dec")) {
            in_dec = true;
        }
        if (opts >> GetOpt::OptionPresent('t', "print-table")) {
            print_table = true;
        }

        opts >> GetOpt::Option('f', "input-file", ifname, "");
        opts >> GetOpt::Option('i', "input-bits", input_bitsize);
        opts >> GetOpt::Option('o', "output-bits", output_bitsize, (size_t)0);

        DataSet set(input_bitsize, output_bitsize, in_dec, print_table);

        if (ifname != "") {
            std::ifstream infile(ifname.c_str());
            set.ReadData(infile);
        } else {
            set.ReadData(std::cin);
        }

        if (opts >> GetOpt::OptionPresent('d', "diff-table")) {
            std::map<uint, uint> diff_table = CalcDiffTable(set);
            diff_table.erase(set.num_inputs_);

            std::cout << std::endl << "Differentials:";
            std::cout << std::endl << std::endl;
            std::map<uint, uint>::iterator iter;
            for (iter = diff_table.begin(); iter != diff_table.end(); ++iter) {
                std::cout << std::left << "# "
                    << std::setw(6) << std::left
                    << (*iter).first << ": " << (*iter).second << std::endl;
            }
        }

        if (opts >> GetOpt::OptionPresent('l', "linear-table")) {
            std::map<uint, uint> lat = CalcLat(set);
            lat.erase(set.num_inputs_/2);

            std::cout << std::endl << "Approximations:";
            std::cout << std::endl << std::endl;
            std::map<uint, uint>::iterator iter;
            for (iter = lat.begin(); iter != lat.end(); ++iter) {
                std::cout << std::left << "# "
                    << std::setw(6) << std::left << (*iter).first
                    << ": " << (*iter).second << std::endl;
            }
        }

        if (set.print_table_) {
            set.PrintTable();
        }
    }
    catch(const GetOpt::ParsingErrorEx& ex) {
        std::cerr << "Parsing Error" << std::endl;
        return 1;
    }
    catch(const GetOpt::InvalidFormatEx& ex) {
        std::cerr << "Invalid Format" << std::endl;
        return 1;
    }
    catch(const GetOpt::ArgumentNotFoundEx& ex) {
        std::cerr << "Argument Not Found" << std::endl;
        return 1;
    }
    catch(const GetOpt::TooManyOptionsEx& ex) {
        std::cerr << "Too Many Options" << std::endl;
        return 1;
    }
    catch(const GetOpt::TooManyArgumentsEx& ex) {
        std::cerr << "Too Many Arguments" << std::endl;
        return 1;
    }
    catch(const GetOpt::OptionNotFoundEx& ex) {
        std::cerr << "Option Not Found" << std::endl;
        return 1;
    }
    catch(...) {
        std::cerr << "Some other unhandled exception" << std::endl;
        return 1;
    }


    return 0;
}


/*!
 * \brief Display Cryptan usage information.
 *
 * Shown on call without arguments. Contains description of all possible
 * options and their description as well as basic principles of program usage.
 */

void usage() {
    std::cout << VERSION << std::endl << std::endl;
    std::cout << "Description here..." << std::endl << std::endl;
    std::cout << "OPTIONS:" << std::endl
        << "-h, --help\t\t\tDisplay help information." << std::endl
        << "    --version\t\t\tDisplay version of Cryptan you are using."
        << std::endl
        << "-i, --input-bits <int>\t\tThe size of input data block in bits"
        "\n\t\t\t\t(For instance: DES SBoxes have 6 bit input block size."
        << std::endl
        << "    --output-bits <int>\t\tThe size of output data block in bits"
        "\n\t\t\t\t(For instance: DES SBoxes have 4 bit output block size."
        << std::endl
        << "-f, --input-file <string>\tData file with inputs and outputs of"
        "permutation."
        << std::endl
        << "-o, --output-file <string>\tFile for saving the results of"
        "computing (currently not used)."
        << std::endl
        << "-d, --diff-table\t\tCompute Difference Distribution Table."
        << std::endl
        << "-l, --linear-table\t\tCompute Linear Approximation Table."
        << std::endl
        << "-t, --print-table\t\tPrint computed table to stdout."
        << std::endl
        << "-c, --dec\t\t\tRead data in decimal format."
        << std::endl << std::endl;
}

/*! \mainpage Cryptan Documentation
 *
 *
 * \section intro_sec Introduction
 *
 * Cryptan is designed for simple evaluation of differential and linear
 * characteristics of permutations. The program only needs all inputs and
 * corresponding outputs arranged in columns: \n
 *
 * 0000   1113 \n
 * 0001   1719 \n
 * ... \n
 * where the first column contains inputs and the second column contains
 * corresponding outputs. That way they don't necessarily have to be sorted in
 * any specific order.
 *
 * More functionality is to be implemented eventually.
 *
 *
 * \section install_sec Installation
 *
 * Under construction...
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */
