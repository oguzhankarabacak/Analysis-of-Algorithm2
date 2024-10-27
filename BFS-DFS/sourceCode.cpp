#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream> 
#include <cmath>
#include <vector>
#include <time.h>
using namespace std; 

struct Node{  //Node Structure
	vector<int> numbers;  //for keep numbers
	vector<char> letters;  //for keep letters
	vector<Node*> childs;  //for keep childs(subtrees)

};
struct Q_node {  //my queue node
    Node* data;  //keep Node pointer
    Q_node* next;
    Q_node(Node* d){  //constructor
        data = d;
        next = NULL;
    }
};
  
struct Queue {  //queue structure
    Q_node *front, *last;  //front and last 
    int size;
    Queue(){ front = last = NULL; size=0;}  //constructor
  	Node* get_front() {
  		return front->data;
	  }
    void push(Node* x)  //queue push function 
    {
  		Q_node* temp = new Q_node(x);  //create new node 
		if (last == NULL) {  //if queue is empty 
            front = last = temp;
            size += 1;
            return;
        }
        last->next = temp;  //if not empty 
        last = temp; //last is added node
        size += 1;  //increment size
    }
  
    void pop()   //queue push function
    {
        if (front == NULL) return;  //if it is empty 

        Q_node* temp = front;  //temp keep front 
        front = front->next; 
        if (front == NULL)  //if is empty 
		{
			last = NULL;   
			size = 0;  
		 } 
        delete (temp);  //delete first node of queue
        size -=1;  //decrement size 
    }
    int get_size(){return size;	}
};

class Tree {
	  	Node* root;  //for root 
		int finish_df;  //is finish for df
		int finish_bf;  //is finish for bf
		int total_number_of_visited;  //visited node number 
		int total_letter_size;  //total layer size 
		string s1;
		string s2;
		string s3;
		vector<char> all_letters;
		Node* correct_node;
		int maks_node;
		int maks_node_dfs;
	  
	public:
		
		Tree(string s1_,string s2_,string s3_,vector<char> letters_){  //constructor
			this->correct_node=NULL;
		  	this->root=NULL; 
		  	this->maks_node=0;
		  	this->maks_node_dfs=0;
		  	this->total_number_of_visited=0;
		  	finish_df = -1;  //initiale -1 
		  	finish_bf = -1;
		  	this->s1=s1_;
		  	this->s2=s2_;
			this->s3=s3_;
			this->all_letters=letters_;
		}	  
		Node* get_correct_node(){	return this->correct_node;	}  //get correct_node 
		Node* get_root(){	return this->root;  }  //get root 
	 	int get_total_node_bf(){	return this->total_number_of_visited;  }
	 	int get_max_node(){ return	this->maks_node;}
	 	int get_max_node_dfs(){ return	this->maks_node_dfs;}
	  	void create_tree(vector<int>,Node*,int);  //create tree 
	  	void Dfs(Node*,int);  //for dfs 
	  	void Bfs(int,Node*);  //for bfs 
};
void Tree::create_tree(vector<int> number_list,Node* root_ , int letter_size){  //recursive function so second parameter is root_, 
	if (root==NULL){  //if tree is empty  create  new node for root 
		this->root=new Node;
		create_tree(root->numbers,root,0);
		return;
	}
	this->maks_node_dfs += 1;  //increment maks_node_dfs number 
	if(letter_size != this->all_letters.size()){	//if it is not in the last layer 
		for(int i=0;i<10-letter_size;i++){  //number of child node added to parent  
			Node* new_child=new Node;
			for(int k=0;k!=number_list.size();k++)	new_child->numbers.push_back(number_list[k]);  //take all numbers from parent node 
			
			vector<int> added_num;  
			int same=0;  
			
			for(int t=0;t<10;t++){   //dont added the number which same with parent numbers , first constraint if t=1 , w cannot be 1 
				for(int m=0;m != number_list.size();m++){
					if(t == number_list[m]){
						same=1;
						break;
					}
				}
				if (same == 0) added_num.push_back(t);  //if it is not same , add to node 
				same=0;
			}
			new_child->numbers.push_back(added_num[i]);
			for (int j=0;j<=letter_size;j++) 	new_child->letters.push_back(this->all_letters[j]);  //take letters from all_letters till layer number
			
			root_->childs.push_back(new_child);
			
			this->create_tree(root_->childs[i]->numbers,root_->childs[i],root_->childs[i]->letters.size());  //call recursive function again for node childs 
		}
	}		
}

