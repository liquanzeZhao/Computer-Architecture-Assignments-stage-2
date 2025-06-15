
# Test case for fmsub instruction
# fmsub_test.s
.include "test_defines.h"
.text
.align 2
.globl _start
_start:
    TEST_INIT
    
    # Load test floating point values
    # fa0 = 2.0, fa1 = 3.0, fa2 = 1.0
    li t0, 0x40000000    # 2.0 in IEEE 754 single precision
    fmv.w.x fa0, t0
    li t0, 0x40400000    # 3.0 in IEEE 754 single precision
    fmv.w.x fa1, t0
    li t0, 0x3F800000    # 1.0 in IEEE 754 single precision
    fmv.w.x fa2, t0
    
    # Test fmsub: fa3 = fa0 * fa1 - fa2 = 2.0 * 3.0 - 1.0 = 5.0
    fmsub.s fa3, fa0, fa1, fa2
    
    # Convert result back to integer for comparison
    fmv.x.w t0, fa3
    li t1, 0x40A00000    # 5.0 in IEEE 754 single precision
    TEST_EXPECT t0, 0x40A00000
    
    # Test with zero values
    # fa0 = 0.0, fa1 = 5.0, fa2 = 0.0
    li t0, 0x00000000    # 0.0
    fmv.w.x fa0, t0
    li t0, 0x40A00000    # 5.0
    fmv.w.x fa1, t0
    li t0, 0x00000000    # 0.0
    fmv.w.x fa2, t0
    
    # Test fmsub: fa3 = 0.0 * 5.0 - 0.0 = 0.0
    fmsub.s fa3, fa0, fa1, fa2
    fmv.x.w t0, fa3
    TEST_EXPECT t0, 0x00000000
    
    TEST_END