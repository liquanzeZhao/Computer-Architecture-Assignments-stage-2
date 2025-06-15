# FCLASS instruction test
# fclass.s rd, rs1 classifies the floating point number
# Returns a 10-bit mask indicating the class of the floating point number
    .include "test_defines.h"
    .text
    .align    2
    .globl    _start
_start:
    TEST_INIT
    
    # Test positive normal number (1.0)
    # Should return bit 6 set (0x40)
    li        t0, 0x3F800000    # 1.0
    fmv.w.x   f1, t0
    fclass.s  t1, f1
    TEST_EXPECT t1, 0x40        # Positive normal number
    
    # Test negative normal number (-1.0)  
    # Should return bit 1 set (0x02)
    li        t0, 0xBF800000    # -1.0
    fmv.w.x   f2, t0
    fclass.s  t1, f2
    TEST_EXPECT t1, 0x02        # Negative normal number
    
    # Test positive zero
    # Should return bit 4 set (0x10)
    li        t0, 0x00000000    # +0.0
    fmv.w.x   f3, t0
    fclass.s  t1, f3
    TEST_EXPECT t1, 0x10        # Positive zero
    
    # Test negative zero
    # Should return bit 3 set (0x08)
    li        t0, 0x80000000    # -0.0
    fmv.w.x   f4, t0
    fclass.s  t1, f4
    TEST_EXPECT t1, 0x08        # Negative zero
    
    # Test positive infinity
    # Should return bit 7 set (0x80)
    li        t0, 0x7F800000    # +inf
    fmv.w.x   f5, t0
    fclass.s  t1, f5
    TEST_EXPECT t1, 0x80        # Positive infinity
    
    # Test negative infinity
    # Should return bit 0 set (0x01)
    li        t0, 0xFF800000    # -inf
    fmv.w.x   f6, t0
    fclass.s  t1, f6
    TEST_EXPECT t1, 0x01        # Negative infinity
    
    # Test quiet NaN
    # Should return bit 9 set (0x200)
    li        t0, 0x7FC00000    # qNaN
    fmv.w.x   f7, t0
    fclass.s  t1, f7
    TEST_EXPECT t1, 0x200       # Quiet NaN
    
    TEST_END