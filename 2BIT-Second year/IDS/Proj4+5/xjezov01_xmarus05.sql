-- SQL skript pre predmet IDS
--
-- @author: Filip Ježovica, xjezov01
-- @author: Marek Marušic, xmarus05
SET serveroutput ON;
--SET AUTOCOMMIT OFF; -- OPTIMALIZACIA

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

-- zmazanie sekvencii pre PKAUTOINCREMENT
DROP SEQUENCE PacientSEQPK;
DROP SEQUENCE NavstevaSEQPK;
DROP SEQUENCE LiekSEQPK;
DROP SEQUENCE PredpisSEQPK;

-- vytvorit sekvencie pre PKAUTOINCREMENT
CREATE SEQUENCE PacientSEQPK;
CREATE SEQUENCE NavstevaSEQPK;
CREATE SEQUENCE LiekSEQPK;
CREATE SEQUENCE PredpisSEQPK;


-- *************************************************************************************************************************************
-- **************************************************** VYTVORENIE TABULIEK ************************************************************
-- *************************************************************************************************************************************
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

---GENERALIZACIA
CREATE TABLE Ockovanie (
  ID NUMBER NOT NULL,
  Doba_ucinku NUMBER NOT NULL,
  Vyrobca VARCHAR2(30) NOT NULL
);

CREATE TABLE Vysetrenie (
  ID NUMBER NOT NULL,
  Casova_narocnost NUMBER NOT NULL
);
---END GENERALIZACIA

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


-- *************************************************************************************************************************************
-- ******************************************************** KLUCE **********************************************************************
-- *************************************************************************************************************************************
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
ALTER TABLE  Faktura ADD CONSTRAINT FK_NavstevaOrdinacie FOREIGN KEY(id_NavstevaOrdinacie) REFERENCES NavstevaOrdinacie;    ---0..n
ALTER TABLE  Faktura ADD CONSTRAINT FK_Poistovna FOREIGN KEY(id_Poistovna) REFERENCES Poistovna;                            ---0..n
ALTER TABLE  Pacient ADD CONSTRAINT FK_Poistovna2 FOREIGN KEY(id_Poistovna) REFERENCES Poistovna;                           ---0..n
ALTER TABLE  NavstevaOrdinacie ADD CONSTRAINT FK_Pacient FOREIGN KEY(id_Pacient) REFERENCES Pacient;                        ---1..n
ALTER TABLE  Plan ADD CONSTRAINT FK_Pacient2 FOREIGN KEY(id_Pacient) REFERENCES Pacient;                                    ---1..n


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



-- *************************************************************************************************************************************
-- **************************************************** TRIGGERY ***********************************************************************
-- *************************************************************************************************************************************

-- ********************** TRIGGER @ EXTERNEPRACOVISKO **************************
-- ******************* OVERENIE ICPE AMBULANCIE ( LUHN ) ***********************
-- *****************************************************************************
ALTER session SET nls_date_format='dd.mm.yyyy';
CREATE OR REPLACE TRIGGER tr_luhn
  BEFORE INSERT OR UPDATE OF ICPE ON ExternePracovisko
  FOR EACH ROW
DECLARE
   holder NUMBER := 0;
   y NUMBER := 0;
   conv_string VARCHAR2(20);
   vysledok NUMBER := 0;
BEGIN
--nove vkladane ICPE sa rozdeli na znaky
 conv_string := to_char(:new.ICPE);
 --kazdy znak sa zkonvertuje na cislo, kazde druhe vynasobime *2
  for x in 1..length(conv_string) loop
    y := to_number(substr(conv_string, -x, 1));
    if mod(x,2) = 0 then
      y := y * 2;
      
      if y > 9 then
        y := y - 9;
      end if;
    end if;
    --drzi hodnotu medzi vysledku 
    holder := holder + y; 
  end loop;
  vysledok :=  MOD(holder, 10);
  --ak holder( drzi hodnotu medzi vypoctu) nieje delitelny 10 -> ERROR
  IF ( vysledok <> 0 ) THEN
    Raise_Application_Error (-20203, 'Zadane ICPE nesplna luhnov algoritmus');
  END IF;
END tr_luhn;
/
show errors
ALTER session SET nls_date_format='dd.mm.yyyy';


-- ************************* TRIGGER @ NAVSTEVA ********************************
-- ************ AUTOINKREMENT pri vkladani do tabulky NAVSTEVA *****************
-- *****************************************************************************
ALTER session SET nls_date_format='dd.mm.yyyy';
CREATE OR REPLACE TRIGGER AUTOINC_NAVSTEVA
  BEFORE INSERT ON NavstevaOrdinacie
  FOR EACH ROW
