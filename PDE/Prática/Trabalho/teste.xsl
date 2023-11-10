<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="html" version="4.01" encoding="UTF-8"/>
    <xsl:variable name="cor-base">white</xsl:variable>
    <xsl:variable name="cor-fundo">lightGrey</xsl:variable>
    <xsl:template match="receitas">
        <HTML>
            <HEAD>
                <META http-equiv="Content-Type" content="text/html; charset=utf-8"/>
                <META http-equiv="Expires" content="0"/>
                <TITLE>  <xsl:value-of select="cabecalho/titulo"/></TITLE>
                <style>
                    .center
                    {
                    text-align: center;
                    }
                </style>
            </HEAD>
            <BODY BGCOLOR="{$cor-base}">
                <xsl:apply-templates select="cabecalho"/>
                <hr/>
                <xsl:call-template name="tabela-conteudo"/>
                <hr/>
                <xsl:apply-templates select="Parte" mode="normal"/>
                <xsl:apply-templates select="Seção" mode="normal"/>
            </BODY>
        </HTML>
    </xsl:template>
    <xsl:template name="tabela-conteudo">
        <ol type="1">
            <xsl:apply-templates select="Parte"  mode="tabela-conteudo"/>
            <xsl:apply-templates select="Seção" mode="tabela-conteudo"/>
        </ol>
    </xsl:template>

    <xsl:template match="cabecalho">
        <div class="center">
            <h1> <xsl:value-of select="titulo"/></h1>
            <h3> <xsl:value-of select="autor"/></h3>
            <h3> <xsl:value-of select="data-publicacao"/></h3>
            <p><xsl:value-of select="resumo"/></p>
        </div>
    </xsl:template>

    <xsl:template match="Parte" mode="tabela-conteudo">
        <li>
            <!-- FALTA O HREF -->
            <a href=""><xsl:value-of select="titulo"/></a>
            <ol type="1">
                <xsl:apply-templates select="receita" mode="tabela-conteudo"/>
                <xsl:apply-templates select="Seção" mode="tabela-conteudo"/>
            </ol>
        </li>
    </xsl:template >

    <xsl:template match="Parte" mode="normal">
        <hr/>
        <h2 id=""><xsl:value-of select="titulo"/></h2>
        <h3><xsl:value-of select="texto-introdutorio"/></h3>
        <hr/>
        <xsl:apply-templates select="receita" mode="normal"/>
        <xsl:apply-templates select="Seção" mode="normal"/>
    </xsl:template >

    <xsl:template match="Seção" mode="tabela-conteudo">
        <li>
            <a href=""><xsl:value-of select="titulo"/></a>
            <!-- FALTA O HREF -->
            <ol type="1">
                <xsl:apply-templates select="receita" mode="tabela-conteudo"/>
                <xsl:apply-templates select="sub-Seção" mode="tabela-conteudo"/>
            </ol>
        </li>
    </xsl:template>

    <xsl:template match="Seção" mode="normal">
        <hr/>
        <h2 id=""><xsl:value-of select="titulo"/></h2>
        <h3><xsl:value-of select="texto-introdutorio"/></h3>
        <hr/>
        <xsl:apply-templates select="receita" mode="normal"/>
        <xsl:apply-templates select="sub-Seção" mode="normal"/>
    </xsl:template>

    <xsl:template name="sub-Seção">
        <hr/>
        <h2 id=""><xsl:value-of select="titulo"/></h2>
        <h3><xsl:value-of select="texto-introdutorio"/></h3>
        <hr/>
        <xsl:apply-templates select="receita" mode="normal"/>
    </xsl:template>

    <xsl:template match="receita" mode="tabela-conteudo">
       <li>
            <a href=""><xsl:value-of select="nome"/></a>
        </li>
    </xsl:template>

    <xsl:template match="receita" mode="normal">
        <hr/>
        <div class="center">
            <h2 id=""><xsl:value-of select="nome"/></h2>
            <xsl:apply-templates select="ingredientes"/>
            <xsl:apply-templates select="instruções"/>
        </div>
        <hr/>
        <xsl:apply-templates select="receita" mode="normal"/>
    </xsl:template>

    <xsl:template match="ingredientes">
        <ol>
            <xsl:apply-templates select="ingrediente"/>
        </ol>
    </xsl:template>

    <xsl:template match="ingrediente">
        <xsl:variable name="id" select="@id"/>
        <li id="{$id}">
            <xsl:value-of select="/"/>
        </li>
    </xsl:template>



</xsl:stylesheet>

