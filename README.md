

Autor - Matej Olexa, xolexa03
Dátum - 16.11.2024

Popis implementácie

Implementácia IMAP klienta imapcl ktorý pomocou protokolu RFC3501 umožňuje sťahovanie správ uložených na serveri. Po spustení sa klient autentifikuje, pomocou vybraných argumentov stiahne potrebné správy do vybraného adresára a preruší spojenie. Program je písaný v programovacom jazyku C++ a splňuje požiadavky zadanie bez rozšírenia interaktivity.

Príklad spustenia (za predpokladu existencie priečinku saved_emails a súboru auth_file)

make
/imapcl imap.centrum.sk -p 993 -T -a auth_file -o saved_emails

Zoznam odovzdaných súborov

Makefile
README
documentation.pdf
mockup.py
src/
    arg_parser.cpp
    arg_pasrer.h
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
