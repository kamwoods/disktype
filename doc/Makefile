# Makefile for disktype documentation

XSLFLAGS = --nonet --catalogs

SOURCE            = disktype.xml
SOURCE_DEP        = $(SOURCE) usage.xml terms.xml formats.xml
TARGET_HTML_ONE   = $(patsubst %.xml,%.html,$(SOURCE))
TARGET_HTML_CHUNK = html/index.html
TARGET_LATEX      = $(patsubst %.xml,%.tex,$(SOURCE))
TARGET_PDF        = $(patsubst %.xml,%.pdf,$(SOURCE))

XSL_HTML_ONE      = html-one.xsl
XSL_HTML_CHUNK    = html-chunk.xsl
XSL_LATEX         = latex.xsl


all: $(TARGET_HTML_CHUNK)

$(TARGET_HTML_ONE): $(SOURCE_DEP) $(XSL_HTML_ONE)
	xsltproc $(XSLFLAGS) -o $@ $(XSL_HTML_ONE) $<

$(TARGET_HTML_CHUNK): $(SOURCE_DEP) $(XSL_HTML_CHUNK)
	if [ ! -d html ]; then mkdir html ; fi
	xsltproc $(XSLFLAGS) -o $@ $(XSL_HTML_CHUNK) $<

$(TARGET_LATEX): $(SOURCE_DEP) $(XSL_LATEX)
	xsltproc $(XSLFLAGS) -o $@ $(XSL_LATEX) $<

$(TARGET_PDF): $(TARGET_LATEX)
	pdflatex $<
	pdflatex $<
	pdflatex $<

check:
	xmllint --noout --valid $(SOURCE)

clean:
	rm -f *.html html/*.html *.pdf
	rm -f *.tex *.log *.aux *.toc *.out *.glo *.idx

# EOF
