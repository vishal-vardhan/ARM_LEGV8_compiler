#ifndef __VARIABLE_H__
#define __VARIABLE_H__
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <fstream>
#include <iomanip>

#define MAX_MEM_SIZE 1000
#define REG_COUNT 32
#define XZR_INDEX 31
#define FLAGS_SIZE 7
#define SP_ADDR 28
#define MEM_FILE "MEMORY.txt"
#define REG_FILE "REGISTERS.txt"

#define EQ_FLAG 0
#define NE_FLAG 1
#define LT_FlAG 2
#define LE_FlAG 3
#define GT_FLAG 4
#define GE_FLAG 5
#define Z_FLAG 6


extern int PC;
extern int SP;
extern std::stack<int> stk;
extern std::map<char, int> offset;
extern std::map<std::string, int> opcode_map;
extern std::vector<std::vector<int>> lines;
extern int X[REG_COUNT]; 
extern float S[REG_COUNT];
extern double D[REG_COUNT];
extern std::vector<std::pair<int, std::string>> unparsed;
extern std::map<std::string, int> labels;
extern bool FLAGS[FLAGS_SIZE];
extern double MEMORY[MAX_MEM_SIZE];

void initialize();
void store_memory();
void set_flags(int n);
void print_lines();
void store_registers();

#endif