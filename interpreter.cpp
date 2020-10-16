#include "variables.h"

using std::string;
using std::vector;
using std::ifstream;
using std::map;
using std::cout;
using std::endl;


int parseRegister(string &str){
	if(str == "sp") return SP_ADDR;
	if(str == "xzr") return XZR_INDEX;
	int n = 0;
	for(int i=1;i<str.size();i++)
		n = n*10 + (str[i]-'0');
	// n += offset[str[0]];
	return n;
}

void parseLine(string &str){
	vector<string> res;
	string word = "";
	for(int i=0;i<str.size();i++){
		str[i] = (str[i] >= 'A' && str[i] <= 'Z')?str[i]+('a'-'A'):str[i];
		switch(str[i]){
			case ':':
				labels[word] = lines.size();
				word = "";
				break;
			case '@':
			case ';':
				if(word != "") res.push_back(word);
				i = str.size();
				break;
			case '[':
			case ']':
			case ' ':
			case '\t':
			case '\n':
			case ',':
				if(word != "") res.push_back(word);
				word = "";
				break;
			default:
				word += str[i];
		}
	}
	if(res.size() == 0) return;
	vector<int> line;
	//push number aliases to lines
	switch(res[0][0]){
		case 'b':
		case 'c':
			line.push_back(opcode_map[res[0]]);
			if(labels.find(res[1]) != labels.end())
				line.push_back(labels.find(res[1])->second);
			else{
				unparsed.push_back(make_pair(lines.size(), res[1]));
				line.push_back(-1);
			}
			lines.push_back(line);
			break;
		default:
			line.push_back(opcode_map[res[0]]);
			for(int i=1;i<res.size();i++)
				line.push_back(parseRegister(res[i]));
			lines.push_back(line);
	}
}

void assignLabels(){
	for(int i=0;i<unparsed.size();i++)
		lines[unparsed[i].first][1] = labels[unparsed[i].second];
}


