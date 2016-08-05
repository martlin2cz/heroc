## heroc doc
## Martin Jasek
## 6. 8. 2016
## KMI UP, inf.upol.cz
## vychodil.inf.upol.cz

all: latexdoc

clean: clean_latex

#################################################

latexdoc: latexdoc.tex
	#vlna latexdoc.tex
	pdflatex latexdoc.tex
	pdflatex latexdoc.tex
	pdflatex latexdoc.tex

#################################################

clean_latex:
	rm latexdoc.aux latexdoc.log latexdoc.toc

