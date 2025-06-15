.include "test_defines.h"
	.text
	.align	2
	
	.globl	_start
_start:
	TEST_INIT
	
	# Test Case 1: Basic positive addition
	# 1.0 + 2.0 = 3.0
	li		t0, 0x3F800000		# 1.0 in IEEE 754 format
	li		t1, 0x40000000		# 2.0 in IEEE 754 format
	li		t2, 0x40400000		# 3.0 in IEEE 754 format (expected)
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fadd.s		f2, f0, f1
	fmv.x.w		t3, f2
	TEST_EXPECT	t3, 0x40400000
	
	# Test Case 2: Adding zero
	# 5.0 + 0.0 = 5.0
	li		t0, 0x40A00000		# 5.0
	li		t1, 0x00000000		# 0.0
	li		t2, 0x40A00000		# 5.0 (expected)
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fadd.s		f2, f0, f1
	fmv.x.w		t3, f2
	TEST_EXPECT	t3, 0x40A00000
	
	# Test Case 3: Adding negative numbers
	# -1.0 + -2.0 = -3.0
	li		t0, 0xBF800000		# -1.0
	li		t1, 0xC0000000		# -2.0
	li		t2, 0xC0400000		# -3.0 (expected)
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fadd.s		f2, f0, f1
	fmv.x.w		t3, f2
	TEST_EXPECT	t3, 0xC0400000
	
	# Test Case 4: Adding positive and negative (result positive)
	# 5.0 + (-2.0) = 3.0
	li		t0, 0x40A00000		# 5.0
	li		t1, 0xC0000000		# -2.0
	li		t2, 0x40400000		# 3.0 (expected)
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fadd.s		f2, f0, f1
	fmv.x.w		t3, f2
	TEST_EXPECT	t3, 0x40400000
	
	# Test Case 5: Adding positive and negative (result negative)
	# 2.0 + (-5.0) = -3.0
	li		t0, 0x40000000		# 2.0
	li		t1, 0xC0A00000		# -5.0
	li		t2, 0xC0400000		# -3.0 (expected)
	fmv.w.x		f0, t0
	fmv.w.x		f1, t1
	fadd.s		f2, f0, f1
	fmv.x.w		t3, f2
	TEST_EXPECT	t3, 0xC0400000
	
	TEST_END