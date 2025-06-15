	.macro  TEST_INIT
	addi	a0, zero, 0
	addi	a1, zero, 0
	la		gp, __global_pointer$
	.endm

	.macro  TEST_FLOAT_OP1 _num, _inst, _flags, _result, _val1
_test_\_num:
	addi a1, zero, \_num
	la  t0, _test_data_\_num 
	flw f10, 0(t0)
	flw f11, 4(t0)
	fmv.x.w t4, f11
	\_inst f13, f10
	fmv.x.w t3, f13
 	fsflags t5, x0
	li t6, (\_flags)
	bne	t3, t4, _test_finish
	bne	t5, t6, _test_finish
	.pushsection .data
	.align 2
_test_data_\_num:
	.float \_val1
	.float \_result
	.popsection
	.endm

	.macro  TEST_FLOAT_HEX_OP1 _num, _inst, _flags, _result, _val1
_test_\_num:
	addi a1, zero, \_num
	li t0, (\_val1)
	fmv.w.x f10, t0
	li t4, (\_result)
	\_inst f13, f10
	fmv.x.w t3, f13
 	fsflags t5, x0
	li t6, (\_flags)
	bne	t3, t4, _test_finish
	bne	t5, t6, _test_finish
	.endm

	.macro  TEST_FLOAT_OP2 _num, _inst, _flags, _result, _val1, _val2
_test_\_num:
	addi a1, zero, \_num
	la  t0, _test_data_\_num 
	flw f10, 0(t0)
	flw f11, 4(t0)
	flw f12, 8(t0)
	fmv.x.w t4, f12
	\_inst f13, f10, f11
	fmv.x.w t3, f13
 	fsflags t5, x0
	li t6, (\_flags)
	bne	t3, t4, _test_finish
	bne	t5, t6, _test_finish
	.pushsection .data
	.align 2
_test_data_\_num:
	.float \_val1
	.float \_val2
	.float \_result
	.popsection
	.endm

	.macro  TEST_FLOAT_HEX_OP2 _num, _inst, _flags, _result, _val1, _val2
_test_\_num:
	addi a1, zero, \_num
	li t0, (\_val1)
	fmv.w.x f10, t0
	li t0, (\_val2)
	fmv.w.x f11, t0
	li t4, (\_result)
	\_inst f13, f10, f11
	fmv.x.w t3, f13
 	fsflags t5, x0
	li t6, (\_flags)
	bne	t3, t4, _test_finish
	bne	t5, t6, _test_finish
	.endm

	.macro  TEST_FLOAT_OP3 _num, _inst, _flags, _result, _val1, _val2, _val3
_test_\_num:
	addi a1, zero, \_num
	la  t0, _test_data_\_num 
	flw f10, 0(t0)
	flw f11, 4(t0)
	flw f12, 8(t0)
	flw f13, 12(t0)
	fmv.x.w t4, f13
	\_inst f13, f10, f11, f12
	fmv.x.w t3, f13
 	fsflags t5, x0
	li t6, (\_flags)
	bne	t3, t4, _test_finish
	bne	t5, t6, _test_finish
	.pushsection .data
	.align 2
_test_data_\_num:
	.float \_val1
	.float \_val2
	.float \_val3
	.float \_result
	.popsection
	.endm

	.macro  TEST_FLOAT_CMP _num, _inst, _flags, _result, _val1, _val2
_test_\_num:
	addi a1, zero, \_num
	la  t0, _test_data_\_num 
	flw f10, 0(t0)
	flw f11, 4(t0)
	li  t4, (\_result)
	\_inst t3, f10, f11
 	fsflags t5, x0
	li t6, (\_flags)
	bne	t3, t4, _test_finish
	bne	t5, t6, _test_finish
	.pushsection .data
	.align 2
_test_data_\_num:
	.float \_val1
	.float \_val2
	.popsection
	.endm

	.macro  TEST_FLOAT_HEX_CMP _num, _inst, _flags, _result, _val1, _val2
_test_\_num:
	addi a1, zero, \_num
	li t0, (\_val1)
	fmv.w.x f10, t0
	li t0, (\_val2)
	fmv.w.x f11, t0
	li  t4, (\_result)
	\_inst t3, f10, f11
 	fsflags t5, x0
	li t6, (\_flags)
	bne	t3, t4, _test_finish
	bne	t5, t6, _test_finish
	.endm

	.macro  TEST_FLOAT_CVT_WS _num, _inst, _flags, _result, _val1
_test_\_num:
	addi a1, zero, \_num
	la  t0, _test_data_\_num 
	flw f10, 0(t0)
	li  t4, (\_result)
	\_inst t3, f10, rtz
 	fsflags t5, x0
	li t6, (\_flags)
	bne	t3, t4, _test_finish
	bne	t5, t6, _test_finish
	.pushsection .data
	.align 2
_test_data_\_num:
	.float \_val1
	.popsection
	.endm

	.macro  TEST_FLOAT_CVT_SW _num, _inst, _result, _val1
_test_\_num:
	addi a1, zero, \_num
	la  t0, _test_data_\_num 
	lw  t4, 0(t0)
	li  t3, (\_val1)
	\_inst f10, t3
	fmv.x.w t3, f10
	bne	t3, t4, _test_finish
	.pushsection .data
	.align 2
_test_data_\_num:
	.float \_result
	.popsection
	.endm

	.macro  TEST_FLOAT_FCLASS _num, _result, _val1
_test_\_num:
	addi a1, zero, \_num
	li t0, (\_val1)
	fmv.w.x f10, t0
	fclass.s t4, f10
	li t5, (\_result)
	bne	t4, t5, _test_finish
	.endm

	.macro  TEST_EXPECT _r, _v
	li	a1, (\_v)
	bne	\_r, a1, _test_finish
	.endm

    .macro  TEST_FAIL
	addi	a0, zero, 0
	jr	zero
	.endm

	.macro  TEST_END
	addi	a0, zero, 1
_test_finish:
	addi	a1, a1, 0
	jr	zero
	.endm

