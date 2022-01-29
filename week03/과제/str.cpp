#include <iostream>
#include <cstdlib>
#include <string.h>
#include "Str.h"
using namespace std;

Str::Str(int leng) {
	len = leng;
}

Str::Str(const char *neyong) {
	str = neyong;
	len = strlen(str);
}

Str::~Str() {

}

int Str::length() {
	return len;
}

const char* Str::contents() {
	return str;
}

int Str::compare(class Str& a) {
	return strcmp(str, a.contents());
}

int Str::compare(const char *a) {
	return strcmp(str, a);
}

void Str::operator=(const char *a) {
	str = a;
}

void Str::operator=(class Str& a) {
	str = a.contents();
}
