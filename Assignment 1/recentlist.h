template <typename T>

class RecentList{
	struct Node{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data=T{},Node* nx=nullptr,Node* pr=nullptr){
			data_ = data;
			next_ = nx;
			prev_ = pr;
		}
	};

	Node* front_;
	Node* back_;
public:
	class const_iterator{
		friend class RecentList<T>;
	protected:
		Node* curr_;
		const RecentList* theList_;
		const_iterator(Node* curr, const RecentList* theList){
			theList_ = theList;
			curr_ = curr;
		}
	public:
		const_iterator(){
			curr_=nullptr;
			theList_=nullptr;
		}
		const_iterator operator++(){
            //++x
			curr_=curr_->next_;
			return *this;
		}
		const_iterator operator++(int){
			//x++
			const_iterator old=*this;
			curr_=curr_->next_;
			return old;
		}
		const_iterator operator--(){
			//--x
			if(curr_){
				curr_=curr_->prev_;	
			} else if(theList_){
				curr_=theList_->back_;
			}
			return *this;
		}
		const_iterator operator--(int){
			//x--
			const_iterator old=*this;
			if(curr_){
				curr_=curr_->prev_;
			} else if(theList_){
				curr_=theList_->back_;
			}
			return old; 
		}
		bool operator==(const_iterator rhs){
			bool rc=false;
			if(theList_==rhs.theList_ && curr_==rhs.curr_) {
				rc=true;
			}
			return rc;
		}
		bool operator!=(const_iterator rhs){
			return !(*this==rhs);
		}
		const T& operator*()const{
			return curr_->data_;
		}
	};
	class iterator:public const_iterator{
		friend class RecentList<T>;

	protected:
            iterator(Node* curr, RecentList* theList){
			this->theList_ = theList;
			this->curr_ = curr;
		}
	public:
		iterator(){}
		//++x
		iterator operator++(){
			if (this->curr_) {
				this->curr_ = this->curr_->next_;
			}
			return *this;
		}
		//x++
		iterator operator++(int){
			iterator old=*this;
			if (this->curr_) {
				this->curr_ = this->curr_->next_;
			}
			return old;
		}
		//--x
		iterator operator--(){
			if(this->curr_){
				this->curr_=this->curr_->prev_;	
			} else if(this->theList_){
				this->curr_=this->theList_->back_;
			}
			return *this;
		}
		//x--
		iterator operator--(int){
			iterator old=*this;
			if(this->curr_){
				this->curr_=this->curr_->prev_;
			} else if(this->theList_){
				this->curr_=this->theList_->back_;
			}
			return old;
		}
		T& operator*(){
			return this->curr_->data_;
		}
		const T& operator*()const{
			return this->curr_->data_;
		}
	};
	RecentList();
	~RecentList();
	RecentList(const RecentList& rhs);
	RecentList& operator=(const RecentList& rhs);
	RecentList(RecentList&& rhs);
	RecentList& operator=(RecentList&& rhs);

	//return the node after begining sentinel
	iterator begin() {
		return iterator(front_->next_,this);
	}

	//return the last sentinel
	iterator end() {
		return iterator(back_,this);
	}

	//return the node after begining sentinel
	const_iterator cbegin() const {
		return const_iterator(front_->next_,this);
	}

	//return the last sentinel
	const_iterator cend() const {
		return const_iterator(back_,this);
	}
	void insert(const T& data);
	iterator search(const T& data);
	iterator erase(iterator it);
	iterator erase(iterator first, iterator last);
	bool empty() const;
	int size() const;
};


template <typename T>
RecentList<T>::RecentList(){

	//creating front and back nodes and make sentinels point at one another
	front_=new Node(); 
	back_=new Node();
	front_->next_=back_;
	back_->prev_=front_;

}


//deallocate/delete all nodes
template <typename T>
RecentList<T>::~RecentList(){

	Node* curr = front_;
	while(curr){
	   Node* rm = curr;   
	   curr = curr->next_;
	   delete rm;
	}

} 

//copy all nodes from constant RecentList data that is passed in
template <typename T>
RecentList<T>::RecentList(const RecentList& rhs){
	
	//copy constructor called when a new object is created from an existing object, as a copy of the existing object 

	front_ = new Node();
	back_ = new Node();
	front_->next_ = back_;
	back_->prev_ = front_;

	//loop through rhs from first node to end node
	//continue creating new node with data starting from rhs first node. Point next to back node, point prev to node before back until no more nodes(nullptr)
	for (const_iterator i = rhs.cbegin(); i != rhs.cend(); i++) {
		Node* nn = new Node(*i, back_, back_->prev_); 
		back_->prev_->next_ = nn; 
		back_->prev_ = nn; 
	}
	
}

