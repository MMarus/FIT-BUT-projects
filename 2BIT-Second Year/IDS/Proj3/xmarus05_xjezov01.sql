-- SQL skript pre predmet IDS
--
-- @author: Filip Ježovica, xjezov01
-- @author: Marek Marušic, xmarus05

-- zmazanie tabuliek pre opetovne spustanie skriptu
DROP TABLE NavstevaOrdinacie CASCADE CONSTRAINTS;
DROP TABLE Faktura CASCADE CONSTRAINTS;
DROP TABLE Liek CASCADE CONSTRAINTS;
DROP TABLE Poistovna CASCADE CONSTRAINTS;
DROP TABLE Pacient CASCADE CONSTRAINTS;
DROP TABLE ExternePracovisko CASCADE CONSTRAINTS;
DROP TABLE Vykon CASCADE CONSTRAINTS;
DROP TABLE Ockovanie CASCADE CONSTRAINTS;
DROP TABLE Vysetrenie CASCADE CONSTRAINTS;
DROP TABLE Plan CASCADE CONSTRAINTS;
DROP TABLE PredpisanyLiek CASCADE CONSTRAINTS;
DROP TABLE Odporucenie CASCADE CONSTRAINTS;
DROP TABLE PocasNavstevy CASCADE CONSTRAINTS;
DROP TABLE VykonMaPlan CASCADE CONSTRAINTS;



-- ********** Vytvorenie tabuliek a zakladnych stlpcov *************
CREATE TABLE NavstevaOrdinacie (
	ID NUMBER NOT NULL,
	Datum DATE NOT NULL,
	Poznamky VARCHAR2(255),
	id_Pacient NUMBER NOT NULL
);

CREATE TABLE Faktura (
	ID NUMBER NOT NULL,
	Datum_vystavenia DATE NOT NULL,
	Suma NUMBER NOT NULL,
	Splatnost DATE NOT NULL,
	id_NavstevaOrdinacie NUMBER NOT NULL,
	id_Poistovna NUMBER NOT NULL
);

CREATE TABLE Liek (
	ID NUMBER NOT NULL,
	Nazov VARCHAR2(50) NOT NULL,
	Zlozenie VARCHAR2(50) NOT NULL,
	Forma_podania VARCHAR2(50) NOT NULL,
	Odporucane_davkovanie VARCHAR2(50) NOT NULL,
	Popis VARCHAR2(255) NOT NULL
);

CREATE TABLE Plan (
	ID NUMBER NOT NULL,
	Planovany_datum DATE NOT NULL,
	Poznamky VARCHAR2(255),
	id_Pacient NUMBER NOT NULL
);

CREATE TABLE Poistovna (
	ID NUMBER NOT NULL,
	Nazov VARCHAR2(50) NOT NULL,
	Adresa VARCHAR2(100) NOT NULL
);


CREATE TABLE Pacient (
	ID NUMBER NOT NULL,
	Rodne_cislo NUMBER NOT NULL,
	Meno VARCHAR2(25) NOT NULL,
	Priezvisko VARCHAR2(25) NOT NULL,
	Adresa VARCHAR2(100) NOT NULL,
	Krvna_skupina VARCHAR2(3) NOT NULL,
	Poznamky VARCHAR2(255),
	id_Poistovna NUMBER NOT NULL
);

-- tu je CHECK ICPE
CREATE TABLE ExternePracovisko (
	ID NUMBER NOT NULL,
	Nazov VARCHAR2(30) NOT NULL,
	Adresa VARCHAR2(100) NOT NULL,
 	Specializacia VARCHAR2(50) NOT NULL,
 	Lekar VARCHAR2(50) NOT NULL,
  	ICPE NUMBER NOT NULL
);


CREATE TABLE Vykon (
	ID NUMBER NOT NULL,
	Nazov VARCHAR2(30) NOT NULL,
	Popis VARCHAR2(255) NOT NULL
);

---DEGENERALIZACIA
CREATE TABLE Ockovanie (
	ID NUMBER NOT NULL,
	Doba_ucinku NUMBER NOT NULL,
	Vyrobca VARCHAR2(30) NOT NULL
);


CREATE TABLE Vysetrenie (
	ID NUMBER NOT NULL,
	Casova_narocnost NUMBER NOT NULL
);
---END DEGENERALIZACIA

