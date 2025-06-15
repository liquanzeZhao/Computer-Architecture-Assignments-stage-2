# Test case for fdiv instruction
# fdiv_test.s
.include "test_defines.h"
.text
.align 2
.globl _start
_start:
    TEST_INIT
    
    # Load test values: 6.0 / 2.0 = 3.0
    li t0, 0x40C00000    # 6.0 in IEEE 754 single precision
    fmv.w.x fa0, t0
    li t0, 0x40000000    # 2.0 in IEEE 754 single precision
    fmv.w.x fa1, t0
    
    # Test fdiv: fa2 = fa0 / fa1 = 6.0 / 2.0 = 3.0
    fdiv.s fa2, fa0, fa1
    
    # Verify result
    fmv.x.w t0, fa2
    TEST_EXPECT t0, 0x40400000    # 3.0
    
    # Test division by 1.0
    li t0, 0x40800000    # 4.0
    fmv.w.x fa0, t0
    li t0, 0x3F800000    # 1.0
    fmv.w.x fa1, t0
    
    # Test fdiv: fa2 = 4.0 / 1.0 = 4.0
    fdiv.s fa2, fa0, fa1
    fmv.x.w t0, fa2
    TEST_EXPECT t0, 0x40800000    # 4.0
    
    # Test with fractional result: 1.0 / 2.0 = 0.5
    li t0, 0x3F800000    # 1.0
    fmv.w.x fa0, t0
    li t0, 0x40000000    # 2.0
    fmv.w.x fa1, t0
    
    fdiv.s fa2, fa0, fa1
    fmv.x.w t0, fa2
    TEST_EXPECT t0, 0x3F000000    # 0.5
    
    TEST_END