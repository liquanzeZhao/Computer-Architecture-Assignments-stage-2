/* ****************************************************************
RISC-V Instruction Set Simulator
Computer Architecture, Semester 1, 2025

Class members for processor

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

/*
lee@DESKTOP-VAIUJCV:~/cav2/stage-1B-liquanzeZhao/tests/harness_tests$ ../../rv64sim/rv64sim < harness_test_lui.cmd | diff --strip-trailing-cr - expected/harness_test_lui.log
2c2
< fffffffffff53000
---
> ffffffffa5a53000

*/
void processor::instruction_lui(unsigned int rd, int64_t immed_U) {
	if (verbose)
		cout << "lui: rd = " << dec << rd <<
		", immed_U = " << setw(16) << setfill('0') << hex << immed_U << endl;
	set_reg(rd, immed_U);
	cycle_count += CYCLE_COUNT_OTHER;
}


// Constructor
processor::processor(memory* main_memory, bool verbose, bool stage2) {
	this->main_memory = main_memory;
	this->verbose = verbose;
	this->stage2 = stage2;

	pc = 0x0000000000000000ULL;  // Initialise PC
	for (unsigned int i = 0; i < 32; i++)  // Initialise registers
		x[i] = 0;


	// TODO

	cycle_count = 0;
	instr_count = 0;

	// TODO
	stop_on_error = false; 
}

// Display PC value
void processor::show_pc() {
	cout << setw(16) << setfill('0') << hex << pc << endl;
}

// Set PC to new value
void processor::set_pc(uint64_t new_pc) {
	pc = new_pc;
}

// Display register value
void processor::show_reg(unsigned int reg_num) {
	cout << setw(16) << setfill('0') << hex << x[reg_num] << endl;
}

// Set register to new value
void processor::set_reg(unsigned int reg_num, uint64_t new_value) {
	if (reg_num != 0)
		x[reg_num] = new_value;
}














// Execute a number of instructions
//重点
void processor::execute(unsigned int num, bool breakpoint_check) {
	stop_on_error = false;
	for (unsigned int i = 0; i < num; i++) {
		if (breakpoint_check && breakpoint_is_set && pc == breakpoint_address) {
			cout << "Breakpoint reached at " << setw(16) << setfill('0') << hex << pc << endl;
			break;
		}
		if (pc % 4 != 0) {
			cout << "Error: misaligned pc" << endl;
			break;
		}

		uint64_t full_data = main_memory->read_doubleword(pc & ~0x7ULL);
		unsigned int offset = pc % 8;
		instruction = (uint32_t)((full_data >> (offset * 8)) & 0xFFFFFFFF);

		uint64_t old_pc = pc;
		stop_on_error = false;

		if (verbose)
			cout << "Fetch: pc = " << setw(16) << setfill('0') << hex << pc << ", ir = " << setw(8) << setfill('0') << hex << instruction << endl;

		decode_and_dispatch();

		if (stop_on_error)
			break;

		if (pc == old_pc) {
			pc += 4;
		}


		instr_count++;
	}
}









// Clear breakpoint
void processor::clear_breakpoint() {
	breakpoint_is_set = false;
}

// Set breakpoint at an address
void processor::set_breakpoint(uint64_t address) {
	if (verbose)
		cout << "Breakpoint set at " << setw(16) << setfill('0') << hex << address << endl;
	breakpoint_address = address;
	breakpoint_is_set = true;
}

// Show privilege level
// Empty implementation for stage 1, required for stage 2
void processor::show_prv() {
    if (privilege_level==PRIVILEGE_LEVEL_USER)
    {
      cout<<"user"<<endl;
    }
    else
    {
      cout<<"machine"<<endl;
    }
}

// Set privilege level
// Empty implementation for stage 1, required for stage 2
void processor::set_prv(unsigned int prv_num) {
  if ( prv_num==0 || prv_num==3 )
  {
      privilege_level = (Privilege_level)prv_num;
  }   
  else if (debug)
  {
    cout<<"Only support User or Machine Mode"<<endl;
  }
}