BEGIN
  :new.ID := NavstevaSEQPK.nextval;
END;
/
show errors
ALTER session SET nls_date_format='dd.mm.yyyy';


-- ************************** TRIGGER @ PACIENT ********************************
-- ************ AUTOINKREMENT pri vkladani do tabulky PACIENT ******************
-- *****************************************************************************
ALTER session SET nls_date_format='dd.mm.yyyy';
CREATE OR REPLACE TRIGGER AUTOINC_PACIENT
  BEFORE INSERT ON Pacient
  FOR EACH ROW
BEGIN
  :new.ID := PacientSEQPK.nextval;
END;
/
show errors
ALTER session SET nls_date_format='dd.mm.yyyy';


-- ************************* TRIGGER @ LIEK ************************************
-- ************** AUTOINKREMENT pri vkladani do tabulky LIEK *******************
-- *****************************************************************************
ALTER session SET nls_date_format='dd.mm.yyyy';
CREATE OR REPLACE TRIGGER AUTOINC_LIEK
  BEFORE INSERT ON Liek
  FOR EACH ROW
BEGIN
  :new.ID := LiekSEQPK.nextval;
END;
/
show errors
ALTER session SET nls_date_format='dd.mm.yyyy';


-- ********************** TRIGGER @ PREDPISANYLIEK *****************************
-- ********* AUTOINKREMENT pri vkladani do tabulky PREDPISANYLIEK **************
-- *****************************************************************************
ALTER session SET nls_date_format='dd.mm.yyyy';
CREATE OR REPLACE TRIGGER AUTOINC_PREDPIS
  BEFORE INSERT ON PredpisanyLiek
  FOR EACH ROW
BEGIN
  :new.ID := PredpisSEQPK.nextval;
END;
/
show errors
ALTER session SET nls_date_format='dd.mm.yyyy';



-- *************************************************************************************************************************************
-- ******************************************************* PROCEDURY *******************************************************************
-- *************************************************************************************************************************************

-- ******************************** FUNCKIA ************************************
-- ************ vrati aktualny datum posunuty o argument (POSUN) ***************
-- *****************************************************************************
create or replace FUNCTION  datum(posun IN NUMBER)
RETURN DATE
is 
    today DATE;
begin
    SELECT SYSDATE+posun INTO today FROM DUAL ;
    RETURN today;
end;
/

-- ******************************* PROCEDURA ***********************************
-- ********************* vypise plan na zadany pocet dni ***********************
-- ************************* (den, pacient, vykon) *****************************
-- *****************************************************************************
ALTER session SET nls_date_format='dd.mm.yyyy';
CREATE OR REPLACE PROCEDURE  show_plan(pocet_dni IN NUMBER)
is 
    --premenne
    -- POUZITIE EXPLICITNEHO KURZORU ( dalej sa vyuziva pomocou open, fetch)
    cursor c1 IS SELECT * FROM Plan WHERE Planovany_datum BETWEEN datum(-1) AND datum(pocet_dni) ORDER BY Planovany_datum;
    data_from_plan c1%ROWTYPE;
    v_meno Pacient.Meno%TYPE;
    v_priezvisko Pacient.Priezvisko%TYPE;
    --vykony 
begin
    open c1;
    <<OUTER_LOOP>>
    loop
        fetch c1 into data_from_plan;
        exit when c1%NOTFOUND;
    
        SELECT Meno, Priezvisko INTO v_meno, v_priezvisko  FROM Pacient WHERE Pacient.ID = data_from_plan.ID;
        
        -- POUZITIE IMPLICITNEHO KURZORU priamo vo for cykle
        <<INNER_LOOP>>
        for row1 in (SELECT Nazov  FROM VykonMaPlan JOIN Vykon ON VykonMaPlan.id_Vykon = Vykon.ID WHERE VykonMaPlan.id_Plan = data_from_plan.ID )
        loop
            DBMS_OUTPUT.PUT_LINE( 'Datum: ' ||    data_from_plan.Planovany_datum || ' (' || to_char(data_from_plan.Planovany_datum,'DY') || '), Meno: ' || v_meno || ', Priezvisko: ' ||  v_priezvisko  || ', Vykon: ' || row1.Nazov );
        end loop INNER_LOOP;

    end loop OUTER_LOOP;
    
    -- ODCHYTAVANIE CHYB (ak nastane akakolvek chyba)
    EXCEPTION
        WHEN OTHERS THEN
        Raise_Application_Error (-20999, 'Nastala chyba v procedure SHOW_PLAN!');

