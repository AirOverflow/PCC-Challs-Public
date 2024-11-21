#include <iostream>
#include <array>
#include <string>
#include <utility>
#include <cstring>

#define FLAG_SIZE 0x50

const std::array<int, FLAG_SIZE> flag = {
    0x72 , 0x53 , 0x26 , 0x38 , 0x45 , 0x33 ,
    -0x69, 0x52 , 0x74 , 0x2a , 0x71 , 0x30 ,
    -0x69, 0x9  , 0x3  , 0xb  , 0x2  , -0x69,
    0x31 , 0x90 , -0x69, 0x32 , 0x18 , 0x16 ,
    0x0B , -0x69, 0x5  , 0x12 , 0x36 , 0x0b ,
    0x24 , 0x32 , -0x69, 0x32 , 0x36 , 0x74 ,
    -0x69, 0x18 , 0x59 , 0x57 , 0x40 , -0x69,
    0x70 , 0x3  , 0xE2 , 0x83 , -0x69, 0x61 ,
    0x30 , -0x69, 0x29 , 0x5f , 0x43 , 0x1f ,
    -0x69, 0x7  , 0x2e , 0x25 , 0xf  , -0x69,
    0x48 , 0x28 , 0x28 , 0x21 , 0x2b , -0x69,
    0x52 , 0x74 , 0x2a , 0x71 , 0x30 , -0x69,
    0x61 , 0x37 , -0x69, 0x3b , 0x92 , 0x32,
    0xe6 , 0xc0
};

const int underscores[] = { 6, 12, 17, 20, 25, 32, 36, 41, 46, 49, 54, 59, 65, 71, 74 };

bool validate(std::string input) {

    // I wanted to implement these as functors
    // but the disassembly would've been too complex.
    // So, lambdas ftw.

    // UPDATE: NVM, the disassembly seems pretty similar.
    const auto plus    = [](const int a, const int b) -> int { return a + b; };
    const auto minus   = [](const int a, const int b) -> int { return a - b; };
    const auto _xor    = [](const int a, const int b) -> int { return a ^ b; };
    const auto r_shift = [](const int a, const int b) -> int { return a >> b; };
    const auto l_shift = [](const int a, const int b) -> int { return a << b; };
    const auto _and    = [](const int a, const int b) -> int { return a & b; };
    const auto _or     = [](const int a, const int b) -> int { return a | b; };
    const auto _not    = [](const int a) -> int { return ~a; };

    const auto validate_underscore = [&](int idx) -> bool {
        return input[idx] != minus(_xor(r_shift(l_shift(_xor(_not(flag[idx]), 0x15), 4), 8), 99), 5);
    };

    const auto five_str_mangler = [&](int start) -> bool {
        std::string substr;
        substr = input.substr(start, 5);
        for(int i = 0; i < 5; i++) {
            if(_xor(substr[i], 0x3F+i) != flag[start+i])
                return false;
        }
        return true;
    };

    const auto mangle = [&](int idx, int key = 0xD0) -> int {
        return r_shift(
            _xor((input[idx] >> 2), ((key + input[idx]) % 0xFF)), 2);
    };

    if(input.size() != FLAG_SIZE)
        return false;

    // P
    if(input[0] != ((flag[0] & 0xE0) - 0x10))
        return false;

    // CC
    if(plus(input[1], 0x10) != flag[1] || minus(0x69, input[2]) != flag[2])
        return false;

    // {
    if(r_shift(_xor(r_shift(input[3], 2), 0xFF), 2) != flag[3])
        return false;

    // z
    if(input[4] != _xor(flag[4], 0x3F))
        return false;

    // 3
    if(input[5] != flag[5]) return false;

    // _
    for(const auto& underscore : underscores)
        if(validate_underscore(underscore))
            return false;

    // m4k3s
    if(!five_str_mangler(7))
        return false;

    // l1f3
    for(int i = 13; i <= 16; ++i)
        if(mangle(i) != flag[i])
            return false;

    // s0
    if(
        _not(_or(l_shift(input[18], 2), -250)) != flag[18] ||
        _xor(input[19], 0xa0) != flag[19]
    ) return false;

    // much
    if(
        _xor(input[20], input[21]) != flag[21] ||
        _xor(input[21], input[22]) != flag[22] ||
        _xor(input[22], input[23]) != flag[23] ||
        _xor(input[23], input[24]) != flag[24]
    ) return false;

    // 34s1er
    if(
        mangle(26, 0xE6) != flag[26] ||
        mangle(27, 0x10) != flag[27] ||
        mangle(28, 0x54) != flag[28] ||
        mangle(29, 0xF0) != flag[29] ||
        mangle(30, 0x26) != flag[30] ||
        mangle(31, 0x62) != flag[31]
    ) return false;

    // bu7
    if(
        _xor(input[33], input[0]) != flag[33] ||
        _xor(input[34], input[1]) != flag[34] ||
        _xor(input[35], input[2]) != flag[35]
    ) return false;

    // th1s
    if(
        _xor(input[37], input[13]) != flag[37] ||
        _xor(input[38], input[14]) != flag[38] ||
        _xor(input[39], input[15]) != flag[39] ||
        _xor(input[40], input[16]) != flag[40]
    ) return false;

    // fl4g
    if(
        _xor(r_shift(input[42], 2), 0x69) != flag[42] ||
        _xor(r_shift(input[43], 2), 0x18) != flag[43] ||
        _xor(r_shift(input[44], 2), 0xEF) != flag[44] ||
        _xor(r_shift(input[45], 2), 0x9A) != flag[45]
    ) return false;

    // 1s
    if(
        _xor(input[47], input[0]) != flag[47] ||
        _xor(input[48], input[1]) != flag[48]
    ) return false;

    // v3ry
    if(
        _xor(input[50], input[12]) != flag[50] ||
        _xor(input[51], input[13]) != flag[51] ||
        _xor(input[52], input[14]) != flag[52] ||
        _xor(input[53], input[15]) != flag[53]
    ) return false;

    // l0ng
    if(
        mangle(55, 0x9A) != flag[55] ||
        mangle(56, 0x85) != flag[56] ||
        mangle(57, 0x1E) != flag[57] ||
        mangle(58, 0xBC) != flag[58]
    ) return false;

    // which
    if(!five_str_mangler(60))
        return false;

    // m4k3s
    if(!five_str_mangler(66))
        return false;

    // 1t
    if(
        _xor(input[72], input[0]) != flag[72] ||
        _xor(input[73], input[1]) != flag[73]
    ) return false;

    // h4rd
    if(
        _xor(r_shift(input[75], 2), 0x21) != flag[75] ||
        _xor(r_shift(input[76], 2), 0x9F) != flag[76] ||
        _xor(r_shift(input[77], 2), 0x2E) != flag[77] ||
        _xor(r_shift(input[78], 2), 0xFF) != flag[78]
    ) return false;

    // }
    if(
        l_shift(minus(plus(mangle(79, 0xEF), 30), 52), 5) != flag[79]
    ) return false;

    return true;
}

int main(int argc, char* argv[], char* envp[]) {

    bool found = false;
    std::string _input;
    
    std::cout << "All I ask for, is the flag. Nothing else: ";
    std::cin >> _input;

    if((found = validate(_input))) {
        std::cout << "Good job!" << std::endl;
    } else {
        std::cout << "Should've tried harder." << std::endl;
    }
    return found;
}