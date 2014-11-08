<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version='1.0'
                xmlns="http://www.w3.org/TR/xhtml1/transitional"
                exclude-result-prefixes="#default">

<xsl:import href="/sw/share/xml/xsl/docbook-xsl/html/docbook.xsl" />

<xsl:variable name="section.autolabel">1</xsl:variable>
<xsl:variable name="section.label.includes.component.label">1</xsl:variable>
<xsl:variable name="toc.section.depth">2</xsl:variable>
<xsl:variable name="generate.section.toc.level">0</xsl:variable>

<xsl:param name="generate.toc">
book      toc
</xsl:param>

</xsl:stylesheet>
