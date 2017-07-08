#include "sd_iostream.h"

using namespace SD;

fstream::fstream(char *path, uint8_t access, SDIntf *sdIntf){
	this->path= path;
	this->access= access;
	this->sdIntf= sdIntf;
	this->ready= SD_IOSTREAM_NOTREADY;
}

uint8_t fstream::open(){
	if(this->access==SD_IOSTREAM_ACCESS_IN){

		ready= SD_IOSTREAM_READY;
	} else if(this->access==SD_IOSTREAM_ACCESS_OUT) {
		
		ready= SD_IOSTREAM_READY;
	}		
}

uint8_t fstream::close(){
	
}

//Write methods
int16_t fstream::write(const char* s){
	if(this->access==SD_IOSTREAM_ACCESS_IN && this->ready && sdIntf->IsCardReady()){
		
	}
	
	return SD_IOSTREAM_ERROR;
}

int16_t fstream::writeLine(const char* s){
	if(this->access==SD_IOSTREAM_ACCESS_IN && this->ready && sdIntf->IsCardReady()){
		
	}
	
	return SD_IOSTREAM_ERROR;
}
		
//Read methods
char* fstream::read(uint32_t n){
	if(this->access==SD_IOSTREAM_ACCESS_OUT && this->ready && sdIntf->IsCardReady()){
		
	}

}

char* fstream::readLine(){
	if(this->access==SD_IOSTREAM_ACCESS_OUT && this->ready && sdIntf->IsCardReady()){
		
	}
	
}

