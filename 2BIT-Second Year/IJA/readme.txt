##################################################
############# Labyrinth IJA 2015 #################
##################################################

autor: Filip Ježovica, xjezov01@stud.fit.vutbr.cz
autor: Marek Marušic , xmarus05@stud.fit.vutbr.cz

Info:
Táto hra bola školským projektom predmetu IJA - FIT VUT Brno CZ.
Ako poklady zobrazované v hre boli použité Google free icons.

Licencia:
Hru je možné voľne šíriť, i modifikovať.

Preklad a spustenie:
ant compile     Hra sa skompiluje pomocou príkazu ant compile
                kde sa zo zdrojových súborov vytvorí
                jar súbor uložený do zložky ./dest-client
                + je vytvorená i JAVADOC dokumentácia

ant run         Hra bude spustená

ant clean       Vymaže sa spustiteľný jar súbor, dokumentácia,
                preložené súbory .class v adresári build


Pravidlá hry:
Najskôr je potrebné zobrať si kartu - na karte je poklad, ktorý hľadá.
Hráč hľadá svoju poklad na hracom poli.
Každé kolo môže hráč pootočiť voľným kameňom a následne ho vložiť
V tejto chvíli je možné voľbu vrátiť tlačidlom UNDO alebo potvrdiť.
Teraz môže hráč chodiť po cestičke klikaním o jedno políčko vedľa.
Následne hráč potvrdí svoje prechádzanie po poli -> na rade bude ďaľší hráč.

Jeden hráč môže zobrať viac pokladov.
Vyhráva hráč ktorý nazbiera ako prvý toľko pokladov ako počet pokladov v balíčku / počet hráčov.
