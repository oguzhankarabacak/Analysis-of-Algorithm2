#include<iostream>
#include<stdio.h>
#include<map>
#include <utility>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
//Oguzhan Karabacak 150170021
using namespace std;
#define INF 10000000


class Graph
{
		multimap< string, pair<string,int> > adj;   //for keeping graph 
		multimap< pair<string,string> , int > order_local ;		 //save the first read locasyon
	public:
		void addEdge(string u, string v, int w);  //create graph function 
		void dijkstra(string);  //find shortest path 
		int check_5(string);    //check enemy spot is smaller than 5 
		void printPath(map<string,string> , map<string,int> );  //for print 
};
void Graph::addEdge(string u,string v,int w){
	order_local.insert(make_pair(make_pair(u,v),w));   //first store in order_local to the first read 
	adj.insert(make_pair( u,make_pair(v,w) ));         // graph is undirect so added two times, a-b, b-a 
	adj.insert(make_pair(v,make_pair(u,w)));
};

string minDistance(map<string,int> dist, map<string,bool> sptSet, map<string,string> parent) //dist store distances, sptSet is visited , parent is verteces
{ 
    int min = INF;  //create first INF
	string min_ve;    
  	map<string,int>::iterator itr;
  	for(itr=dist.begin();itr!=dist.end();itr++){   //find the shortest vertex of current vertex 
		if(sptSet[itr->first] == false && dist[itr->first] <= min){  //if finded 
			min=dist[itr->first];   //new min distance 
			min_ve=itr->first;   //new min_index is shortest vertex 
		}
	}  
	return min_ve; 
} 

int Graph::check_5(string s1){
	
	map<string,pair<string,int> >::iterator itr1;
	for(itr1=this->adj.begin();itr1 != adj.end();itr1++){
		if(itr1->first == s1){
			if(itr1->second.first.substr(0,1)=="E") return itr1->second.second;  //if exist neighbour vertex ,return this distance 
		}
	}
	return 6;  //if not exist return 6 , 6 is not important 
}

void Graph::printPath(map<string,string> parent, map<string,int> dist) {  //to print path, parent is vertices and distances between 
	map<string,string>::iterator prt_itr;
	map<string,int>::iterator dist_itr;
	
	vector<string > path;
	int total=0;  //total cost 
	string target="Mo";  //the target 
	for(prt_itr=parent.begin();prt_itr!=parent.end();prt_itr++){
		if(prt_itr->first == target){  //if target is finded 
			path.push_back(prt_itr->first);  //add path 
			total += dist.find(target)->second;
			target=prt_itr->second;  //new target is neighbour of old target 
			prt_itr=parent.begin();  //and start again 
		}
	}
	for(int i=path.size()-1;i != -1 ;i--) cout << path[i] << " ";  //print vertices 
	cout << total << endl;  //print total 

}

void Graph::dijkstra(string src) 
{ 
//	string src="Ma";
	map<string,pair<string,int> >::iterator itr1;
    map<string,string> parent;
	for(itr1=adj.begin();itr1 != adj.end();itr1++){  //first there is nothing in shortest path  
		parent[itr1->first]="None";
		parent[itr1->second.first]="None";  //None is mean that , has not neighbour vertex 
	}
      
    map<string,int> dist;
	for(itr1=adj.begin();itr1 != adj.end();itr1++){
		dist.insert(make_pair(itr1->first,INF));  //first all vertex is infinite by 'Ma' 
		dist.insert(make_pair(itr1->second.first,INF));  //a-b=infinite  , b-a=infinite
	}
    map<string,bool> sptSet;
    for(itr1=adj.begin();itr1 != adj.end();itr1++){  //first shortest path is empty , visited is empty 
		sptSet.insert(make_pair(itr1->first,false));
		sptSet.insert(make_pair(itr1->second.first,false));
	}
    dist[src] = 0; //src distance is 0 
    parent[src] = "None"; // First node is root of shortest path 
  	
    for (unsigned int count = 0; count < parent.size()-1; count++)
    { 
        string u = minDistance(dist, sptSet,parent);   //get nearest neighbour 
        sptSet[u] = true; //save in visited 
		multimap<string, pair<string,int> >::iterator itr2;
		
		for(itr2=adj.find(u);itr2 != adj.end();itr2++){
			//if not visited and parent is min distance  and distance is not infinite and total weight is smaller than old weight 
			if(sptSet[itr2->second.first]==false && itr2->first == u &&  dist[u] != INF && (dist[u] + itr2->second.second ) < dist[itr2->second.first]  ){
				
				if(itr2->second.first.substr(0,1)=="E") continue;  //if vertex is enemy , do not anything, turn loop 
				if(this->check_5(itr2->second.first)<5)	continue;  //if enemy is smaller than 5 , do not anyting turn loop  
				parent[itr2->second.first]=u;   //parent of vertex is min distance 
				dist[itr2->second.first]=itr2->second.second;  //distance is changed  
			}
		}
    } 
    this->printPath(parent,dist);  //finally print 
}
int main()
{
	string fname;
	cin >> fname;
	ifstream city_plan(fname.c_str());  //get file 
	
	Graph g;
	string startVertex = "Ma";  //start vertex 
	string loca1;
	string loca2;
	string cost_s;
	int cost;
	if(!city_plan){
		cerr << "File cannot be opened!";  //if file cannot open
		return 0;
	}
	while(!city_plan.eof()){
		getline(city_plan,loca1,',');  //get loca1 
		getline(city_plan,loca2,',');  //get loca2  
		getline(city_plan,cost_s,'\n');  //get weight  
		stringstream s1(cost_s);
		s1 >> cost;
		g.addEdge(loca1,loca2,cost);   //add edge 
	}
	g.dijkstra(startVertex);  //start dijkstra 
    return 0;
}

