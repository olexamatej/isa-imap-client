Author - Matej Olexa, xolexa03 Date - 16.11.2024

Implementation Description

The implementation of the IMAP client `imapcl` which, using the RFC3501 protocol, allows downloading messages stored on a server. After launching, the client authenticates, downloads the necessary messages to a selected directory using specified arguments, and then terminates the connection. The program is written in C++ and meets the assignment requirements without extending interactivity.

Example of usage (assuming the existence of the `saved_emails` directory and the `auth_file`):

```
make /imapcl imap.centrum.sk -p 993 -T -a auth_file -o saved_emails
```

List of submitted files:

Makefile  
README  
documentation.pdf  
mockup.py  
src/  
arg_parser.cpp  
arg_parser.h  
client.cpp 
client.h  
commands.cpp  
commands.h  
connection.cpp   
connection.h  
file_manager.cpp  
file_manager.h  
main.cpp  
msg_parser.cpp  
msg_parser.h  
runner.cpp  
runner.h 