end;
/
show errors
ALTER session SET nls_date_format='dd.mm.yyyy';

-- ******************************* PROCEDURA ***********************************
-- ******************* kolko je pacientov u akej poistovne *********************
-- *****************************************************************************
ALTER session SET nls_date_format='dd.mm.yyyy';
CREATE OR REPLACE PROCEDURE  stat_Poist
is 
    count_All NUMBER;
begin
--Pocet pacientov dokopy
---FOR in poistovne
--- ziskame pocet pacientov / pocet dokopy 

    SELECT COUNT(*) INTO count_All FROM Pacient;
    
    -- POUZITIE IMPLICITNEHO KURZORA ( priamo v cykle for )
    for row1 in (SELECT Poistovna.Nazov, COUNT(Pacient.Meno) as CNT FROM Pacient, Poistovna WHERE Pacient.id_Poistovna = Poistovna.ID GROUP BY Poistovna.Nazov)
        loop
            DBMS_OUTPUT.PUT_LINE(  ROUND( (row1.CNT/count_All*100),2 )    ||'% pacientov v poistovni: ' ||    row1.Nazov );
        end loop;
    
    -- ODCHYTAVANIE CHYB ( delenie nulov, ostatne chyby )
    EXCEPTION
        WHEN ZERO_DIVIDE THEN
            Raise_Application_Error (-20005, 'ERROR: DELENIE NULOV v procedure STAT_POIST!');
        
        WHEN OTHERS THEN
            Raise_Application_Error (-20999, 'ERROR: Nastala chyba v procedure STAT_POIST!');

end;
/
show errors
ALTER session SET nls_date_format='dd.mm.yyyy';


-- *************************************************************************************************************************************
-- ****************************************************** INSERTY **********************************************************************
-- *************************************************************************************************************************************
--Poistovne
INSERT INTO Poistovna VALUES('000', 'Vseobecna poistovna', '612000 Brno');
INSERT INTO Poistovna VALUES('001', 'INA poistovna', '112000 Praha');
INSERT INTO Poistovna VALUES('002', 'Materializovana poistovna', '64000 Strání');
INSERT INTO Poistovna VALUES('003', 'Velka poistovna', '42000 Brno');
INSERT INTO Poistovna VALUES('004', 'MALA poistovna', '42000 Brno');

--Pracoviska
INSERT INTO ExternePracovisko VALUES('001' , 'Nemocnica v Brne' , 'Semilasso, 11111 Brno' , 'Ocne oddelenie' , 'MuDr. Jaro Jaroslav', '10023612');
INSERT INTO ExternePracovisko VALUES('002' , 'NsP NMNV' , 'Nemocnicna 21, 99999 Nove Mesto nad Vahom' , 'Krcne oddelenie' , 'MuDr. Jan Janíček', '10023612');
INSERT INTO ExternePracovisko VALUES('003' , 'Nemocnica v Brne' , 'Ulice 33 55555 Uherske hradiste' , 'Kardio oddelenie' , 'MuDr. Peter Petríček', '10023612');
INSERT INTO ExternePracovisko VALUES('004' , 'NsP v Brode' , 'Ulice 34 55555 Uherske hradiste' , 'Ušné oddelenie' , 'MuDr. Juraj Jurkový', '10023612');
INSERT INTO ExternePracovisko VALUES('005' , 'Poliklinika a.s.' , 'Ulica 12 333333 Bratislava' , 'Úrazove oddelenie' , 'MuDr. Aneta Anetová', '10023612');

