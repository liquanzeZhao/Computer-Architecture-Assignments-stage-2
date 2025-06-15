.include "test_defines.h"
	.text
	.align	2
	
	.globl	_start
_start:
	TEST_INIT
	
	# Test Case 1: Basic multiplication
	# 2.0 * 3.0 = 6.0
	li		t0, 0x40000000		# 2.0
	li		t1, 0x40400000		# 3.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmul.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x40C00000		# 6.0
	
	# Test Case 2: Multiply by one
	# 5.0 * 1.0 = 5.0
	li		t0, 0x40A00000		# 5.0
	li		t1, 0x3F800000		# 1.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmul.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x40A00000		# 5.0
	
	# Test Case 3: Multiply by zero
	# 5.0 * 0.0 = 0.0
	li		t0, 0x40A00000		# 5.0
	li		t1, 0x00000000		# 0.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmul.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x00000000		# 0.0
	
	# Test Case 4: Multiply negative numbers
	# -2.0 * -3.0 = 6.0
	li		t0, 0xC0000000		# -2.0
	li		t1, 0xC0400000		# -3.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmul.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x40C00000		# 6.0
	
	# Test Case 5: Multiply positive and negative
	# 2.0 * -3.0 = -6.0
	li		t0, 0x40000000		# 2.0
	li		t1, 0xC0400000		# -3.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmul.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0xC0C00000		# -6.0
	
	# Test Case 6: Fractional multiplication
	# 0.5 * 0.5 = 0.25
	li		t0, 0x3F000000		# 0.5
	li		t1, 0x3F000000		# 0.5
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmul.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x3E800000		# 0.25
	
	TEST_END