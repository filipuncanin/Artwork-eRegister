
#### MIHAJLO KARADŽIĆ

#### FILIP UNČANIN

# Elektronski registar za evidenciju

# izdatih umetničkih dela


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

## TCP protokol

```
 Uspostavljanje veze između dva hosta TCP protokol ostvaruje kroz
postupak koji se naziva trostruko rukovanje (Three-Way Handshake)
 Trostruko rukovanje predstavlja razmenu tri upravljačke poruke
između hostova koji žele da komuniciraju
```

### Trostruko rukovanje (TCP)

```
 U prvom koraku računar koji inicira vezu prvo šalje SYN
(Synchronization) poruku koja predstavlja segment sa postavljenim
SYN bitom na 1, čime se druga strana obaveštava da se s njom želi
komunicirati
 U drugom koraku pozvani računar odgovara tako što šalje segment
kojim potvrđuje prijem
 U trećem koraku računar koji je inicirao vezu šalje segment kojim
potvrđuje prijem segmenta i počinje da šalje svoje podatke
 Kada se prenesu svi podaci, računari trostrukim rukovanjem
razmenjuju segmente koji sadrže FIN (Final) bit kojim se veza prekida
```

### Adresiranje aplikacija (TCP)

```
 Za adresiranje aplikacija i procesa kao krajnjih tačaka (end points) u
komunikaciji koriste se virtuelni portovi
 Svaka strana u komunikaciji (aplikacija) koristi svoj vlastiti broj porta
 Naziv virtuelni dolazi od toga što se jedna fizička veza koristi za
uspostavljanje više softverskih veza
 Brojevi portova se kreću od 0 do 65534 i upisuju se u polje dužine 16
bita u TCP zaglavlju
 Na osnovu broja porta, protokoli transportnog sloja tačno znaju kojoj
aplikaciji, odnosno procesu treba da isporuče podatke
```

### Pouzdana isporuka (TCP)

```
 TCP protokol se koristi za slanje veće količine podataka
 On je napravljen tako da osigura pouzdanu isporuku podataka
preko nepouzdanih mreža koje mogu da imaju različitu topologiju,
tehnologiju, propusni opseg, vreme kašnjenja, veličinu paketa itd
 TCP je konekciono orijentisan protokol. To znači da pre nego počne
sa slanjem podataka, prethodno uspostavi vezu sa prijemnim
računarom
 Pouzdana isporuka paketa se ostvaruje sistemom sekvenciranja i
potvrđivanja
```

### Sekvenciranje (TCP)

```
 Sekvenciranjem se segmenti dele na manje pakete koji se onda
šalju jednom ili preko više različitih putanja do prijemnog računara
 Ovi paketi na odredište mogu da stignu u različito vreme i u
nepravilnom rasporedu
 Na osnovu broja paketa na prijemnom računaru se vrši njihovo
ponovno spajanje u originalni segment
 Za svaki ispravno primljeni paket, prijemni računar šalje potvrdu o
njegovom prijemu
 Prijemni računar ne šalje potvrdu o paketima koji nisu stigli ili su stigli
oštećeni
 Umesto toga, predajni računar za svaki paket kada ga pošalje
postavi tajmer na određeno vreme
 Ako vreme istekne pre nego što stigne potvrda o prijemu paketa,
paket se ponovo šalje
```

## Aplikacija

```
 Aplikacija se sastoji od datoteka server.c, artworks.txti
user_accounts.txtsa serverske strane i client.csa klijentske strane
 Prvo pokrećemo server a zatim klijent
 Nakon uspostave veze ispisuje se serverska poruka klijentu
 Klijenat je u obavezi da se prijavi na samom početku
```

## Aplikacija

```
 artworks.txtpredstavlja „bazu“ umetničkih dela
 user_accounts.txt predstavlja ,,bazu“ korisnika
```

## Aplikacija

```
 Sam ,,meni“ na klijentskoj strani je poprilično intuitivan i dobro
osmišljen tako da lako oslikava komande iz zadatka
 Svaka komanda otvara novi nivo i izbacuje svoje jedinstvene opcije
```
