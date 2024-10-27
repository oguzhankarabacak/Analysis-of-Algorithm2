/* 
Oðuzhan Karabacak
150170021
compile : g++ -std=c++11 -Wall -Werror 150170021.cpp -o 150170021
run : ./150170021 <input_file_name>.txt <output_file_name>.txt     
run type : second type calico (e2-files_as_main_parameters.t)
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;


class Smith{  //Smith-Waterman class 
	private:
		int match;  //match number 
		int unmatch;  //unmatch number 
		int indel;  //gap penalty number 
		int score;  //total score 
	public:
		Smith(int,int,int);  //constructor 
		int findScore(char, char);  //for calculate cell of matrix 
		int findMax(int array[], int length); // to find Max score 
		vector<string> findCommon(string,string); //find common sequences 
		vector<string> printCommon(string,string);  //for print 
		int getScore(){  //getscore function
			return this->score;
		};
		
};
Smith::Smith(int ma,int unma,int gap){  //constructor 
	match=ma;  //match number 
	unmatch=unma; //unmatch number
	indel=gap; //gap penalty
	score=0;  //first score 
}

int Smith::findScore(char a, char b)
{
	int result;
	if(a==b) result=this->match;  //match  //if match return match number 
	else result=this->unmatch;  //unmatch //if unmatch return unmatch number 
	
	return result;
}

int Smith::findMax(int array[], int length){  //find max score 
	int max = array[0];

	for(int i=1; i<length; i++){  //traverse all matrix 
		if(array[i] > max)	max = array[i];
	}
	return max;  //return max 
}

vector<string> Smith::findCommon(string s1,string s2){  //for find most common sequences
	
	int s1_len = s1.length(); // initialize some variables
	int s2_len = s2.length();
	
	int matrix[s1_len+1][s2_len+1]; // initialize matrix, all entries is 0 initially 
	for(int i=0;i<=s1_len;i++){
		for(int j=0;j<=s2_len;j++)	matrix[i][j]=0;
	}

	int neighbors[4];
	
	for (int i=1;i<=s1_len;i++) {	
		for(int j=1;j<=s2_len;j++) {
			neighbors[0] = matrix[i-1][j-1]+findScore(s1[i-1],s2[j-1]);  //calculate up-left entry + unmatch/match using findScore function 
			neighbors[1] = matrix[i-1][j]+this->indel; //left entry + gap penalty 
			neighbors[2] = matrix[i][j-1]+this->indel; //up entry + gap penalty
			neighbors[3] = 0;  //last value is 0 
			matrix[i][j] = findMax(neighbors,4);  //find max value and assign to current entry 
		}       
	}
	
	int matrix_max = 0;  //find max score 
	int i_max=0, j_max=0;  //max score indexes 
	for(int i=0;i<s1_len+1;i++) {
		for(int j=0;j<s2_len+1;j++)	{  //traverse all matrix 
			if(matrix[i][j]>matrix_max)	{
				matrix_max = matrix[i][j];
				i_max=i;
				j_max=j;
			}
		}
	}
	int size = 1;  	//how many most common
	vector<int> first;  //first indexes of matrix for max score 
	first.push_back(i_max);
	first.push_back(j_max);
	vector<vector<int> > locations_max;
	locations_max.push_back(first);  //add to vector first max score
	int i_max2=0, j_max2=0;
	for(int i=0;i<s1_len+1;i++){
		for(int j=0;j<s2_len+1;j++) {  //traverse all matrix and find score which is equal to max score 
			if(matrix[i][j]==matrix_max && i_max != i && j_max != j) {
				size += 1;
				i_max2=i;
				j_max2=j;
				vector<int> a;
				a.push_back(i_max2);
				a.push_back(j_max2);
				locations_max.push_back(a);  //add to vector
			}
		}
	}
	vector<string> common_seq;  //common sequences vector
//	cout << "Score: " << matrix_max << " ";  //print score 

	score=matrix_max;
	if(matrix_max == 0){  //if matrix is 0 , return empty common_seq 
		return common_seq;
	}
	for(int i=0;i<size;i++){  //loop by size number for all common sequences 
		
		vector<int>  st_index;
		int current_i=locations_max[i][0];
		int current_j=locations_max[i][1];
		st_index.push_back(current_i-1);
		while (matrix[current_i][current_j] != 0 ){  //traceback
			current_i = current_i-1;
			current_j = current_j-1;	
			if (matrix[current_i][current_j] != 0)	st_index.push_back(current_i-1);  //if matrix is not zero add to traceback vector 
		}
		string total_com="";
		for(int i=st_index.size()-1;i != -1; i--){
			total_com += s1[st_index[i]];  //add to all letter in traceback path 
		} 
		common_seq.push_back(total_com);  //add to vector 
	}
	return common_seq;
	
}
void removeDuplicates(vector<string>& vec) {  //for unique 
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
}
vector<string> Smith::printCommon(string s1,string s2){  //to print all most common sequences 
//	if(s1>s2)	cout << s2 << " - " << s1 << "\n";  //first print words 
//	else cout << s1 << " - " << s2 << endl;
	
	vector<string> com_arr=this->findCommon(s1,s2);  //call findCommon function for words 
	removeDuplicates(com_arr);  //remove duplicate values 
//	if(com_arr.empty()) cout << "Sequence(s):";
//	else cout << "Sequence(s): ";
	
//	for(unsigned int i=0;i!=com_arr.size();i++){  //print all common seq.
//		if(i>0)	cout << " ";
//		cout << '"'<< com_arr[i] << '"';
//	}
//	cout << "\n";
	return com_arr;  //return 
}

int main(int argc, char **argv)
{
	ifstream file;
	file.open(argv[1]);  //open file
	if (!file){
		cerr << "File cannot be opened!" << endl;;  //if file cannot open
		exit(1);
	}
	vector<string> words;
	
	string s2;
	while(!file.eof()){
		getline(file,s2,'\n');
		words.push_back(s2);
	}
//	int pr1=atoi(argv[3]);
//	int pr2=atoi(argv[4]);
//	int pr3=atoi(argv[5]);
//  Smith s(pr1,pr2,pr3);
	Smith s(1,-2,-4);  //create object 
	vector<string> ret;
	sort(words.begin(),words.end());  //first sort all words 
	file.close();
	ofstream filep;
	filep.open(argv[2]);
	for(unsigned int i=0;i<words.size();i++){
		for(unsigned int j=i+1;j<words.size();j++){
			if (words[i] > words[j]) filep << words[j] << " - " << words[i] << "\n";  //print to file 
			else filep << words[i] <<" - " << words[j] << "\n";  
			ret = s.printCommon(words[i] ,words[j] );
			filep << "Score: " << s.getScore() << " ";
			if(ret.empty())	filep << "Sequence(s):";
			else filep << "Sequence(s): ";
			for(unsigned int i=0;i!=ret.size();i++){  //for print to file 
				if(i>0)	filep << " ";
				filep << '"'<< ret[i] << '"';
			}
			filep << "\n";
		}
	}
	filep.close();
	return 0;
}


