# FCVT instruction test
# Various floating point conversion instructions
    .include "test_defines.h"
    .text
    .align    2
    .globl    _start
_start:
    TEST_INIT
    
    # Test fcvt.w.s (float to signed int)
    # Convert 3.7 to integer, should truncate to 3
    li        t0, 0x406CCCCD    # 3.7 in IEEE 754 format
    fmv.w.x   f1, t0
    fcvt.w.s  t1, f1
    TEST_EXPECT t1, 3
    
    # Test fcvt.w.s with negative number
    # Convert -2.9 to integer, should truncate to -2
    li        t0, 0xC03CCCCD    # -2.9 in IEEE 754 format
    fmv.w.x   f2, t0
    fcvt.w.s  t1, f2
    TEST_EXPECT t1, -2
    
    # Test fcvt.wu.s (float to unsigned int)
    # Convert 5.8 to unsigned integer, should truncate to 5
    li        t0, 0x40B9999A    # 5.8 in IEEE 754 format
    fmv.w.x   f3, t0
    fcvt.wu.s t1, f3
    TEST_EXPECT t1, 5
    
    # Test fcvt.s.w (signed int to float)
    # Convert integer 7 to float 7.0
    li        t0, 7
    fcvt.s.w  f4, t0
    fmv.x.w   t1, f4
    li        t2, 0x40E00000    # 7.0 in IEEE 754 format
    TEST_EXPECT t1, 0x40E00000
    
    # Test fcvt.s.w with negative integer
    # Convert integer -4 to float -4.0
    li        t0, -4
    fcvt.s.w  f5, t0
    fmv.x.w   t1, f5
    li        t2, 0xC0800000    # -4.0 in IEEE 754 format
    TEST_EXPECT t1, 0xC0800000
    
    # Test fcvt.s.wu (unsigned int to float)
    # Convert unsigned integer 10 to float 10.0
    li        t0, 10
    fcvt.s.wu f6, t0
    fmv.x.w   t1, f6
    li        t2, 0x41200000    # 10.0 in IEEE 754 format
    TEST_EXPECT t1, 0x41200000
    
    # Test fcvt.w.s with zero
    # Convert 0.0 to integer 0
    li        t0, 0x00000000    # 0.0
    fmv.w.x   f7, t0
    fcvt.w.s  t1, f7
    TEST_EXPECT t1, 0
    
    # Test fcvt.s.w with zero
    # Convert integer 0 to float 0.0
    li        t0, 0
    fcvt.s.w  f8, t0
    fmv.x.w   t1, f8
    TEST_EXPECT t1, 0x00000000  # 0.0
    
    TEST_END