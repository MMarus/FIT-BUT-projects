#Autor: Marek Marušic - 1BIT, VUT FIT
#		xmarus05@stud.fit.vutbr.cz
#Datum: 22.4.2014
#Popis: Bibliografické citace

latexfile=proj5

all:
	latex $(latexfile).tex
	dvips $(latexfile).dvi
	ps2pdf $(latexfile).ps

clean:
	rm -f $(latexfile).aux $(latexfile).dvi $(latexfile).ps $(latexfile).bbl $(latexfile).blg $(latexfile).log
