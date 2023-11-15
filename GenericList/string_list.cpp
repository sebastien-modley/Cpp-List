// StringList.cpp : This file contains the 'main_StringList' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

#include "string_list.h"
#include <cassert>

List_String::List_String() : data(nullptr), capacity(0), count(0) {

}


void swap(List_String& first, List_String& second) {
	//explanation: calling swap makes it unqualified, meaning if swap(T,T) is specially defined, it is called, otherwise, call std::swap(T,T)
	swap(first.capacity, second.capacity);
	swap(first.count, second.count);
	swap(first.data, second.data);
}
//Rule of 3

//Destructor
List_String::~List_String() {
	delete[] data;
}

//Copy constructor
List_String::List_String(const List_String& other) : capacity(other.capacity), count(other.count) {
	data = CreateDeepCopy(other.data, other.capacity);
}

//Copy assignement
List_String& List_String::operator=(const List_String& other) {
	//Note: other is not a const reference but a copy of the value, allowing for use of swap logic (since "other" will be destroyed after
	//Long definition:
		//List_String tmp(other);
		//swap(*this, tmp);
		//return *this;
	//clearer definition:
	return *this = List_String(other); //calls operator=(&& List_String(&)))
	
}


//Rule of 5
//R-value references (&&) explained: http://thbecker.net/articles/rvalue_references/section_01.html 
//basic explanation: if a function argument is &&, whatever it references will stop existing at the end of the function
List_String::List_String(List_String&& other) : data(nullptr), capacity(0), count(0) {
	swap(*this, other);
}

List_String& List_String::operator=(List_String&& other) {
	//warning for future use: might need to do additional cleaning here if currently controlling external objects (since control will be passed over to other and deleted)
	swap(*this, other);
	return *this;
}



int List_String::Capacity() const { return capacity; }

bool List_String::Capacity(int new_capacity) {
	if (new_capacity < 0) return false; //allowed values must be >= 0
	if (new_capacity < count) return false; //can't have shorter capacity than element count
	if (new_capacity == capacity) return true; //no change
	if (new_capacity == 0) { //empty non-empty array
		delete[] data;
		data = nullptr;
		return true;
	};

	std::string* new_data = CreateDeepCopy(data, new_capacity, count);

	delete[] data;

	data = new_data;
	capacity = new_capacity;
	return true;
}

int List_String::Count() const { return count; }

std::string List_String::ToString() const {
	if (count == 0) return "";
	std::string result = "(";
	for (int i = 0; i < count - 1; i++) {
		result += data[i];
		result += ", ";
	}
	result += data[count - 1];
	result += ")";
	return result;
}


bool List_String::Add(const std::string& new_val) {
	if (capacity < count + 1) {
		Capacity(capacity == 0 ? 1 : capacity * 2); //double array size
	}
	data[count++] = new_val;
	return true;
}

bool List_String::Contains(const std::string& val) const {
	return IndexOf(val) != -1;
}

bool List_String::RemoveAt(int index) {
	if (index < 0) return false;
	if (index >= count) return false;
	//allowed setting: index = [0, count-1], count > 0 (data is initialized)

	memmove(data + index, data + index + 1, (count - index - 1) * sizeof(std::string)); //shallow shuffle left
	count--;
	return true;
}


int List_String::IndexOf(const std::string& val) const {
	for (int i = 0; i < count; i++) {
		if (data[i] == val) return i;
	}
	return -1;
}


bool List_String::Remove(const std::string& val) {
	int index = IndexOf(val);
	return RemoveAt(index);
}

std::string List_String::operator[](int index) const {
	return Get(index);
}

std::string List_String::Get(int index) const {
	if (index >= count || index < 0) throw std::out_of_range(string("Cannot get element at out_of_range index: ") + to_string(index) + string(")"));

	return data[index];
}


std::string* List_String::CreateDeepCopy(std::string* data, size_t data_size, size_t copy_size) {
	if (copy_size > data_size || copy_size < 0) {
		throw std::out_of_range(string("Cannot copy array of size ") + to_string(copy_size) + string(" onto array of size ") + to_string(copy_size) + string("."));
	}
	std::string* new_data = data_size > 0 ? new std::string[data_size] : nullptr;

	//For improved performance, replace copy by memcpy+fill (no deep copy of non-POD objects) or by using a swap method
	if (copy_size > 0) { //data != nullptr
		copy(data, data + copy_size, new_data);
	}

	return new_data;
}

std::string* List_String::CreateDeepCopy(std::string* data, size_t data_size) {
	return CreateDeepCopy(data, data_size, data_size);
}



List_String foo(List_String org) {
	List_String tmp(org);
	return tmp;
}


//can iterate through list without using iterators if internal data is contiguous
std::string* List_String::begin() const {
	return data;
}
std::string* List_String::end() const {
	return data+count;
}


void Main_Test_List_String() {
	List_String string_list;
	std::cout << "List count: " << string_list.Count() << "\n";
	
	std::string s1 = "List";
	string_list.Add(s1);
	std::cout << "Added element: " << s1 << "\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	std::cout << "\n";

	s1 = "<string>";
	string_list.Add(s1);
	std::cout << "Added element: " << s1 << ".\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	std::cout << "\n";

	std::cout << "Current list: " << string_list.ToString() << ".\n";
	
	std::cout << "\n\n";

	s1 = "<string>";
	string_list.Remove(s1);
	std::cout << "Removed element: " << s1 << ".\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	std::cout << "\n";

	s1 = "<T>";
	string_list.Add(s1);
	std::cout << "Added element: " << s1 << "\n";
	std::cout << "List count: " << string_list.Count() << "\n";
	
	std::cout << "Current list: " << string_list.ToString() << ".\n";

	std::cout << "Element at index 0: " << string_list.Get(0) << ".\n";
	std::cout << "Element at index 1: " << string_list[1] << ".\n";

	std::cout << "\n\n";

	List_String copy1(string_list);
	std::cout << "Copied list using copy constructor.\n";
	std::cout << "Copied list: " << copy1.ToString() << ".\n";

	std::cout << "\n\n";

	List_String copy2 = string_list;
	std::cout << "Copied list using copy assignement.\n";
	std::cout << "Copied list: " << copy2.ToString() << ".\n";

	std::cout << "\n\n";

	List_String copy3(foo(string_list));
	std::cout << "Copied list using move constructor.\n";
	std::cout << "Copied list: " << copy3.ToString() << ".\n";

	std::cout << "\n\n";

	List_String copy4 = foo(string_list);
	std::cout << "Copied list using move assignement.\n";
	std::cout << "Copied list: " << copy4.ToString() << ".\n";

	std::cout << "\n\n";

	std::cout << "Looping through data...\n";
	for (auto e : string_list) {
		std::cout << e << "\n";
	}

	std::cout << "\n\n";

	std::cout << "Testing iteration logic..\n";
	for (auto& str : string_list) {
		str += "[processed]";
	}

	for (const auto& str : string_list) {
		assert(str.find("[processed]") != std::string::npos);
	}
	std::cout << string_list.ToString() << "\n";


}