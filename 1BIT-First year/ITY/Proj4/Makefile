#Autor: Marek Marušic - 1BIT, VUT FIT
#		xmarus05@stud.fit.vutbr.cz
#Datum: 22.4.2014
#Popis: Bibliografické citace

latexfile=proj4

all:
	latex $(latexfile).tex
	bibtex $(latexfile).aux
	latex $(latexfile).tex
	latex $(latexfile).tex
	dvips -t a4 $(latexfile).dvi
	ps2pdf $(latexfile).ps

clean:
	rm -f $(latexfile).aux $(latexfile).dvi $(latexfile).ps $(latexfile).bbl $(latexfile).blg $(latexfile).log