CREATE TABLE PredpisanyLiek (
    ID NUMBER NOT NULL,
	PocetBaleni NUMBER NOT NULL,
	Davkovanie VARCHAR2(50) NOT NULL,
	id_NavstevaOrdinacie NUMBER NOT NULL,
	id_Liek NUMBER NOT NULL
);

CREATE TABLE Odporucenie (
    ID NUMBER NOT NULL,
    id_NavstevaOrdinacie NUMBER NOT NULL,
    id_ExternePracovisko NUMBER NOT NULL
);

CREATE TABLE PocasNavstevy (
    ID NUMBER NOT NULL,
    id_NavstevaOrdinacie NUMBER NOT NULL,
    id_Vykon NUMBER NOT NULL
);

CREATE TABLE VykonMaPlan (
    ID NUMBER NOT NULL,
    id_Vykon NUMBER NOT NULL,
    id_Plan NUMBER NOT NULL
);


-- **************** Primare kluce ************************
ALTER TABLE NavstevaOrdinacie ADD CONSTRAINT PK_NavstevaOrdinacie PRIMARY KEY (ID);
ALTER TABLE Faktura ADD CONSTRAINT PK_Faktura PRIMARY KEY (ID);
ALTER TABLE Liek ADD CONSTRAINT PK_Liek PRIMARY KEY (ID);
ALTER TABLE Poistovna ADD CONSTRAINT PK_Poistovna PRIMARY KEY (ID);
ALTER TABLE Pacient ADD CONSTRAINT PK_Pacient PRIMARY KEY (ID);
ALTER TABLE ExternePracovisko ADD CONSTRAINT PK_ExternePracovisko PRIMARY KEY (ID);
ALTER TABLE Vykon ADD CONSTRAINT PK_Vykon PRIMARY KEY (ID);
ALTER TABLE Ockovanie ADD CONSTRAINT PK_Ockovanie PRIMARY KEY (ID);
ALTER TABLE Vysetrenie ADD CONSTRAINT PK_Vysetrenie PRIMARY KEY (ID);
ALTER TABLE Plan ADD CONSTRAINT PK_Plan PRIMARY KEY (ID);

--Pimarne kluce pre N:N
ALTER TABLE Odporucenie ADD CONSTRAINT PK_Odporucenie PRIMARY KEY (id_NavstevaOrdinacie, id_ExternePracovisko);
ALTER TABLE PredpisanyLiek ADD CONSTRAINT PK_PredpisanyLiek PRIMARY KEY (id_NavstevaOrdinacie, id_Liek);
ALTER TABLE PocasNavstevy ADD CONSTRAINT PK_PocasNavstevy PRIMARY KEY (id_NavstevaOrdinacie, id_Vykon);
ALTER TABLE VykonMaPlan ADD CONSTRAINT PK_VykonMaPlan PRIMARY KEY (id_Vykon, id_Plan);


-- *************** FOREIGN kluce **************************
-- Pridanie Vztahov 1 : N
ALTER TABLE  Faktura ADD CONSTRAINT FK_NavstevaOrdinacie FOREIGN KEY(id_NavstevaOrdinacie) REFERENCES NavstevaOrdinacie;    ---0..n     je
ALTER TABLE  Faktura ADD CONSTRAINT FK_Poistovna FOREIGN KEY(id_Poistovna) REFERENCES Poistovna;                            ---0..n     je
ALTER TABLE  Pacient ADD CONSTRAINT FK_Poistovna2 FOREIGN KEY(id_Poistovna) REFERENCES Poistovna;                            ---0..n     je
ALTER TABLE  NavstevaOrdinacie ADD CONSTRAINT FK_Pacient FOREIGN KEY(id_Pacient) REFERENCES Pacient;                        ---1..n     je
ALTER TABLE  Plan ADD CONSTRAINT FK_Pacient2 FOREIGN KEY(id_Pacient) REFERENCES Pacient;                                     ---1..n     je


