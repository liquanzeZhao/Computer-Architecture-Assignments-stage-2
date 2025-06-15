.include "test_defines.h"
	.text
	.align	2
	
	.globl	_start
_start:
	TEST_INIT
	
	# Test Case 1: Less than comparison (true)
	# 1.0 < 2.0 should return 1
	li		t0, 0x3F800000		# 1.0
	li		t1, 0x40000000		# 2.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	flt.s		t2, f0, f1
	TEST_EXPECT	t2, 1
	
	# Test Case 2: Equal comparison
	# 1.0 < 1.0 should return 0
	li		t0, 0x3F800000		# 1.0
	li		t1, 0x3F800000		# 1.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	flt.s		t2, f0, f1
	TEST_EXPECT	t2, 0
	
	# Test Case 3: Greater than comparison
	# 2.0 < 1.0 should return 0
	li		t0, 0x40000000		# 2.0
	li		t1, 0x3F800000		# 1.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	flt.s		t2, f0, f1
	TEST_EXPECT	t2, 0
	
	# Test Case 4: Negative number comparisons
	# -2.0 < -1.0 should return 1
	li		t0, 0xC0000000		# -2.0
	li		t1, 0xBF800000		# -1.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	flt.s		t2, f0, f1
	TEST_EXPECT	t2, 1
	
	# Test Case 5: Positive and negative zero
	# -0.0 < +0.0 should return 0 (they are equal)
	li		t0, 0x80000000		# -0.0
	li		t1, 0x00000000		# +0.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	flt.s		t2, f0, f1
	TEST_EXPECT	t2, 0
	
	# Test Case 6: Comparison with NaN
	# 1.0 < NaN should return 0
	li		t0, 0x3F800000		# 1.0
	li		t1, 0x7FC00000		# NaN
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	flt.s		t2, f0, f1
	TEST_EXPECT	t2, 0
	
	TEST_END