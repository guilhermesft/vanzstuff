package main

/*
#include <stdio.h>

FILE *file;

int finish() {
	fflush(file);
	fclose(file);
}

int init(const char* file_name) {
	if (file){
		finish();
	}
	file = fopen(file_name, "w+");
}

int write_file(const char* data, size_t len) {
	if (!file){
		printf("File is not open");
		return 1;
	}
	if (fwrite(data, sizeof(char), len, file)) {
		return 0;
	}
	printf("Error on writing...");
	return 1;

}
*/
import "C"
import "os"

func main() {
	C.init(C.CString(os.Args[1]))
	for _, a := range os.Args[2:] {
		C.write_file(C.CString(a+"\n"), C.size_t(len(a)+1))
	}
	C.finish()
}