void execute(){
	string s;
	int tmp;
	switch(lines[PC][0]){
		case 0:
			//B label
			PC = lines[PC][1];
			break;
		case 1:
			//FMULS
			S[lines[PC][1]] = S[lines[PC][2]]*S[lines[PC][3]];
			PC++;
			break;
		case 2:
			//FDIVS
			S[lines[PC][1]] = S[lines[PC][2]] / S[lines[PC][3]];
			PC++;
			break;
		case 3:
			//FCMPS
			FLAGS[EQ_FLAG] = S[lines[PC][1]] == S[lines[PC][2]];
			FLAGS[NE_FLAG] = S[lines[PC][1]] != S[lines[PC][2]];
			FLAGS[LT_FlAG] = S[lines[PC][1]] < S[lines[PC][2]];
			FLAGS[LE_FlAG] = S[lines[PC][1]] <= S[lines[PC][2]];
			FLAGS[GT_FLAG] = S[lines[PC][1]] > S[lines[PC][2]];
			FLAGS[GE_FLAG] = S[lines[PC][1]] >= S[lines[PC][2]];
			PC++;
			break;
		case 4:
			//FADDS
			S[lines[PC][1]] = S[lines[PC][2]]+S[lines[PC][3]];
			PC++;
			break;
		case 5:
			//FSUBS
			S[lines[PC][1]] = S[lines[PC][2]]-S[lines[PC][3]];
			PC++;
			break;
		case 6:
			//FMULD
			D[lines[PC][1]] = D[lines[PC][2]]*D[lines[PC][3]];
			PC++;
			break;
		case 7:
			//FDIVD
			D[lines[PC][1]] = D[lines[PC][2]]/D[lines[PC][3]];
			PC++;
			break;
		case 8:
			//FCMPD
			FLAGS[EQ_FLAG] = D[lines[PC][1]] == D[lines[PC][2]];
			FLAGS[NE_FLAG] = D[lines[PC][1]] != D[lines[PC][2]];
			FLAGS[LT_FlAG] = D[lines[PC][1]] < D[lines[PC][2]];
			FLAGS[LE_FlAG] = D[lines[PC][1]] <= D[lines[PC][2]];
			FLAGS[GT_FLAG] = D[lines[PC][1]] > D[lines[PC][2]];
			FLAGS[GE_FLAG] = D[lines[PC][1]] >= D[lines[PC][2]];
			PC++;
			break;
		case 9:
			//FADDD
			D[lines[PC][1]] = D[lines[PC][2]]+D[lines[PC][3]];
			PC++;
			break;
		case 10:
			//FSUBD
			D[lines[PC][1]] = D[lines[PC][2]]-D[lines[PC][3]];
			PC++;
			break;
		case 11:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//STURB
			PC++;
			break;
		case 12:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//LDURB
			PC++;
			break;
		case 13:
			//B.EQ
			if(FLAGS[0]) PC = lines[PC][1];
			else PC++;
			break;
		case 14:
			//B.LT
			if(FLAGS[2]) PC = lines[PC][1];
			else PC++;
			break;
		case 15:
			//B.LE
			if(FLAGS[3]) PC = lines[PC][1];
			else PC++;
			break;
		case 16:
			//B.GT
			if(FLAGS[4]) PC = lines[PC][1];
			else PC++;
			break;
		case 17:
			//B.GE
			if(FLAGS[5]) PC = lines[PC][1];
			else PC++;
			break;
		case 18:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//STURH
			PC++;
			break;
		case 19:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//LDURH
			PC++;
			break;
		case 20:
			//AND
			tmp = X[lines[PC][2]] & X[lines[PC][3]];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp; 
			PC++;
			break;
		case 21:
			//ADD
			tmp = X[lines[PC][2]] + X[lines[PC][3]];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			PC++;
			break;
		case 22:
			//ADDI
			tmp = X[lines[PC][2]] + lines[PC][3];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			PC++;
			break;
		case 23:
			//ANDI
			tmp = X[lines[PC][2]] & lines[PC][3];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			PC++;
			break;
		case 24:
			//BL
			stk.push(PC+1);
			PC = lines[PC][1];
			break;
		case 25:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//SDIV
			break;
		case 26:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//UDIV
			break;
		case 27:
			//MUL
			tmp = X[lines[PC][2]] * X[lines[PC][3]];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			break;
		case 28:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//SMULH
			break;
		case 29:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//UMULH
			break;
		case 30:
			//ORR
			tmp = X[lines[PC][2]] | X[lines[PC][3]];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			break;
		case 31:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//ADDS
			break;
		case 32:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//ADDIS
			break;
		case 33:
			//ORRI
			tmp = X[lines[PC][2]] | lines[PC][3];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			break;
		case 34:
			//CBZ
			if(FLAGS[Z_FLAG]) PC = lines[PC][1];
			else PC++;
			break;
		case 35:
			//CBNZ
			if(!FLAGS[Z_FLAG]) PC = lines[PC][1];
			else PC++;
			break;
		case 36:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//STURW
			break;
		case 37:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//LDURSW
			break;
		case 38:
			//STURS
			MEMORY[X[lines[PC][2]]+(lines[PC][3]/8)] = (double) S[lines[PC][1]];
			PC++;
			break;
		case 39:
			//LDURS
			S[lines[PC][1]] = (float) MEMORY[X[lines[PC][2]]+(lines[PC][3]/8)];
			PC++;
			break;
		case 40:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//STXR
			break;
		case 41:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//LDXR
			break;
		case 42:
			//EOR
			tmp = X[lines[PC][2]] ^ X[lines[PC][3]];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			PC++;
			break;
		case 43:
			//SUB
			tmp = X[lines[PC][2]] - X[lines[PC][3]];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			PC++;
			break;
		case 44:
			//SUBI
			tmp = X[lines[PC][2]] - lines[PC][3];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			PC++;
			break;
		case 45:
			//EORI
			tmp = X[lines[PC][2]] ^ lines[PC][3];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			PC++;
			break;
		case 46:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//MOVZ
			break;
		case 47:
			//LSR
			tmp = X[lines[PC][2]] >> lines[PC][3];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			PC++;
			break;
		case 48:
			//LSL
			tmp = X[lines[PC][2]] << lines[PC][3];
			set_flags(tmp);
			if(lines[PC][1] != XZR_INDEX) X[lines[PC][1]] = tmp;
			PC++;
			break;
		case 49:
			//BR
			PC = stk.top();
			stk.pop();
			break;
		case 50:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//ANDS
			break;
		case 51:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//SUBS
			break;
		case 52:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//SUBIS
			break;
		case 53:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//ANDIS
			break;
		case 54:
			std::cout<<"Operation not implemented yet"<<std::endl;
			//MOVK
			break;
		case 55:
			//STUR
			MEMORY[X[lines[PC][2]]+(lines[PC][3]/8)] = (double) X[lines[PC][1]];
			PC++;
			break;
		case 56:
			//LDUR
			X[lines[PC][1]] = (int) MEMORY[X[lines[PC][2]]+(lines[PC][3]/8)];
			PC++;
			break;
		case 57:
			//STURD
			MEMORY[X[lines[PC][2]]+(lines[PC][3]/8)] = D[lines[PC][1]];
			PC++;
			break;
		case 58:
			//LDURD
			D[lines[PC][1]] = MEMORY[X[lines[PC][2]]+(lines[PC][3]/8)];
			PC++;
			break;
		default:
			throw "Unknown operation execution at line %d", PC; 
	}
}

void start(){
	PC = 0;
	while(PC < lines.size()){
		execute();
	}
}


int main(int argc, char* argv[]){
	initialize();
	//setting input stream
    ifstream file;
    string str;
	//replace this with arvgv[3]
	// std::cout<<"argc: "<<argc<<std::endl;

    file.open(argv[1]);
	if(argc > 1)
		std::cout<< "Executing file: " << argv[1] << std::endl;
    while((!file.eof()) && file){
        getline(file, str);
		parseLine(str);
    }
	file.close();
	assignLabels();

	start();

	store_memory();
	store_registers();
    return 0;
}