# Kooperativno, natjecateljsko i mješovito kooperativno-natjecateljsko podržano učenje za igru Zmija s više agenata


## Uvod

Ovaj rad nastao je pod mentorstvom [prof. dr. sc. Domagoja Jakobovića][1] u sklopu kolegija Završni rad koji se podučava na 3. godini preddiplomskog studija Fakulteta elektrotehnike i računarstva Sveučilišta u Zagrebu.

## Opis 

Glavnih cilj projekta jest učenje i simulacija modela raznih metoda strojnog učenja u okruženju popularne igre Snake za više agenata.
Napravljene su 3 inačice višeagentnog učenja:
- *Kooperativna inačica*
- *Natjecateljska inačica*
- *Mješovita kooperativna natjecateljska inačica*
* svaka inačica implementirana je na zasebnoj grani (eng. branch) repozitorija
  
Implementiran model zmije/tima zmija:
- **GP** (*Genetic Programming*)
- ( **NN** (Neural Network) i **CGP** (Cartesian Genetic Programming) implementirani u sklopu Projekta R primjenjivi su samo u Natjecateljskoj inačici i izmjenjivi u  `zmijica.config` datoteci) 


## Pokretanje

Za pregledavanje, uređivanje i kompajliranje izvornog koda preporuča se [Visual studio][2].

Prije pokretanja `Zmijica.exe` moguće je konfigurirati parametre u `zmijica.config`. Neki od parametara uključuju veličinu mape, broj zmija, konfiguraciju modela...

### Kontrole za grafički prikaz (omogućen samo u Natjecateljskoj verziji rada)
- `x` - ubrzanje rada programa
- `y` - usporavanje rada programa
- `s` - spremanje modela


## Komponentni prikaz

**Timske komponente**
![image](https://github.com/ekas13/zmijice_private/assets/73114843/fbc5e753-e536-455b-bc19-d15f96814f1f)

**Natjecateljska inačica**
![image](https://github.com/ekas13/zmijice_private/assets/73114843/a6e56171-4b2b-4a91-a4b2-781f0a6b6e20)

#### Prikaz korisničkog sučelja
![alt-text](https://i.ibb.co/p12BhhG/zmija1.png)

## Korištene tehnologije
- **C++**

[1]: http://www.zemris.fer.hr/~yeti/index.html
[2]: https://visualstudio.microsoft.com/
