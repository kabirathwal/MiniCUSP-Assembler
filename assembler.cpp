//============================================================================
// Name        : assembler.cpp
// Author      : Kabir Athwal
// Version     : 2.0
//============================================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <unordered_map>
using namespace std;

int main(int argc, char * argv[]) {
// int argc, char * argv[]


// declarations, maps
	fstream inFile;
//	inFile.open("my_instructions.txt");
	inFile.open(argv[1]);

	unordered_map<string, string> instructions;
	instructions["LDA"] = "00";
	instructions["LDX"] = "01";
	instructions["LDS"] = "02";
	instructions["LDF"] = "03";
	instructions["STA"] = "04";
	instructions["STX"] = "05";
	instructions["STS"] = "06";
	instructions["STF"] = "07";
	instructions["PSH"] = "08";
	instructions["POP"] = "09";
	instructions["CLR"] = "0A";
	instructions["SET"] = "0B";
	instructions["PSHA"] = "FFF010";
	instructions["PSHX"] = "FFF011";
	instructions["PSHF"] = "FFF012";
	instructions["POPA"] = "FFF013";
	instructions["POPX"] = "FFF014";
	instructions["POPF"] = "FFF015";
	instructions["ADA"] = "10";
	instructions["ADX"] = "11";
	instructions["ADS"] = "12";
	instructions["ADF"] = "13";
	instructions["SBA"] = "14";
	instructions["SBX"] = "15";
	instructions["SBS"] = "16";
	instructions["SBF"] = "17";
	instructions["MUL"] = "18";
	instructions["DIV"] = "19";
	instructions["MOD"] = "1A";
	instructions["CMA"] = "20";
	instructions["CMX"] = "21";
	instructions["CMS"] = "22";
	instructions["CMF"] = "23";
	instructions["SHRA"] = "FFF022";
	instructions["SHLA"] = "FFF023";
	instructions["JSR"] = "41";
	instructions["RTN"] = "FFF040";
	instructions["JEQ"] = "48";
	instructions["JLT"] = "4A";
	instructions["JGE"] = "4B";
	instructions["NOP"] = "FFF038";
	instructions["HLT"] = "FFFFFF";

	unordered_map<string, string> addrMode;
	addrMode["#"] = "0";
	addrMode["+"] = "4";
	addrMode["*"] = "6";
	addrMode["&"] = "8";
	addrMode["# !"] = "1";
	addrMode["!"] = "3";
	addrMode["+ !"] = "5";
	addrMode["* !"] = "7";
	addrMode["& !"] = "9";

	unordered_map<string, string> labels;

// PASS ONE (adds labels and instructions)
	string temp;
	int counter = 0;

	while(inFile >> temp)
	{
//		cout << temp << endl;

		int tempLen = temp.length() - 1;

		if(temp[tempLen] == ':')
		{
			if(counter <= 9)
			{
				temp.erase(tempLen, tempLen);

				auto it = labels.find(temp);
				if(it != instructions.end())
				{
					cerr << "ASSEMBLER ERROR: label defined multiple times." << endl;
				}


				labels[temp] = "00" + to_string(counter);
			}
			else
			{
				char t = char(55 + counter);
				string tt = "00";
				tt += t;

				temp.erase(tempLen, tempLen);

				auto it = labels.find(temp);
				if(it != instructions.end())
				{
					cerr << "ASSEMBLER ERROR: label defined multiple times." << endl;
				}

				labels[temp] = tt;
			}
		}

		int len = temp.length();
		if(len == 3 and isupper(temp[0]) and temp != "HLT" and temp != "NOP" and temp != "RTN")
		{
			string t1, t2, t3;

			inFile >> t1;

//			cout << temp << " ";						// OUTPUT
//			cout << "t1: " << t1 << " ";				// OUTPUT

			if(t1 == "#" or t1 == "+" or t1 == "*" or t1 == "!" or t1 == "+" or t1 == "&")
			{
				inFile >> t2;

//				cout << "t2: " << t2 << " ";			// OUTPUT

				if(t2 == "#" or t2 == "+" or t2 == "*" or t2 == "!" or t2 == "+" or t2 == "&")
				{
					inFile >> t3;

//					cout << "t3: " <<  t3 << endl;		// OUTPUT
					string t32;

					if(t3[0] == '$')
					{
						t32 = t3;
						t32.erase(0, 1);

//						cout << "\nT3 TRY 2 " << t32 << endl << endl;				// OUTPUT
					}
					else							// fix to base 16 if no $
					{
						t32 = t3;
						int base16 = stoi(t32);
						string b16 = "0123456789ABCDEF";
						string base16Final;

					    while(base16 > 0)
					    {
					    	base16Final = b16[base16 % 16] + base16Final;
					    	base16 /= 16;
					    }

					    t32 = base16Final;
					}

					int t32Len = 3 - t32.length();
					for(int i = 0; i < t32Len; i++)
					{
						t32 = "0" + t32;
					}

//					cout << "\nT3 TRY 2P2 " << t32 << endl << endl;		// OUTPUT

					labels[t3] = t32;
				}
				if(isdigit(t2[0]) or isdigit(t2[1]) or isupper(t2[0]) or isupper(t2[1]))
				{
					string t22;

					if(t2[0] == '$')
					{
						t22 = t2;
						t22.erase(0, 1);

//						cout << "\nT2 TRY 2 " << t22 << endl << endl;		// OUTPUT
					}
					else						// fix to base 16 if no $
					{
						t22 = t2;
						int base16 = stoi(t22);
						string b16 = "0123456789ABCDEF";
						string base16Final;

					    while(base16 > 0)
					    {
					    	base16Final = b16[base16 % 16] + base16Final;
					    	base16 /= 16;
					    }

					    t22 = base16Final;
					}

					int t22Len = 3 - t22.length();
					for(int i = 0; i < t22Len; i++)
					{
						t22 = "0" + t22;
					}

//					cout << "\nT2 TRY 2P2 " << t22 << endl << endl;		// OUTPUT

					labels[t2] = t22;
				}
			}
			if(isdigit(t1[0]) or isdigit(t1[1]) or isupper(t1[0]) or isupper(t1[1]))
			{
				string t12;

				if(t1[0] == '$')
				{
					t12 = t1;
					t12.erase(0, 1);

//					cout << "\nT2 TRY 2 " << t12 << endl << endl;			// OUTPUT
				}
				else				// IF THERE IS NO $, THEN IT IS IN BASE 16 FIX ITTTT
				{
					t12 = t1;
					int base16 = stoi(t12);
					string b16 = "0123456789ABCDEF";
					string base16Final;

				    while(base16 > 0)
				    {
				    	base16Final = b16[base16 % 16] + base16Final;
				    	base16 /= 16;
				    }

				    t12 = base16Final;
				}

				int t12Len = 3 - t12.length();
				for(int i = 0; i < t12Len; i++)
				{
					t12 = "0" + t12;
				}

//				cout << "\nT2 TRY 2P2 " << t12 << endl << endl;			// OUTPUT

				labels[t1] = t12;
			}

		}

		if(isupper(temp[0]) or temp[0] == '.')
		{
			counter++;
		}
	}


	inFile.close();

// TEST OUTPUT:
//	cout << "\nCounter: " << counter << endl << endl;			// OUTPUT


	for(auto x : labels)
	{
//		cout << x.first << " " << x.second << endl;				// OUTPUT
	}
//	cout << endl << endl;										// OUTPUT


// PASS TWO:
//	inFile.open("my_instructions.txt");
	inFile.open(argv[1]);

	ofstream machineCode;
//	machineCode.open("machineCode.txt");
	machineCode.open(argv[2]);

	string temp2;

	while(getline(inFile, temp2))
	{
		string s1, s2, s3, s4;
		if(!isupper(temp2[0]) and temp2[0] != ' ' and temp2[0] != '.')
		{
//			cout << "IT IS A LABEL" << endl;
		}
		else
		{
//			cout << temp2 << endl;							// OUTPUT

			if(temp2[0] == ' ' and temp2[0] != '.')			// MAKE SURE THIS IS WORKING PROPERLY
			{
				temp2.erase(0, 4);
			}
//			cout << temp2 << endl;							// OUTPUT

// FOR .word INSTRUCTIONS:

			stringstream ss(temp2);
			getline(ss, s1, ' ');
			if(s1 == ".word")
			{
				getline(ss, s2, ' ');

				int s2Len =  6 - s2.length();
				for(int i = 0; i < s2Len; i++)
				{
					s2 = "0" + s2;
				}
				machineCode << s2 << " " << endl;				// MACHINE OUTPUT
//				cout << s2 << " " << endl;						// MACHINE OUTPUT
			}

			int s1Len = s1.length();

// FOR INSTRUCTIONS WITH NO OPERAND:
			if((isupper(s1[0]) and s1Len == 4) or s1 == "HLT" or s1 == "NOP" or s1 == "RTN")
			{
				getline(ss, s2, ' ');

				auto it = instructions.find(s1);
				if(it != instructions.end())
				{
					machineCode << it -> second << endl;			// MACHINE OUTPUT
//					cout << it -> second << endl;					// MACHINE OUTPUT
				}
			}

// FOR INSTRUCTIONS WITH AN OPERAND:
			if(isupper(s1[0]) and s1Len == 3 and s1 != "HLT" and s1 != "NOP" and s1 != "RTN")
			{
				auto it = instructions.find(s1);
				if(it != instructions.end())
				{
					machineCode << it -> second;					// MACHINE OUTPUT
//					cout << it -> second;							// MACHINE OUTPUT
				}

	// To get addressing mode and operand:
				getline(ss, s2, ' ');
				if(s2 == "#" or s2 == "+" or s2 == "*" or s2 == "!" or s2 == "&")
				{
					getline(ss, s3, ' ');

					if(s3 == "!")
					{
						s2 = s2 + " " + s3;

						auto it2 = addrMode.find(s2);
						if(it2 != addrMode.end())
						{
							machineCode << it2 -> second;			// MACHINE OUTPUT
//							cout << it2 -> second;					// MACHINE OUTPUT
						}


						getline(ss, s4, ' ');
						auto it3 = labels.find(s4);
						if(it3 != labels.end())
						{
							machineCode << it3 -> second << endl;		// MACHINE OUTPUT
//							cout << it3 -> second << endl;				// MACHINE OUTPUT
						}
						else
						{
							cerr << "ASSEMBLER ERROR: undefined label." << endl;
						}

						if((s1 == "STA" or s1 == "STX" or s1 == "STS" or s1 == "STF" or s1 == "POP" or s1 == "CLR"
							or s1 == "SET" or s1 == "JSR" or s1 == "JEQ" or s1 == "JLT" or s1 == "JGE") and (s2 == "#" or s2 == "# !"))
						{
							cerr << "ASSEMBLER ERROR: instruction using unsupported addressing mode." << endl;
						}
					}
					else
					{
						auto it2 = addrMode.find(s2);
						if(it2 != addrMode.end())
						{
							machineCode << it2 -> second;			// MACHINE OUTPUT
//							cout << it2 -> second;					// MACHINE OUTPUT
						}



						getline(ss, s3, ' ');
						auto it3 = labels.find(s3);
						if(it3 != labels.end())
						{
							machineCode << it3 -> second << endl;		// MACHINE OUTPUT
//							cout << it3 -> second << endl;				// MACHINE OUTPUT
						}
						else
						{
							cerr << "ASSEMBLER ERROR: undefined label." << endl;
						}

						if((s1 == "STA" or s1 == "STX" or s1 == "STS" or s1 == "STF" or s1 == "POP" or s1 == "CLR"
							or s1 == "SET" or s1 == "JSR" or s1 == "JEQ" or s1 == "JLT" or s1 == "JGE") and (s2 == "#" or s2 == "# !"))
						{
							cerr << "ASSEMBLER ERROR: instruction using unsupported addressing mode." << endl;
						}
					}
				}
				else
				{
					machineCode << "2";								// MACHINE OUTPUT
//					cout << "2";									// MACHINE OUTPUT

					auto it3 = labels.find(s2);
					if(it3 != labels.end())
					{
						machineCode << it3 -> second << endl;		// MACHINE OUTPUT
//						cout << it3 -> second << endl;				// MACHINE OUTPUT
					}
					else
					{
						cerr << "ASSEMBLER ERROR: undefined label." << endl;
					}
				}
			}
		}
	}


	inFile.close();
	machineCode.close();

	return 0;
}
