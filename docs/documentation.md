# Client for a chat server using IPK24-CHAT protocol

Matej Olexa (xolexa03) 1.4.2024

## Obsah
[Popis](#popis)  
[Spustenie](#Spustenie)  
[Zoznam odovzdaných súborov](#Zoznam-odovzdaných-súborov)  
## Popis

Implementácia IMAP klienta ktorý pomocou protokolu RFC3501 umožňuje sťahovanie správ uložených na serveri. Po spustení sa klient autentifikuje, pomocou vybraných argumentov stiahne potrebné správy do vybraného adresára a preruší spojenie.

## Spustenie

`imapcl server_ [-p port] [-T [-c certfile] [-C certaddr]] [-n] [-h] -a auth_file [-b MAILBOX] -o out_dir`

##### Povinné argumenty
- `server`
- `-a <auth_file>`
- `-o <outdir>`
##### Nepovinné argumenty
- `-p <port>`
- `-T`
- `-c <certfile>`
- `-C <certaddr>`
- `-n`
- `-h`
- `-b <MAILBOX>`

`./imapcl imap.centrum.sk -T -p 993 -a auth_file -o saved_emails`

## Zoznam odovzdaných súborov

- `arg_parser.cpp`
- `arg_pasrer.h`
- `client.cpp`
- `client.h`
- `commands.cpp`
- `commands.h`
- `connection.cpp`
- `connection.h`
- `file_manager.cpp`
- `file_manager.h`
- `main.cpp`
- `msg_parser.cpp`
- `msg_parser.h`
- `runner.cpp`
- `runner.h`

## Teória 

### IMAP
IMAP (Internet Message Access Protocol) umožňuje klientom prístup k emailom umiestneným na mail serveru. Narozdiel od POP3, ktorý email po stiahnutí vymaže zo serveru, IMAP tieto maily po stiahnutí na serveri ponechá a synchronizuje svoje akcie s ostatnými zariadeniami.  
Každá správa počas komunikácie s IMAP serverom sa začína s unikátnym reťazcom `tag`, tento tag sa mení (inkrementuje) každou správou poslanou z klienta - server svoje odpovede posiela s rovnakým tagom. Vďaka tomu vie klient zistiť, na ktorú spravu server odpovedá.  
Dôležitou časťou je, že IMAP server môže kedykoľvek počas komunikácie poslať klientovi správu `BYE`, ktorá ukončuje spojenie. 

#### Dôležité Operácie
- `LOGIN` - prihlásenie uživateľa na server
- `LIST` - vypísanie všetkých schránok
- `SELECT` - otváranie konkrétnej schránky
- `FETCH` - získanie konkrétného obsahu schránky
- `SEARCH` - vyhľadanie podľa konkrétných údajov
- `LOGOUT` - odhlásenie a ukončenie spojenia

#### Workflow
Príklad možnej postupnosti využitia IMAPu

1. `Pripojenie sa na server`
2. `Prihlásenie sa`
3. `Vybratie si schránky`
4. `Stiahnutie chcených emailov`
5. `Odhlásenie sa`

#### Odpovede od servera
Odpovede poslané od servera majú na začiatku kód statusu
- `OK` - príkaz uspel
- `NO` - príkaz neuspel
- `BAD` - nesprávna syntax

#### Flags

Pre vyberanie emailov môžeme použiť aj tzv. `flags` (príznaky), tieto nám umožňuju vylučiť nechcené emaily. Každý mail server môže mať svoje vlastné príznaky, avšak existujú štandartné ktoré sa vyskytujú na všetkých.

##### Štandartné príznaky
- `\Seen`
- `\Flagged`
- `\Answered`
- `\Draft`


#### Štruktúra emailu
Každý email je rozdelený na 2 hlavné časti -- Hlavička (Header) a telo (body). Okrem toho, sa k ním ukladajú metadata pre bližšie informácie.

#### Hlavička
V hlavičke sa nachádzajú informácie o smerovaní emailu. Vyskytuje sa vo vrchnej časti emailu.  
Zvykne obsahovat -
- `From:` - Odosielateľ emailu
- `To:` - Príjmateľ emailu
- `Subject:` - Predmet emailu
- `Date:` - Dátum emailu
- `Message-ID:` - Unikátny identifikátor emailu
- `Reply-To` - Špecifikuje adresu na ktorej správu email odpovedal

### SSL


### 
Tento projekt bol implementovaný na základe požiadavkov zadania [2]. 


Počas implementácie UDP varianty klienta je kvôli spôsobu fungovania tohto protokolu potrebné uchovávanie informácie o `ID packetu` a posielanie/príjmanie `confirm` správ, čím sa reprodukovala funkcia TCP a nebolo možné použiť spoločné metódy na odosielanie a príjmanie packetov.

## Testovanie


## Sources
[1] [IPK24Chat Protocol](https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master#developer-environment-and-virtualization-remarks)
