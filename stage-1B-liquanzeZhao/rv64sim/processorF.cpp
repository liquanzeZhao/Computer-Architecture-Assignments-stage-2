/* ****************************************************************
RISC-V Instruction Set Simulator
Computer Architecture, Semester 1, 2025

Class members for processorF

**************************************************************** */

#include <iostream>
#include <iomanip> 
#include <stdlib.h>
#include <fenv.h>

#include <cmath>
#include <math.h>

#include "memory.h"
#include "processor.h"

using namespace std;

typedef union  _u32_f32_t
{
	float f32_val;

	unsigned int u32_val;
} u32_f32_union;

// reads & writes the Rounding Mode field function 
unsigned int processor::get_frm()
{
	return ((fcsr & FCSR_FRM_MASK) >> FCSR_FRM_POS);
}

void processor::set_frm(unsigned int new_frm)
{
	int mode = FE_TONEAREST;
	switch (new_frm)
	{
	case 0: mode = FE_TONEAREST; break;
	case 1: mode = FE_TOWARDZERO; break;
	case 2: mode = FE_DOWNWARD; break;
	case 3: mode = FE_UPWARD; break;
	case 4: mode = FE_TONEAREST; break;
	}
	fesetround(mode);

	fcsr = (fcsr & ~FCSR_FRM_MASK) | ((new_frm & FRM_VALID_BITS_MASK) << FCSR_FRM_POS);
}


void processor::set_freg(unsigned int reg_num, float newValue)
{
	f[reg_num] = newValue;
}

void processor::set_fflags(unsigned int newFflags)
{
	fcsr = (fcsr & ~FCSR_FFLAGS_MASK) | ((newFflags & FFLAGS_VALID_BITS_MASK) << FCSR_FFLAGS_POS);
}

unsigned int processor::get_fflags()
{
	unsigned int ret = (fcsr & FCSR_FFLAGS_MASK) >> FCSR_FFLAGS_POS;
	return ret;
}

void processor::show_freg(unsigned int regNum)
{
	cout << f[regNum] << endl;
}


void processor::instruction_flw(unsigned int rd, unsigned int rs1, int64_t imm)
{
	if (verbose)
		cout << "flw: rd = " << dec << rd << ", rs1 = " << dec << rs1 << ", immed_I = " << setw(16) << setfill('0') << hex << imm << endl;

	uint64_t mem_addr = x[rs1] + imm;
	if (mem_addr % 4 != 0)
	{
		cout << "Error: misaligned address for flw" << endl;
	}
	else
	{
		float data = main_memory->read_float(mem_addr);
		set_freg(rd, data);
		cycle_count += CYCLE_COUNT_LOAD;
	}
}

void processor::instruction_fsw(unsigned int rs1, unsigned int rs2, int64_t imm)
{
	if (verbose)
		cout << "fsw: rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 << ", immed_S = " << setw(16) << setfill('0') << hex << imm << endl;

	uint64_t mem_addr = x[rs1] + imm;
	if (mem_addr % 4 != 0)
	{
		cout << "Error: misaligned address for fsw" << endl;
	}
	else
	{
		float data = f[rs2];
		main_memory->write_float(mem_addr, data);
		cycle_count += CYCLE_COUNT_STORE;
	}
}

