#include "connection.h"

Connection::Connection(){
    this->encryption_ = false;
    this->only_message_headers_ = false;
    this->only_new_messages_ = false;
    
}