// Display CSR value
// Empty implementation for stage 1, required for stage 2
void processor::show_csr(unsigned int csr_num) {
    bool illegalNum;
    uint64_t csr_value;
    if (!stage2) 
    {
      cout << "not for stage 1" << endl;
    } 
    else 
    {
        read_csr(csr_num, illegalNum, csr_value);
        if (illegalNum)
        {
            cout << "Illegal CSR " << endl;
        } 
        else 
        {
            cout << setw(16) << setfill('0') << hex << csr_value << endl;
        }
    }
}

// Set CSR to new value
// Empty implementation for stage 1, required for stage 2
void processor::set_csr(unsigned int csr_num, uint64_t new_value) {
    bool illegalNum;
    if (!stage2)
    {
        cout << "not for stage 1" << endl;
    } 
    else
    {
        write_csr(csr_num, new_value, illegalNum);
        if (illegalNum)
        {
            cout << "Illegal CSR " << endl;
        }
    }
}

uint64_t processor::get_instruction_count() {
	return instr_count;
}


// Used for Postgraduate assignment. Undergraduate assignment can return 0.
uint64_t processor::get_cycle_count() {
	return cycle_count;
}

// Called when illegal instruction encountered
void processor::instruction_illegal() {
	if (stage2) {
		// TODO
	}
	else {
		cout << "Error: illegal instruction" << endl;
	}
}















