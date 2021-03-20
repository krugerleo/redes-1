#include "communication.h"
#include "commands.h"

void server(){
    Package_header *msg;

    msg = recieve_msg();
    printf("%u %u %u %u %s %u\n",
        msg->mark
        , msg->size
        , msg->seq
        , msg->type
        , msg->data
        , msg->par
    );

    if(msg->type == CDT){
        printf("lits");
        list();
    
        changeDir(msg->data);
        printf("lists");
        list();
    }
    
}
int main(){
	init_socket();
	while(1){
		server();
	}

}