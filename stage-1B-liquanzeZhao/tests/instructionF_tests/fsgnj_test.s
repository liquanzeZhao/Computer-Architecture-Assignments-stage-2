# Test case for fsgnj instruction
# fsgnj_test.s
.include "test_defines.h"
.text
.align 2
.globl _start
_start:
    TEST_INIT
    
    # Test fsgnj (copy sign): positive source, positive sign
    li t0, 0x40400000    # +3.0
    fmv.w.x fa0, t0
    li t0, 0x40000000    # +2.0
    fmv.w.x fa1, t0
    
    # fsgnj: fa2 gets magnitude of fa0 with sign of fa1 = +3.0
    fsgnj.s fa2, fa0, fa1
    fmv.x.w t0, fa2
    TEST_EXPECT t0, 0x40400000    # +3.0
    
    # Test fsgnj: positive source, negative sign
    li t0, 0x40400000    # +3.0
    fmv.w.x fa0, t0
    li t0, 0xC0000000    # -2.0
    fmv.w.x fa1, t0
    
    # fsgnj: fa2 gets magnitude of fa0 with sign of fa1 = -3.0
    fsgnj.s fa2, fa0, fa1
    fmv.x.w t0, fa2
    TEST_EXPECT t0, 0xC0400000    # -3.0
    
    # Test fsgnj: negative source, positive sign
    li t0, 0xC0400000    # -3.0
    fmv.w.x fa0, t0
    li t0, 0x40000000    # +2.0
    fmv.w.x fa1, t0
    
    # fsgnj: fa2 gets magnitude of fa0 with sign of fa1 = +3.0
    fsgnj.s fa2, fa0, fa1
    fmv.x.w t0, fa2
    TEST_EXPECT t0, 0x40400000    # +3.0
    
    # Test fsgnj: negative source, negative sign
    li t0, 0xC0400000    # -3.0
    fmv.w.x fa0, t0
    li t0, 0xC0000000    # -2.0
    fmv.w.x fa1, t0
    
    # fsgnj: fa2 gets magnitude of fa0 with sign of fa1 = -3.0
    fsgnj.s fa2, fa0, fa1
    fmv.x.w t0, fa2
    TEST_EXPECT t0, 0xC0400000    # -3.0
    
    # Test with zero
    li t0, 0x00000000    # +0.0
    fmv.w.x fa0, t0
    li t0, 0xC0000000    # -2.0
    fmv.w.x fa1, t0
    
    # fsgnj: fa2 gets magnitude of fa0 with sign of fa1 = -0.0
    fsgnj.s fa2, fa0, fa1
    fmv.x.w t0, fa2
    TEST_EXPECT t0, 0x80000000    # -0.0
    
    TEST_END