#include <iostream>
#include <stdlib.h>
#include <fstream>

class listNode{
	public:
	std::string data;
	listNode* next;
	
	listNode(std::string data){
		this->data = data;
		this->next = nullptr;
	}
};
class hashTable{
	public:
	std::string data;
	int bucketSize;
	listNode** hashtable;
	
	hashTable(int bucketSize){
		this->bucketSize = bucketSize;
		this->hashtable = new listNode*[bucketSize];
		for(int i=0;i<bucketSize;i++){
			this->hashtable[i] = new listNode("dummy");
		}
	}
	int hashString(std::string data, int bucketSize){
		long unsigned int val = 1;
		int length = data.length();
		int i = 0;
		while(i < length){
			val = val*32+(int)data[i];
			i++;
		}
		return val%this->bucketSize;
	}
	void hashInsert(int index, std::string data, std::ofstream& out1, std::ofstream& out2){
		listNode* spot = this->findSpot(index, data);
		if(spot->next != nullptr && spot->next->data == data){
			out2<<"Data is already in table, no insertion takes place\n";
		}
		else{
			listNode* newNode = new listNode(data);
			newNode->next = spot->next;
			spot->next = newNode;
			this->printList(out2);
		}
	}
	listNode* findSpot(int index, std::string data){
		listNode* spot = this->hashtable[index];
		while(spot->next != nullptr && spot->next->data < data){
			spot = spot->next;
		}
		return spot;
	}
	void printList(std::ofstream& output){
		for(int i=0;i<this->bucketSize;i++){
			listNode* reader = this->hashtable[i];
			output<<"hashTable["<<i<<"]:";
			while(reader->next != nullptr){
				output<<"("<<reader->data<<","<<reader->next->data<<")->";
				reader = reader->next;
			}
			output<<"\n";
		}
		output<<"~~~~~~~~~~~~~~~~~~~\n";
	}
};
int main(int argc, char* argv[]){
	if(argc != 4){
		std::cout<<"You must include 3 arguments (inFile, outFile1, outFile2)"<<std::endl;
		return -1;
	}
	std::ifstream inFile(argv[1]);
	if(!inFile.good()){
		std::cout<<"Invalid input file."<<std::endl;
		return -1;
	}
	std::ofstream out1;
	std::ofstream out2;
	out1.open(argv[2],std::ios::out);
	out2.open(argv[3],std::ios::out);
	
	hashTable* ht = new hashTable(29);
	ht->printList(out2);
	std::string data;
	while(inFile >> data){
		int index = ht->hashString(data, 29);
		ht->hashInsert(index, data, out1, out2);
		ht->printList(out2);
	}
	ht->printList(out1);
	inFile.close();
}