#ifndef DYNAMIC_QUEUE_AS_ARRAY_H
#define DYNAMIC_QUEUE_AS_ARRAY_H

#include "ece250.h"
#include "Exception.h"

template <typename Object>
class Dynamic_queue_as_array
{
	private:
		int initial_size;
		int array_size;
		Object *array;
		// other integer member variables, as necessary
		int ihead;
		int itail;
		int count; //number of element in the queue
	public:
		Dynamic_queue_as_array( int = 10 );
		Dynamic_queue_as_array( const Dynamic_queue_as_array & );
		~Dynamic_queue_as_array();

		Dynamic_queue_as_array &operator = ( const Dynamic_queue_as_array & );

		Object head() const;
		int size() const;
		bool empty() const;
		int capacity() const;

		void enqueue( const Object & );
		Object dequeue();
		void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator << ( std::ostream &, const Dynamic_queue_as_array<T> & );
};

template <typename Object>
Dynamic_queue_as_array<Object>::Dynamic_queue_as_array( int n ) {
	if (n <= 0) {
		array = new Object[1];
	}
	else{
		array = new Object[n];
	}
	
	count = 0;
	itail = 0;
	ihead = 0;
	initial_size = n; //of array 
	array_size = n; //of array
	
}

template <typename Object>
Dynamic_queue_as_array<Object>::Dynamic_queue_as_array( const Dynamic_queue_as_array<Object> &queue ) {
	// Initialize your object and then call operator=
	*this = queue;
}

template <typename Object>
Dynamic_queue_as_array<Object>::~Dynamic_queue_as_array() {
	delete[] array;
}

template <typename Object>
Dynamic_queue_as_array<Object> &Dynamic_queue_as_array<Object>::operator = ( const Dynamic_queue_as_array<Object> &rhs ) {
	// empty the current queue, if necessary, deallocating memory
	// make a copy of the queue rhs
	
	if(rhs.empty())
	{
		return *this;
	}
	if (capacity()!= rhs.capacity()) {
		delete array;
		Object *new_array;
		new_array = new Object[rhs.capacity()];
		array_size = rhs.capacity();
		array = new_array;
	}
	//now both array sizes are the same
	
	
	else {
		while (count != 0) {
		
				dequeue();
			}
		}
	//at this point our queue is empty and array size is the same as rhs
	
	if (rhs.size() == 0) {
		//already have an exact copy
		
		return *this;
	}
	
	else if(rhs.ihead > rhs.itail){
		//copy such that exact copy is made, thus no normalization implemented
		
		for (int i = 0; i<= itail; ++i) {
			array[i] = rhs.array[i];
		}
		
		for (int i = ihead; i<array_size; ++i) {
			array[i] = rhs.array[i];
		}
	}
	
	else {
		//copy like normal
		
		for (int i = 0; i<array_size; ++i) {
			array[i] = rhs.array[i];
		}
	}


}

template <typename Object>
int Dynamic_queue_as_array<Object>::size() const {
	return count;
}

template <typename Object>
int Dynamic_queue_as_array<Object>::capacity() const {
	return array_size;
}

template <typename Object>
bool Dynamic_queue_as_array<Object>::empty() const {
	if (count == 0) {
		return true;
	}
	else {
		return false;
	}
	
}

template <typename  Object>
Object Dynamic_queue_as_array<Object>::head() const {
	if (count == 0) {
		throw underflow();
	}
	
	return array[ihead];
		
}

template <typename Object>
void Dynamic_queue_as_array<Object>::enqueue( const Object &obj ) {
	
	int tmp_size;

	//if array was full and thus size needed to be doubled
	if(count == array_size)
	{
		tmp_size = array_size * 2;
		Object *tmp = new Object[tmp_size];
		
		//if wrap around present
		if(itail<ihead)
		{
			int q = 0;
			
			//head to end
			for (int i = ihead; i<array_size; ++i) {
				tmp[q] = array[i];
				++q;
			}
			//front to tail
			for (int i = 0; i<=itail; ++i) {
				tmp[q] = array[i];
				++q;
			}
			ihead = 0;
			itail = array_size - 1;
			delete[] array; //delete smaller array because now we have the populated expanded one
			array = tmp;
			
			array[q] = obj;
			
			//enqueue
			++itail;
			array[itail] = obj;
		}
		//no wrap around
		else {
			for (int i = 0; i<array_size; ++i) {
				tmp[i] = array[i];
			}
			
			ihead = 0;
			//set to last element in queue
			itail = array_size -1;
			delete[] array; //delete smaller array because now we have the populated expanded one
			array = tmp;
			//actual enqueue action
			++itail;
			array[itail] = obj;
		}
		//update to doubles size
		array_size = tmp_size;
	}
	//if array not logically full but reached physical end
	else 
	{
		if(itail==(array_size-1))
		{
			//wrap around and enqueue
			itail = 0;
			array[itail] = obj;
		}
		else if (count == 0) {
				array[itail] = obj;
					
			}
			else {
				//normal enqueue
				++itail;
				array[itail] = obj;
			}

	}
 	
	++count;
}

template <typename Object>
Object Dynamic_queue_as_array<Object>::dequeue() {
	if (count == 0) 
	{
		throw underflow();
	} 
	
	Object hold;
	hold = array[ihead]; 
	
	if (ihead==array_size-1) 
	{	
		
		if (itail<ihead) 
		{
				//there is a wrap
				ihead = 0;
		}
		
		//must be ihead = itail case so one element
		else {
			//do nothing except reset head and tail pointers to initial condition
			ihead = 0;
			itail = 0;
		}

	}
	//one element only in queue
	else if(count == 1){
		
		//do nothing except reset pointers to initial condition
		
		ihead = 0;
		itail = 0;
		
	}
		
	else
	{
		//normal
			
		++ihead;
	}
	
	--count;

	 	
	if (count == (1/4) * array_size)
	{
		if ((1/2) * array_size >= initial_size) {
			
			int tmp_size = (1/2) * array_size;
			Object *tmp = new Object[tmp_size];

			//wrapped
			if (itail<ihead) {
				int q = 0;
				for (int i = ihead; i < array_size; ++i) {
					tmp[q] = array[i];
					++q;
				}
				
				for (int i = 0; i<= itail; ++i) {
					tmp[q] = array[i];
					++q;
				}
			}
		
		
			//if not wrapped
			
			else {
				for (int i = 0; i<array_size; ++i) {
					tmp[i] = array[i];
				}
			}
			ihead = 0;
			itail = array_size - 1; //reset to reflect normalized array
			array = tmp;
			array_size = tmp_size;
		}	
		 
		
	}
	//return the thing we are dequeueing 
	return hold;
}

template <typename Object>
void Dynamic_queue_as_array<Object>::clear() {
	
	itail = 0;
	ihead = 0;
	count = 0;
	array_size = initial_size;
} 

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator << ( std::ostream &out, const Dynamic_queue_as_array<T> &queue ) {
	// I don't know how you are implementing your queue so I cannot print it.
	// If you want, you can print whatever you want here and then call cout
	// in the driver.

	// Remember to redirect to out, e.g.,
	//      out << "Hello!";

	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
