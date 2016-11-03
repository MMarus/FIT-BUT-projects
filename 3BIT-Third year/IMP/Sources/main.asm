;Autor: Marek Marusic
;Email: xmarus05@stud.fit.vutbr.cz
;kostra suboru vygenerovana pomocou Code Warrior
;"seg_tbl" a "interrupt vector table" zdroj: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IMP-IT/lectures/dem_cv/imp_demo002.asm.txt?cid=10315
;Subor je original
;Posledna zmena: 17.12.2015 23:15

;*** MOJE PREMENNE ***
DILswitch:              equ    $000000F0 
continueFlag:           equ    $000000F1   

leftSegOUT:             equ    $000000E0           ;Prevedena hodnota pripravena na vypis
rightSegOUT:            equ    $000000E1           ;Prevedena hodnota pripravena na vypis

leftSEG7:               equ    $000000E2           ;Naozajstna hodnota
rightSEG7:              equ    $000000E3           ;Naozajstna hodnota

CNTN0:                  equ     $000000C0
CNTN1:                  equ     $000000C1
CNTN2:                  equ     $000000C2
CNTN3:                  equ     $000000C3
  

;*******************************************************************
;* This stationery serves as the framework for a user application. *
;* For a more comprehensive program that demonstrates the more     *
;* advanced functionality of this processor, please see the        *
;* demonstration applications, located in the examples             *
;* subdirectory of the "Freescale CodeWarrior for HC08" program    *
;* directory.                                                      *
;*******************************************************************

; Include derivative-specific definitions
            INCLUDE 'derivative.inc'
;
; export symbols
;
            XDEF _Startup         
            ABSENTRY _Startup

;
; variable/data section
;
            ORG    RAMStart         ; Insert your data definition here
TMP         DS.B   1

;
; code section
;
            ORG    ROMStart


_Startup:
            LDHX   #RAMEnd+1        ; initialize the stack pointer
            TXS
            sei                     ; disable interrupts globally
            
            
            CLRA
            STA SOPT1
            
            lda #000
            sta DILswitch
            sta continueFlag
            
INIT:            ;INICIALIZUJEME PREMENNE A PAMET
            lda #000        
            sta CNTN0
            sta CNTN1
            sta CNTN2
            sta CNTN3                   ;vynuluje se stav cítace
                            
            sta leftSegOUT             ;vynuluje se 7Seg 
            sta rightSegOUT            ;vynuluje se 7Seg 
            sta leftSEG7
            sta rightSEG7
        
            lda #%10000000            ;vynulujeme DILswitch a zachovame horny bit
            AND DILswitch
            STA DILswitch 
            
                        
; interrupt subsystem tests
            cli           ; 1) enable ints globally (0->CCR.I)
            jsr tpm1_init ; 2) enable local int mask+TMP1 start        

mainLoop:              
      
      BRCLR 7, DILswitch, INIT
      BRSET 7, DILswitch, SET1        
      BRA    mainLoop
            

SET1:                  
            BRSET 6, DILswitch, START1   ;Ked je 6 bit nastaveny pusti sa citanie
            JSR OUTPUT1              ;VYPISEME SEG7 na output
            
            
            BRSET 4, DILswitch, RBLIKcall  ; BLIK PRAVY KED JE  v 4. bite 1
            jsr LBLIK
            BRA mainLoop
RBLIKcall:
            jsr RBLIK            
            BRA mainLoop       ; Looop pre SET1 status





 ;Vynulovanie DILSwitch (stop)
STOP1:      ; porade se zastaví cítání, vynuluje se stav cítace i 7Seg a prepínace 6 az 0 se nastaví do spodní polohy.
            jsr ClrFirst
            BRA INIT
            BRA mainLoop           
          

START1:
            BRCLR 7, DILswitch, STOP1
            BRCLR 5, DILswitch, DECREMENT3
            BRSET 5, DILswitch, INCREMENT3           

;DECREMENT 0 nibblu  
DECREMENT3:
            LDA CNTN3
            CBEQA #$0, DECREMENT2       ;Porovname ci uz je 1111 1111 (vsetky bity nastavene) -> pretecenie
            DEC CNTN3

            MOV CNTN3,rightSEG7           ;Ukladanie do praveho seg            
            LDA #$03
            STA leftSEG7
            JSR OUTPUT1              ;VYPISEME SEG7 na output
            
            BRA mainLoop       ; Looop pre START1 status

