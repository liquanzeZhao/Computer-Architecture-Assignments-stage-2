# FNMSUB instruction test
# fnmsub.s rd, rs1, rs2, rs3 computes -(rs1 * rs2) + rs3
    .include "test_defines.h"
    .text
    .align    2
    .globl    _start
_start:
    TEST_INIT
    
    # Load simple test values
    # f1 = 2.0, f2 = 3.0, f3 = 1.0
    li        t0, 0x40000000    # 2.0 in IEEE 754 format
    li        t1, 0x40400000    # 3.0 in IEEE 754 format
    li        t2, 0x3F800000    # 1.0 in IEEE 754 format
    fmv.w.x   f1, t0
    fmv.w.x   f2, t1
    fmv.w.x   f3, t2
    
    # Test fnmsub: -(2.0 * 3.0) + 1.0 = -6.0 + 1.0 = -5.0
    fnmsub.s  f4, f1, f2, f3
    fmv.x.w   t3, f4
    TEST_EXPECT t3, 0xC0A00000  # -5.0
    
    # Test with 1.0 * 1.0 + 1.0 = -(1.0) + 1.0 = 0.0
    fnmsub.s  f5, f3, f3, f3
    fmv.x.w   t3, f5
    TEST_EXPECT t3, 0x00000000  # 0.0
    
    TEST_END