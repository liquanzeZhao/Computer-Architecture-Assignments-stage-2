.include "test_defines.h"
	.text
	.align	2
	
	.globl	_start
_start:
	TEST_INIT
	
	# Test Case 1: Equal positive numbers
	# 1.0 == 1.0 should return 1
	li		t0, 0x3F800000		# 1.0
	li		t1, 0x3F800000		# 1.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	feq.s		t2, f0, f1
	TEST_EXPECT	t2, 1
	
	# Test Case 2: Equal negative numbers
	# -2.0 == -2.0 should return 1
	li		t0, 0xC0000000		# -2.0
	li		t1, 0xC0000000		# -2.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	feq.s		t2, f0, f1
	TEST_EXPECT	t2, 1
	
	# Test Case 3: Unequal numbers
	# 1.0 == 2.0 should return 0
	li		t0, 0x3F800000		# 1.0
	li		t1, 0x40000000		# 2.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	feq.s		t2, f0, f1
	TEST_EXPECT	t2, 0
	
	# Test Case 4: Positive and negative zero
	# +0.0 == -0.0 should return 1
	li		t0, 0x00000000		# +0.0
	li		t1, 0x80000000		# -0.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	feq.s		t2, f0, f1
	TEST_EXPECT	t2, 1
	
	# Test Case 5: NaN comparison
	# NaN == 1.0 should return 0
	li		t0, 0x7FC00000		# NaN
	li		t1, 0x3F800000		# 1.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	feq.s		t2, f0, f1
	TEST_EXPECT	t2, 0
	
	# Test Case 6: NaN == NaN should return 0
	li		t0, 0x7FC00000		# NaN
	li		t1, 0x7FC00000		# NaN
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	feq.s		t2, f0, f1
	TEST_EXPECT	t2, 0
	
	TEST_END