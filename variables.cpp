#include "variables.h"


int PC;
int SP;
std::stack<int> stk;
std::map<char, int> offset;
std::map<std::string, int> opcode_map;
std::vector<std::vector<int>> lines;
int X[REG_COUNT]; 
float S[REG_COUNT];
double D[REG_COUNT];
std::vector<std::pair<int, std::string>> unparsed;
std::map<std::string, int> labels;

//8KB memory
double MEMORY[MAX_MEM_SIZE];


//0: EQ
//1: NE
//2: LT
//3: LE
//4: GT
//5: GE
//6: ZF
bool FLAGS[FLAGS_SIZE];


bool is_empty(std::ifstream& pFile){
    return pFile.peek() == std::ifstream::traits_type::eof();
}

void mem_initialize(){
    std::ofstream file;
    file.open(MEM_FILE);
    for(int i=0; i < MAX_MEM_SIZE; i++)
        file << i <<" "<< i << std::endl;
    file.close();
}

void load_memory(){
    std::ifstream file(MEM_FILE);
    if(!file || is_empty(file)) 
        return;
    int i=0;
    int n;
    //code for error checking : index out of bounds
    while((!file.eof()) && i<MAX_MEM_SIZE){
        file>>n>>MEMORY[i];
        i++;
    }
}

void load_registers(){
    std::ifstream file(REG_FILE);
    if(!file || is_empty(file))
        return;
    int i=0;
    int n;
    //TODO: add functionality to load S and D type registers
    while(!file.eof() && i<REG_COUNT){
        file>>n>>X[i];
        i++;
    }
}

void store_registers(){
    std::ofstream file(REG_FILE);
    for(int i=0;i<REG_COUNT;i++){
        file<<i<<" "<<X[i]<<std::endl;
    }
}

void initialize(){
    PC = 0;
    SP = MAX_MEM_SIZE;
    X[XZR_INDEX] = 0;
    load_memory();
    load_registers();
	// offset['x'] = 0;
	// offset['s'] = 32;
	// offset['d'] = 64;
	// offset['#'] = 96;

	opcode_map["b"] = 0;
    opcode_map["fmuls"] = 1;
    opcode_map["fdivs"] = 2;
    opcode_map["fcmps"] = 3;
    opcode_map["fadds"] = 4;
    opcode_map["fsubs"] = 5;
    opcode_map["fmuld"] = 6;
    opcode_map["fdivd"] = 7;
    opcode_map["fcmpd"] = 8;
    opcode_map["faddd"] = 9;
    opcode_map["fsubd"] = 10;
    opcode_map["sturb"] = 11;
    opcode_map["ldurb"] = 12;
    opcode_map["b.eq"] = 13;
    opcode_map["b.lt"] = 14;
    opcode_map["b.le"] = 15;
    opcode_map["b.gt"] = 16;
    opcode_map["b.ge"] = 17;
    opcode_map["sturh"] = 18;
    opcode_map["ldurh"] = 19;
    opcode_map["and"] = 20;
    opcode_map["add"] = 21;
    opcode_map["addi"] = 22;
    opcode_map["andi"] = 23;
    opcode_map["bl"] = 24;
    opcode_map["sdiv"] = 25;
    opcode_map["udiv"] = 26;
    opcode_map["mul"] = 27;
    opcode_map["smulh"] = 28;
    opcode_map["umulh"] = 29;
    opcode_map["orr"] = 30;
    opcode_map["adds"] = 31;
    opcode_map["addis"] = 32;
    opcode_map["orri"] = 33;
    opcode_map["cbz"] = 34;
    opcode_map["cbnz"] = 35;
    opcode_map["sturw"] = 36;
    opcode_map["ldursw"] = 37;
    opcode_map["sturs"] = 38;
    opcode_map["ldurs"] = 39;
    opcode_map["stxr"] = 40;
    opcode_map["ldxr"] = 41;
    opcode_map["eor"] = 42;
    opcode_map["sub"] = 43;
    opcode_map["subi"] = 44;
    opcode_map["eori"] = 45;
    opcode_map["movz"] = 46;
    opcode_map["lsr"] = 47;
    opcode_map["lsl"] = 48;
    opcode_map["br"] = 49;
    opcode_map["ands"] = 50;
    opcode_map["subs"] = 51;
    opcode_map["subis"] = 52;
    opcode_map["andis"] = 53;
    opcode_map["movk"] = 54;
    opcode_map["stur"] = 55;
    opcode_map["ldur"] = 56;
    opcode_map["sturd"] = 57;
    opcode_map["ldurd"] = 58;
}


void store_memory(){
    std::ofstream file(MEM_FILE);
    int i=0;
    //code for error checking : index out of bounds
    while(i < MAX_MEM_SIZE){
        file << i << " " << std::setprecision(15) << MEMORY[i] << std::endl;
        i++;
    }
}

void set_flags(int n){
    FLAGS[0] = (n == 0);
    FLAGS[1] = (n != 0);
    FLAGS[2] = (n < 0);
    FLAGS[3] = (n <= 0);
    FLAGS[4] = (n > 0);
    FLAGS[5] = (n >= 0);
    FLAGS[6] = (n == 0); //correct
}

void print_lines(){
    for(int i=0;i<lines.size();i++){
        for(int j=0;j<lines[i].size();j++)
            std::cout<<lines[i][j]<<" ";
        std::cout<<std::endl;
    }
}