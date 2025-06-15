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

};

#endif
