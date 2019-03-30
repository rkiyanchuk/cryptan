/* 
 * Copyright 2011 Zoresvit (c) <zoresvit@gmail.com>
 * 
 * 
 */

#include <iostream>
#include <set>
#include <iomanip>

#include "snow_3g.h"

using namespace std;

int main(int argc, char** argv) {


    extern u32 FSM_R1;
    extern u32 FSM_R2;
    extern u32 FSM_R3;
    extern u32 LFSR_S15;
    extern u32 LFSR_S5;

    FSM_R1 = 0;
    FSM_R2 = 0;
    FSM_R3 = 0;
    LFSR_S15 = 0;
    LFSR_S5 = 0;

    /*
       u32 k[4] = {0x2BD6459F, 0x82C5B300, 0x952C4910, 0x4881FF48};
       u32 iv[4] = {0xEA024714, 0xAD5C4D84, 0xDF1F9B25, 0x1C0BF45F};

       size_t len = 25000000;
       u32* plain = new u32 [len];
       for (unsigned int i = 0; i < len; i++) {
       plain[i] = 0;
       }

    Initialize(k, iv);
    GenerateKeystream(len, plain);

    for (unsigned int i = 0; i < len; i++) {
    cout << std::hex << plain[i] << endl;
    }
    */

    /*
       size_t card = 0;
       for (unsigned int i = 0; i < 0xFFFFFFFF - 30030; i += 30030) {
       for (unsigned int j = 0; j < 30030; ++j) {
       f = ClockFSM();
       }
       card = gamma.size();
       gamma.insert(f);

       cout << std::hex << i << '\t' << f << endl;
       if (card == gamma.size()) {
       cout << "set:\t" << setfill('0') << setw(8) << gamma.size() << endl;
       cout << "period:\t" << setfill('0') << setw(8) << card << endl << endl;
       break;
       }
       cout << "card: " << std::dec << card << endl;
       }
       */
    return 0;
}

