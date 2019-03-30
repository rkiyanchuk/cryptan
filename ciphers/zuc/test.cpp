/* 
 * Copyright 2011 Zoresvit (c) <zoresvit@gmail.com>
 * 
 * 
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <ctime>

typedef unsigned char u8;
typedef unsigned int u32;

void Initialization(u8* k, u8* iv);
void GenerateKeystream(u32* pKeystream, int KeystreamLen);
u32 F();

using namespace std;

extern u32 BRC_X0;
extern u32 BRC_X1;
extern u32 BRC_X2;
extern u32 BRC_X3;
extern u32 F_R1;
extern u32 F_R2;

string ToBin(u32 word) {
    string bin = "";
    for (int i = 0; i < sizeof(u32) * 8; ++i) {
        if (word & (1 << i)) {
            bin += '1';
        } else {
            bin += '0';
        }
    }
    // std::reverse(bin.begin(), bin.end());
    return bin;
}

int main(int argc, char** argv) {
    srand(time(NULL));

    const int keynum = 55;
    const int kstream_len = 31250;
    u32 kstream[kstream_len] = {0};

    u8 keys_sparce[keynum][16] = {{0}};
    u8 iv[keynum] = {0};
    int val = 0x80;
    int byte = 0;
    for (int k = 0; k < keynum; ++k) {
        if (k % 8 == 0 && k != 0) {
            byte += 1;
            val = 0x80;
        }
        keys_sparce[k][byte] = val; 
        val >>= 1;
    }

    u8 keys_random[keynum][16] = {{0}};
    for (int k = 0; k < keynum; ++k) {
        for (int i = 0; i < 16; ++i) {
            keys_random[k][i] = rand();
        }
    }

    ofstream descr("zuc_stream_rand.txt", ios_base::out);
    ofstream f("zuc_stream_rand.dat", ios_base::out);

    for (int i = 0; i < keynum; ++i) {
        Initialization(keys_random[i], iv);
        GenerateKeystream(kstream, kstream_len);

        descr << "key\t" << std::dec << i << ":\t";
        for (int j = 0; j < 16; ++j) {
            descr << std::hex << setw(2) << setfill('0') << static_cast<int>(keys_random[i][j]) << ' ';
        }
        descr << endl;

        // cout << std::dec << i << '\n';
        for (int c = 0; c < kstream_len; ++c) {
            // cout << std::hex << setw(8) << setfill('0') << kstream[c] << ' ';
            f << ToBin(kstream[c]) << endl;
        }
        // cout << endl << endl << endl;
    }


    /*
    for (int i = 0; i < keynum; ++i) {
        Initialization(keys_sparce[i], iv);
        GenerateKeystream(kstream, kstream_len);
        cout << std::dec << i << '\n';
        for (int i = 0; i < kstream_len; ++i) {
            cout << std::hex << setw(8) << setfill('0') << kstream[i] << ' ';
        }
        cout << endl << endl << endl;
    }

    for (int i = 0; i < keynum; ++i) {
        cout << std::dec << i << '\t';
        for (int j = 0; j < 16; ++j) {
            cout << std::hex << setw(2) << setfill('0') << static_cast<int>(keys_sparce[i][j]) << ' ';
        }
        cout << endl;
    }
    
    for (int i = 0; i < keynum; ++i) {
        cout << std::dec << i << '\t';
        for (int j = 0; j < 16; ++j) {
            cout << std::hex << setw(2) << setfill('0') << static_cast<int>(keys_random[i][j]) << ' ';
        }
        cout << endl;
    }
    */


#if 0
    F_R1 = 0xF2A4BD85;
    F_R2 = 0;
    BRC_X0 = 0;
    BRC_X1 = 0;
    BRC_X2 = 0;
    unsigned long state = 0;
    unsigned long ticks = 0;
    unsigned long start = 0;

    vector<unsigned long> seq;  // initial sequence
    vector<unsigned long>::iterator it;

    /*
     * FSM state at tick 0x4FFFFFFFF:
     * R1: 1287757794 (0x4CC19FE2)
     * R2: 1700628884 (0x655D8994)
     *
     */

    cout << "clocking FSM..." << endl;
    for (unsigned long i = 0; i <= 0xFFFFFF; ++i) {
        F();
        ++ticks;
    }

    return 0;

    cout << "searching for period..." << endl;
    start = ticks;
    state = F_R1;

    while(1) {
        F();
        ++ticks;
        if (state == F_R1) {
            cout << "period:\t" << std::dec << ticks - start << endl;
            cout << "state:\t" << std::hex << state << endl;
            break;
        }
    }

    /*
    cout << "Generating sequence..." << endl;
    for (unsigned long i = 0; i < 100000; ++i) {
        F();
        ++ticks;
        state = F_R1; 
        seq.push_back(state);    
    }
    start = ticks;
    cout << "Sorting the sequence..." << endl;
    sort(seq.begin(), seq.end());
    cout << "Looking for equal elements..." << endl;
    it = adjacent_find(seq.begin(), seq.end());
    if (it != seq.end()) {
        cout << "Found equal elements: " << std::hex << *it << '\t' << *(it + 1) << endl;
        return 0;
    } else {
        cout << "None equal elements found in sequance :(" << endl;
    }

    cout << "=========================" << endl;
    cout << "Perform long search" << endl << endl;

    while(1) {
        for (int i = 0; i < 10000; ++i) {
            F();
            ++ticks;
        }
            state = F_R1;

        if(binary_search(seq.begin(), seq.end(), state)) {
            cout << "FOUND CYCLE at tick " << ticks - start << endl;
            cout << "state:\t" << std::hex << state << endl;
            break;
        } 
    }
    */
#endif

    return 0;
}