--Lieky (AUTOINCREMENT)
INSERT INTO Liek VALUES('', 'Penicilin 500mg', 'latka1 15mg, latka2 20mg' , 'oralne' , '1tbl/8h', 'Nejaky popis');
INSERT INTO Liek VALUES('', 'Ibalgin 500mg', 'latka1 20mg, latka2 20mg' , 'oralne' , '2tbl/4h', 'Nejaky popis2');
INSERT INTO Liek VALUES('', 'Brufen 250', 'latka1 1mg, latka2 200mg' , 'oralne' , '20ml/5h', 'Nejaky popis3');
INSERT INTO Liek VALUES('', 'Nasivin 15', 'latka16 1mg, latka2 3mg' , 'nasalne' , '2x/8h', 'Nejaky popis4');
INSERT INTO Liek VALUES('', 'Nasivin 25', 'latka51 15mg, latka2 3mg' , 'nasalne' , '2x/8h', 'Nejaky popis5');
INSERT INTO Liek VALUES('', 'Liecivo1', 'latka15 12mg, latka2 3mg' , 'oralne' , '2x/8h', 'Nejaky popis1');
INSERT INTO Liek VALUES('', 'Liecivo2', 'latka21 51mg, latka2 3mg' , 'nasalne' , '2x/5h', 'Nejaky popis2');
INSERT INTO Liek VALUES('', 'Liecivo3', 'latka21 51mg, latka2 3mg' , 'oralne' , '3x/14h', 'Nejaky popis3');
INSERT INTO Liek VALUES('', 'Liecivo4', 'latka21 51mg, latka2 3mg' , 'nasalne' , '2x/2h', 'Nejaky popis4');
INSERT INTO Liek VALUES('', 'Liecivo5', 'latka21 51mg, latka2 3mg' , 'oralne' , '4x/3h', 'Nejaky popis5');
INSERT INTO Liek VALUES('', 'Liecivo6', 'latka15 12mg, latka2 3mg' , 'oralne' , '2x/5h', 'Nejaky popis6');
INSERT INTO Liek VALUES('', 'Liecivo7', 'latka21 51mg, latka2 3mg' , 'oralne' , '2x/7h', 'Nejaky popis7');
INSERT INTO Liek VALUES('', 'Liecivo8', 'latka21 51mg, latka2 3mg' , 'nasalne' , '5x/1h', 'Nejaky popis8');
INSERT INTO Liek VALUES('', 'Liecivo9', 'latka21 51mg, latka2 3mg' , 'nasalne' , '2x/2h', 'Nejaky popis9');
INSERT INTO Liek VALUES('', 'Liecivo10', 'latka21 51mg, latka2 3mg' , 'nasalne' , '2x/8h', 'Nejaky popis10');

--Pacienti (maju poistovnu) (AUTOINCREMENT)
INSERT INTO Pacient VALUES('', '0000001111', 'Filip', 'Jezovica', 'Bozetechova 2, 61200 Brno', 'A', 'Alergia na Penicilin.', '001');
INSERT INTO Pacient VALUES('', '1111112222', 'Marek', 'Marusic',  'Bozetechova 2, 61200 Brno', 'B', 'Alergia na Ibalgin.', '002');
INSERT INTO Pacient VALUES('', '2222223333', 'Nikto', 'Niktos',  'Bozetechova 1, 61200 Brno', '0', 'Bez poznamky.', '002');
INSERT INTO Pacient VALUES('', '3333334444', 'Volakto', 'Nejaky',  'Bozetechova 1, 61200 Brno', 'B+', 'Bez poznamky.', '002');

--NavstevaOrdinacie (ma pacienta) (AUTOINCREMENT)
INSERT INTO NavstevaOrdinacie VALUES('', '20/DEC/2014', 'Vsetko prebehlo ok.', '001');
INSERT INTO NavstevaOrdinacie VALUES('', '20/DEC/2014', 'Toto bol novy pacient, novo registrovany.', '003');
INSERT INTO NavstevaOrdinacie VALUES('', '20/DEC/2015', 'Bez popisu', '003');
INSERT INTO NavstevaOrdinacie VALUES('', '30/DEC/2015', 'abcdcdcsdvgs', '004');

--Plany (nakonci PACIENT)
INSERT INTO Plan VALUES('001', '22/APR/2015' , 'Priniest karticku poistenca.' , '001');
INSERT INTO Plan VALUES('002', '21/APR/2015' , 'Priniest zdravotnu kartu' , '002');
INSERT INTO Plan VALUES('003', '23/APR/2015' , 'Nejaka poznamka.' , '001');

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

--PredpisanyLiek (002 - NAVSTEVA , 001 - LIEK) ***************************************** OPTIMALIZACIA ***************************
INSERT ALL
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '001','1' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '001','2' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '001','3' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '001','4' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '001','5' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '001','6' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '001','7' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '001','8' )
 
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '002','1' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '002','2' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '002','3' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '002','4' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '002','5' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '002','6' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '002','7' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '002','8' )
 
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '003','1' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '003','2' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '003','3' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '003','4' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '003','5' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '003','6' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '003','7' )
 INTO PredpisanyLiek VALUES('', '2', '1tbl/8h', '003','8' )
SELECT * FROM dual;



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

INSERT INTO VykonMaPlan VALUES('004', '003', '002');
INSERT INTO VykonMaPlan VALUES('005', '005', '002');


