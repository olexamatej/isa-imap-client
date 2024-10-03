#include "connection.h"

Connection::Connection(){
    this->encryption = false;
    this->only_message_headers = false;
    this->only_new_messages = false;
    if(this->encryption){
        this->port = "993";
    }else{
        this->port = "143";
    }

}