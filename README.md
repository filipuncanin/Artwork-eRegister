
#### MIHAJLO KARADŽIĆ

#### FILIP UNČANIN

# Elektronski registar za evidenciju izdatih umetničkih dela


## ZADATAK

```
 Realizovati primer namenskog sistema elektronskog registra za
evidenciju izdatih umetničkih dela pomoću TCP protokola.
 Primer se sastoji iz klijentske strane (korisnik) i serverske strane
(elektronski registar).
```

Klijent preko tastature može da zadaje sledeće komande koje se šalju
serveru:

```
 Login – prijava/registracija korisnika (npr. putem korisničkog imena ili broja
članske kartice i korisničke šifre)
 Logout – odjava korisnika
 Search – pretraga dostupnih sadržaja elektronskog registra
 SearchAll – pretraga svih sadržaja elektronskog registra
 Search [id:ID] [author:AUTHOR] [truncated art work name:NAME] [year:YEAR] –
pretraga za zadatke kriterijume (sve ili pojedine)
 CheckStatus – provera dobavljenih sadržaja prijavljenog korsinika
 Reserve [id:ID] – rezervacija umetničkog dela od strane prijavljenog korisnika po
ID-u umetničkog dela
```

Server na date komande šalje odgovarajuće odgovore koji se prikazuju na
ekranu klijentske aplikacije.
Napomene:

```
 Svako umetničko delo elektronskog registra poseduje jedinstven ID
 Svaki korisnik elektronskog registra prilikom procesa registracije dobija jedinstven
broj članske kartice
```

## Aplikacija

```
 Aplikacija se sastoji od datoteka server.c, artworks.txti
user_accounts.txtsa serverske strane i client.csa klijentske strane
 Prvo pokrećemo server a zatim klijent
 Nakon uspostave veze ispisuje se serverska poruka klijentu
 Klijenat je u obavezi da se prijavi na samom početku
```

```
 artworks.txtpredstavlja „bazu“ umetničkih dela
 user_accounts.txt predstavlja ,,bazu“ korisnika
```

```
 Sam ,,meni“ na klijentskoj strani je poprilično intuitivan i dobro
osmišljen tako da lako oslikava komande iz zadatka
 Svaka komanda otvara novi nivo i izbacuje svoje jedinstvene opcije
```


##Kompajliranje:
	-posebno se prevode server.c i client.c u odvojenim terminalima (sa mogućnošću otvaranja više terminala za client.c tj. više klijenata)
	-vrši se pomoću komandi "make server" i "make client"
	-brisanje izvršnih fajlova se vrši sa "make server_clean" i "make client_clean"

##Pokretanje:
	-pokretanje se vrši sa komandama "./server" koji se prvi pokreće a zatim i "./client"
	-za izlaženje iz aplikacije koristimo CTRL+C
