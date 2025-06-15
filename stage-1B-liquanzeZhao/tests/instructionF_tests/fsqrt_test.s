# FSQRT instruction test  
# fsqrt.s rd, rs1 computes square root of rs1
    .include "test_defines.h"
    .text
    .align    2
    .globl    _start
_start:
    TEST_INIT
    
    # Test sqrt(4.0) = 2.0
    li        t0, 0x40800000    # 4.0 in IEEE 754 format
    fmv.w.x   f1, t0
    fsqrt.s   f2, f1
    fmv.x.w   t1, f2
    TEST_EXPECT t1, 0x40000000  # 2.0
    
    # Test sqrt(1.0) = 1.0
    li        t0, 0x3F800000    # 1.0
    fmv.w.x   f3, t0
    fsqrt.s   f4, f3
    fmv.x.w   t1, f4
    TEST_EXPECT t1, 0x3F800000  # 1.0
    
    # Test sqrt(0.0) = 0.0
    li        t0, 0x00000000    # 0.0
    fmv.w.x   f5, t0
    fsqrt.s   f6, f5
    fmv.x.w   t1, f6
    TEST_EXPECT t1, 0x00000000  # 0.0
    
    # Test sqrt(9.0) = 3.0
    li        t0, 0x41100000    # 9.0
    fmv.w.x   f7, t0
    fsqrt.s   f8, f7
    fmv.x.w   t1, f8
    TEST_EXPECT t1, 0x40400000  # 3.0
    
    # Test sqrt(0.25) = 0.5
    li        t0, 0x3E800000    # 0.25
    fmv.w.x   f9, t0
    fsqrt.s   f10, f9
    fmv.x.w   t1, f10
    TEST_EXPECT t1, 0x3F000000  # 0.5
    
    TEST_END