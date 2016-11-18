#include "string.h"
#include <stdexcept>
#ifndef Assert
#include <cassert>
#define Assert assert
#endif


void String::reset() {
	if (content_ != nullptr && size_ > 0) {
		delete [] content_;
	}
	content_ = nullptr;
	length_ = 0;
	size_ = 0;
}

bool String::check() {
	return (length_ <= size_) && !(size_ == 0 && content_ != nullptr); 
}

void String::resize(size_t new_size) {
	if (size_ >= new_size) return;
	Assert(new_size >= length_);
	Assert(length_ <= size_);

	char* new_content = new char[new_size];

	if (content_ != nullptr) {
		memcopy(content_, new_content, length_);
		delete [] content_;
	}
	content_ = new_content;
	size_ = new_size;
}

void memcopy(char* src, char* dst, size_t len) {
	if (len == 0) return;
	if (src == dst) return;
	if (src < dst) {
		size_t i = len;
		do {
			i--;
			dst[i] = src[i];
		} while(i > 0);
	} else {
		for(size_t i = 0; i < len; i++) {
			dst[i] = src[i];
		}
	}
}


String::String(size_t size): length_(0), content_(nullptr), size_(size) {
	if (size > 0) {
		content_ = new char[size];
	}
	Assert(check());
}

String::String(char* literal): size_(0), content_(nullptr), length_(0) {
	size_t len = 0;
	while(literal[len]) len++;

	if(len == 0) {
		return;
	}
	length_ = len;
	size_ = len;
	content_ = new char[len];

	memcopy(literal, content_, len);

	Assert(check());
}

String::String(): length_(0), content_(nullptr), size_(0) {}


String::String(String const& a): length_(a.length_), size_(a.length_) {
	content_ = new char[length_];
	memcopy(a.content_, content_, length_);
	Assert(check());
}


String::String(String&& a): length_(a.length_), size_(a.size_), content_(a.content_) {
	a.length_ = 0;
	a.size_ = 0;
	a.content_ = nullptr;
}


String::~String() {
	reset();
}


String& String::operator=(String const& a) {
	length_ = a.length_;
	size_ = a.size_;
	if (size_ > 0) {
		content_ = new char[size_];
		memcopy(a.content_, content_, length_);
	} else {
		content_ = nullptr;
	}
	Assert(check());
	return *this;
}

String String::operator=(String &a) {
	length_ = a.length_;
	size_ = a.size_;
	content_ = content_;
	Assert(check());
	return *this;
}


size_t String::size() const {
	return length_;
}


String operator+(String const& a, String const& b) {
	size_t new_len = a.length_ + b.length_;
	if (new_len < a.length_) {
		throw std::overflow_error("overflow");
	}
	char* new_content = new char[new_len];

	String new_string;
	new_string.size_ = new_len;
	new_string.length_ = new_len;
	new_string.content_ = new_content;

	for(size_t i = 0; i < a.length_; i++) {
		new_content[i] = a.content_[i];
	}

	for(size_t i = 0; i < b.length_; i++) {
		new_content[i + a.length_] = b.content_[i];
	}

	Assert(new_string.check());
	return new_string;
}

String& String::operator+=(char const& a) {
	resize(length_ + 1);
	content_[length_] = a;
	length_++;

	Assert(check());
	return *this;
}	

String& String::operator+=(String const& a) {
	if (length_ + a.length_ < length_) {
		throw std::overflow_error("overflow");
	}
	resize(length_ + a.length_);
	memcopy(a.content_, content_ + length_, a.length_);
	length_ += a.length_;

	Assert(check());
	return *this;
}

char& String::operator[](size_t position) {
	if (position >= length_) {
		throw std::out_of_range("out of bounds []-operator call");
	}

	return content_[position];
}


char String::operator[](size_t position) const {
	if (position >= length_) {
		throw std::out_of_range("out of bounds []-operator call");
	}

	return content_[position];
}

