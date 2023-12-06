<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:dc="http://purl.org/dc/elements/1.1/"
                xmlns:tpde="http://www.dcc.fc.up.pt/trabalhoPDE" xml:base="http://www.dcc.fc.up.pt/trabalhoPDE" xmlns:r="trabalhoPDE">

    <xsl:output indent="yes" method="xml"/>
    <xsl:template match="/">
        <rdf:RDF>
            <rdf:Description rdf:about="livro-receitas">
                <rdf:type rdf:resource="livro"/>
                <xsl:apply-templates/>
            </rdf:Description>
        </rdf:RDF>
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
        <dc:description><xsl:value-of select="."/></dc:description>
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

    <xsl:template match="r:resumo">
        <dc:description><xsl:value-of select="."/></dc:description>
    </xsl:template>

    <xsl:template match="r:receita">
        <tpde:receita><xsl:value-of select="r:nome"/></tpde:receita>
    </xsl:template>



</xsl:stylesheet>