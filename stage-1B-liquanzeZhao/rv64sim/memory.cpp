/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2025

   Class members for memory

**************************************************************** */

#include <iostream> 
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <cstdio>

#include "memory.h"
using namespace std;

// Constructor
memory::memory(bool verbose) {
  this->verbose = verbose;
}

// Check if a page of store is allocated, and allocate if not
void memory::validate (uint64_t address) {
  if (store[address & 0xFFFFFFFFFFFFF000ULL].empty()) {
    store[address & 0xFFFFFFFFFFFFF000ULL] = vector<uint64_t>(512, 0x0ULL);
  }
}

// Read a doubleword of data from a doubleword-aligned address.
// If the address is not a multiple of 8, it is rounded down to a multiple of 8.
uint64_t memory::read_doubleword (uint64_t address) {
  uint64_t doubleword_index = (address & 0xfffULL) >> 3;
  uint64_t data;
  validate(address);
  data = store[address & 0xFFFFFFFFFFFFF000ULL][doubleword_index];
  if (verbose)
    cout << "Memory read doubleword: address = " << setw(16) << setfill('0') << hex << address <<
      ", data = " << setw(16) << setfill('0') << hex << data << endl;
  return data;
}

// Write a doubleword of data to a doubleword-aligned address.
// If the address is not a multiple of 8, it is rounded down to a multiple of 8.
// The mask contains 1s for bytes to be updated and 0s for bytes that are to be unchanged.
void memory::write_doubleword (uint64_t address, uint64_t data, uint64_t mask) {
  uint64_t doubleword_index = (address & 0xfffULL) >> 3;
  uint64_t old_data;
  if (verbose)
    cout << "Memory write doubleword: address = " << setw(16) << setfill('0') << hex << address <<
      ", data = " << setw(16) << setfill('0') << hex << data <<
      ", mask = " << setw(16) << setfill('0') << hex << mask << endl;
  validate(address);
  old_data = store[address & 0xFFFFFFFFFFFFF000ULL][doubleword_index];
  store[address & 0xFFFFFFFFFFFFF000ULL][doubleword_index] = (data & mask) | (old_data & ~mask);
}

// Load a hex image file and provide the start address for execution from the file in start_address.
// Return true if the file was read without error, or false otherwise.
bool memory::load_file(string file_name, uint64_t &start_address) {
  ifstream input_file(file_name);
  string input;
  unsigned int line_count = 0;
  unsigned int byte_count = 0;
  char record_start;
  char byte_string[3];
  char halfword_string[5];
  unsigned int record_length;
  unsigned int record_address;
  unsigned int record_type;
  unsigned int record_data;
  unsigned int record_checksum;
  bool end_of_file_record = false;
  uint64_t load_address;
  uint64_t load_data;
  uint64_t load_mask;
  uint64_t load_base_address = 0x0000000000000000ULL;
  start_address = 0x0000000000000000ULL;
  if (input_file.is_open()) {
    while (true) {
      line_count++;
      input_file >> record_start;
      if (record_start != ':') {
	cout << "Input line " << dec << line_count << " does not start with colon character" << endl;
	return false;
      }
      input_file.get(byte_string, 3);
      sscanf(byte_string, "%x", &record_length);
      input_file.get(halfword_string, 5);
      sscanf(halfword_string, "%x", &record_address);
      input_file.get(byte_string, 3);
      sscanf(byte_string, "%x", &record_type);
      switch (record_type) {
      case 0x00:  // Data record
	for (unsigned int i = 0; i < record_length; i++) {
	  input_file.get(byte_string, 3);
	  sscanf(byte_string, "%x", &record_data);
	  load_address = (load_base_address | (uint64_t)(record_address)) + i;
	  load_data = (uint64_t)(record_data) << ((load_address % 8) * 8);
	  load_mask = 0x00000000000000ffULL << ((load_address % 8) * 8);
	  write_doubleword(load_address & 0xfffffffffffffff8ULL, load_data, load_mask);
	  byte_count++;
	}
	break;
      case 0x01:  // End of file
	end_of_file_record = true;
	break;
      case 0x02:  // Extended segment address (set bits 19:4 of load base address)
	load_base_address = 0x0000000000000000ULL;
	for (unsigned int i = 0; i < record_length; i++) {
	  input_file.get(byte_string, 3);
	  sscanf(byte_string, "%x", &record_data);
	  load_base_address = (load_base_address << 8) | (record_data << 4);
	}
	break;
      case 0x03:  // Start segment address (ignored)
	for (unsigned int i = 0; i < record_length; i++) {
	  input_file.get(byte_string, 3);
	  sscanf(byte_string, "%x", &record_data);
	}
	break;
      case 0x04:  // Extended linear address (set upper halfword of load base address)
	load_base_address = 0x0000000000000000ULL;
	for (unsigned int i = 0; i < record_length; i++) {
	  input_file.get(byte_string, 3);
	  sscanf(byte_string, "%x", &record_data);
	  load_base_address = (load_base_address << 8) | (record_data << 16);
	}
	break;
      case 0x05:  // Start linear address (set execution start address)
	start_address = 0x0000000000000000ULL;
	for (unsigned int i = 0; i < record_length; i++) {
	  input_file.get(byte_string, 3);
	  sscanf(byte_string, "%x", &record_data);
	  start_address = (start_address << 8) | record_data;
	}
	break;
      }
      input_file.get(byte_string, 3);
      sscanf(byte_string, "%x", &record_checksum);
      input_file.ignore();
      if (end_of_file_record)
	break;
    }
    input_file.close();
    cout << dec << byte_count << " bytes loaded, start address = "
	 << setw(16) << setfill('0') << hex << start_address << endl;
    return true;
  }
  else {
    cout << "Failed to open file" << endl;
    return false;
  }
}

//stage2

void memory::write_float(uint64_t addr, float data)
{
	validate(addr);
	vector<uint64_t>& mem_block = store[addr & 0xFFFFFFFFFFFFF000ULL];
	float* mem_data = (float*)mem_block.data();
	uint64_t float_index = (addr & 0xfffULL) >> 2;
	mem_data[float_index] = data;
	if (verbose)
		cout << "Memory write float: address = " << setw(16) << setfill('0') << hex << addr <<
		", data = " << data << endl;
}

float  memory::read_float(uint64_t addr)
{
	validate(addr);
	vector<uint64_t>& mem_block = store[addr & 0xFFFFFFFFFFFFF000ULL];
	const float* mem_data = (const float*)mem_block.data();
	uint64_t float_index = (addr & 0xfffULL) >> 2;
	float float_data = mem_data[float_index];
	if (verbose)
		cout << "Memory read float: address = " << setw(16) << setfill('0') << hex << addr <<
		", data = " << float_data << endl;
	return float_data;
}