// decode instruction and dispatch
void processor::decode_and_dispatch() {
	uint32_t opcode = instruction & 0x0000007F; // Extract 7-bit opcode
	uint32_t funct3 = (instruction >> 12) & 0x00000007; // Extract 3-bit funct3
	unsigned int rd = (instruction >> 7) & 0x0000001F;  // Extract 5-bit rd
	unsigned int rs1 = (instruction >> 15) & 0x0000001F; // Extract 5-bit rs1

	unsigned int rs2 = (instruction >> 20) & 0x0000001F; // Extract 5-bit rs2
	uint32_t funct7 = (instruction >> 25) & 0x0000007F; // Extract 7-bit funct7




	// Immediate values
	// I-type immediate

	int64_t immed_I = (int64_t)((instruction >> 20) & 0xFFF); // I-type immediate
	if (immed_I & 0x800) immed_I |= 0xFFFFFFFFFFFFF000; // Sign-extend 12 bits


	/*提取高 20 位：0xFFFFF000
	转换为 int32_t：0xFFFFF000 被视为负数，符号扩展为 0xFFFFFFFFFFFFF000。
	转换为 int64_t：保持符号扩展结果，最终值为 0xFFFFFFFFFFFFF000。*/
	int64_t immed_U = (instruction & 0xFFFFF000); // 提取高 20 位并保留低 12 位为 0
	immed_U = (int64_t)((int32_t)immed_U); // 符号扩展到 64 位




	// S-type immediate

	int32_t immed_S = ((instruction >> 7) & 0x1F) | ((instruction >> 25) << 5); // S-type immediate
	if (immed_S & 0x800) immed_S |= 0xFFFFF000; // Sign-extend 12 bits
	int64_t immed_S_ext = (int64_t)immed_S;




	// B-type immediate
	// BNE 指令的立即数格式为 imm[12|10:5|4:1|11]
	// 其中 imm[12] 是第 31 位，imm[10:5] 是第 25 位到第 30 位，imm[4:1] 是第 8 位到第 11 位，imm[11] 是第 7 位

	int32_t immed_B = (((instruction >> 31) & 0x1) << 12) | // 第 12 位（符号位）
		(((instruction >> 7) & 0x1) << 11) | // 第 11 位
		(((instruction >> 25) & 0x3F) << 5) | // 第 5-10 位
		(((instruction >> 8) & 0xF) << 1);    // 第 1-4 位
	if (immed_B & 0x1000) // 检查符号位（第 12 位）
		immed_B |= 0xFFFFE000; // 符号扩展到 32 位




	// J-type immediate
	// JAL 指令的立即数格式为 imm[20|10:1|11|19:12|20]
	// 其中 imm[20] 是第 31 位，imm[10:1] 是第 21 位到第 30 位，imm[11] 是第 20 位，imm[19:12] 是第 12 位到第 19 位

	int32_t immed_J = (((instruction >> 21) & 0x3FF) << 1)     // bits [30:21] -> imm[10:1]
		| (((instruction >> 20) & 0x1) << 11)      // bit 20 -> imm[11]
		| (((instruction >> 12) & 0xFF) << 12)     // bits [19:12] -> imm[19:12]
		| (((instruction >> 31) & 0x1) << 20);     // bit 31 -> imm[20]
	if (immed_J & 0x100000) immed_J |= 0xFFE00000;  // sign extend 21 bits -> 32 bits
	int64_t immed_J_ext = (int64_t)immed_J;






	// Decode and execute instructions
	switch (opcode) {
	case 0x37: // LUI
		instruction_lui(rd, immed_U);
		break;
	case 0x17: // AUIPC
		instruction_auipc(rd, immed_U);
		break;
	case 0x6F: // JAL
		instruction_jal(rd, immed_J_ext);
		break;
	case 0x67: // JALR
		switch (funct3) {
		case 0x0:
			instruction_jalr(rd, rs1, immed_I);
			break;
		default:
			instruction_illegal();
		}
		break;
	case 0x63: // Branch instructions
		switch (funct3) {
		case 0x0: instruction_beq(rs1, rs2, immed_B); break;
		case 0x1: instruction_bne(rs1, rs2, immed_B); break;
		case 0x4: instruction_blt(rs1, rs2, immed_B); break;
		case 0x5: instruction_bge(rs1, rs2, immed_B); break;
		case 0x6: instruction_bltu(rs1, rs2, immed_B); break;
		case 0x7: instruction_bgeu(rs1, rs2, immed_B); break;
		default:  instruction_illegal();
		}
		break;
	case 0x03: // LOAD
		switch (funct3) {
		case 0x0: instruction_lb(rd, rs1, immed_I); break;
		case 0x1: instruction_lh(rd, rs1, immed_I); break;
		case 0x2: instruction_lw(rd, rs1, immed_I); break;
		case 0x3: instruction_ld(rd, rs1, immed_I); break;
		case 0x4: instruction_lbu(rd, rs1, immed_I); break;
		case 0x5: instruction_lhu(rd, rs1, immed_I); break;
		case 0x6: instruction_lwu(rd, rs1, immed_I); break;
		default:  instruction_illegal();
		}
		break;
	case 0x23: // STORE
		switch (funct3) {
		case 0x0: instruction_sb(rs1, rs2, immed_S_ext); break;
		case 0x1: instruction_sh(rs1, rs2, immed_S_ext); break;
		case 0x2: instruction_sw(rs1, rs2, immed_S_ext); break;
		case 0x3: instruction_sd(rs1, rs2, immed_S_ext); break;
		default:  instruction_illegal();
		}
		break;
	case 0x13: // I-type ALU
		switch (funct3) {
		case 0x0: instruction_addi(rd, rs1, immed_I); break;
		case 0x1: instruction_slli(rd, rs1, immed_I); break;
		case 0x2: instruction_slti(rd, rs1, immed_I); break;
		case 0x3: instruction_sltiu(rd, rs1, immed_I); break;
		case 0x4: instruction_xori(rd, rs1, immed_I); break;
		case 0x5:
			if ((immed_I & 0x400) == 0) // SRLI
				instruction_srli(rd, rs1, immed_I);
			else // SRAI
				instruction_srai(rd, rs1, immed_I);
			break;
		case 0x6: instruction_ori(rd, rs1, immed_I); break;
		case 0x7: instruction_andi(rd, rs1, immed_I); break;
		default:  instruction_illegal();
		}
		break;
	case 0x1B: // I-type ALU (32-bit)
		switch (funct3) {
		case 0x0: instruction_addiw(rd, rs1, immed_I); break;
		case 0x1: instruction_slliw(rd, rs1, immed_I); break;
		case 0x5:
			if ((immed_I & 0x400) == 0) // SRLIW
				instruction_srliw(rd, rs1, immed_I);
			else // SRAIW
				instruction_sraiw(rd, rs1, immed_I);
			break;
		default:  instruction_illegal();
		}
		break;
	case 0x33: // R-type ALU
		switch (funct3) {
		case 0x0:
			if (funct7 == 0x00) instruction_add(rd, rs1, rs2);
			else if (funct7 == 0x20) instruction_sub(rd, rs1, rs2);
			else instruction_illegal();
			break;
		case 0x1: instruction_sll(rd, rs1, rs2); break;
		case 0x2: instruction_slt(rd, rs1, rs2); break;
		case 0x3: instruction_sltu(rd, rs1, rs2); break;
		case 0x4: instruction_xor(rd, rs1, rs2); break;
		case 0x5:
			if (funct7 == 0x00) instruction_srl(rd, rs1, rs2);
			else if (funct7 == 0x20) instruction_sra(rd, rs1, rs2);
			else instruction_illegal();
			break;
		case 0x6: instruction_or(rd, rs1, rs2); break;
		case 0x7: instruction_and(rd, rs1, rs2); break;
		default:  instruction_illegal();
		}
		break;
	case 0x3B: // R-type ALU (32-bit)
		switch (funct3) {
		case 0x0:
			if (funct7 == 0x00) instruction_addw(rd, rs1, rs2);
			else if (funct7 == 0x20) instruction_subw(rd, rs1, rs2);
			else instruction_illegal();
			break;
		case 0x1: instruction_sllw(rd, rs1, rs2); break;
		case 0x5:
			if (funct7 == 0x00) instruction_srlw(rd, rs1, rs2);
			else if (funct7 == 0x20) instruction_sraw(rd, rs1, rs2);
			else instruction_illegal();
			break;
		default: instruction_illegal();
		}
		break;
	case 0x73: // SYSTEM
	    if (stage2)
	    {
            Zicsr_dispatch(instruction); 
	    }
        else
        {
    		switch (funct3) {
    		case 0x0:
    			if ((instruction >> 20) == 0) instruction_ecall();
    			else if ((instruction >> 20) == 1) instruction_ebreak();
    			//else instruction_illegal();
    			break;
    		default:
    			//instruction_illegal();
                ;
    		}	            
        }

		break;
	case 0x0F: // FENCE
		instruction_fence();
		break;
	default:
		//stage2
		unsigned int rs3 = (instruction & 0xf8000000) >> 27;
		switch (opcode)
		{
		case 0x07: // flw 
			instruction_flw(rd, rs1, immed_I);
			break;
		case 0x27: // fsw 
			instruction_fsw(rs1, rs2, immed_S);
			break;
		case 0x43: // fmadd.s
			instruction_fmadd_s(rd, funct3, rs1, rs2, rs3);
			break;
		case 0x47: // fmsub.s
			instruction_fmsub_s(rd, funct3, rs1, rs2, rs3);
			break;
		case 0x4b: // fnmadd.s
			instruction_fnmadd_s(rd, funct3, rs1, rs2, rs3);
			break;
		case 0x4f: // fnmsub.s
			instruction_fnmsub_s(rd, funct3, rs1, rs2, rs3);
			break;
		case 0x53:
			instruction_fop(rd, funct3, rs1, rs2, funct7);
			break;
		default:
			instruction_illegal();
			break;
		}
	}
}

