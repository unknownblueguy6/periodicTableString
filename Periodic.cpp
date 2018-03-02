#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <cctype>
#include <utility> 

using namespace std;

struct periodic{
	unsigned num;
	string sym;
	string name;
};

periodic sortedTable[26][12];	

periodic getElement(string element){
	periodic final;
	final.num = stoi(element);
	final.sym = isalpha(element[int(log10(final.num))+2]) ? element.substr(int(log10(final.num))+1, 2) : element.substr(int(log10(final.num))+1, 1)	;
	final.name = element.substr(element.find(" ")+1);
	return final; 
}

void getSortedTable(){
	ifstream table ("sorted.txt");
	string element;
	unsigned letter = 0;
	unsigned counter = 0;
	if (table.is_open()) {

		while(getline(table, element)) {
			if (element.empty()){
				++letter;
				counter = 0;
			}
			
			else {
				sortedTable[letter][counter] = getElement(element);
				++counter; 
			
				if ((sortedTable[letter][counter-1].num == 77) || (sortedTable[letter][counter-1].num == 94)){
					++letter;
				}
			}

		}
	}
}

bool inTable(string a){
	if (a.length() == 1){
		
		if (islower(a[0])){
			if (sortedTable[int(a[0])-97][0].sym.length() == 1 && sortedTable[int(a[0])-97][0].sym[0] == toupper(a[0])){
				return true;
			}
			return false;
		}
		
		else{
			if (sortedTable[int(a[0])-65][0].sym.length() == 1 && sortedTable[int(a[0])-65][0].sym[0] == a[0]){
				return true;
			}
			return false;
		}
	}

	else{
		char b = toupper(a[0]), c = tolower(a[1]);
		unsigned i = 0;
		
		while (true){
			if (sortedTable[int(b)-65][i].sym.length() == 0){
				return false;
			}
			if (sortedTable[int(b)-65][i].sym.length() == 2 && sortedTable[int(b)-65][i].sym[1] == c){
				return true;
			}
			++i;
		}
	}
}

pair<bool, string> resultCombiner(pair<bool, string> r1, pair<bool, string> r2){
	if (r1.first && r2.first){
		return make_pair(true, r1.second+r2.second);
	}
	return make_pair(false, "");
}

pair<bool, string> checker(string input){
	if (!(input.find('q') == -1 || input.find('Q') == -1 || input.find('J') == -1 || input.find('j') == -1)){ 
		return make_pair(false, "");
	}

	static unsigned len = input.length();
	
	if (input.length() == 1){
		if (inTable(input)){
			input[0] = toupper(input[0]);
			
			return make_pair(true, input);
		}
		else{
			
			return make_pair(false, "");
		}
	}

	if(input.length() == 2){

		if (inTable(input)){
			input[0] = toupper(input[0]);
			input[1] = tolower(input[1]);
			
			return make_pair(true, input);
		}
		else{
			pair <bool, string> result = resultCombiner(checker(input.substr(0, 1)), checker(input.substr(1, 1) ));
			if (result.first){
				return result;
			}
			return make_pair(false, "");
		}
	}
	
	if (input.length() % 2){ 
		pair<bool, string> result = resultCombiner(checker(input.substr(0, input.length()/2)), checker(input.substr(input.length()/2, input.length() - input.length()/2)));
		if(!result.first){
			result = resultCombiner(checker(input.substr(0, input.length()/2 + 1)), checker(input.substr(input.length()/2 + 1, input.length() - input.length()/2 - 1)));
		}
		if (result.first && result.second.length() == len){
			cout << result.second << endl;
		}
		
		return result;
	}

	else{
		pair<bool, string> result = resultCombiner(checker(input.substr(0, input.length()/2-1)), checker(input.substr(input.length()/2-1, input.length() - input.length()/2 + 1)));	
		if(!result.first){
			result = resultCombiner(checker(input.substr(0, input.length()/2 + 1)), checker(input.substr(input.length()/2 + 1, input.length() - input.length()/2 - 1)));
		}
		if(!result.first){
			result = resultCombiner(checker(input.substr(0, input.length()/2)), checker(input.substr(input.length()/2, input.length()/2)));
		}
		if (result.first && result.second.length() == len){
			cout << result.second << endl;
		}
		return result;
	}
}

int main(){
	getSortedTable();
	string input;
	getline(cin, input);
	pair <bool, string> result;

	result = checker(input);
	//cout << result.second;
	return 0;
}
