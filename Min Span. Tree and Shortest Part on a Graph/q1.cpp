#include<iostream>
#include<stdio.h>
#include<map>
#include <utility>
#include <iterator>
#include <fstream>
#include <sstream>

//Oguzhan Karabacak 150170021
using namespace std;
#define INF 10000000

class Graph
{
		multimap< string, pair<string,int> > adj;  //to create graph 
		multimap< pair<string,string> , int > order_local ;		//to save first order 
	public:
		void addEdge(string u, string v, int w);
		void FindPrimMST(string);  //find minimum spannig tree function 
		void printMST(map<string,string> , map<string,int> );  //print Mst
		
		
};
void Graph::addEdge(string u,string v,int w){
	order_local.insert(make_pair(make_pair(u,v),w));
	adj.insert(make_pair( u,make_pair(v,w) ));  //graph is undirected , so a-b,b-a to graph 
	adj.insert(make_pair(v,make_pair(u,w)));
};

void Graph::printMST(map<string,string> parent, map<string,int> key) //print function, neighbour(parent and child vertices) and its weight 
{ 
	multimap< int, pair<string,string> > last;
    map<string,string>::iterator itr;
    multimap< pair<string,string> , int >::iterator order_itr;
	int total=0;  //firstl total is zero 
    for (itr=parent.begin();itr != parent.end(); itr++){
    	if(itr->first != "None" && itr->second != "None") {  //if dont access the  root 
    		if(this->order_local.find(make_pair(itr->second,itr->first)) != this->order_local.end()){  //vertices is added to last map , this is check, direct is same in first order 
    			last.insert(make_pair(key.find(itr->first)->second,make_pair(itr->second,itr->first)));
			}
			else{
				last.insert(make_pair(key.find(itr->first)->second,make_pair(itr->first,itr->second))); //vertices is added to last map 
			}
		}
	 
	} 
    multimap< int, pair<string,string> >::iterator last_itr;
    for(last_itr=last.begin();last_itr != last.end();last_itr++){
    	total += last_itr->first;  //added to total 
    	cout << last_itr->second.first << " " << last_itr->second.second << " " << last_itr->first << endl;  //print vertices 
	}
	cout << total << endl;  //print total 
	cout << endl;
} 

string minKey(map<string,int> key, map<string,bool> mstSet, map<string,string> parent)  {   //find the nearest child 
    int min = INF;
	string min_v; 
  	map<string,int>::iterator itr;
  	for(itr=key.begin();itr!=key.end();itr++){
		if(mstSet[itr->first] == false && key[itr->first] < min){  //if child is not in mst and its the nearest to parent 
			min=key[itr->first];  //change min 
			min_v=itr->first; 
		}
	}  
	return min_v; 
} 

