.include "test_defines.h"

.text
.align	2

.globl	_start
_start:
    TEST_INIT

    # Load test values
    li      t1, 0x3F800000  # 1.0 (single-precision float)
    li      t2, 0x40000000  # 2.0
    li      t3, 0x40400000  # 3.0
    li      t4, 0xBF800000  # -1.0
    li      t5, 0x00000000  # 0.0
    
    # Test 1: 1.0 + 2.0 = 3.0
    fmv.w.x f0, t1          # Load 1.0 to f0
    fmv.w.x f1, t2          # Load 2.0 to f1
    fadd.s  f2, f0, f1     # f2 = f0 + f1
    fmv.x.w a1, f2          # Move result to integer register
    TEST_EXPECT a1, 0x40400000  # Expect 3.0
    
    # Test 2: 1.0 + (-1.0) = 0.0
    fmv.w.x f3, t4          # Load -1.0 to f3
    fadd.s  f4, f0, f3     # f4 = f0 + f3
    fmv.x.w a1, f4          # Move result to integer register
    TEST_EXPECT a1, 0x00000000  # Expect 0.0
    
    # Test 3: 2.0 + 0.0 = 2.0
    fmv.w.x f5, t5          # Load 0.0 to f5
    fadd.s  f6, f1, f5     # f6 = f1 + f5
    fmv.x.w a1, f6          # Move result to integer register
    TEST_EXPECT a1, 0x40000000  # Expect 2.0
    
    # Test 4: 3.0 + (-1.0) = 2.0
    fmv.w.x f7, t3          # Load 3.0 to f7
    fadd.s  f8, f7, f3     # f8 = f7 + f3
    fmv.x.w a1, f8          # Move result to integer register
    TEST_EXPECT a1, 0x40000000  # Expect 2.0

    TEST_END