int check(Node* root,string s1,string s2,string s3,int letter_size){  //for constraints and decryption of words
	int s1_len=s1.length();
	int s2_len=s2.length();
	int s3_len=s3.length();
	int total1=0;
	int total2=0;
	for(int j=0;j<s1_len;j++){
		for(int i=0;i<root->letters.size();i++){
			if(s1[j] == root->letters[i]){
				total1 += root->numbers[i]*pow(10,(s1_len-j-1));  //second constraints O+O = R + 10*C1 , C1 + W + W = U + 10*C2  for first word 
				if(j==0 && root->numbers[i]==0)	return -1; //for third constraint if t=0 or w=0 dont decrypt
				break;
			} 
		}
	}
	for(int j=0;j<s2_len;j++){
		for(int i=0;i<root->letters.size();i++){
			if(s2[j] == root->letters[i]){
				total2 += root->numbers[i]*pow(10,(s2_len-j-1)); //second constraints O+O = R + 10*C1 ,  C1 + W + W = U + 10*C2  for second word 
				if(j==0 && root->numbers[i]==0) return -1; //for third constraint if t=0 or w=0 dont decrypt
				break;
			} 
		}
	}
	int total=total1+total2;  //total sum of first two words 
	int last=0;
	for(int j=0;j<s3_len;j++){  //calculate result , decrypt result 
		for(int i=0;i<root->letters.size();i++){
			if(s3[j] == root->letters[i]){
				last += root->numbers[i]*pow(10,(s3_len-j-1));	 //second constraints O+O = R + 10*C1 ,  C1 + W + W = U + 10*C2 for thirdword 
				if(j==0 && root->numbers[i]==0) return -1;
				break;
			} 
		}
	}
	if(total==last){  //check word1+word2 is equal word3 
        int check_last=0;
        for(int i=0;i<root->letters.size();i++){  //second check first constraint , if t=1 , w cannot be 1 
          	for(int j=i+1;j<root->letters.size();j++){
            	if (root->numbers[i]==root->numbers[j])	return -1;
			}
    	}
		return 1; 
    } 
	else return -1;	
}


void Tree::Dfs(Node* root_,int letter_size){ //dfs function 
	if(root_ == NULL) return;  //if root is null 
	
	if(this->finish_df != 1){  //if cannot find correct node 
		if(root_->letters.size() == letter_size){  //if it is in last layer 
			this->finish_df=check(root_,this->s1,this->s2,this->s3,letter_size);  //check root_ , whether is correct answer 
			if(this->finish_df == 1){  //if find correct_node 
				this->correct_node=root_; //assign root_ to correct_node
				return;
			} 	
		}
		this->total_number_of_visited += 1;  //increment total_number_of_visited by 1 

		for(int i=0;i<root_->childs.size();i++)	Dfs(root_->childs[i],letter_size);	 //recall recursive function for all childs of node 
	}
}

