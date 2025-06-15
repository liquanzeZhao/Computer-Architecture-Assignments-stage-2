.include "test_defines.h"
	.text
	.align	2
	
	.globl	_start
_start:
	TEST_INIT
	
	# Test Case 1: Basic subtraction
	# 3.0 - 1.0 = 2.0
	li		t0, 0x40400000		# 3.0
	li		t1, 0x3F800000		# 1.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fsub.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x40000000		# 2.0
	
	# Test Case 2: Subtract zero
	# 5.0 - 0.0 = 5.0
	li		t0, 0x40A00000		# 5.0
	li		t1, 0x00000000		# 0.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fsub.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x40A00000		# 5.0
	
	# Test Case 3: Subtract from zero
	# 0.0 - 3.0 = -3.0
	li		t0, 0x00000000		# 0.0
	li		t1, 0x40400000		# 3.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fsub.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0xC0400000		# -3.0
	
	# Test Case 4: Subtract negative number
	# 2.0 - (-1.0) = 3.0
	li		t0, 0x40000000		# 2.0
	li		t1, 0xBF800000		# -1.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fsub.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x40400000		# 3.0
	
	# Test Case 5: Subtract same number (result is zero)
	# 2.0 - 2.0 = 0.0
	li		t0, 0x40000000		# 2.0
	li		t1, 0x40000000		# 2.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fsub.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x00000000		# 0.0
	
	# Test Case 6: Negative numbers subtraction
	# -1.0 - (-3.0) = 2.0
	li		t0, 0xBF800000		# -1.0
	li		t1, 0xC0400000		# -3.0
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fsub.s		f2, f0, f1
	fmv.x.w		t2, f2
	TEST_EXPECT	t2, 0x40000000		# 2.0
	
	TEST_END