--N:N
---Odporucenie tabulka
ALTER TABLE  Odporucenie ADD CONSTRAINT FK_NavstevaOrdinacie3 FOREIGN KEY(id_NavstevaOrdinacie) REFERENCES NavstevaOrdinacie ON DELETE CASCADE;
ALTER TABLE  Odporucenie ADD CONSTRAINT FK_ExternePracovisko FOREIGN KEY(id_ExternePracovisko) REFERENCES ExternePracovisko ON DELETE CASCADE;
---PredpisanyLiek tabulka
ALTER TABLE  PredpisanyLiek ADD CONSTRAINT FK_NavstevaOrdinacie4 FOREIGN KEY(id_NavstevaOrdinacie) REFERENCES NavstevaOrdinacie ON DELETE CASCADE;
ALTER TABLE  PredpisanyLiek ADD CONSTRAINT FK_Liek FOREIGN KEY(id_Liek) REFERENCES Liek ON DELETE CASCADE;
---PocasNavstevy tabulka
ALTER TABLE  PocasNavstevy ADD CONSTRAINT FK_NavstevaOrdinacie5 FOREIGN KEY(id_NavstevaOrdinacie) REFERENCES NavstevaOrdinacie ON DELETE CASCADE;
ALTER TABLE  PocasNavstevy ADD CONSTRAINT FK_Vykon FOREIGN KEY(id_Vykon) REFERENCES Vykon ON DELETE CASCADE;
---VykonMaPlan tabulka
ALTER TABLE  VykonMaPlan ADD CONSTRAINT FK_Plan FOREIGN KEY(id_Plan) REFERENCES Plan ON DELETE CASCADE;
ALTER TABLE  VykonMaPlan ADD CONSTRAINT FK_Vykon2 FOREIGN KEY(id_Vykon) REFERENCES Vykon ON DELETE CASCADE;

---DEGENERALIZACIA
ALTER TABLE  Ockovanie ADD CONSTRAINT FK_Vykon3 FOREIGN KEY(ID) REFERENCES Vykon ON DELETE CASCADE;
ALTER TABLE  Vysetrenie ADD CONSTRAINT FK_Vykon4 FOREIGN KEY(ID) REFERENCES Vykon ON DELETE CASCADE;

--check ICPE
--ALTER TABLE ExternePracovisko ADD CONSTRAINT CK_ExternePracovisko CHECK ( luhn(ICPE) = 0 )

--TRIGGER NA CHECK LUNGH ALGORITMU
ALTER session SET nls_date_format='dd.mm.yy';
SET serveroutput ON;

CREATE OR REPLACE TRIGGER tr_luhn
  BEFORE INSERT OR UPDATE OF ICPE ON ExternePracovisko
  FOR EACH ROW
DECLARE
   holder NUMBER := 0;
   y NUMBER := 0;
   conv_string VARCHAR2(20);
   vysledok NUMBER := 0;
BEGIN
 conv_string := to_char(:new.ICPE);
  for x in 1..length(conv_string) loop
    y := to_number(substr(conv_string, -x, 1));
    if mod(x,2) = 0 then
      y := y * 2;
      if y > 9 then
        y := y - 9;
      end if;
    end if;
    holder := holder + y; 
  end loop;
  vysledok :=  MOD(holder, 10);
  IF ( vysledok <> 0 ) THEN
    Raise_Application_Error (-20203, 'Zadane ICPE nesplna luhnov algoritmus');
  END IF;
END tr_luhn;
/
show errors
ALTER session SET nls_date_format='dd.mm.yyyy';


--CHECK rodneho cisla 
--ALTER TABLE Pacinet ADD CONSTRAINT CK_Rodne_cislo CHECK ( Rodne_cislo LIKE '[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]' OR Rodne_cislo LIKE '[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]' );


-- *************** VKLADANIE DO TABULIEK ********************
--Poistovne
INSERT INTO Poistovna VALUES('000', 'Vseobecna poistovna', '612000 Brno');
INSERT INTO Poistovna VALUES('001', 'INA poistovna', '112000 Praha');
INSERT INTO Poistovna VALUES('002', 'Dalsia poistovna', '64000 Strání');
INSERT INTO Poistovna VALUES('003', 'Velka poistovna', '42000 Brno');
INSERT INTO Poistovna VALUES('004', 'MALA poistovna', '42000 Brno');

