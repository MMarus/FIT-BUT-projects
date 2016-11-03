# Makefile pre projekt 3 z Typografie a publikovani
#
# Autor: Marek Marusic
#
# Email: xmarus05@stud.fit.vutbr.cz

latexfile=proj3

all:
	latex $(latexfile).tex
	latex $(latexfile).tex 
	dvips -t a4 $(latexfile).dvi
	ps2pdf -sPAPERSIZE=a4 $(latexfile).ps 

clean:
	-rm -f $(latexfile).aux $(latexfile).ps $(latexfile).dvi $(latexfile).log