void processor::instruction_fmadd_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int rs3)
{
	if (verbose)
		cout << "fmadd: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1
		<< ", rs2 = " << dec << rs2
		<< ", rs3 = " << dec << rs3 << endl;

	u32_f32_union d1, d2, d3, value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	d3.f32_val = f[rs3];
	value.f32_val = (d1.f32_val * d2.f32_val) + d3.f32_val;

	if (d1.f32_val == 0.0f || d2.f32_val == 0.0f)
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else if (!isfinite(d1.f32_val) || !isfinite(d2.f32_val))
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else
	{
		set_fflags(0);
	}

	set_frm(rm);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fmsub_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int rs3)
{
	if (verbose)
		cout << "fmsub: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1
		<< ", rs2 = " << dec << rs2
		<< ", rs3 = " << dec << rs3 << endl;

	u32_f32_union d1, d2, d3, value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	d3.f32_val = f[rs3];
	value.f32_val = (d1.f32_val * d2.f32_val) - d3.f32_val;

	if (d1.f32_val == 0.0f || d2.f32_val == 0.0f)
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else if (!isfinite(d1.f32_val) || !isfinite(d2.f32_val))
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else
	{
		set_fflags(0);
	}

	set_frm(rm);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fnmadd_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int rs3)
{
	if (verbose)
		cout << "fnmadd: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1
		<< ", rs2 = " << dec << rs2
		<< ", rs3 = " << dec << rs3 << endl;

	u32_f32_union d1, d2, d3, value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	d3.f32_val = f[rs3];
	value.f32_val = -(d1.f32_val * d2.f32_val) + d3.f32_val;

	if (d1.f32_val == 0.0f || d2.f32_val == 0.0f)
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else if (!isfinite(d1.f32_val) || !isfinite(d1.f32_val))
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else
	{
		set_fflags(0);
	}

	set_frm(rm);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fnmsub_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int rs3)
{
	if (verbose)
		cout << "fnmsub: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1
		<< ", rs2 = " << dec << rs2
		<< ", rs3 = " << dec << rs3 << endl;

	u32_f32_union d1, d2, d3, value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	d3.f32_val = f[rs3];
	value.f32_val = -(d1.f32_val * d2.f32_val) - d3.f32_val;

	if (d1.f32_val == 0.0f || d2.f32_val == 0.0f)
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else if (!isfinite(d1.f32_val) || !isfinite(d1.f32_val))
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else
	{
		set_fflags(0);
	}

	set_frm(rm);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fop(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int funct7)
{
	switch (funct7)
	{
	case 0x10:
		switch (rm)
		{
		case 0x00:
			instruction_fsgnj_s(rd, rs1, rs2);
			break;
		case 0x02:
			instruction_fsgnjx_s(rd, rs1, rs2);
			break;
		case 0x01:
			instruction_fsgnjn_s(rd, rs1, rs2);
			break;
		default:
			instruction_illegal();
			break;
		}
		break;
	case 0x00:
		instruction_fadd_s(rd, rm, rs1, rs2);
		break;
	case 0x08:
		instruction_fmul_s(rd, rm, rs1, rs2);
		break;
	case 0x04:
		instruction_fsub_s(rd, rm, rs1, rs2);
		break;
	case 0x0c:
		instruction_fdiv_s(rd, rm, rs1, rs2);
		break;
	case 0x2c:
		switch (rs2)
		{
		case 0x00:
			instruction_fsqrt_s(rd, rm, rs1);
			break;
		default:
			instruction_illegal();
			break;
		}
		break;
	case 0x14:
		switch (rm)
		{
		case 0x01:
			instruction_fmax_s(rd, rs1, rs2);
			break;
		case 0x00:
			instruction_fmin_s(rd, rs1, rs2);
			break;
		default:
			instruction_illegal();
			break;
		}
		break;
	case 0x60:
		switch (rs2)
		{
		case 0x00:
			instruction_fcvt_w_s(rd, rm, rs1);
			break;
		case 0x02:
			instruction_fcvt_l_s(rd, rm, rs1);
			break;
		case 0x01:
			instruction_fcvt_wu_s(rd, rm, rs1);
			break;
		case 0x03:
			instruction_fcvt_lu_s(rd, rm, rs1);
			break;
		default:
			instruction_illegal();
			break;
		}
		break;
	case 0x70:
		switch (rm)
		{
		case 0x01:
			instruction_fclass_s(rd, rs1);
			break;
		case 0x00:
			instruction_fmv_x_w(rd, rs1);
			break;
		default:
			instruction_illegal();
			break;
		}
		break;
	case 0x68:
		switch (rs2)
		{
		case 0x00:
			instruction_fcvt_s_w(rd, rm, rs1);
			break;
		case 0x02:
			instruction_fcvt_s_l(rd, rm, rs1);
			break;
		case 0x01:
			instruction_fcvt_s_wu(rd, rm, rs1);
			break;
		case 0x03:
			instruction_fcvt_s_lu(rd, rm, rs1);
			break;
		default:
			instruction_illegal();
			break;
		}
		break;
	case 0x50:
		switch (rm)
		{
		case 0x01:
			instruction_flt_s(rd, rs1, rs2);
			break;
		case 0x00:
			instruction_fle_s(rd, rs1, rs2);
			break;
		case 0x02:
			instruction_feq_s(rd, rs1, rs2);
			break;
			break;
		default:
			instruction_illegal();
			break;
		}
		break;
	case 0x78:
		switch (rs2)
		{
		case 0x00:
			instruction_fmv_w_x(rd, rs1);
			break;
		default:
			instruction_illegal();
			break;
		}
		break;
	default:
		instruction_illegal();
		break;
	}
}

void processor::instruction_fadd_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fadd: rd = " << dec << rd << ", rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	u32_f32_union value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	value.f32_val = d1.f32_val + d2.f32_val;

	if (debug)
		cout << "fadd: " << d1.f32_val << " + " << d2.f32_val << " = " << value.f32_val << endl;

	set_fflags(0);
	set_frm(rm);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fsub_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fsub: rd = " << dec << rd << ", rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	u32_f32_union value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	value.f32_val = d1.f32_val - d2.f32_val;

	if (debug)
		cout << "fsub: " << d1.f32_val << " - " << d2.f32_val << " = " << value.f32_val << endl;

	set_fflags(0);
	set_frm(rm);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fmul_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fmul: rd = " << dec << rd << ", rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	u32_f32_union value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	value.f32_val = d1.f32_val * d2.f32_val;

	if (debug)
		cout << "fmul: " << d1.f32_val << " * " << d2.f32_val << " = " << value.f32_val << endl;

	set_fflags(0);
	set_frm(rm);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fdiv_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fdiv: rd = " << dec << rd << ", rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	u32_f32_union value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	value.f32_val = d1.f32_val / d2.f32_val;

	if (debug)
		cout << "fdiv: " << d1.f32_val << " / " << d2.f32_val << " = " << value.f32_val << endl;

	set_fflags(0);
	set_frm(rm);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fsqrt_s(unsigned int rd, unsigned int rm, unsigned int rs1)
{
	if (verbose)
		cout << "fsqrt: rd = " << dec << rd << ", rs1 = " << dec << rs1 << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	d1.f32_val = f[rs1];
	d2.f32_val = sqrtf(d1.f32_val);

	if (!isfinite(d2.f32_val))
	{
		d2.u32_val = 0x7fc00000; // NaN 
		set_fflags(FCSR_FFLAGS_NV);
	}
	else
	{
		set_fflags(0);
	}

	if (debug)
		cout << "fsqrt: " << d1.f32_val << " = " << d2.f32_val << endl;

	set_frm(rm);
	set_freg(rd, d2.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fsgnj_s(unsigned int rd, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fsgnj.s: rd = " << dec << rd << ", rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	u32_f32_union value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	value.u32_val = (d1.u32_val & 0x7fffffff) | (d2.u32_val & 0x80000000);

	if (debug)
		cout << "fsgnj.s: " << setw(8) << setfill('0') << hex << d1.u32_val
		<< " & " << setw(8) << setfill('0') << hex << d2.u32_val
		<< " = " << setw(8) << setfill('0') << hex << value.u32_val << endl;

	set_fflags(0);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fsgnjn_s(unsigned int rd, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fsgnjn.s: rd = " << dec << rd << ", rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	u32_f32_union value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	value.u32_val = (d1.u32_val & 0x7fffffff) | ((~d2.u32_val) & 0x80000000);

	if (debug)
		cout << "fsgnjn.s: " << setw(8) << setfill('0') << hex << d1.u32_val
		<< " & " << setw(8) << setfill('0') << hex << d2.u32_val
		<< " = " << setw(8) << setfill('0') << hex << value.u32_val << endl;

	set_fflags(0);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fsgnjx_s(unsigned int rd, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fsgnjn.s: rd = " << dec << rd << ", rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	u32_f32_union value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];
	value.u32_val = (d1.u32_val & 0x7fffffff) | ((d1.u32_val ^ d2.u32_val) & 0x80000000);

	if (debug)
		cout << "fsgnjn.s: " << setw(8) << setfill('0') << hex << d1.u32_val
		<< " & " << setw(8) << setfill('0') << hex << d2.u32_val
		<< " = " << setw(8) << setfill('0') << hex << value.u32_val << endl;

	set_fflags(0);
	set_freg(rd, value.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fmin_s(unsigned int rd, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fmin: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1
		<< ", rs2 = " << dec << rs2 << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	u32_f32_union value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];

	if (debug)
		cout << "fmin: " << d1.f32_val << " , " << d2.f32_val << endl;

	if ((isnan(d1.f32_val) && (d1.u32_val & 0x00400000) == 0)
		|| (isnan(d2.f32_val) && (d2.u32_val & 0x00400000) == 0))
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else
	{
		set_fflags(0);
	}

	if (!isfinite(d1.f32_val) && !isfinite(d2.f32_val))
	{
		value.u32_val = 0x7fc00000;
		set_freg(rd, value.f32_val);
	}
	else if (!isfinite(d1.f32_val))
	{
		set_freg(rd, d2.f32_val);
	}
	else if (!isfinite(d2.f32_val))
	{
		set_freg(rd, d1.f32_val);
	}
	else
	{
		if (signbit(d1.f32_val))
		{
			set_freg(rd, d1.f32_val > d2.f32_val ? d2.f32_val : d1.f32_val);
		}
		else
		{
			set_freg(rd, d1.f32_val < d2.f32_val ? d1.f32_val : d2.f32_val);
		}
	}
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fmax_s(unsigned int rd, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fmax: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1
		<< ", rs2 = " << dec << rs2 << endl;

	u32_f32_union d1, d2, value;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];

	if (debug)
		cout << "fmax: " << d1.f32_val << " , " << d2.f32_val << endl;

	if ((isnan(d1.f32_val) && (d1.u32_val & 0x00400000) == 0)
		|| (isnan(d2.f32_val) && (d2.u32_val & 0x00400000) == 0))
	{
		set_fflags(FCSR_FFLAGS_NV);
	}
	else
	{
		set_fflags(0);
	}

	if (!isfinite(d1.f32_val) && !isfinite(d2.f32_val))
	{
		value.u32_val = 0x7fc00000;
		set_freg(rd, value.f32_val);
	}
	else if (!isfinite(d1.f32_val))
	{
		set_freg(rd, d2.f32_val);
	}
	else if (!isfinite(d2.f32_val))
	{
		set_freg(rd, d1.f32_val);
	}
	else
	{
		if (signbit(d1.f32_val))
		{
			set_freg(rd, d1.f32_val > d2.f32_val ? d1.f32_val : d2.f32_val);
		}
		else
		{
			set_freg(rd, d1.f32_val < d2.f32_val ? d2.f32_val : d1.f32_val);
		}
	}
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_feq_s(unsigned int rd, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "feq.s: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1
		<< ", rs2 = " << dec << rs2 << endl;
	if (debug)
		cout << "feq.s: " << f[rs1] << " == " << f[rs2] << endl;

	u32_f32_union d1;
	u32_f32_union d2;
	d1.f32_val = f[rs1];
	d2.f32_val = f[rs2];

	// signaling NaN 
	if ((isnan(d1.f32_val) && (d1.u32_val & 0x00400000) == 0)
		|| (isnan(d2.f32_val) && (d2.u32_val & 0x00400000) == 0))
	{
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, 0);
	}
	else
	{
		set_fflags(0);
		set_reg(rd, (d1.f32_val == d2.f32_val) ? 1 : 0);
	}
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_flt_s(unsigned int rd, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "flt.s: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1
		<< ", rs2 = " << dec << rs2 << endl;
	if (debug)
		cout << "flt.s: " << f[rs1] << " < " << f[rs2] << endl;

	float    fd1 = f[rs1];
	float    fd2 = f[rs2];
	if (!isfinite(fd1) || !isfinite(fd2))
	{
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, 0);
	}
	else
	{
		set_fflags(0);
		set_reg(rd, (fd1 < fd2) ? 1 : 0);
	}
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fle_s(unsigned int rd, unsigned int rs1, unsigned int rs2)
{
	if (verbose)
		cout << "fle.s: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1
		<< ", rs2 = " << dec << rs2 << endl;
	if (debug)
		cout << "fle.s: " << f[rs1] << " <= " << f[rs2] << endl;

	float    fd1 = f[rs1];
	float    fd2 = f[rs2];
	if (!isfinite(fd1) || !isfinite(fd2))
	{
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, 0);
	}
	else
	{
		set_fflags(0);
		set_reg(rd, (fd1 <= fd2) ? 1 : 0);
	}
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fclass_s(unsigned int rd, unsigned int rs1)
{
	if (verbose)
		cout << "fclass.s: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1 << endl;

	u32_f32_union data_f32;
	data_f32.f32_val = f[rs1];

	uint32_t fpClass = fpclassify(data_f32.f32_val);
	uint32_t value = 9;
	switch (fpClass)
	{
	case FP_NAN:
		value = (data_f32.u32_val & 0x00400000) ? 9 : 8;
		break;
	case FP_INFINITE:
		value = signbit(data_f32.f32_val) ? 0 : 7;
		break;
	case FP_ZERO:
		value = signbit(data_f32.f32_val) ? 3 : 4;
		break;
	case FP_SUBNORMAL:
		value = signbit(data_f32.f32_val) ? 2 : 5;
		break;
	case FP_NORMAL:
		value = signbit(data_f32.f32_val) ? 1 : 6;
		break;
	}
	if (debug)
		cout << "fclass.s: " << f[rs1] << " = " << dec << value << endl;

	set_reg(rd, (uint64_t)(1ULL << value));
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fmv_x_w(unsigned int rd, unsigned int rs1)
{
	if (verbose)
		cout << "fmv.x.w: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1 << endl;

	u32_f32_union data_f32;
	data_f32.f32_val = f[rs1];
	set_reg(rd, (int32_t)data_f32.u32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fmv_w_x(unsigned int rd, unsigned int rs1)
{
	if (verbose)
		cout << "fmv.w.x: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1 << endl;

	u32_f32_union data_f32;
	data_f32.u32_val = (uint32_t)(x[rs1]);
	set_freg(rd, data_f32.f32_val);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fcvt_w_s(unsigned int rd, unsigned int rm, unsigned int rs1)
{
	if (verbose)
		cout << "fcvt.w.s: rd = " << dec << rd << ", rs1 = " << dec << rs1 << endl;

	u32_f32_union data_f32;
	data_f32.f32_val = f[rs1];

	int32_t  sign_bit = (int32_t)(data_f32.u32_val >> 31) & 0x01;
	int32_t  exp = ((data_f32.u32_val >> 23) & 0x00ff);
	int32_t di_data = (int32_t)data_f32.f32_val;

	set_frm(rm);
	if (exp == 0xFF && sign_bit != 0)
	{
		di_data = 0;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, di_data);
	}
	else if ((exp - 127) >= 31)
	{
		di_data = sign_bit ? 0x80000000 : 0x7FFFFFFF;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, di_data);
	}
	else
	{
		set_fflags(0);
		set_reg(rd, di_data);
	}

	if (debug)
		cout << "fcvt.w.s: " << data_f32.f32_val << " = " << dec << di_data << endl;

	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fcvt_wu_s(unsigned int rd, unsigned int rm, unsigned int rs1)
{
	if (verbose)
		cout << "fcvt.wu.s: rd = " << dec << rd << ", rs1 = " << dec << rs1 << endl;

	u32_f32_union data_f32;
	data_f32.f32_val = f[rs1];

	int32_t  sign_bit = (int32_t)(data_f32.u32_val >> 31) & 0x01;
	int32_t  exp = ((data_f32.u32_val >> 23) & 0x00ff);
	uint32_t di_data = (uint32_t)data_f32.f32_val;

	set_frm(rm);
	if (((int64_t)data_f32.f32_val) < 0)
	{
		di_data = 0;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, 0);
	}
	else if (exp == 0xFF && sign_bit != 0)
	{
		di_data = 0;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, 0);
	}
	else if ((exp - 127) >= 32)
	{
		di_data = 0xFFFFFFFF;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, di_data);
	}
	else
	{
		set_fflags(0);
		set_reg(rd, di_data);
	}

	if (debug)
		cout << "fcvt.wu.s: " << data_f32.f32_val << " = " << dec << di_data << endl;

	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fcvt_s_w(unsigned int rd, unsigned int rm, unsigned int rs1)
{
	if (verbose)
		cout << "fcvt.s.w: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1 << endl;

	int32_t d_data = (int32_t)(x[rs1]);
	float   f_data = (float)d_data;
	if (debug)
		cout << "fcvt.s.w: " << dec << d_data << " = " << f_data << endl;

	set_frm(rm);
	set_freg(rd, f_data);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fcvt_s_wu(unsigned int rd, unsigned int rm, unsigned int rs1)
{
	if (verbose)
		cout << "fcvt.s.wu: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1 << endl;

	uint32_t di = (uint32_t)(x[rs1]);
	float   df = (float)di;
	if (debug)
		cout << "fcvt.s.wu: " << dec << di << " = " << df << endl;

	set_frm(rm);
	set_freg(rd, df);
	cycle_count += CYCLE_COUNT_OTHER;
}

// RV64F Standard Ext
void processor::instruction_fcvt_l_s(unsigned int rd, unsigned int rm, unsigned int rs1)
{
	if (verbose)
		cout << "fcvt.l.s: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1 << endl;

	u32_f32_union data_f32;
	data_f32.f32_val = f[rs1];

	int32_t  sign_bit = (int32_t)(data_f32.u32_val >> 31) & 0x01;
	int32_t  exp = ((data_f32.u32_val >> 23) & 0x00ff);
	int64_t di_data = (int64_t)data_f32.f32_val;

	set_frm(rm);
	if (exp == 0xFF && sign_bit != 0)
	{
		// NaN
		di_data = 0;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, di_data);
	}
	else if ((exp - 127) > 63)
	{
		// Overflow 
		di_data = sign_bit ? 0x8000000000000000ULL : 0xFFFFFFFFFFFFFFFFULL;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, di_data);
	}
	else
	{
		set_fflags(0);
		set_reg(rd, di_data);
	}

	if (debug)
		cout << "fcvt.l.s: " << data_f32.f32_val << " = " << dec << di_data << endl;

	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fcvt_lu_s(unsigned int rd, unsigned int rm, unsigned int rs1)
{
	if (verbose)
		cout << "fcvt.lu.s: rd = " << dec << rd << ", rs1 = " << dec << rs1 << endl;

	u32_f32_union data_f32;
	data_f32.f32_val = f[rs1];

	int32_t  sign_bit = (int32_t)(data_f32.u32_val >> 31) & 0x01;
	int32_t  exp = ((data_f32.u32_val >> 23) & 0x00ff);
	uint64_t d_val = (uint64_t)data_f32.f32_val;

	set_frm(rm);
	if (((int64_t)data_f32.f32_val) < 0)
	{
		d_val = 0;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, 0);
	}
	else if (exp == 0xFF && sign_bit != 0)
	{// NaN
		d_val = 0;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, d_val);
	}
	else if ((exp - 127) > 64)
	{
		// OF 
		d_val = 0xFFFFFFFFFFFFFFFFULL;
		set_fflags(FCSR_FFLAGS_NV);
		set_reg(rd, d_val);
	}
	else
	{
		set_fflags(0);
		set_reg(rd, d_val);
	}

	if (debug)
		cout << "fcvt.lu.s: " << data_f32.f32_val << " = " << dec << d_val << endl;

	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fcvt_s_l(unsigned int rd, unsigned int rm, unsigned int rs1)
{
	if (verbose)
		cout << "fcvt.s.l: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1 << endl;

	int64_t d_value = (int64_t)(x[rs1]);
	float   f_value = (float)d_value;
	if (debug)
		cout << "fcvt.s.l: " << dec << d_value << " = " << f_value << endl;

	set_frm(rm);
	set_freg(rd, f_value);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_fcvt_s_lu(unsigned int rd, unsigned int rm, unsigned int rs1)
{
	if (verbose)
		cout << "fcvt.s.lu: rd = " << dec << rd
		<< ", rs1 = " << dec << rs1 << endl;

	uint64_t d_value = x[rs1];
	float   f_value = (float)d_value;
	if (debug)
		cout << "fcvt.s.lu: " << dec << d_value << " = " << f_value << endl;

	set_frm(rm);
	set_freg(rd, f_value);
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::read_csr(unsigned int csrNum, bool& illegalCsrNum, uint64_t& csrValue) {
	csrValue = 0;
	illegalCsrNum = false;
	switch (csrNum) {
	case CSR_NUM_FRM:    csrValue = (fcsr & FCSR_FRM_MASK) >> FCSR_FRM_POS; break;
	case CSR_NUM_FFLAGS: csrValue = (fcsr & FCSR_FFLAGS_MASK) >> FCSR_FFLAGS_POS; break;
	case CSR_NUM_FCSR:   csrValue = (fcsr & FCSR_VALID_BITS_MASK); break;
	default: illegalCsrNum = true;
	}
}

void processor::write_csr(unsigned int csr_num, uint64_t new_value, bool& illegalCsrNum) {
	illegalCsrNum = false;
	switch (csr_num) {
	case CSR_NUM_FRM:
		fcsr = (fcsr & ~FCSR_FRM_MASK) | ((new_value & FRM_VALID_BITS_MASK) << FCSR_FRM_POS);
		break;
	case CSR_NUM_FFLAGS:
		fcsr = (fcsr & ~FCSR_FFLAGS_MASK) | ((new_value & FFLAGS_VALID_BITS_MASK) << FCSR_FFLAGS_POS);
		break;
	case CSR_NUM_FCSR:
		fcsr = (new_value & FCSR_VALID_BITS_MASK);
		break;
	default: illegalCsrNum = true;
	}
}

void processor::Zicsr_dispatch(uint32_t instruction) {
	unsigned int rd = (instruction & 0x00000F80) >> 7;
	unsigned int csr_num = (instruction & 0xFFF00000) >> 20;
	unsigned int funct3 = (instruction & 0x00007000) >> 12;
	unsigned int uimm = (instruction & 0x000F8000) >> 15;
	unsigned int rs1 = (instruction & 0x000F8000) >> 15;

	if (stage2) {
		switch (funct3) {
		case 0x2: instruction_csrrs(rd, rs1, csr_num); break;
		case 0x1: instruction_csrrw(rd, rs1, csr_num); break;
		case 0x5: instruction_csrrwi(rd, uimm, csr_num); break;
		default: instruction_illegal();
		}
	}
	else {
		instruction_illegal();
	}

}

void processor::instruction_csrrw(unsigned int rd, unsigned int rs1, unsigned int csr_num) {
	bool illegalCsrNum;
	uint64_t csrValue, newValue;
	if (stage2)
	{
		if (verbose)
			cout << "csrrw: rd = " << dec << rd << ", rs1 = " << dec << rs1 <<
			", csr_num = " << setw(3) << setfill('0') << hex << csr_num << endl;
		cycle_count += CYCLE_COUNT_OTHER;
		read_csr(csr_num, illegalCsrNum, csrValue);
		if (illegalCsrNum)
		{
			instruction_illegal();
		}
		else
		{
			newValue = x[rs1];
			write_csr(csr_num, newValue, illegalCsrNum);
			set_reg(rd, csrValue);
		}
	}
	else {
		instruction_illegal();
	}
}

void processor::instruction_csrrs(unsigned int rd, unsigned int rs1, unsigned int csr_num) {
	bool illegalCsrNum;
	uint64_t csrValue, newValue;
	if (stage2) {
		if (verbose)
			cout << "csrrs: rd = " << dec << rd << ", rs1 = " << dec << rs1 <<
			", csr_num = " << setw(3) << setfill('0') << hex << csr_num << endl;
		cycle_count += CYCLE_COUNT_OTHER;
		read_csr(csr_num, illegalCsrNum, csrValue);
		if (illegalCsrNum)
		{
			instruction_illegal();
		}
		else
		{
			if (rs1 != 0)
			{
				newValue = csrValue | x[rs1];
				write_csr(csr_num, newValue, illegalCsrNum);
				set_reg(rd, csrValue);
			}
			else {
				set_reg(rd, csrValue);
			}
		}
	}
	else {
		instruction_illegal();
	}
}

void processor::instruction_csrrwi(unsigned int rd, unsigned int uimm, unsigned int csr_num) {
	bool illegalCsrNum;
	uint64_t csrValue, newValue;
	if (stage2) {
		if (verbose)
			cout << "csrrwi: rd = " << dec << rd <<
			", uimm = " << setw(2) << setfill('0') << hex << uimm <<
			", csr_num = " << setw(3) << setfill('0') << hex << csr_num << endl;
		cycle_count += CYCLE_COUNT_OTHER;
		read_csr(csr_num, illegalCsrNum, csrValue);
		if (illegalCsrNum)
		{
			instruction_illegal();
		}
		else
		{
			newValue = (uint64_t)uimm;
			write_csr(csr_num, newValue, illegalCsrNum);
			set_reg(rd, csrValue);
		}
	}
	else {
		instruction_illegal();
	}
}