void Graph::FindPrimMST(string src) {  //FindPrimMST  function  
//	string src="GP";
	map<string,pair<string,int> >::iterator itr1;
    map<string,string> parent;
	for(itr1=adj.begin();itr1 != adj.end();itr1++){ //first mst is empty , there is no mst 
		parent[itr1->first]="None";
		parent[itr1->second.first]="None";
	}
      
    map<string,int> key;
	for(itr1=adj.begin();itr1 != adj.end();itr1++){  //first all keys is infinite 
		key.insert(make_pair(itr1->first,INF));
		key.insert(make_pair(itr1->second.first,INF));
	}
    map<string,bool> mstSet;  //mstSet is empty in first 
    for(itr1=adj.begin();itr1 != adj.end();itr1++){
		mstSet.insert(make_pair(itr1->first,false));
		mstSet.insert(make_pair(itr1->second.first,false));
	}
	    key[src] = 0; //src distance is 0 
    parent[src] = "None"; // first vertex  is root of MST 
  	string opt_church="None";  //is the nearest church 
        	
	map<string, pair<string,int> >::iterator itr2_church;
    int min=INF;  //first min in infinite 
    for(itr2_church = adj.find(src); itr2_church != adj.end() ; itr2_church++){
        if( itr2_church->first == "GP" && itr2_church->second.first.substr(0,2) == "Ch"){  //if gp and ch are  finded 
        	if(itr2_church->second.second < min){  //if find the smaller change min
        		opt_church=itr2_church->second.first;  //change the nearest church 
        		min=itr2_church->second.second;  //change distance 
			}
		}
	}
 	parent[opt_church]=src;  //church parent is GP 
	key[opt_church]=min; 
		 	
	for(itr2_church=adj.find(opt_church);itr2_church != adj.end();itr2_church++){   //take all distance value of childs of opt_church and change parent and key value
		if(itr2_church->first == opt_church  && itr2_church->second.second < key[itr2_church->second.first] ){  
		 	parent[itr2_church->second.first]=itr2_church->first;   //change all near values to opt_church
		 	key[itr2_church->second.first] = itr2_church->second.second;
		}
	}
		
	string hi;
	map<string, pair<string,int> >::iterator itr2_hipp;
	for(itr2_hipp = adj.find(src); itr2_hipp != adj.end(); itr2_hipp++ ){
		if (itr2_hipp->first == "GP" && itr2_hipp->second.first.substr(0,4)=="Hipp"){  // hipp and gp connection directly 
			parent[itr2_hipp->second.first]=src;
		  	key[itr2_hipp->second.first]=itr2_hipp->second.second;
		  	hi=itr2_hipp->second.first;
		}
	}
			  
	for(itr2_hipp=adj.find(hi);itr2_hipp != adj.end();itr2_hipp++){   //take all distance value of childs of Hipp  and change parent and key value
		if(itr2_hipp->first == hi  && itr2_hipp->second.second < key[itr2_hipp->second.first] ){
		 	if(itr2_hipp->first == opt_church || itr2_hipp->second.first == opt_church) continue;
		 		if((itr2_hipp->second.first.substr(0,3) == "Bas" && itr2_hipp->first.substr(0,4)=="Hipp"  ) || (itr2_hipp->second.first.substr(0,4) == "Hipp" && itr2_hipp->first.substr(0,3)=="Bas"  )   ) continue;  //if child is Bas, Parent is Hipp , or child is Hipp , parent is Bas
				parent[itr2_hipp->second.first]=itr2_hipp->first;
		 		key[itr2_hipp->second.first] = itr2_hipp->second.second;
		}
	}
    for (unsigned int count = 0; count < parent.size()-1; count++){ 
        string u = minKey(key, mstSet,parent); 
        mstSet[u] = true;
		map<string, pair<string,int> >::iterator itr2;
		for(itr2=adj.find(u);itr2 != adj.end();itr2++){
			if(mstSet[itr2->second.first]==false && itr2->second.second < key[itr2->second.first] && itr2->first == u ){
				if(itr2->second.first == opt_church) continue;  //if vertex is the first connection church 
				if(itr2->second.first.substr(0,4) == "Hipp" && parent[itr2->second.first] == "GP") continue;  //if Hipp(child of GP ) and GP is connection, dont change 
				if(itr2->second.first.substr(0,2) == "Hp" && u.substr(0,2) == "Hp") continue;   //if hp and hp is connection, dont change 
				if((itr2->second.first.substr(0,3) == "Bas" && u.substr(0,4)=="Hipp"  ) || (itr2->second.first.substr(0,4) == "Hipp" && u.substr(0,3)=="Bas"  )   ) continue;  //if child is Bas, Parent is Hipp , or child is Hipp , parent is Bas
				parent[itr2->second.first]=u;  //parent of itr2->second.first is u 
				key[itr2->second.first]=itr2->second.second; //key of itr2->second.first is vertex weight with parent 
			}
		}
	} 
    printMST(parent,key);  //print tree 
}
int main()
{
	string fname;
	cin >> fname;
	ifstream city_plan(fname.c_str());  //read file 
	
	
	Graph g;
	string startVertex = "GP";  //startVertex 
	string loca1;
	string loca2;
	string cost_s;
	int cost;
	if(!city_plan){
		cerr << "File cannot be opened!";  //if file cannot open
		return 0;
	}
	while(!city_plan.eof()){
		getline(city_plan,loca1,',');
		getline(city_plan,loca2,',');
		getline(city_plan,cost_s,'\n');
		stringstream s1(cost_s);
		s1 >> cost;
		g.addEdge(loca1,loca2,cost);  //add to graph 
	}
		g.FindPrimMST(startVertex);  //FindPrimMST

    return 0;
}