-- *************************************************************************************************************************************
-- ***************************************************** SELECTY ***********************************************************************
-- *************************************************************************************************************************************

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

-- Vypise pacienta, ktoreho vykon stal poistovnu 0-100 E/CZK 
SELECT * FROM Pacient WHERE ID IN 
(SELECT id_Pacient FROM NavstevaOrdinacie WHERE ID IN
(SELECT id_NavstevaOrdinacie FROM Faktura WHERE Suma > 0 and Suma < 100));



-- *************************************************************************************************************************************
-- ************************************************** MATERIALIZOVANY POHLAD ***********************************************************
-- *************************************************************************************************************************************

-- ( Oblubenost poistovne - Vypise vsetky poistovne a ku kazdej kolko pacientov ju ma, zoradene od najoblubenejsej )
CREATE MATERIALIZED VIEW LOG ON Pacient WITH PRIMARY KEY, ROWID(id_Poistovna)
INCLUDING NEW VALUES;

CREATE MATERIALIZED VIEW LOG ON Poistovna WITH PRIMARY KEY, ROWID(Nazov)
INCLUDING NEW VALUES;

DROP MATERIALIZED VIEW MV_OBL_POIS;
CREATE MATERIALIZED VIEW MV_OBL_POIS
BUILD IMMEDIATE        -- naplni pohlad ihned po jeho vytvoreni
REFRESH FAST ON COMMIT -- aktualizuje pohlad podla logov MASTER TAB po commite
ENABLE QUERY REWRITE   -- moze ho pouzit optimalizator napr keby sa vola rovkaky select tak sa zavola tento pohlad
AS SELECT Poistovna.Nazov as NAZOV_POISTOVNE, COUNT(Pacient.id_Poistovna) AS POCET_LUDI
FROM Pacient, Poistovna WHERE Poistovna.ID = Pacient.id_Poistovna GROUP BY Poistovna.Nazov ;


-- DEMOSTRACIA MV -> najskor sa pozrieme na materializovany pohlad mame vnom poistovnu:'MATERIALIZOVANA' ktora ma X pacientov
SELECT * FROM MV_OBL_POIS;
-- vlozime dalsieho pacienta ktory patri do urcitej POISTOVNE (demonstracia Materialised view)
INSERT INTO Pacient VALUES('', '3333333333', 'Novy', 'Pacient',  'Materializovany 3, 61200 Pohlad', 'B+', 'Bez poznamky.', '002');
INSERT INTO Pacient VALUES('', '4444444444', 'Novy', 'Pacient',  'Materializovany 4, 61200 Pohlad', 'B+', 'Bez poznamky.', '002');
COMMIT;

-- po pridani sa pozrieme na MV a vidime ze po COMMITE ma 'MATERIALIZOVANA' poistovna uz X+2 pacientov - uspesny update
SELECT * FROM MV_OBL_POIS;




-- *************************************************************************************************************************************
-- ************************************************* UDELENIE PRAV *********************************************************************
-- *************************************************************************************************************************************

-- sestricka musi doplnat poskytnute informacie o liekoch,extrenych pracoviskach,...
GRANT ALL ON Liek TO XJEZOV01;
GRANT ALL ON ExternePracovisko TO XJEZOV01;
GRANT ALL ON Poistovna TO XJEZOV01;
GRANT ALL ON Vykon TO XJEZOV01;
GRANT ALL ON Ockovanie TO XJEZOV01;
GRANT ALL ON Vysetrenie TO XJEZOV01;

-- sestricka moze i pridavat vykony (napr ockovanie, odber krvi, administrativne zalezitosti), menit PLANY
GRANT ALL ON PocasNavstevy TO XJEZOV01;
GRANT ALL ON VykonMaPlan TO XJEZOV01;
GRANT ALL ON Plan TO XJEZOV01;

-- sestricka moze vlozit,upravit,vypisat PACIENTOV a NAVSTEVU v ORDINACII , NEMOZE MAZAT
GRANT INSERT,UPDATE,SELECT ON NavstevaOrdinacie TO XJEZOV01;
GRANT INSERT,UPDATE,SELECT ON Pacient TO XJEZOV01;

--Lieky moze predpisat iba doktor, sestricka iba prehliadat
GRANT SELECT ON PredpisanyLiek TO XJEZOV01;
GRANT SELECT ON Odporucenie TO XJEZOV01;

-- mozme vlozit fakturu a vypisat,  NEMOZE sa odstranit ani upravovat
GRANT SELECT, INSERT ON Faktura TO XJEZOV01;

