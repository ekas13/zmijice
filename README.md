# Učenje igre Zmija za jednu i više zmija s odabranim modelima učenja i algoritmima ponašanja

## Uvod

Ovaj projekt nastao je pod mentorstvom [prof. dr. sc. Domagoja Jakobovića][1] u sklopu kolegija Projekt R koji se podučava na 3. godini preddiplomskog studija Fakulteta elektrotehnike i računarstva Sveučilišta u Zagrebu.

## Opis projekta

Glavnih cilj projekta jest učenje i simulacija modela raznih metoda strojnog učenja u okruženju popularne igre Snake.
Implementirani modeli su:
- **NN** (*Neural Network*)
- **GP** (*Genetic Programming*)
- **CGP** (*Cartesian Genetic Programming*)

Mogući načini rada uključuju učenje modela da igra u okruženju s dva ili više igrača, kao i protiv čovjeka.
Također je implementirana i takozvana "hamiltonska" zmija koja upotrebljava algoritam pronalaženja Hamiltonovog ciklusa za navigiranje po mapi.

## Pokretanje

Izvršna verzija programa za windows_x64 platformu dostupna je [ovdje][2].
Sve dostupne verzije potražite u [`Releases`][3] kategoriji repozitorija.

Za pregledavanje, uređivanje i kompajliranje izvornog koda preporuča se [Visual studio][4].

Prije pokretanja `Zmijica.exe` moguće je konfigurirati parametre u `zmijica.config`. Neki od parametara uključuju način rada, veličinu mape, broj zmija, konfiguraciju svakog modela...
U toj datoteci je također moguće postaviti pokretanje spremljenih modela iz `./saves`.

## Kontrole

### *Learn* način rada
- `x` - ubrzanje rada programa
- `y` - usporavanje rada programa
- `s` - spremanje modela

### *Play* način rada
- `↑`,`↓`,`←` ,`→`  - upravljanje (ako je igrač ljudski)
- `r` - ponovno pokretanje

## Komponentni prikaz

![alt text](https://i.ibb.co/jMsgmRy/Picture1.png)

#### Prikaz korisničkog sučelja
![alt-text](https://i.ibb.co/p12BhhG/zmija1.png)

## Korištene tehnologije

- **C++**
- **SFML**

## Tim

- Mislav Jelić
- Eva Kaštelan
- Luka Nestić
- Hana Ujčić
- Joško Vrsalović

[1]: http://www.zemris.fer.hr/~yeti/index.html
[2]: https://gitlab.com/api/v4/projects/40715631/packages/generic/Zmijica/1.0/Zmijica_v1.0_windows_x64.zip
[3]: https://gitlab.com/lnestic/projekt-r-zmijica/-/releases
[4]: https://visualstudio.microsoft.com/
