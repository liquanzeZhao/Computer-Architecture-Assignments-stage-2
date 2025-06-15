/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2025

   Class members for processor

**************************************************************** */

#include <iostream>
#include <iomanip> 
#include <stdlib.h>

#include "memory.h"
#include "processor.h"

using namespace std;

// Constructor
processor::processor (memory* main_memory, bool verbose, bool stage2) {
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
void processor::execute(unsigned int num, bool breakpoint_check) {
    // TODO
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
    // TODO
}

// Set privilege level
// Empty implementation for stage 1, required for stage 2
void processor::set_prv(unsigned int prv_num) {
    // TODO
}

// Display CSR value
// Empty implementation for stage 1, required for stage 2
void processor::show_csr(unsigned int csr_num) {
    // TODO
}

// Set CSR to new value
// Empty implementation for stage 1, required for stage 2
void processor::set_csr(unsigned int csr_num, uint64_t new_value) {
    // TODO
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
    } else {
	cout << "Error: illegal instruction" << endl;
    }
}


// decode instruction and dispatch
void processor::decode_and_dispatch () {
    uint32_t opcode    = instruction & 0x0000007f;
    uint32_t funct3    = (instruction >>  0) & 0x00000000; //TODO
    unsigned int rd    = (instruction >>  0) & 0x00000000; //TODO
    unsigned int rs1   = (instruction >>  0) & 0x00000000; //TODO
    
    // TODO

    int64_t  immed_I   = (int64_t)((instruction & 0x00000000) >> 0); //TODO
    int64_t  immed_U   = (int64_t)((instruction & 0x00000000) >> 0); //TODO

    // TODO

    switch (opcode) {
    case 0x37: instruction_lui   (rd, immed_U); break;
    case 0x03:
        switch (funct3) {
        case 0x0: instruction_lb(rd, rs1, immed_I); break;
        default: instruction_illegal();
        }
        break;
    default:
        ;
    }


    // TODO

}


void processor::instruction_lui   (unsigned int rd, int64_t immed_U) {
    if (verbose)
        cout << "lui: rd = " << dec << rd <<
            ", immed_U = " << setw(16) << setfill('0') << hex << immed_U << endl;
    set_reg(rd, immed_U);
    cycle_count += CYCLE_COUNT_OTHER;
}

void processor::instruction_lb (unsigned int rd, unsigned int rs1, int64_t immed_I) {
    if (verbose)
        cout << "lb: rd = " << dec << rd << ", rs1 = " << dec << rs1 <<
            ", immed_I = " << setw(16) << setfill('0') << hex << immed_I << endl;
    uint64_t address = 0;  //TODO
    uint64_t data = 0;     //TODO
    int8_t byte;
    switch (address % 8) {
    case 0: break; //TODO
    case 1: break; //TODO
    case 2: break; //TODO
    case 3: break; //TODO
    case 4: break; //TODO
    case 5: break; //TODO
    case 6: break; //TODO
    case 7: break; //TODO
    }
    set_reg(rd, (int64_t)byte);
    cycle_count += CYCLE_COUNT_LOAD;
}


