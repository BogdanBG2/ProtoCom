                        Protocoale de Comunicatie	
                    Tema 3 - Comunicatie cu REST API
                        Bogdan-Andrei Buga, 322CB

	Tema are la baza o varianta adaptata a scheletului laboratorului 10
in ceea ce tine de comunicarea clientului cu server-ul si generarea cererilor
HTTP de tip GET, POST si DELETE.
	La rularea programului, daca clientul se poate conecta la server, acesta
va introduce la tastatura comenzi pana la prima comanda de "exit". Comenzile 
implementate sunt: 
        
        1) register
            - Nu se poate efectua cand clientul este deja logat (cookie nenul)
            - Citeste un username si o parola de la tastatura
            - Parseaza manual datele in format JSON
            - Trimite catre server o cerere de tip POST

        2) login
            - Nu se poate efectua cand clientul este deja logat (cookie nenul)
            - Citeste un username si o parola de la tastatura
            - Parseaza manual datele in format JSON
            - Trimite catre server o cerere de tip POST
            - Extrage un cookie de sesiune
              daca logarea este efectuata cu succes

        3) enter_library
            - Nu se poate efectua daca clientul nu este logat (cookie nul)
            - Trimite catre server o cerere de tip GET
            - Extrage un token JWT daca cererea este buna

        4) get_books
            - Nu se poate efectua daca clientul nu este logat (cookie nul)
              sau daca clientul logat nu are acces la librarie (token nul)
            - Trimite catre server o cerere de tip GET
            - Serverul va returna o lista de obiecte JSON cu toate cartile
              din contul clientului curent; mai specific, vor fi afisate
              ID-ul si titlul fiecarei carti

        5) get_book
            - Nu se poate efectua daca clientul nu este logat (cookie nul)
              sau daca clientul logat nu are acces la librarie (token nul)
              sau daca ID-ul cartii cerute nu a fost citit cum trebuie
            - Trimite catre server o cerere de tip GET
            - Serverul va returna un obiect JSON ce contine toate informatiile
              despre cartea cu ID-ul respectiv

        6) add_book
            - Nu se poate efectua daca clientul nu este logat (cookie nul)
              sau daca clientul logat nu are acces la librarie (token nul)
              sau daca numarul de pagini al cartii nu a fost citit cum trebuie
            - Se citesc toate datele necesare de la tastatura;
              acestea vor fi parsate manual in format JSON
              pentru a putea fi trimise catre server 
        	- Trimite catre server o cerere de tip POST

        7) delete_book
            - Nu se poate efectua daca clientul nu este logat (cookie nul)
              sau daca clientul logat nu are acces la librarie (token nul)
              sau daca ID-ul cartii cerute nu a fost citit cum trebuie
            - Trimite catre server o cerere de tip DELETE

        8) logout
            - Nu se poate efectua daca clientul nu este logat (cookie nul)
            - Trimite catre server o cerere de tip GET
            - Reseteaza valorile cookie-ului si a token-ului

    La prima comanda de "exit", se elibereaza toata memoria alocata pe heap
si se inchide conexiunea cu serverul, astfel terminandu-se si rularea
programului.
    
    Am ales sa parsez manual datele in format JSON deoarece, personal, a fost
mai simplu decat sa folosesc o anumita biblioteca.