## Esittely

Tämän sanapelin tarkoitus on toimia englanti-suomi-opetusohjelmana.
Pelikierroksella kysytään kymmenen englanninkielistä sanaa. Pelaaja valitsee
englanninkieliselle sanalle suomenkielisen vastineen kahdeksasta eri
vaihtoehdosta. Pisteet määräytyvät vastauksen nopeuden mukaan. Väärästä
vastauksesta pelaaja menettää rangaistuksena yhden kysyttävän
englanninkielisen sanan (ja siitä saatavat mahdolliset pisteet sen mukana).

## Info

Kysyttävät sanaparit ovat tekstitiedostossa sanat.txt, joka sijaitsee samassa
hakemistossa kuin pelitiedosto. Sanaparit tallennetaan tiedostoon riveittäin
muodossa: "englanninkielinen_sana=suomenkieliset_vastineet". Esim. "code=koodi,
salakieli, laki". Uusia sanapareja voi lisätä -a option avulla:

	./enkku -a englanti suomi

Esim.

	./enkku -a code "koodi, salakieli, laki"


Maksimipistemäärä on 200 pistettä. Pisteet määräytyvät vastauksen nopeuden mukaan:
<= 3sek		20 pistettä
<= 5sek		10 pistettä
<= 6sek		 8 pistettä
<= 8sek		 6 pistettä
<= 10sek	 4 pistettä
<= 12sek	 2 pistettä
>  12sek	 1 piste


Top10-lista tallennetaan tiedostoon scores.txt.





