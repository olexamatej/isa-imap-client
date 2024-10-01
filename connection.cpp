#include "connection.h"

Connection::Connection(){
    this->encryption = false;
    this->messages_new = false;
    this->messages_heads = false;
    if(this->encryption){
        this->port = "993";
    }else{
        this->port = "143";
    }

}