--Pracoviska
INSERT INTO ExternePracovisko VALUES('001' , 'Nemocnica v Brne' , 'Semilasso, 11111 Brno' , 'Ocne oddelenie' , 'MuDr. Jaro Jaroslav', '10023612');
INSERT INTO ExternePracovisko VALUES('002' , 'NsP NMNV' , 'Nemocnicna 21, 99999 Nove Mesto nad Vahom' , 'Krcne oddelenie' , 'MuDr. Jan Janíček', '10023612');
INSERT INTO ExternePracovisko VALUES('003' , 'Nemocnica v Brne' , 'Ulice 33 55555 Uherske hradiste' , 'Kardio oddelenie' , 'MuDr. Peter Petríček', '10023612');
INSERT INTO ExternePracovisko VALUES('004' , 'NsP v Brode' , 'Ulice 34 55555 Uherske hradiste' , 'Ušné oddelenie' , 'MuDr. Juraj Jurkový', '10023612');
INSERT INTO ExternePracovisko VALUES('005' , 'Poliklinika a.s.' , 'Ulica 12 333333 Bratislava' , 'Úrazove oddelenie' , 'MuDr. Aneta Anetová', '10023612');

--Lieky
INSERT INTO Liek VALUES('001', 'Penicilin 500mg', 'latka1 15mg, latka2 20mg' , 'oralne' , '1tbl/8h', 'Nejaky popis');
INSERT INTO Liek VALUES('002', 'Ibalgin 500mg', 'latka1 20mg, latka2 20mg' , 'oralne' , '2tbl/4h', 'Nejaky popis2');
INSERT INTO Liek VALUES('003', 'Brufen 250', 'latka1 1mg, latka2 200mg' , 'oralne' , '20ml/5h', 'Nejaky popis3');
INSERT INTO Liek VALUES('004', 'Nasivin 15', 'latka1 1mg, latka2 3mg' , 'nasalne' , '2x/8h', 'Nejaky popis4');

--Pacienti (maju poistovnu)
INSERT INTO Pacient VALUES('001', '0000001111', 'Filip', 'Jezovica', 'Bozetechova 2, 61200 Brno', 'A', 'Alergia na Penicilin.', '001');
INSERT INTO Pacient VALUES('002', '1111112222', 'Marek', 'Marusic',  'Bozetechova 2, 61200 Brno', 'B', 'Alergia na Ibalgin.', '002');
INSERT INTO Pacient VALUES('003', '2222223333', 'Nikto', 'Niktos',  'Bozetechova 1, 61200 Brno', '0', 'Bez poznamky.', '002');
INSERT INTO Pacient VALUES('004', '3333334444', 'Volakto', 'Nejaky',  'Bozetechova 1, 61200 Brno', 'B+', 'Bez poznamky.', '002');

--NavstevaOrdinacie (ma pacienta)
INSERT INTO NavstevaOrdinacie VALUES('001', '20/DEC/2014', 'Vsetko prebehlo ok.', '001');
INSERT INTO NavstevaOrdinacie VALUES('002', '20/DEC/2014', 'Toto bol novy pacient, novo registrovany.', '003');
INSERT INTO NavstevaOrdinacie VALUES('003', '20/DEC/2015', 'Vsetko prebehlo ok.', '002');

--Plany (maju PACIENTA a VYKON)
INSERT INTO Plan VALUES('001', '20/DEC/2015' , 'Priniest karticku poistenca.' , '001');
INSERT INTO Plan VALUES('002', '25/DEC/2015' , 'Priniest zdravotnu kartu' , '002');
INSERT INTO Plan VALUES('003', '30/DEC/2015' , 'Nejaka poznamka.' , '001');

--Faktury (ma : Navstevu ordinacie, Poistovnu)
INSERT INTO Faktura VALUES('001', '20/DEC/2015', '50', '30/DEC/2014', '001', '001');
INSERT INTO Faktura VALUES('002', '20/DEC/2015', '20', '30/DEC/2014', '002', '002');
INSERT INTO Faktura VALUES('003', '20/DEC/2015', '300', '30/DEC/2015', '003', '002');

--Vykon
INSERT INTO Vykon VALUES('001', 'Prehliadka', 'Pravidelna.');
INSERT INTO Vykon VALUES('002', 'Meranie tlaku', 'meranie tlakovym digitalnym pristrojom');
INSERT INTO Vykon VALUES('003', 'Odber krvi', 'Iihla 2mm.');
INSERT INTO Vykon VALUES('004', 'Ockovanie TBC', 'Iihla 1mm.');
INSERT INTO Vykon VALUES('005', 'Ockovanie ZLTACKA', 'Davka c.1');

