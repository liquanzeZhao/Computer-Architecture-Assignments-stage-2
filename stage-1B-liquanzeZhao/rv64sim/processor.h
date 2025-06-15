#ifndef PROCESSOR_H
#define PROCESSOR_H

/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2025

   Class for processor

**************************************************************** */

#include "memory.h"

using namespace std;

class processor {

    private:
	// TODO: Add private members here
	
	 bool stop_on_error; // 新增：异常终止标志

        // constants
        static const int CYCLE_COUNT_BRANCH_TAKEN = 2;
	static const int CYCLE_COUNT_BRANCH_UNTAKEN = 1;
	static const int CYCLE_COUNT_BRANCH_UNCONDITIONAL = 2;
  	static const int CYCLE_COUNT_LOAD = 3;
  	static const int CYCLE_COUNT_LOAD_WITH_EXCEPTION = 1;
  	static const int CYCLE_COUNT_STORE = 2;
  	static const int CYCLE_COUNT_STORE_WITH_EXCEPTION = 1;
  	static const int CYCLE_COUNT_OTHER = 1;
  	static const int CYCLE_COUNT_TRAP = 2;

  	enum Privilege_level {PRIVILEGE_LEVEL_USER       = 0,
       	                      PRIVILEGE_LEVEL_SUPERVISOR = 1,
                              PRIVILEGE_LEVEL_RESERVED   = 2,
                              PRIVILEGE_LEVEL_MACHINE    = 3};
    Privilege_level privilege_level;
	uint64_t pc;
	uint64_t next_pc;
	uint32_t instruction;
	uint64_t x[32];
        uint64_t cycle_count;
	uint64_t instr_count;
  
	memory* main_memory;
        bool verbose;
        bool stage2;
        bool breakpoint_is_set;
        uint64_t breakpoint_address;

	void instruction_illegal();
  	void decode_and_dispatch ();