-- prava na metody ( moze sputat aj sestricka )
GRANT EXECUTE ON show_plan TO XJEZOV01;
GRANT EXECUTE ON stat_Poist TO XJEZOV01;

--pridelit prava na pohlad
GRANT SELECT ON MV_OBL_POIS TO XJEZOV01;





-- *************************************************************************************************************************************
-- ************************************************ EXPLAIN PLAN ***********************************************************************
-- *************************************************************************************************************************************
--kolko razy bol predpisany urcity liek
DROP INDEX INDEX_LIEK;

explain plan for
select Nazov, sum(POCETBALENI) as POCET_BALENI, count(Nazov) as POCET_PREDPISOV from  PredpisanyLiek P join Liek L on L.ID = P.id_Liek  group by Nazov ORDER BY POCET_BALENI DESC;

select plan_table_output from table(dbms_xplan.display());



CREATE INDEX INDEX_LIEK ON PredpisanyLiek(POCETBALENI);

explain plan for
select Nazov, sum(POCETBALENI) as POCET_BALENI, count(Nazov) as POCET_PREDPISOV from  PredpisanyLiek P join Liek L on L.ID = P.id_Liek  group by Nazov ORDER BY POCET_BALENI DESC;

select plan_table_output from table(dbms_xplan.display());


-- + DEMONSTRACIA OPTIMALIZATORU NA MATERIALIZOVANOM POHLADE
explain plan for
SELECT Poistovna.Nazov as NAZOV_POISTOVNE, COUNT(Pacient.id_Poistovna) AS POCET_LUDI
FROM Pacient, Poistovna WHERE Poistovna.ID = Pacient.id_Poistovna GROUP BY Poistovna.Nazov ;

select plan_table_output from table(dbms_xplan.display());


-- *************************************************************************************************************************************
-- ************************************************ EXEC PROCEDUR **********************************************************************
-- *************************************************************************************************************************************
exec show_plan(5);
exec stat_Poist;


COMMIT;



-- *************************************************************************************************************************************
-- ******************************************** DEMONSTRACIA TRANSAKCII ***************************************************************
-- *************************************************************************************************************************************
SET AUTOCOMMIT OFF; -- v kazdom @TERMINALY
-- *******************************************
-- ********* TRANSAKCIA # 1 (@TERM 1) ********
-- *******************************************
-- ($KROK 1)
-- ulozime si stav do savepoint ( pre domonstraciu UVOLNENIA ZAMKU pri pouziti ROLLBACK, ktory NEsposobi ODOMKNUTIE UZ ZABLOKOVANYCH)
savepoint before_Poznamky_Lock;
update NavstevaOrdinacie set Poznamky = 'Test transakcie1'; -- TRANSAKCIA # 1  -> zacne vlastnit zamok - uzamkne pristup

-- ($KROK 3)
rollback to savepoint before_Poznamky_Lock; -- UVLONENIE ZAMKU bez skoncenia TRANSAKCIE # 1
--                                          -- tych ktory su uz zablokovany NEPUSTI ( TRANSAKCIA # 2 v $KROKU 2 ostava blokovana )
--                                          -- ak pride nova transakcia (napr TRANSAKCIA # 3) moze vykonavat zmeny

-- ($KROK 5)
commit; -- potrvdi prevadzane zmeny a ODBLOKUJE TRANSAKCIU # 2

-- *******************************************
-- ********* TRANSAKCIA # 2 (@TERM 2) ********
-- *******************************************
-- ($KROK 2)
update NavstevaOrdinacie set Poznamky = 'Test transakcie c.2'; --  tato transakcia bude ZABLOKOVANA koli TRANSAKCII # 1, bude cakat na odblokovanie

-- ($KROK 6) transakcia bola odblokovana , mozme pokracovat v TRANSAKCII # 2 - ta ZACNE VLASTNIT ZAMOK
commit; -- potvrdi zmeny a UVOLNI ZAMOK

-- *******************************************
-- ********* TRANSAKCIA # 3 (@TERM 3) ********
-- *******************************************
-- ($KROK 4)
update NavstevaOrdinacie set Poznamky = 'Test transakcie c.3'; -- tato transakcia sa moze vykonat ZACNE VLASTNIT ZAMOK
commit; -- potvrdi zmeny a ULOVNI ZAMOK ( TRANSAKCIA # 2 je stale blokovana TRANSAKCIOU # 1, uvolnime ju v kroku 5 )