;DECREMENT 1 nibblu
DECREMENT2:            
            LDA CNTN2
            CBEQA #$0, DECREMENT1       ;Porovname ci uz je 1111 1111 (vsetky bity nastavene) -> pretecenie
            DEC CNTN2
            MOV CNTN2,rightSEG7           ;Ukladanie do praveho seg
            LDA #$02
            STA leftSEG7
            JSR OUTPUT1              ;VYPISEME SEG7 na output
            
            LDA #$F          ;Nastavime spodnych 8 bit (CNT1)
            STA CNTN3
            BRA mainLoop       ; Looop pre START1 status
mainLoop2:
            BRA mainLoop
;DECREMENT 2 nibblu
DECREMENT1:            
            LDA CNTN1
            CBEQA #$0, DECREMENT0      ;Porovname ci uz je 1111 1111 (vsetky bity nastavene) -> pretecenie
            DEC CNTN1
            
            MOV CNTN1,rightSEG7           ;Ukladanie do praveho seg
            LDA #$01
            STA leftSEG7
            JSR OUTPUT1              ;VYPISEME SEG7 na output
            
            LDA #$F          ;Nastavime spodnych 8 bit (CNT1)
            STA CNTN3
            STA CNTN2
            
            BRA mainLoop2       ; Looop pre START1 status      

;DECREMENT 3 nibblu
DECREMENT0:            
            LDA CNTN0
            CBEQA #$0, ENDSTATE       ;Porovname ci uz je 1111 1111 (vsetky bity nastavene) -> pretecenie
            DEC CNTN0
            MOV CNTN0,rightSEG7           ;Ukladanie do praveho seg 
            LDA #$00                ;Ukladanie do laveho seg
            STA leftSEG7
            JSR OUTPUT1              ;VYPISEME SEG7 na output
            
            LDA #$F          ;Nastavime spodnych 8 bit (CNT1)
            STA CNTN3
            STA CNTN2
            STA CNTN1
            
            BRA mainLoop2       ; Looop pre START1 status   
            
STOP2:
            BRA STOP1    
START2:
            BRA START1                         

;INCREMENTACIA 0 nibblu            
INCREMENT3:
            LDA CNTN3
            CBEQA #$F, INCREMENT2       ;Porovname ci uz je 1111 (vsetky bity nastavene) -> pretecenie
            INC CNTN3
            MOV CNTN3,rightSEG7           ;Ukladanie do praveho seg 
            LDA #$03                ;Ukladanie do laveho seg
            STA leftSEG7
            JSR OUTPUT1              ;VYPISEME SEG7 na output
            BRA mainLoop2       ; Looop pre START1 status

;INCREMENTACIA 1 nibblu 
INCREMENT2:            
            LDA   CNTN2
            CBEQA #$F, INCREMENT1        ;KONEC DOPOCITALI SME
            INC CNTN2
            MOV CNTN2,rightSEG7           ;Ukladanie do praveho seg 
            LDA #$02                ;Ukladanie do laveho seg
            STA leftSEG7 
            JSR OUTPUT1              ;VYPISEME SEG7 na output
            LDA #$00    ;Vynulujeme spodnych 8 bit (CNT1)
            STA CNTN3
            BRA mainLoop2       ; Looop pre START1 status 
STOP3:
      BRA STOP2
mainLoop3:
            BRA mainLoop2      
;INCREMENTACIA 2 nibblu  
INCREMENT1:            
            LDA   CNTN1
            CBEQA #$F, INCREMENT0        ;KONEC DOPOCITALI SME
            INC CNTN1
            MOV CNTN1,rightSEG7           ;Ukladanie do praveho seg 
            LDA #$01                ;Ukladanie do laveho seg
            STA leftSEG7
            JSR OUTPUT1              ;VYPISEME SEG7 na output 
            LDA #$00    ;Vynulujeme spodnych 8 bit (CNT1)
            STA CNTN3
            STA CNTN2
            BRA mainLoop3       ; Looop pre START1 status      

;INCREMENTACIA 3 nibblu  
INCREMENT0:            
            LDA   CNTN0
            CBEQA #$F, ENDSTATE        ;KONEC DOPOCITALI SME
            INC CNTN0
            MOV CNTN0, rightSEG7           ;Ukladanie do praveho seg 
            LDA #$00                ;Ukladanie do laveho seg
            STA leftSEG7
            JSR OUTPUT1              ;VYPISEME SEG7 na output
            LDA #$00    ;Vynulujeme spodnych 8 bit (CNT1)
            STA CNTN3
            STA CNTN2
            STA CNTN1
            BRA mainLoop3       ; Looop pre START1 status            
            
