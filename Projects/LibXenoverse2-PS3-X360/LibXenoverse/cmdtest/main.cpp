#include "LibXenoverse.h"

int main(int argc, char** argv) {
	LibXenoverse::initializeDebuggingLog();

	LibXenoverse::EAN *ean = new LibXenoverse::EAN();
	//ean->load("230_FRZ_MKM.ean");
	ean->load("000_GOK_KMH.ean");

	//here you could add some test, and fast switch between them.

	return 0;
}