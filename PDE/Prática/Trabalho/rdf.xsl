<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:dc="http://purl.org/dc/elements/1.1/"
                xmlns:tpde="rdfs.xml" xml:base="rdfs.xml">

    <xsl:output indent="yes" method="xml"/>
    <xsl:template match="/">
        <rdf:RDF>
            <rdf:Description>
                <xsl:apply-templates/>
            </rdf:Description>
        </rdf:RDF>
    </xsl:template>

    <xsl:template match="cabecalho">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="titulo">
        <dc:title><xsl:value-of select="."/></dc:title>
    </xsl:template>

    <xsl:template match="autor">
        <dc:creator><xsl:value-of select="."/></dc:creator>
    </xsl:template>

    <xsl:template match="data-publicacao">
        <dc:date><xsl:value-of select="."/></dc:date>
    </xsl:template>

    <xsl:template match="resumo">
        <dc:description><xsl:value-of select="."/></dc:description>
    </xsl:template>

    <xsl:template match="parte">
        <tpde:conteudo>
            <tpde:parte>
                <xsl:apply-templates/>
            </tpde:parte>
        </tpde:conteudo>
    </xsl:template>

    <xsl:template match="titulo">
        <dc:title><xsl:value-of select="."/></dc:title>
    </xsl:template>

    <xsl:template match="resumo">
        <dc:description><xsl:value-of select="."/></dc:description>
    </xsl:template>

    <xsl:template match="receita">

    </xsl:template>



</xsl:stylesheet>