ENDSTATE:            
            JSR OUTPUT1              ;VYPISEME SEG7 na output            
            JSR OUTPUT1              ;VYPISEME SEG7 na output            
            jsr ZHASNI
            JSR OUTPUT1              ;VYPISEME SEG7 na output    
            JSR OUTPUT1              ;VYPISEME SEG7 na output    
            JSR OUTPUT1              ;VYPISEME SEG7 na output                                               
            
            BRA STOP3
            ;ZABLIKAME DISPLAY 3X a vratime sa do pociatocneho stavu -> STOP1 asi

LBLIK:
            ;jsr WAITING ; SVIETIM
            LDA #$00       ;Vypneme Lavy segment (blikanie)
            STA leftSegOUT
            jsr WAITING ; ZHASNEM
            LDA DILswitch ; Hodime do pameti DILswitch
            AND #%00001111  ;Skopirujeme hodnotu z DILswitch 3-0bitov
            
            CMP #$04
            BLT Save
            LDA #$03
Save:                      
            STA leftSEG7
                          
            rts       ; Looop pre SET1 status

RBLIK:
            ;jsr WAITING ; SVIETIM
            LDA #$00
            STA rightSegOUT
            jsr WAITING ; ZHASNEM
            LDA DILswitch ; Hodime do pameti DILswitch
            AND #%00001111  ;Skopirujeme hodnotu z DILswitch 3-0
            STA rightSEG7   ;DO RIGHT SEG7
            
            ;Tiez skopirovat do CNTNX podla leftSEG7                            
            
            LDX leftSEG7     ;Nacitame hodnotu z leftSEG7    0-3
            STA CNTN0,X             ;ULOZIME DATA DO 0 - 3 nibblu
 
            rts       ; Looop pre SET1 status


ZHASNI: 
      LDA #$00
      STA leftSegOUT           
      STA rightSegOUT  
      jsr WAITING              ;Nechame svietit         
      jsr WAITING              ;Nechame svietit         
      jsr WAITING              ;Nechame svietit         
      rts      
            
OUTPUT1: ;Ukaz cislo na display

      LDA leftSEG7          ;Dame 8 bitov segmentu do pameti
      AND #%00001111    ;Zobereme spodne 4 bity
      STA TMP           ;Ulozime ich 
      CLRH              
      LDX TMP
      LDA seg_tbl, X
      STA leftSegOUT
      
                        ;TO ISTE ALE TERAZ PRE PRAVY SEGMENT
      LDA rightSEG7          ;Dame 8 bitov segmentu do pameti
      AND #%00001111    ;Zobereme spodne 4 bity      
      STA TMP           ;Ulozime ich       
      CLRH              ;????
      LDX TMP
      LDA seg_tbl, X
      STA rightSegOUT
      jsr WAITING ; SVIETIM     
      
      RTS  

WAITING:
      ;Nekonececny WHILE
Looop:
      BRSET 1, continueFlag, continueMAIN
      BRA Looop

      continueMAIN:      ;Pokracuje sa v Maine
      BCLR 1, continueFlag          
      RTS      
            
            
 
 
;-----------------------------------------------------
; TPM1 module initialized to generate
; an (overflow) interrupt each 0x4000 clk ticks. 
tpm1_init:
      mov #0, TPM1SC
      mov #0, TPM1CNT
      mov #$0A, TPM1MODH  ; interrupt (overflow)
      mov #$00, TPM1MODL  ; period
      
      ; 0100 1111
      mov #%01001111, TPM1SC    ; TPM1 start+int. enable
      ;mov #$4f, TPM1SC    ; TPM1 start+int. enable
      rts
      
 ClrFirst:
      LDA #%01111111
      AND DILswitch
      STA DILswitch     
      rts
        

             
;**************************************************************
;*                   7-segment table                          *
;**************************************************************
            

