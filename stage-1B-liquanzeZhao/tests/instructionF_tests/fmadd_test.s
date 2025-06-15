# Test case for fmadd instruction
# fmadd_test.s
.include "test_defines.h"
.text
.align 2
.globl _start
_start:
    TEST_INIT
    
    # Load test values
    # fa0 = 2.0, fa1 = 3.0, fa2 = 1.0
    li t0, 0x40000000    # 2.0
    fmv.w.x fa0, t0
    li t0, 0x40400000    # 3.0
    fmv.w.x fa1, t0
    li t0, 0x3F800000    # 1.0
    fmv.w.x fa2, t0
    
    # Test fmadd: fa3 = fa0 * fa1 + fa2 = 2.0 * 3.0 + 1.0 = 7.0
    fmadd.s fa3, fa0, fa1, fa2
    
    # Verify result
    fmv.x.w t0, fa3
    TEST_EXPECT t0, 0x40E00000    # 7.0
    
    # Test with zero multiplication: 0.0 * 5.0 + 2.0 = 2.0
    li t0, 0x00000000    # 0.0
    fmv.w.x fa0, t0
    li t0, 0x40A00000    # 5.0
    fmv.w.x fa1, t0
    li t0, 0x40000000    # 2.0
    fmv.w.x fa2, t0
    
    fmadd.s fa3, fa0, fa1, fa2
    fmv.x.w t0, fa3
    TEST_EXPECT t0, 0x40000000    # 2.0
    
    # Test identity: 1.0 * 4.0 + 0.0 = 4.0
    li t0, 0x3F800000    # 1.0
    fmv.w.x fa0, t0
    li t0, 0x40800000    # 4.0
    fmv.w.x fa1, t0
    li t0, 0x00000000    # 0.0
    fmv.w.x fa2, t0
    
    fmadd.s fa3, fa0, fa1, fa2
    fmv.x.w t0, fa3
    TEST_EXPECT t0, 0x40800000    # 4.0
    
    TEST_END