void processor::instruction_lb(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "lb: rd = " << dec << rd << ", rs1 = " << dec << rs1 <<
		", immed_I = " << setw(16) << setfill('0') << hex << immed_I << endl;

	// 计算有效地址
	uint64_t address = x[rs1] + immed_I;
	// 对齐到 8 字节边界并读取双字（64 位）
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t data = main_memory->read_doubleword(aligned_addr);

	// 提取目标字节并符号扩展
	int8_t byte;
	switch (address & 0x7) {
	case 0: byte = (int8_t)(data & 0xFF); break;
	case 1: byte = (int8_t)((data >> 8) & 0xFF); break;
	case 2: byte = (int8_t)((data >> 16) & 0xFF); break;
	case 3: byte = (int8_t)((data >> 24) & 0xFF); break;
	case 4: byte = (int8_t)((data >> 32) & 0xFF); break;
	case 5: byte = (int8_t)((data >> 40) & 0xFF); break;
	case 6: byte = (int8_t)((data >> 48) & 0xFF); break;
	case 7: byte = (int8_t)((data >> 56) & 0xFF); break;
	}

	set_reg(rd, (int64_t)byte);
	cycle_count += CYCLE_COUNT_LOAD;
}
void processor::instruction_addi(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "addi: rd = " << dec << rd << ", rs1 = " << dec << rs1 <<
		", immed_I = " << setw(16) << setfill('0') << hex << immed_I << endl;
	set_reg(rd, x[rs1] + immed_I);
	cycle_count += CYCLE_COUNT_OTHER;
}