std::ostream& operator<<(std::ostream& out, String const& a) {
	for(size_t i = 0; i < a.length_; i++) {
		out << a.content_[i];
	}
	return out;
}

std::istream& operator>>(std::istream& in, String &a) {
	char temp;

	while(in >> temp) {
		a += temp;
	}

	return in;
}


String::iterator String::begin() {
	return String::iterator(*this, 0);
}

String::iterator String::end() {
	return String::iterator(*this, length_);
}


void String::push_back(char c) {
	*this += c;
}

char String::pop_back() {
	if(length_ == 0) {
		throw std::out_of_range("pop_back() called empty string");
	}

	char ret = content_[length_ - 1];
	length_--;

	Assert(check());
	return ret;
}

void String::insert(size_t index, char c) {
	if (index > length_) {
		throw new std::out_of_range("index out range");
	}
	resize(length_ + 1);
	memcopy(content_ + index, content_ + index + 1, length_ - index);
	content_[index] = c;
	length_++;
	Assert(check());
}

void String::insert(size_t index, char* str) {
	if (index > length_) {
		throw new std::out_of_range("index out of range");
	}
	size_t len = 0;
	while (str[len]) {
		len++;
	}

	if (length_ + len < length_) {
		throw std::overflow_error("overflow");
	}
	resize(length_ + len);
	memcopy(content_ + index, content_ + index + len, length_ - index);
	memcopy(str, content_ + index, len);

	length_ += len;
	Assert(check());
}

void String::insert(size_t index, String const& str) {
	if (index > length_) {
		throw new std::out_of_range("index out of range");
	}

	if (length_ + str.length_ < length_) {
		throw std::overflow_error("overflow");
	}
	resize(length_ + str.length_);
	memcopy(content_ + index, content_ + index + str.length_, length_ - index);
	memcopy(str.content_, content_ + index, str.length_);

	length_ += str.length_;
	Assert(check());
}

void String::erase(size_t from, size_t to) {
	if(from >= length_ || to >= length_ || to < from) {
		throw std::out_of_range("invalid parameters");
	}

	size_t len = to - from + 1;
	memcopy(content_ + to + 1, content_ + from, length_ - to - 1);
	length_ -= len;
	Assert(check());
}

void String::swap(String& a) {
	size_t tmp_size = a.size_;
	size_t tmp_len = a.length_;
	char* tmp_content = a.content_;

	a.size_ = size_;
	a.length_ = length_;
	a.content_ = content_;

	size_ = tmp_size;
	length_ = tmp_len;
	content_ = tmp_content;
}

char* String::to_C_string() {
	char* cstr = new char[length_ + 1];
	memcopy(content_, cstr, length_);
	cstr[length_] = '\0';
	return cstr;
}


String::iterator::iterator(String& string, size_t position): string_(string), pos_(position) {}

bool String::iterator::check() {
	return (pos_ <= string_.length_ && pos_ >= 0);
}

char String::iterator::operator*() {
	if (pos_ < string_.length_) {
		return string_.content_[pos_];
	}

	throw std::runtime_error("invalid iterator");
}

String::iterator& String::iterator::operator++() {
	if (pos_ >= string_.length_) {
		throw std::out_of_range("out of range");
	}

	pos_++;
	Assert(check());
	return *this;
}

String::iterator String::iterator::operator++(int) {
	String::iterator tmp = *this;
	++*this;
	return tmp;
}

String::iterator& String::iterator::operator--() {
	if (pos_ == 0) {
		throw std::out_of_range("out of range");
	}
	pos_--;

	return *this;
}

String::iterator String::iterator::operator--(int) {
	String::iterator tmp = *this;
	--*this;
	return tmp;
}

bool String::iterator::operator==(iterator const& a) {
	return (&a.string_ == &string_ && a.pos_ == pos_);
}

bool String::iterator::operator!=(iterator const& a) {
	return !(*this==a);
}