--Ockovanie (prve je FK pre Ockovanie) preratane na mesiace
INSERT INTO Ockovanie VALUES('004', '6', 'VAKCINOTROPIL');
INSERT INTO Ockovanie VALUES('005', '12', 'ZENTIVA');

--Vysetrenie (prve je FK pre Vysetrenie)  preratane na minuty
INSERT INTO Vysetrenie VALUES('001', '15');
INSERT INTO Vysetrenie VALUES('003', '20');
INSERT INTO Vysetrenie VALUES('002', '10');

--PredpisanyLiek (002 - NAVSTEVA , 001 - LIEK)
INSERT INTO PredpisanyLiek VALUES('001', '2', '1tbl/8h', '002', '001');
INSERT INTO PredpisanyLiek VALUES('002', '1', '10ml/8h', '002', '003');
INSERT INTO PredpisanyLiek VALUES('003', '5', '3tbl/12h', '001', '002');

--Odporucenie (ID,  NAVSTEVA,  EXTERNE PRACOVISKO)
INSERT INTO Odporucenie VALUES('001', '001' , '004');
INSERT INTO Odporucenie VALUES('001', '001' , '002');
INSERT INTO Odporucenie VALUES('001', '003' , '001');

--PocasNavstevy (ID,  NAVSTEVA, VYKON)
INSERT INTO PocasNavstevy VALUES('001', '002', '001');
INSERT INTO PocasNavstevy VALUES('002', '002', '002');
INSERT INTO PocasNavstevy VALUES('003', '001', '003');

--VykonMaPlan (ID, VYKON, PLAN)
INSERT INTO VykonMaPlan VALUES('001', '001', '001');
INSERT INTO VykonMaPlan VALUES('002', '002', '001');
INSERT INTO VykonMaPlan VALUES('003', '003', '001');


-- SQL skript obsahující dotazy SELECT musí obsahovat konkrétně alespoň dva dotazy využívající spojení dvou tabulek

-- Vypise informacie o Pacientovi (meno, priezvisko) a kedy navstivil ordinaciu
SELECT Pacient.Meno, Pacient.Priezvisko, NavstevaOrdinacie.Datum
FROM NavstevaOrdinacie, Pacient
WHERE Pacient.ID = NavstevaOrdinacie.id_Pacient;

-- Vypise nazov poistovne a vsetky jej faktury (cena, splatnost)
SELECT Poistovna.Nazov, Faktura.Suma, Faktura.Splatnost
FROM Poistovna, Faktura
WHERE Poistovna.ID = Faktura.id_Poistovna;

-- Vyber info o Predpisanych liekoch pocas navstevy, ( Vypise Datum Navstevy, Nazov lieku a predpisany pocet baleni a predpisane davkovanie) 
SELECT NavstevaOrdinacie.Datum,NavstevaOrdinacie.Poznamky, Liek.Nazov, Liek.Zlozenie, Liek.Forma_podania, PredpisanyLiek.PocetBaleni, PredpisanyLiek.Davkovanie
FROM NavstevaOrdinacie, Liek, PredpisanyLiek
WHERE NavstevaOrdinacie.ID = PredpisanyLiek.id_NavstevaOrdinacie and Liek.ID = PredpisanyLiek.id_Liek;

-- Ktora Poistovna je u pacientov najoblubenejsia
SELECT id_Poistovna, COUNT(id_Poistovna) FROM Pacient GROUP BY id_Poistovna;
-- Kolko sa spolu vyfakturovalo za urcitu poistovnu
SELECT Poistovna.Nazov, SUM(Faktura.Suma) FROM Faktura, Poistovna WHERE Faktura.id_Poistovna = Poistovna.ID  GROUP BY Poistovna.Nazov;

-- Vyber iba tie Vykony ktore boli uz niekedy pouzite (pocas Navstevy)
SELECT Vykon.ID, Vykon.Nazov, Vykon.Popis 
FROM Vykon 
WHERE EXISTS 
(SELECT id_Vykon FROM PocasNavstevy WHERE Vykon.ID = PocasNavstevy.id_Vykon);

-- Vypise pacienta, ktoreho vykon stal poistovnu 0-100 korun 
SELECT * FROM Pacient WHERE ID IN 
(SELECT id_Pacient FROM NavstevaOrdinacie WHERE ID IN
(SELECT id_NavstevaOrdinacie FROM Faktura WHERE Suma > 0 and Suma < 100));
