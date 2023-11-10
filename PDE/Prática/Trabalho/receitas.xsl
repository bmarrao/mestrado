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
                <div class="center">
                    <h1><center><xsl:value-of select="cabecalho/titulo"/></center></h1>
                    <h3><center></center></h3>
                    <h3><center><xsl:value-of select="cabecalho/data-publicacao"/></center></h3>
                    <p><xsl:value-of select="cabecalho/resumo"/></p>
                </div>
                <hr/>
                <ol type="1">
                    <xsl:apply-templates select="parte" mode="tabela-conteudo"/>
                    <xsl:apply-templates select="seção" mode="tabela-conteudo"/>
                </ol>
            </BODY>
        </HTML>
    </xsl:template>

    <xsl:template match="parte" mode="tabela-conteudo">
        <xsl:choose>
            <xsl:when test="teste = tabela-conteudo">
                <li>
                    <!-- FALTA O HREF -->
                    <a href=""><xsl:value-of select="titulo"/></a>
                    <ol type="1">
                        <xsl:apply-templates select="receita" mode="tabela-conteudo"/>
                        <xsl:apply-templates select="seção" mode="tabela-conteudo"/>
                    </ol>
                </li>
            </xsl:when>
            <xsl:otherwise>

            </xsl:otherwise>
        </xsl:choose>

    </xsl:template >

    <xsl:template match="seção" mode="tabela-conteudo">
        <li>
            <!-- FALTA O HREF -->
            <a href=""><xsl:value-of select="titulo"/></a>
            <ol type="1">
                <xsl:apply-templates select="receita" mode="tabela-conteudo"/>
                <xsl:apply-templates select="sub-seção" mode="tabela-conteudo"/>
            </ol>
        </li>
    </xsl:template>

    <xsl:template match="receita" mode="tabela-conteudo">
        <li>
            <a href=""><xsl:value-of select="nome"/></a>
        </li>
    </xsl:template>

</xsl:stylesheet>
