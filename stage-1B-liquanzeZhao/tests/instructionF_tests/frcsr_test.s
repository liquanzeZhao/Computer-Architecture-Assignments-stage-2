# Test case for frcsr instruction (already provided in example)
# frcsr_test.s
.include "test_defines.h"
.text
.align 2
.globl _start
_start:
    TEST_INIT
    
    # Test default CSR state
    frcsr t0
    TEST_EXPECT t0, 0x00000000
    
    # Set CSR to a known value and read it back
    li t1, 0x000000E0  # Set rounding mode and flags
    fscsr t0, t1
    frcsr t0
    TEST_EXPECT t0, 0x000000E0
    
    # Clear CSR and verify
    li t1, 0x00000000
    fscsr t0, t1
    frcsr t0
    TEST_EXPECT t0, 0x00000000
    
    TEST_END