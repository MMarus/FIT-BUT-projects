latexfile = proj1

TEX = pdflatex

$(latexfile).dvi : $(latexfile).tex 
	while ($(TEX) $(latexfile) ; \
	grep -q "Rerun to get cross" $(latexfile).log ) do true ; \
	done

$(latexfile).pdf : $(latexfile).ps
	ps2pdf $(latexfile).ps $(latexfile).pdf

pdf : $(latexfile).pdf

$(latexfile).ps : $(latexfile).dvi
	dvips $(latexfile)

ps : $(latexfile).ps 

$(latexfile).tar.gz : $(latexfile).tex 
	tar -czvf $(latexfile).tar.gz $^
tarball: $(latexfile).tar.gz