;-----------------------------------------------------
; 7-seg  control     corresponding 7-seg (hexa) digits 
;        bits
;-------------------------------------------------------------------------
;  _       _0_		     _	  _  _     _  _  _   _   _	 _		   _		  _  _
; |_|		 5|_6_|1	    | |	| _| _||_||_ |_   | |_| |_| |_| |_  |   _| |_ |_
; |_|		 4|_3_|2	    |_|	||_	 _|	 | _||_|  | |_|	 _| | | |_| |_ |_| |_ |
;-------------------------------------------------------------------------
seg_tbl     ; prescription for displaying hexa-numbers in 7-segment display			
            fcb  $3F; display 0 (set bits 0-5, clear bit 6)  
            fcb  $06; display 1 (set bits 1,2, clear the others)  
            fcb  $5B; display 2 (set bits 0,1,6,4,3 clear the others)  
            fcb  $4F; display 3 (set bits 0.1,2,3,6 clear the others)  
            fcb  $66; display 4 (set bits 1,2,5,6 clear the others)  
            fcb  $6D; display 5 (set bits 0,5,6,2,3 clear the others)  
            fcb  $7D; display 6 (clear bit 1 set the others)  
            fcb  $07; display 7 (set bits 0,1,2, clear the others)  
            fcb  $7F; display 8 (set all 7 bits)  
            fcb  $6F; display 9 (clear bit 4 set the others)  
            fcb  $77; display A (clear bit 3 set the others)  
            fcb  $7C; display B (clear bits 0,1 set the others)  
            fcb  $39; display C (set bits 0,3,4,5, clear the others)  
            fcb  $5E; display D (clear bits 0,5 set the others)  
            fcb  $79; display E (clear bits 1,2, set the others)  
            fcb  $71; display F (clear bits 1,2,3, set the others)             


;**************************************************************
;*                Interrupt SubSystem Setup                   *
;**************************************************************

;-----------------------------------------------------
; interrupt service routines (ISRs) 
;-----------------------------------------------------
spurious:                           ; placed here so that security value
            nop                     ; does not change all the time.
            rti
;-----------------------------------------------------
isr_tpm1ovf:	
            ;Pri interupte nastavime ze mozeme pokracovat
            BSET 1, continueFlag
            
            lda TPM1SC              ;int ack: 1) read int-flag
            bclr 7, TPM1SC          ;         2) clear int-flag
            
            
            rti
;-----------------------------------------------------
isr_swi:	
            nop	
            rti
;-----------------------------------------------------
; interrupt vector table

            ORG   $FFC0
            DC.W  spurious         ; unused              $FFC0
            DC.W  spurious         ; unused              $FFC2
            DC.W  spurious         ; Vrtc                $FFC4
            DC.W  spurious         ; Viic                $FFC6
            DC.W  spurious         ; Vacmp               $FFC8
            DC.W  spurious         ; Vadc                $FFCA
            DC.W  spurious         ; Vkeyboard           $FFCC
            DC.W  spurious         ; Vsci2tx             $FFCE
            DC.W  spurious         ; Vsci2rx             $FFD0
            DC.W  spurious         ; Vsci2err            $FFD2
            DC.W  spurious         ; Vsci1tx             $FFD4
            DC.W  spurious         ; Vsci1rx             $FFD6
            DC.W  spurious         ; Vsci1err            $FFD8
            DC.W  spurious         ; Vtpm2ovf            $FFDA
            DC.W  spurious         ; Vtpm2ch1            $FFDC
            DC.W  spurious         ; Vtpm2ch0            $FFDE
            DC.W  isr_tpm1ovf      ; Vtpm1ovf            $FFE0
            DC.W  spurious         ; Vtpm1ch5            $FFE2
            DC.W  spurious         ; Vtpm1ch4            $FFE4
            DC.W  spurious         ; Vtpm1ch3            $FFE6
            DC.W  spurious         ; Vtpm1ch2            $FFE8
            DC.W  spurious         ; Vtpm1ch1            $FFEA
            DC.W  spurious         ; Vtpm1ch0            $FFEC
            DC.W  spurious         ; VReserved           $FFEE
            DC.W  spurious         ; Vusb                $FFF0
            DC.W  spurious         ; Vspi2               $FFF2
            DC.W  spurious         ; Vspi1               $FFF4
            DC.W  spurious         ; Vlol                $FFF6
            DC.W  spurious         ; Vlvd                $FFF8
            DC.W  spurious         ; Virq                $FFFA
            DC.W  spurious         ; Vswi                $FFFC
            DC.W  _Startup         ; Vreset              $FFFE