  	// instructions
	  void instruction_lui   (unsigned int rd, int64_t immed_U);
	  void instruction_lb    (unsigned int rd, unsigned int rs1, int64_t immed_I);
	  void instruction_addi  (unsigned int rd, unsigned int rs1, int64_t immed_I);
	  void instruction_addiw (unsigned int rd, unsigned int rs1, int64_t immed_I);
	  void instruction_slli  (unsigned int rd, unsigned int rs1, int64_t immed_I);
	  void instruction_bne   (unsigned int rs1, unsigned int rs2, int64_t immed_B);
	  void instruction_jalr  (unsigned int rd, unsigned int rs1, int64_t immed_I);
	  void instruction_addi_align (unsigned int rd, unsigned int rs1, int64_t immed_I);
	  void instruction_add(unsigned int rd, unsigned int rs1, unsigned int rs2);
	  void instruction_sub(unsigned int rd, unsigned int rs1, unsigned int rs2); 
      void instruction_addw(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_subw(unsigned int rd, unsigned int rs1, unsigned int rs2);

      // 逻辑指令
      void instruction_and(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_andi(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_or(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_ori(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_xor(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_xori(unsigned int rd, unsigned int rs1, int64_t immed_I);

      // 比较指令
      void instruction_slt(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_slti(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_sltu(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_sltiu(unsigned int rd, unsigned int rs1, int64_t immed_I);

      // 移位指令
      void instruction_sll(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_sllw(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_slliw(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_sra(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_sraw(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_srai(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_sraiw(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_srl(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_srlw(unsigned int rd, unsigned int rs1, unsigned int rs2);
      void instruction_srli(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_srliw(unsigned int rd, unsigned int rs1, int64_t immed_I);

      // 跳转与分支指令
      void instruction_jal(unsigned int rd, int64_t immed_J);
      void instruction_beq(unsigned int rs1, unsigned int rs2, int64_t immed_B);
      void instruction_blt(unsigned int rs1, unsigned int rs2, int64_t immed_B);
      void instruction_bge(unsigned int rs1, unsigned int rs2, int64_t immed_B);
      void instruction_bltu(unsigned int rs1, unsigned int rs2, int64_t immed_B);
      void instruction_bgeu(unsigned int rs1, unsigned int rs2, int64_t immed_B);

      // 加载指令
      void instruction_lbu(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_lh(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_lhu(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_lw(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_lwu(unsigned int rd, unsigned int rs1, int64_t immed_I);
      void instruction_ld(unsigned int rd, unsigned int rs1, int64_t immed_I);

      // 存储指令
      void instruction_sb(unsigned int rs1, unsigned int rs2, int64_t immed_S);
      void instruction_sh(unsigned int rs1, unsigned int rs2, int64_t immed_S);
      void instruction_sw(unsigned int rs1, unsigned int rs2, int64_t immed_S);
      void instruction_sd(unsigned int rs1, unsigned int rs2, int64_t immed_S);
                                         
      // 上下文/立即数指令
      void instruction_auipc(unsigned int rd, int64_t immed_U);

      // 特殊/系统指令
      void instruction_ecall();
      void instruction_ebreak();
      void instruction_fence();

    public:

  	// Constructor
  	processor(memory* main_memory, bool verbose, bool stage2);

  	// Display PC value
  	void show_pc();

  	// Set PC to new value
  	void set_pc(uint64_t new_pc);

  	// Display register value
  	void show_reg(unsigned int reg_num);

  	// Set register to new value
  	void set_reg(unsigned int reg_num, uint64_t new_value);

  	// Execute a number of instructions
  	void execute(unsigned int num, bool breakpoint_check);

  	// Clear breakpoint
 	void clear_breakpoint();

  	// Set breakpoint at an address
  	void set_breakpoint(uint64_t address);

  	// Show privilege level
  	// Empty implementation for stage 1, required for stage 2
  	void show_prv();

  	// Set privilege level
  	// Empty implementation for stage 1, required for stage 2
  	void set_prv(unsigned int prv_num);

  	// Display CSR value
  	// Empty implementation for stage 1, required for stage 2
  	void show_csr(unsigned int csr_num);

  	// Set CSR to new value
  	// Empty implementation for stage 1, required for stage 2
  	void set_csr(unsigned int csr_num, uint64_t new_value);

  	uint64_t get_instruction_count();

  	// Used for Postgraduate assignment. Undergraduate assignment can return 0.
  	uint64_t get_cycle_count();


    //stage2
 public:
     float    f[32];
     uint32_t fcsr;
     bool debug = false;

     // Stage 2 constants & variables
     static const unsigned int CSR_NUM_FFLAGS = 0x001;
     static const unsigned int CSR_NUM_FRM = 0x002;
     static const unsigned int CSR_NUM_FCSR = 0x003;

     static const uint32_t     FCSR_FFLAGS_MASK = 0x0000001F;
     static const unsigned int FCSR_FFLAGS_POS = 0;
     static const uint32_t     FCSR_FRM_MASK = 0x000000E0;
     static const unsigned int FCSR_FRM_POS = 5;
     static const uint32_t FFLAGS_VALID_BITS_MASK = 0x0000001F;
     static const uint32_t    FRM_VALID_BITS_MASK = 0x00000007;
     static const uint32_t   FCSR_VALID_BITS_MASK = 0x000000FF;

     static const uint32_t   FCSR_FFLAGS_NV = 0x0010; // NV Invalid Operation
     static const uint32_t   FCSR_FFLAGS_DZ = 0x0008; // DZ Divide by Zero
     static const uint32_t   FCSR_FFLAGS_OF = 0x0004; // OF Overflow
     static const uint32_t   FCSR_FFLAGS_UF = 0x0002; // UF Underflow
     static const uint32_t   FCSR_FFLAGS_NX = 0x0001; // NX Inexact

     // RV32F Standard Ext
     void instruction_flw(unsigned int rd, unsigned int rs1, int64_t imm);
     void instruction_fsw(unsigned int rs1, unsigned int rs2, int64_t imm);

     void instruction_fmadd_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int rs3);
     void instruction_fmsub_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int rs3);
     void instruction_fnmadd_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int rs3);
     void instruction_fnmsub_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int rs3);

     void instruction_fop(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2, unsigned int funct7);

     void instruction_fadd_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2);
     void instruction_fsub_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2);
     void instruction_fmul_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2);
     void instruction_fdiv_s(unsigned int rd, unsigned int rm, unsigned int rs1, unsigned int rs2);
     void instruction_fsqrt_s(unsigned int rd, unsigned int rm, unsigned int rs1);

     void instruction_fsgnj_s(unsigned int rd, unsigned int rs1, unsigned int rs2);
     void instruction_fsgnjn_s(unsigned int rd, unsigned int rs1, unsigned int rs2);
     void instruction_fsgnjx_s(unsigned int rd, unsigned int rs1, unsigned int rs2);

     void instruction_fmin_s(unsigned int rd, unsigned int rs1, unsigned int rs2);
     void instruction_fmax_s(unsigned int rd, unsigned int rs1, unsigned int rs2);

     void instruction_feq_s(unsigned int rd, unsigned int rs1, unsigned int rs2);
     void instruction_flt_s(unsigned int rd, unsigned int rs1, unsigned int rs2);
     void instruction_fle_s(unsigned int rd, unsigned int rs1, unsigned int rs2);
     void instruction_fclass_s(unsigned int rd, unsigned int rs1);

     void instruction_fmv_x_w(unsigned int rd, unsigned int rs1);
     void instruction_fmv_w_x(unsigned int rd, unsigned int rs1);

     void instruction_fcvt_w_s(unsigned int , unsigned int , unsigned int );
     void instruction_fcvt_wu_s(unsigned int , unsigned int , unsigned int );
     void instruction_fcvt_s_w(unsigned int , unsigned int , unsigned int );
     void instruction_fcvt_s_wu(unsigned int , unsigned int , unsigned int );

     // RV64F Standard Ext 
     void instruction_fcvt_l_s(unsigned int , unsigned int , unsigned int );
     void instruction_fcvt_lu_s(unsigned int , unsigned int , unsigned int );
     void instruction_fcvt_s_l(unsigned int , unsigned int , unsigned int );
     void instruction_fcvt_s_lu(unsigned int , unsigned int , unsigned int );

     // Zicsr misc 
     void Zicsr_dispatch(uint32_t );
     void read_csr(unsigned int , bool& , uint64_t& );
     void write_csr(unsigned int , uint64_t , bool& );

     // Zicsr inst
     void instruction_csrrw(unsigned int , unsigned int , unsigned int );
     void instruction_csrrs(unsigned int , unsigned int , unsigned int );
     void instruction_csrrwi(unsigned int , unsigned int , unsigned int );

     // helper functions 
     void set_freg(unsigned int , float );
     unsigned int get_frm();
     void set_fflags(unsigned int );
     void set_frm(unsigned int );
     unsigned int get_fflags();
     void show_freg(unsigned int );
     float get_freg(unsigned int reg_num) { return f[reg_num]; }
};

#endif
