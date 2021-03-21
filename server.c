#include "communication.h"
#include "commands.h"
const int ORIGI = 10;
const int DESTI = 01;

void server(){
    Package_header *msg;

    msg = recieve_msg();

    if(msg->type == CDT){
        // while != EOT
    }
    
}
int main(){
	init_socket();
	while(1){
		server();
	}

}