void processor::instruction_addiw(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "addiw: rd = " << dec << rd << ", rs1 = " << dec << rs1 <<
		", immed_I = " << setw(16) << setfill('0') << hex << immed_I << endl;
	set_reg(rd, (int32_t)(x[rs1] + immed_I));
	cycle_count += CYCLE_COUNT_OTHER;
}



void processor::instruction_slli(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "slli: rd = " << dec << rd << ", rs1 = " << dec << rs1 <<
		", immed_I = " << setw(16) << setfill('0') << hex << immed_I << endl;
	set_reg(rd, x[rs1] << (immed_I & 0x3F));
	cycle_count += CYCLE_COUNT_OTHER;
}




void processor::instruction_bne(unsigned int rs1, unsigned int rs2, int64_t immed_B) {
	if (verbose) {
		cout << "bne: rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 <<
			", immed_B = " << setw(16) << setfill('0') << hex << immed_B << endl;
		cout << "PC before BNE: " << setw(16) << setfill('0') << hex << pc << endl;
	}

	int32_t d1 = (int32_t)x[rs1];
	int32_t d2 = (int32_t)x[rs2];

	if ( d1 != d2 ) {
		pc += immed_B; // 更新 PC
		if (verbose) {
			cout << "Branch taken. New PC: " << setw(16) << setfill('0') << hex << pc << endl;
		}
	}
	else {
		if (verbose) {
			cout << "Branch not taken. PC remains: " << setw(16) << setfill('0') << hex << pc << endl;
		}
	}
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_jalr(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "jalr: rd = " << dec << rd << ", rs1 = " << dec << rs1 <<
		", immed_I = " << setw(16) << setfill('0') << hex << immed_I << endl;

	// 符号扩展立即数
	if (immed_I & 0x800) // 检查符号位（第 11 位）
		immed_I |= 0xFFFFFFFFFFFFF000; // 符号扩展到 64 位

	// 计算目标地址并清除最低有效位
	uint64_t target_address = (x[rs1] + immed_I) & ~0x1ULL;

	// 保存返回地址（当前 PC + 4）
	set_reg(rd, pc + 4);

	// 更新 PC 为目标地址
	pc = target_address;

	// 增加周期计数
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_addi_align(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "addi_align: rd = " << dec << rd << ", rs1 = " << dec << rs1 <<
		", immed_I = " << setw(16) << setfill('0') << hex << immed_I << endl;
	set_reg(rd, x[rs1] + (immed_I & 0xFFFFFFFFFFFFFFFCULL)); // Align to 4-byte boundary
	cycle_count += CYCLE_COUNT_OTHER;
}




//50种指令
// 算术指令
void processor::instruction_add(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "add: rd = " << dec << rd << ", rs1 = " << dec << rs1 << ", rs2 = " << dec << rs2 << endl;

	// 执行加法操作
	set_reg(rd, x[rs1] + x[rs2]);

	// 增加周期计数
	cycle_count += CYCLE_COUNT_OTHER;
}
// 算术指令
void processor::instruction_sub(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "sub: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, x[rs1] - x[rs2]);
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_addw(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "addw: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, (int32_t)(x[rs1] + x[rs2]));
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_subw(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "subw: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, (int32_t)(x[rs1] - x[rs2]));
	cycle_count += CYCLE_COUNT_OTHER;
}

// 逻辑指令
void processor::instruction_and(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "and: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, x[rs1] & x[rs2]);
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_andi(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "andi: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, x[rs1] & immed_I);
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_or(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "or: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, x[rs1] | x[rs2]);
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_ori(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "ori: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, x[rs1] | immed_I);
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_xor(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "xor: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, x[rs1] ^ x[rs2]);
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_xori(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "xori: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, x[rs1] ^ immed_I);
	cycle_count += CYCLE_COUNT_OTHER;
}

// 比较指令
void processor::instruction_slt(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "slt: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, (int64_t)x[rs1] < (int64_t)x[rs2] ? 1 : 0);
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_slti(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "slti: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, (int64_t)x[rs1] < immed_I ? 1 : 0);
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_sltu(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "sltu: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, x[rs1] < x[rs2] ? 1 : 0);
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_sltiu(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "sltiu: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, x[rs1] < (uint64_t)immed_I ? 1 : 0);
	cycle_count += CYCLE_COUNT_OTHER;
}

// 移位指令
void processor::instruction_sll(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "sll: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, x[rs1] << (x[rs2] & 0x3F));
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_sllw(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "sllw: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, (int32_t)(x[rs1] << (x[rs2] & 0x1F)));
	cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_slliw(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "slliw: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, (int32_t)(x[rs1] << (immed_I & 0x1F)));
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_sra(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "sra: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, (int64_t)x[rs1] >> (x[rs2] & 0x3F));
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_sraw(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "sraw: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, (int32_t)((int32_t)x[rs1] >> (x[rs2] & 0x1F)));
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_srai(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "srai: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, (int64_t)x[rs1] >> (immed_I & 0x3F));
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_sraiw(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "sraiw: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, (int32_t)((int32_t)x[rs1] >> (immed_I & 0x1F)));
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_srl(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "srl: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, x[rs1] >> (x[rs2] & 0x3F));
	cycle_count += CYCLE_COUNT_OTHER;														
}
void processor::instruction_srlw(unsigned int rd, unsigned int rs1, unsigned int rs2) {
	if (verbose)
		cout << "srlw: rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << endl;
	set_reg(rd, (uint32_t)x[rs1] >> (x[rs2] & 0x1F));
	cycle_count += CYCLE_COUNT_OTHER;
}


/*Files instruction_test_srli.log and expected/instruction_test_srli.log are identical
0000000000000000                                              | 0000000000000001
Instructions executed: 49                                     | Instructions executed: 85
0000000000000000                                              | 0000000000000001
Instructions executed: 54                                     | Instructions executed: 93*/

void processor::instruction_srli(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "srli: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, x[rs1] >> (immed_I & 0x3F));
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_srliw(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "srliw: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	set_reg(rd, (uint32_t)x[rs1] >> (immed_I & 0x1F));
	cycle_count += CYCLE_COUNT_OTHER;
}

// 跳转与分支指令
void processor::instruction_jal(unsigned int rd, int64_t immed_J) {
	if (verbose)
		cout << "jal: rd = " << rd << ", immed_J = " << immed_J << endl;
	set_reg(rd, pc + 4);
	pc += immed_J;
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_beq(unsigned int rs1, unsigned int rs2, int64_t immed_B) {
	if (verbose)
		cout << "beq: rs1 = " << rs1 << ", rs2 = " << rs2 << ", immed_B = " << immed_B << endl;
	if (x[rs1] == x[rs2])
		pc += immed_B;
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_blt(unsigned int rs1, unsigned int rs2, int64_t immed_B) {
	if (verbose)
		cout << "blt: rs1 = " << rs1 << ", rs2 = " << rs2 << ", immed_B = " << immed_B << endl;
	if ((int64_t)x[rs1] < (int64_t)x[rs2])
		pc += immed_B;
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_bge(unsigned int rs1, unsigned int rs2, int64_t immed_B) {
	if (verbose)
		cout << "bge: rs1 = " << rs1 << ", rs2 = " << rs2 << ", immed_B = " << immed_B << endl;
	if ((int64_t)x[rs1] >= (int64_t)x[rs2])
		pc += immed_B;
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_bltu(unsigned int rs1, unsigned int rs2, int64_t immed_B) {
	if (verbose)
		cout << "bltu: rs1 = " << rs1 << ", rs2 = " << rs2 << ", immed_B = " << immed_B << endl;
	if (x[rs1] < x[rs2])
		pc += immed_B;
	cycle_count += CYCLE_COUNT_OTHER;
}
void processor::instruction_bgeu(unsigned int rs1, unsigned int rs2, int64_t immed_B) {
	if (verbose)
		cout << "bgeu: rs1 = " << rs1 << ", rs2 = " << rs2 << ", immed_B = " << immed_B << endl;
	if (x[rs1] >= x[rs2])
		pc += immed_B;
	cycle_count += CYCLE_COUNT_OTHER;
}

// 加载指令
void processor::instruction_lbu(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "lbu: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	uint64_t address = x[rs1] + immed_I;
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t data = main_memory->read_doubleword(aligned_addr);
	uint8_t byte = (data >> ((address & 0x7) * 8)) & 0xFF;
	set_reg(rd, byte);
	cycle_count += CYCLE_COUNT_LOAD;
}
void processor::instruction_lh(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "lh: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	uint64_t address = x[rs1] + immed_I;
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t data = main_memory->read_doubleword(aligned_addr);
	int16_t half = (data >> ((address & 0x7) * 8)) & 0xFFFF;
	set_reg(rd, half);
	cycle_count += CYCLE_COUNT_LOAD;
}
void processor::instruction_lhu(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "lhu: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	uint64_t address = x[rs1] + immed_I;
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t data = main_memory->read_doubleword(aligned_addr);
	uint16_t half = (data >> ((address & 0x7) * 8)) & 0xFFFF;
	set_reg(rd, half);
	cycle_count += CYCLE_COUNT_LOAD;
}
void processor::instruction_lw(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "lw: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	uint64_t address = x[rs1] + immed_I;
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t data = main_memory->read_doubleword(aligned_addr);
	int32_t word = (data >> ((address & 0x7) * 8)) & 0xFFFFFFFF;
	set_reg(rd, word);
	cycle_count += CYCLE_COUNT_LOAD;
}
void processor::instruction_lwu(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "lwu: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	uint64_t address = x[rs1] + immed_I;
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t data = main_memory->read_doubleword(aligned_addr);
	uint32_t word = (data >> ((address & 0x7) * 8)) & 0xFFFFFFFF;
	set_reg(rd, word);
	cycle_count += CYCLE_COUNT_LOAD;
}
void processor::instruction_ld(unsigned int rd, unsigned int rs1, int64_t immed_I) {
	if (verbose)
		cout << "ld: rd = " << rd << ", rs1 = " << rs1 << ", immed_I = " << immed_I << endl;
	uint64_t address = x[rs1] + immed_I;
	if (address % 8 != 0) {
		cout << "Error: misaligned address for ld" << endl;
		//stop_on_error = true; // 关键：设置异常标志	for instruction_sd test
		return;
	}
	uint64_t data = main_memory->read_doubleword(address);
	set_reg(rd, data);
	cycle_count += CYCLE_COUNT_LOAD;
}

// 存储指令
/*Files instruction_test_sb.log and expected/instruction_test_sb.log are identical
0000000000000000                                              | Breakpoint reached at 0000000000000000
Instructions executed: 74                                     | 0000000000000001
> Instructions executed: 86
*/
void processor::instruction_sb(unsigned int rs1, unsigned int rs2, int64_t immed_S) {
	if (verbose)
		cout << "sb: rs1 = " << rs1 << ", rs2 = " << rs2 << ", immed_S = " << immed_S << endl;
	uint64_t address = x[rs1] + immed_S;
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t data = main_memory->read_doubleword(aligned_addr);
	uint8_t value = x[rs2] & 0xFF;
	data &= ~(0xFFULL << ((address & 0x7) * 8));
	data |= ((uint64_t)value << ((address & 0x7) * 8));
	uint64_t mask = 0xFFULL << ((address & 0x7) * 8);
	main_memory->write_doubleword(aligned_addr, data, mask);
	cycle_count += CYCLE_COUNT_STORE;
}
void processor::instruction_sh(unsigned int rs1, unsigned int rs2, int64_t immed_S) {
	if (verbose)
		cout << "sh: rs1 = " << rs1 << ", rs2 = " << rs2 << ", immed_S = " << immed_S << endl;
	uint64_t address = x[rs1] + immed_S;
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t data = main_memory->read_doubleword(aligned_addr);
	uint16_t value = x[rs2] & 0xFFFF;
	data &= ~(0xFFFFULL << ((address & 0x7) * 8));
	data |= ((uint64_t)value << ((address & 0x7) * 8));
	uint64_t mask = 0xFFFFULL << ((address & 0x7) * 8);
	main_memory->write_doubleword(aligned_addr, data, mask);
	cycle_count += CYCLE_COUNT_STORE;
}
void processor::instruction_sw(unsigned int rs1, unsigned int rs2, int64_t immed_S) {
	if (verbose)
		cout << "sw: rs1 = " << rs1 << ", rs2 = " << rs2 << ", immed_S = " << immed_S << endl;
	uint64_t address = x[rs1] + immed_S;
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t data = main_memory->read_doubleword(aligned_addr);
	uint32_t value = x[rs2] & 0xFFFFFFFF;
	data &= ~(0xFFFFFFFFULL << ((address & 0x7) * 8));
	data |= ((uint64_t)value << ((address & 0x7) * 8));
	uint64_t mask = 0xFFFFFFFFULL << ((address & 0x7) * 8);
	main_memory->write_doubleword(aligned_addr, data, mask);
	cycle_count += CYCLE_COUNT_STORE;
}
void processor::instruction_sd(unsigned int rs1, unsigned int rs2, int64_t immed_S) {
	if (verbose)
		cout << "sd: rs1 = " << rs1 << ", rs2 = " << rs2 << ", immed_S = " << immed_S << endl;
	uint64_t address = x[rs1] + immed_S;
	uint64_t aligned_addr = address & ~0x7ULL;
	uint64_t mask = 0xFFFFFFFFFFFFFFFFULL;
	main_memory->write_doubleword(aligned_addr, x[rs2], mask);
	cycle_count += CYCLE_COUNT_STORE;
}


void processor::instruction_auipc(unsigned int rd, int64_t immed_U) {
	if (verbose)
		cout << "auipc: rd = " << rd << ", immed_U = " << immed_U << endl;
	set_reg(rd, pc + immed_U);
	cycle_count += CYCLE_COUNT_OTHER;
}


void processor::instruction_ecall() {
	cout << "ecall: not implemented" << endl;
}
void processor::instruction_ebreak() {
	cout << "ebreak: not implemented" << endl;
}
void processor::instruction_fence() {
	if (verbose)
		cout << "fence" << endl;
	// fence 指令通常不做实际操作
	cycle_count += CYCLE_COUNT_OTHER;
}




