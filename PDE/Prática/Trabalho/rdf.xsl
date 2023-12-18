
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:dc="http://purl.org/dc/elements/1.1/"
                xmlns:tpde="http://www.dcc.fc.up.pt/trabalhoPDE" xml:base="http://www.dcc.fc.up.pt/trabalhoPDE" xmlns:r="trabalhoPDE"
                exclude-result-prefixes="r tpde dc rdf">

    <xsl:output method="xml" indent="yes"/>

    <xsl:template match="/">
        <xsl:text disable-output-escaping='yes' >&#x3c;!DOCTYPE rdf:RDF[
                &#x3c;!ENTITY rdf  "http://www.w3.org/1999/02/22-rdf-syntax-ns#">
                &#x3c;!ENTITY rdfs "http://www.w3.org/2000/01/rdf-schema#">
                &#x3c;!ENTITY dc  "http://purl.org/dc/elements/1.1/">
                &#x3c;!ENTITY  tpde "http://www.dcc.fc.up.pt/trabalhoPDE">
                ]&#x3e;&#xA;</xsl:text>
        <xsl:text disable-output-escaping='yes' >&#x3c;rdf:RDF xmlns:dc="&amp;dc;" xmlns:rdf="&amp;rdf;" xmlns:tpde="&amp;tpde;" xml:base="&amp;tpde;"&#x3e;&#xA;</xsl:text>
        <rdf:Description rdf:about="livro-receitas">
                <rdf:type rdf:resource="livro"/>
                <xsl:apply-templates/>
            </rdf:Description>
        <xsl:text disable-output-escaping='yes' >&#x3c;/rdf:RDF&#x3e;&#xA;</xsl:text>
    </xsl:template>

    <xsl:template match="r:cabecalho">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="r:titulo">
        <dc:title><xsl:value-of select="."/></dc:title>
    </xsl:template>

    <xsl:template match="r:autor">
        <dc:creator><xsl:value-of select="."/></dc:creator>
    </xsl:template>

    <xsl:template match="r:data-publicacao">
        <dc:date><xsl:value-of select="."/></dc:date>
    </xsl:template>

    <xsl:template match="r:resumo">
        <tpde:resumo><xsl:value-of select="."/></tpde:resumo>
    </xsl:template>

    <xsl:template match="r:texto-introdutorio">
        <tpde:resumo><xsl:value-of select="."/></tpde:resumo>
    </xsl:template>

    <xsl:template match="r:parte">
        <tpde:conteudo>
            <tpde:parte>
                <xsl:apply-templates/>
            </tpde:parte>
        </tpde:conteudo>
    </xsl:template>

    <xsl:template match="r:seção">
        <tpde:conteudo>
            <tpde:seção>
                <xsl:apply-templates/>
            </tpde:seção>
        </tpde:conteudo>
    </xsl:template>

    <xsl:template match="r:sub-seção">
        <tpde:conteudo>
            <tpde:sub-seção>
                <xsl:apply-templates/>
            </tpde:sub-seção>
        </tpde:conteudo>
    </xsl:template>

    <xsl:template match="r:titulo">
        <dc:title><xsl:value-of select="."/></dc:title>
    </xsl:template>


    <xsl:template match="r:receita">
        <tpde:receita><xsl:value-of select="r:nome"/></tpde:receita>
    </xsl:template>



</xsl:stylesheet>