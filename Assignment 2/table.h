/*************************************************************/
/*                                                           */
/*    starter file for a2                                    */
/*    v 1.1: removed SimpleTable::grow() function def.       */
/*                                                           */
/*************************************************************/

#include <iostream>
#include <string>
#include <utility>
using namespace std;

template <class TYPE>
class Table{
public:
	Table(){}
	virtual bool update(const string& key, const TYPE& value)=0;
	virtual bool remove(const string& key)=0;
	virtual bool find(const string& key, TYPE& value)=0;
	virtual int numRecords() const = 0;
	virtual bool isEmpty() const = 0;
	virtual ~Table(){}
};

template <class TYPE>
class SimpleTable:public Table<TYPE>{

	struct Record{
		TYPE data_;
		string key_;
		Record(const string& key, const TYPE& data){
			key_=key;
			data_=data;
		}

	};

	Record** records_;   //the table
	int max_;           //capacity of the array
	int size_;          //current number of records held
	int search(const string& key);

public:
	SimpleTable(int capacity);
	SimpleTable(const SimpleTable& other);
	SimpleTable(SimpleTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other);
	virtual const SimpleTable& operator=(SimpleTable&& other);
	virtual ~SimpleTable();
	virtual bool isEmpty() const{return size_==0;}
	virtual int numRecords() const{return size_;}
};


//returns index of where key is found.
template <class TYPE>
int SimpleTable<TYPE>::search(const string& key){
	int rc=-1;
	for(int i=0;i<size_;i++){
		if(records_[i]->key_==key){
			rc=i;
		}
	}
	return rc;
}


template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int capacity): Table<TYPE>(){
	records_=new Record*[capacity];
	max_=capacity;
	size_=0;
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other){
	records_=new Record*[other.max_];
	max_=other.max_;
	size_=0;
	for(int i=0;i<other.size_;i++){
		update(other.records_[i]->key_,other.records_[i]->data_);
	}
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other){
	size_=other.size_;
	max_=other.max_;
	records_=other.records_;
	other.records_=nullptr;
	other.size_=0;
	other.max_=0;
}

template <class TYPE>
bool SimpleTable<TYPE>::update(const string& key, const TYPE& value){
	int idx=search(key);
	if(idx==-1){
		if(size_ < max_){
			records_[size_++]=new Record(key,value);
			for(int i=0;i<size_-1;i++){
				for(int j=0;j<size_-1-i;j++){
					if(records_[j] > records_[j+1]){		
						TYPE tmp=records_[j];
						records_[j]=records_[j+1];
						records_[j+1]=tmp;
					}
				}
			}
		}
	}
	else{
		records_[idx]->data_=value;
	}
	return true;
}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key){
	int idx=search(key);
	if(idx!=-1){
		delete records_[idx];
		for(int i=idx;i<size_-1;i++){
			records_[i]=records_[i+1];
		}
		size_--;
		return true;
	}
	else{
		return false;
	}
}

