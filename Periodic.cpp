#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <cctype>
#include <utility>
#include <algorithm>
#include <set> 

using namespace std;

struct periodic{
	unsigned num; //number
	string sym;   //symbol
	string name;  //name
};

periodic sortedTable[26][12];	

//generates a periodic element from a single line in sorted.txt
periodic getElement(string element){
	periodic final;
	final.num = stoi(element);
	final.sym = isalpha(element[int(log10(final.num))+2]) ? element.substr(int(log10(final.num))+1, 2) : element.substr(int(log10(final.num))+1, 1)	;
	final.name = element.substr(element.find(" ")+1);
	return final; 
}

// makes a usable structure from sorted.txt
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

//Checks whether a 1 char string or 2 char string is in the Periodic Table
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

//Used by the checker() func to combine 2 results. return true only if both the inputs were possible.
pair<bool, string> resultCombiner(pair<bool, string> r1, pair<bool, string> r2){
	if (r1.first && r2.first){
		return make_pair(true, r1.second+r2.second);
	}
	return make_pair(false, "");
}

//the main checker function
pair<bool, string> checker(string input){
	//No elements in the periodic table with 'Q' or 'J'
	static bool QJcheck = false;
	if (!(input.find('q') == -1 || input.find('Q') == -1 || input.find('J') == -1 || input.find('j') == -1) && !QJcheck){ 
		QJcheck = true;
		return make_pair(false, "");
	}
	
	//Block of code which checks a single letter
	if (input.length() == 1){
		if (inTable(input)){
			input[0] = toupper(input[0]);
			
			return make_pair(true, input);
		}
		else{
			
			return make_pair(false, "");
		}
	}

	//Block of code which checks 2 letters 
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
	
	//The two following blocks of code are responsible for splitting the given word into 2 pieces. This 
	// is the recursive part of the function, a meet in the middle approach.

	if (input.length() % 2){ 
		pair<bool, string> result = resultCombiner(checker(input.substr(0, input.length()/2)), checker(input.substr(input.length()/2, input.length() - input.length()/2)));
		if(!result.first){
			result = resultCombiner(checker(input.substr(0, input.length()/2 + 1)), checker(input.substr(input.length()/2 + 1, input.length() - input.length()/2 - 1)));
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
		return result;
	}
}

//Uses code similar to inTable() func to return a periodic element
periodic getElementfromSym(string a){
	if (a.length() == 1){
		return sortedTable[int(a[0])-65][0];
	}

	else{
		char b = a[0], c = a[1];
		unsigned i = 0;
		
		while (true){
			if (sortedTable[int(b)-65][i].sym.length() == 2 && sortedTable[int(b)-65][i].sym[1] == c){
				return sortedTable[int(b)-65][i];
			}
			++i;
		}
	}
}	

//comparision fucntion used by std::set to generate a set of periodic elements, since periodic is a user-defined struct.
inline bool operator<(const periodic& lhs, const periodic& rhs){
	return lexicographical_compare(lhs.sym.begin(), lhs.sym.end(), rhs.sym.begin(), rhs.sym.end());
}

//generates a list of periodic elements used to make up the string.
set <periodic> elementsUsed(string word){
	set <periodic> elements;
	for(unsigned i = 0; i < word.length(); ++i){
		if (isupper(word[i]) && i == word.length() - 1){
			elements.insert(getElementfromSym(word.substr(i, 1)));
		}
		if (isupper(word[i]) && i < word.length()-1 && isupper(word[i+1])){
			elements.insert(getElementfromSym(word.substr(i, 1)));
		}
		if (isupper(word[i]) && i < word.length()-1 && islower(word[i+1])){
			elements.insert(getElementfromSym(word.substr(i, 2)));
		}  
	}
	return elements;
}

int main(){
	getSortedTable();
	
	string input;
	cout << "Enter the string to be tested : " << endl;
	getline(cin, input);
	cout << endl;
	pair <bool, string> result;

	result = checker(input);
	if (!result.first){
		cout << "No possible combination.";
	}
	else{
		cout << "A possible combination is : " << endl;
		cout << result.second << endl << endl;;
		cout << "Number\t\tSymbol\t\tElement" << endl;  
		for(auto element : elementsUsed(result.second)){
	 		cout << element.num << "\t\t" << element.sym << "\t\t" << element.name << endl;
		}
	}
	return 0;
}