//copy all nodes from constant RecentList data that is passed in
template <typename T>
RecentList<T>& RecentList<T>::operator=(const RecentList& rhs){

	//copy assignment operator is called when an already initialized object is assigned a new value from another existing object

	erase(begin(), end()); //erases everything between sentinels

	front_ = new Node();
	back_ = new Node();
	front_->next_ = back_;
	back_->prev_ = front_;

	//loop through rhs from first node to end node
	//continue creating new node with data starting from rhs first node. Point next to back node, point prev to node before back until no more nodes(nullptr)
	for (const_iterator i = rhs.cbegin(); i != rhs.cend(); i++) {
		Node* nn = new Node(*i, back_, back_->prev_); 
		back_->prev_->next_ = nn; 
		back_->prev_ = nn; 
	}
	
}



template <typename T>
RecentList<T>::RecentList(RecentList&& rhs){
	
	//move constructor, 
	//make current front and back point to passed front and back
	//create new nodes where passed front and back are pointed to them
	//then making sentinels point to each other
	front_ = rhs.front_;
	back_ = rhs.back_;
	rhs.front_ = new Node();
	rhs.back_ = new Node();
	rhs.front_->next_ = rhs.back_;
	rhs.back_->prev_ = rhs.front_;

}


//swap where front and back point for both
template <typename T>
RecentList<T>& RecentList<T>::operator=(RecentList&& rhs){
	
	//move assignment operator
	//initialize new node and make current front and back point to passed front and back
	//make passed front and back point to where created nodes are pointing to, which was current front and back
	Node* front = front_; 
	Node* back = back_; 
	front_ = rhs.front_;  
	back_ = rhs.back_;
	rhs.front_ = front; 
	rhs.back_ = back; 

}


//insert new node with passed in data right before last sentinel
template <typename T>
void RecentList<T>::insert(const T& data){
	
	Node* nn = new Node(data, back_, back_->prev_); //new node where next pointing to where back is pointing and new node's prev is pointing to where back_->prev_ is pointing too
	back_->prev_->next_ = nn; //set the node before the one back is pointed to to point to new node
	back_->prev_ = nn; //set last node to point to new node

}


//search for same data that a node contains and move that found node to front of the list
template <typename T>
typename RecentList<T>::iterator RecentList<T>::search(const T& data){
    
	iterator it = begin();
	iterator endIt = end();

	//loop while there are nodes in between 
	while(it != endIt){

		//if num found, return iterator
	   if (it.curr_->data_ == data) {
		// moving the found node to the front
		   
		  //unlink
	      it.curr_->prev_->next_ = it.curr_->next_;
	      it.curr_->next_->prev_ = it.curr_->prev_;
        
		  //link
	      it.curr_->next_ = front_->next_;
	      it.curr_->prev_ = front_;
	      front_->next_->prev_ = it.curr_;
	      front_->next_ = it.curr_;
              
		  return it;
	   }
	   ++it; //only if 'it' != end and to the number were looking for
    }
	
	return this->end(); 
      
}


//this function erases a node and returns iterator to the next node after the erased one
template <typename T>
typename RecentList<T>::iterator RecentList<T>::erase(iterator it){
	
	if (it.curr_  && it != end()){
	   Node* rm = it.curr_;
	   it.curr_->prev_->next_ = it.curr_->next_; 
	   it.curr_->next_->prev_ = it.curr_->prev_;
	  
	   delete rm;  
	}
	return ++it;
}


//erases all node between first and last, including first but not last. Returns iterator to last
template <typename T>
typename RecentList<T>::iterator RecentList<T>::erase(iterator first, iterator last){
	
	Node* rm;
	
	//unlink nodes from and including first to before last
	first.curr_->prev_->next_ = last.curr_; 
	last.curr_->prev_ = first.curr_->prev_;
	
	//delete all node in between and including first
	if(first != last){
       rm = first.curr_;
	   delete rm;
	   ++first;
	}
	return last;

}


//check if list is empty by seeing if 2 sentinels are pointing to each other
template <typename T>
bool RecentList<T>::empty() const{
	
	return front_->next_ == back_;

}


//check the size of list by looping through until it hits last node before nullptr, for every node +1 to counter
template <typename T>
int RecentList<T>::size() const{
    
	int size = 0;

	for (const_iterator it = this->cbegin(); it != this->cend(); it++) {
		size++;
	}
	return size;

}
