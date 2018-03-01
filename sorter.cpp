#include <fstream>
#include <string>

using namespace std;

bool comp(string a, string b){
		if (a[0] > b[0]){
			return true;
		}
		else if (a[0] == b[0]){
			if (a[1] > b[1]){
				return true;
			}
			else return false;
		}
		else return false;
}
int main(){
	struct table{
		string name;
		unsigned num;
	} sorted[118];
	
	string line;
	ifstream periodic ("periodic.txt");
	unsigned num = 1;
	
	if (periodic.is_open()){
		
		while (getline(periodic, line)){
			sorted[num-1].name = line;
			sorted[num-1].num = num;
			++num; 
		
		}
		periodic.close();
	}

	for(unsigned i = 0; i < 118; ++i){
		int j = i - 1;
		string tempName = sorted[i].name;
		unsigned tempNum = sorted[i].num;
		while(j >= 0 && !(comp(tempName, sorted[j].name))){
			sorted[j+1].name = sorted[j].name;
			sorted[j+1].num = sorted[j].num;
			--j;
		}
		sorted[j+1].name = tempName;
		sorted[j+1].num = tempNum;
	} 		

	ofstream sortedTable ("sorted.txt");

	if (sortedTable.is_open()){
		
		for(unsigned i = 0; i < 118; ++i){
			if (i != 0 && (sorted[i-1].name[0] != sorted[i].name[0])){
				sortedTable << endl;
			}
			sortedTable << sorted[i].num << sorted[i].name << endl;
		}
		sortedTable.close();
	}
	return 0;
}