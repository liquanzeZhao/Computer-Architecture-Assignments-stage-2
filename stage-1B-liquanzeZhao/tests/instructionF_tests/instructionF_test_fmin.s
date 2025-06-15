.include "test_defines.h"
	.text
	.align	2
	
	.globl	_start
_start:
	TEST_INIT
	
	# Test Case 1: Basic minimum of positive numbers
	# min(1.0, 2.0) = 1.0
	li		t0, 0x3F800000		# 1.0
	li		t1, 0x40000000		# 2.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmin.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x3F800000
	
	# Test Case 2: Minimum with negative numbers
	# min(-1.0, -2.0) = -2.0
	li		t0, 0xBF800000		# -1.0
	li		t1, 0xC0000000		# -2.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmin.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0xC0000000
	
	# Test Case 3: Minimum with positive and negative
	# min(1.0, -2.0) = -2.0
	li		t0, 0x3F800000		# 1.0
	li		t1, 0xC0000000		# -2.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmin.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0xC0000000
	
	# Test Case 4: Minimum with zero
	# min(0.0, 1.0) = 0.0
	li		t0, 0x00000000		# 0.0
	li		t1, 0x3F800000		# 1.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmin.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x00000000
	
	# Test Case 5: Minimum with positive and negative zero
	# min(+0.0, -0.0) = -0.0
	li		t0, 0x00000000		# +0.0
	li		t1, 0x80000000		# -0.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmin.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x80000000
	
	# Test Case 6: Minimum with NaN (should return non-NaN value)
	# min(1.0, NaN) = 1.0
	li		t0, 0x3F800000		# 1.0
	li		t1, 0x7FC00000		# NaN
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fmin.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x3F800000
	
	TEST_END