template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value){
	int idx=search(key);
	if(idx==-1)
		return false;
	else{
		value=records_[idx]->data_;
		return true;
	}
}

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other){
	if(this!=&other){
		if(records_){
			int sz=size_;
			for(int i=0;i<sz;i++){
				remove(records_[0]->key_);
			}
			delete [] records_;
		}
		records_=new Record*[other.max_];
		max_=other.max_;
		size_=0;
		for(int i=0;i<other.size_;i++){
			update(other.records_[i]->key_,other.records_[i]->data_);
		}

	}
	return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other){
	swap(records_,other.records_);
	swap(size_,other.size_);
	swap(max_,other.max_);
	return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable(){
	if(records_){
		int sz=size_;
		for(int i=0;i<sz;i++){
			remove(records_[0]->key_);
		}
		delete [] records_;
	}
}

template <class TYPE>
class LPTable:public Table<TYPE>{
	
	struct Record {
		TYPE data_;
		string key_;
		
		Record(const string& key, const TYPE& data) {
			key_ = key;
			data_ = data;
		}

	};

	Record** records_;   //the table
	int max_;           //capacity of the array
	int volume_;
	int size_;          //current number of records held
	int search(const string&key);
	hash<string> hashFunction;
public:
	LPTable(int capacity,double maxLoadFactor);
	LPTable(const LPTable& other);
	LPTable(LPTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const LPTable& operator=(const LPTable& other);
	virtual const LPTable& operator=(LPTable&& other);
	virtual ~LPTable();
	virtual bool isEmpty() const { return size_ == 0; }
	virtual int numRecords() const { return size_; }
};

//Search function
//takes in key as argument
//returns index 
template <class TYPE>
int LPTable<TYPE>::search(const string& key) {

	//hash index of key
	int index_ = hashFunction(key) % max_;
	int loop_ = 0;

	//loops through from the hash index until find key or go through table once
	while (records_[index_] != nullptr && loop_ != max_) {

		if (records_[index_]->key_ == key) {
			return index_;
		}
		loop_++;
		index_= (index_+ 1) % max_;

	}

	return index_;


}

//constructor
//takes 2 arguments
//initialization 
template <class TYPE>
LPTable<TYPE>::LPTable(int capcity,double maxLoadFactor): Table<TYPE>(){

	volume_ = capcity * maxLoadFactor;
	records_ = new Record* [capcity];
	max_ = capcity;

	for (int i = 0; i < max_; i++) {
		records_[i] = nullptr;
	}

	size_ = 0;
}

//copy constructor
//takes an argument
//assign data from passed in object to new Record table
template <class TYPE>
LPTable<TYPE>::LPTable(const LPTable<TYPE>& other){

	records_ = new Record*[other.max_];
	max_ = other.max_;
	size_ = 0;

	//copy everything over to current record
	for (int i = 0; i < other.max_; i++) {

		if (other.records_[i] != nullptr) {
			records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
		}

		size_++;
	}

}

//move constructor
//set records_ to point at passed object record
//set the max, and size value to the same as passed object
//set the passed object properties to null and 0 afterwards
template <class TYPE>
LPTable<TYPE>::LPTable(LPTable<TYPE>&& other){

	max_ = other.max_;
	size_ = other.size_;
	records_ = other.records_;

	other.max_ = 0;
	other.size_ = 0; 
	other.records_ = nullptr;

}

//update
//take 2 argument, key and value
//Both add and update return true
//update record value if found matching key in table 
//or add new record if it doesn't exist
//Return false if table is full and key not found
template <class TYPE>
bool LPTable<TYPE>::update(const string& key, const TYPE& value){
	
	int index_ = search(key);

	//if size reaches max load 
	if (size_ == volume_) {

		//if key is not found when size reach limit
		if (records_[index_] == nullptr) {
			return false;
		}

		//matching key in search function (called above), replace the data with the value passed in
		else {

			records_[index_]->data_ = value; //if search found key
			return true;

		}

	}
	//if record don't exist , and there is free space, record with key-value added 
	else if (records_[index_] == nullptr) {

		records_[index_] = new Record(key, value);
		size_++;	
		
		return true;

	}
	
	
}

//remove
//takes a key as argument remove record that has same key
//after deletion find a hash key equal or less then deleted item to fill spot
template <class TYPE>
bool LPTable<TYPE>::remove(const string& key){

	int index_ = search(key);
	int tempIdx_;
	int loop = 0;

	//if key was found in table, delete record at index with key
	if (records_[index_] != nullptr) {

		tempIdx_ = index_;
		delete records_[tempIdx_];
		records_[tempIdx_] = nullptr; //empty index now
		size_--;
		index_ = (index_ + 1) % max_; //move to next index

		//continous loop until a record is null or loop through once(end of array)
		while (records_[index_] != nullptr && loop != max_) {

			int newIdx_ = search(records_[index_]->key_);

			//compare empty space index to current record hash index
			//move record to empty spot
			//delete record of newly found data at old index
			if (newIdx_ != index_ && tempIdx_ >= newIdx_) {

					records_[tempIdx_] = new Record(records_[index_]->key_, records_[index_]->data_);
					tempIdx_ = index_;
					delete records_[tempIdx_];
					records_[tempIdx_] = nullptr;
					size_--;

			}

			index_ = (index_ + 1) % max_;
			loop++;

		}

		return true;

	}

	else {
		return false;
	}



	
}

//find
//take key and value as argument
//search for passed in key 
//set value to data of record found at the index 
//and return true if key was found in the table 
//else return false
template <class TYPE>
bool LPTable<TYPE>::find(const string& key, TYPE& value){
	
	int index_ = search(key);

	if (records_[index_] != nullptr) {

		value = records_[index_]->data_;
		return true;

	}
	else {
		return false;
	}

	
}

//assignment operator
//assign data from passed in object to new Record table
//return object
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(const LPTable<TYPE>& other){

	if (this != &other) {

		delete[] records_;
		
		records_ = new Record*[other.max_];
		max_ = other.max_;
		size_ = 0;

		for (int i = 0; i < other.max_; i++) {

			if (other.records_[i] != nullptr) {
				records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
			}

			size_++;
		}

		return *this;
	}

}

//move operator
//take an argument
//set records_ to point at passed object record
//set the max, and size value to the same as passed object
//set the passed object properties to null and 0 afterwards
//return object
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(LPTable<TYPE>&& other){

	max_ = other.max_;
	size_ = other.size_;
	records_ = other.records_;

	other.max_ = 0;
	other.size_ = 0;
	other.records_ = nullptr;

	return *this;

}

//destructor
template <class TYPE>
LPTable<TYPE>::~LPTable(){

	delete[] records_;

}



