#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>

using namespace std;

long myhash(const char* key) {
	unsigned long hash = 0;
	for (int i = 0; i < sizeof(long); i++) {
		hash = (hash << 4) + (*key++);
		unsigned long g = hash & 0xF0L;
		if (g) hash ^= g >> 24;
		hash &= ~g;
	}
	hash += (hash << 3);
	return hash;
}

char* ltoca(const long l) {
	int size = sizeof(long) / sizeof(char);
	char* a = new char[size];
	char* c = (char*)&l;
	for (int i = 0; i < size; i++) {
		a[i] = *c++;
	}
	return a;
}

int isFile(string file) {
	struct stat buffer;
	return (stat(file.c_str(), &buffer) == 0);
}

bool encrypt(char* file, char* key) {
	if (isFile(file)) {
		ifstream in(file);
		string contents((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
		int size = sizeof(long) / sizeof(char);
		for (int i = 0; i < contents.length(); i++) {
			contents[i] = contents[i] ^ key[i & size];
		}

		ofstream os;
		os.open(file, ios::trunc);
		os << contents;
		os.close();
		return 1;
	}
	else {
		return 0;
	}
}

int main(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++) {
		char* key = ltoca(myhash(argv[i]));
		if (encrypt(argv[i], key)) {
			cout << argv[i] << " successfully converted." << endl;
		}
		else {
			cout << argv[i] << "Failed to convert." << endl;
		}
	}
	return 0;
}