void Tree::Bfs(int level,Node* root_){  //Bfs function
	
	if(this->root==NULL) return;  //if tree is empty 
	
	Queue q;
	q.push(this->root);  //add root to queue
	while(q.get_size()>0){  //if queue is not empty 
		this->total_number_of_visited += 1;	//increment by 1

		Node* current_node = q.get_front();  //assign first element of queue 
		if(current_node->letters.size() == level){  //if it is in last layer 
			this->finish_bf=check(current_node,this->s1,this->s2,this->s3,this->total_letter_size);  //if is correct node return 1 
			if(this->finish_bf == 1) {
				this->correct_node=current_node;
				break;
			}
		}
		if (q.get_size()>this->maks_node) this->maks_node=q.get_size();  //if q.size is bigger , it is new maks_node_number in memory  
		for(int i=0;i<current_node->childs.size();i++){  //apply for childs of node 
			if(current_node->childs[i] != NULL){
				q.push(current_node->childs[i]);
			} 
		}
		q.pop();
	}
}


vector<char> split_string(string s1,string s2,string s3){  //split string 
	vector <char> letters;
	letters.push_back(s1[0]);
	int same=0;
	for(int i=1;i != s1.length();i++){
		for(int j=0;j<letters.size();j++){
			if(s1[i]==letters[j]){  //check letters is exists
				same=1;
				break;
			} 
		}
		if (same == 0) letters.push_back(s1[i]);
		same=0;
	}
	same = 0;
	for(int i=0;i != s2.length();i++){  //for second word 
		for(int j=0;j<letters.size();j++){
			if(s2[i]==letters[j]){ //check letters is exists
				same=1;
				break;
			} 
		}
		if (same == 0) letters.push_back(s2[i]);
		same=0;

	}
	same=0;
	for(int i=0;i != s3.length();i++){  //for third word 
		for(int j=0;j<letters.size();j++){ //check letters is exists
			if(s3[i]==letters[j]){
				same=1;
				break;
			} 
		}
		if (same == 0) letters.push_back(s3[i]);
		same=0;
	}
	return letters;
}

int main(int argc,char **argv){
	
    vector<char> let;
    string type=argv[1];
    string s1=argv[2];
    string s2=argv[3];
    string s3=argv[4];
    string file_type=".txt";
    string file_p=argv[5];
    string file_name=file_p+file_type;
    
    let=split_string(s1,s2,s3);  //first split words and adds letter to let array 
    
    vector<int> number_list;
	Node* a;

	Tree t(s1,s2,s3,let);
    t.create_tree(number_list,a,0);  //create tree 
    Node* root=t.get_root();
    
	clock_t ti;  //for runtime
	
    if (type == "bfs" || type == "BFS" || type == "Bfs" ){  //if type is bfs 
    	ti = (double)clock(); //we take begin time 
    	t.Bfs(let.size(),t.get_root());  //search with bfs 
    	ti =clock()-(double)ti;  //end-begin time  
	}
	else if(type == "dfs" || type == "DFS" || type == "dfs"){ //if type is bfs 
		ti = (double)clock(); //we take begin time 
		t.Dfs(t.get_root(),let.size());  //search with dfs 
		ti =clock()-(double)ti;  //end-begin time 
	}
	else{
		cout << "Invalid Search Type " << endl;
	}
	Node* correct_node=t.get_correct_node();
    ofstream filep; //file object
  	filep.open(file_name.c_str()); //open file
  	filep << "Algorithm: " << type << "\n" ;
  	filep << "Number of the visited nodes: " << t.get_total_node_bf() << "\n";
  	if (type == "bfs" || type == "BFS" || type == "Bfs" ) filep << "Maximum number of nodes kept in the memory: " << t.get_max_node() << "\n";
  	else filep << "Maximum number of nodes kept in the memory: " << t.get_max_node_dfs() << "\n";
  	filep <<"Running Time: " << fixed << (double)ti/CLOCKS_PER_SEC << " seconds \n";
  	filep << "Solution: ";

  	for(int i=0;i<correct_node->numbers.size();i++){  //write correct numbers and letters 
  		filep <<  correct_node->letters[i] << ": " <<correct_node->numbers[i];
  		if(i != correct_node->numbers.size()-1) filep << ", ";
	}
  	
  	filep.close();
	return 0;
}














