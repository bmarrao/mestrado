<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns:r="trabalhoPDE">
    <xsl:output method="html" version="4.01" encoding="UTF-8"/>
    <xsl:variable name="cor-base">white</xsl:variable>
    <xsl:variable name="cor-fundo">lightGrey</xsl:variable>
    <xsl:template match="r:receitas">
        <HTML>
            <HEAD>
                <META http-equiv="Content-Type" content="text/html; charset=utf-8"/>
                <META http-equiv="Expires" content="0"/>
                <TITLE>  <xsl:value-of select="r:cabecalho/titulo"/></TITLE>
                <style>
                    .center
                    {
                    text-align: center;
                    }
                </style>
            </HEAD>
            <BODY BGCOLOR="{$cor-base}">
                <xsl:apply-templates select="r:cabecalho"/>
                <hr/>
                <xsl:call-template name="tabela-conteudo"/>
                <hr/>
                <xsl:apply-templates select="r:parte" mode="normal"/>
                <xsl:apply-templates select="r:seção" mode="normal"/>
            </BODY>
        </HTML>
    </xsl:template>
    <xsl:template name="tabela-conteudo">
        <ol type="1">
            <xsl:apply-templates select="r:parte"  mode="tabela-conteudo"/>
            <xsl:apply-templates select="r:seção" mode="tabela-conteudo"/>
        </ol>
    </xsl:template>
    
    <xsl:template match="r:cabecalho">
        <div class="center">
            <h1> <xsl:value-of select="r:titulo"/></h1>
            <h3> <xsl:value-of select="r:autor"/></h3>
            <h3> <xsl:value-of select="r:data-publicacao"/></h3>
            <p><xsl:value-of select="r:resumo"/></p>
        </div>
    </xsl:template>

    <xsl:template match="r:parte" mode="tabela-conteudo">
        <li>
            <!-- FALTA O HREF -->
            <a href="#{generate-id()}"><xsl:value-of select="r:titulo"/></a>
            <ol type="1">
                <xsl:apply-templates select="r:receita" mode="tabela-conteudo"/>
                <xsl:apply-templates select="r:seção" mode="tabela-conteudo"/>
            </ol>
        </li>
    </xsl:template >

    <xsl:template match="r:parte" mode="normal">
        <hr/>
        <h2 id="#{generate-id()}"><xsl:value-of select="r:titulo"/></h2>
        <p><xsl:value-of select="r:texto-introdutorio"/></p>
        <hr/>
        <xsl:apply-templates select="r:receita" mode="normal"/>
        <xsl:apply-templates select="r:seção" mode="normal"/>
    </xsl:template >

    <xsl:template match="r:seção" mode="tabela-conteudo">
        <xsl:variable name="id" select="normalize-space(r:titulo)"/>
        <li>
            <a href="#{generate-id()}"><xsl:value-of select="r:titulo"/></a>
            <!-- FALTA O HREF -->
            <ol type="1">
                <xsl:apply-templates select="r:receita" mode="tabela-conteudo"/>
                <xsl:apply-templates select="r:sub-seção" mode="tabela-conteudo"/>
            </ol>
        </li>
    </xsl:template>

    <xsl:template match="r:seção" mode="normal">
        <xsl:variable name="id" select="normalize-space(r:titulo)"/>
        <hr/>
        <h2 id="{generate-id()}"><xsl:value-of select="r:titulo"/></h2>
        <p><xsl:value-of select="r:texto-introdutorio"/></p>
        <hr/>
        <xsl:apply-templates select="r:receita" mode="normal"/>
        <xsl:apply-templates select="r:sub-seção" mode="normal"/>
    </xsl:template>


    <xsl:template match="r:sub-seção" mode="tabela-conteudo">
        <xsl:variable name="id" select="normalize-space(r:titulo)"/>
        <li>
            <a href="#{generate-id()}"><xsl:value-of select="r:titulo"/></a>
            <!-- FALTA O HREF -->
            <ol type="1">
                <xsl:apply-templates select="r:receita" mode="tabela-conteudo"/>
            </ol>
        </li>
    </xsl:template>

    <xsl:template match="r:sub-seção" mode="normal">
        <xsl:variable name="id" select="normalize-space(r:titulo)"/>
        <hr/>
        <h3 id="{generate-id()}"><xsl:value-of select="r:titulo"/></h3>
        <p><xsl:value-of select="r:texto-introdutorio"/></p>
        <hr/>
        <xsl:apply-templates select="r:receita" mode="normal"/>
    </xsl:template>

    <xsl:template match="r:receita" mode="tabela-conteudo">

        <li>
            <xsl:choose>
                <xsl:when test="@id"><a href="#{@id}"><xsl:value-of select="r:nome"/></a></xsl:when>
                <xsl:otherwise> <a href="#{generate-id()}"><xsl:value-of select="r:nome"/></a></xsl:otherwise>
            </xsl:choose>

        </li>
    </xsl:template>

    <xsl:template match="r:receita" mode="normal">
        <hr/>
        <xsl:choose>
            <xsl:when test="@id"><h4 class="center" id="{@id}"><xsl:value-of select="r:nome"/></h4></xsl:when>
            <xsl:otherwise><h4 class="center" id="{generate-id()}"><xsl:value-of select="r:nome"/></h4></xsl:otherwise>
        </xsl:choose>
        <xsl:if test="@ilustração"><img src="{@ilustração}" alt="imagem" class="center"/></xsl:if>

        <ol>
            <xsl:if test="@dificuldade"><p>Dificuldade :<xsl:value-of select="@dificuldade"/></p></xsl:if>
            <xsl:if test="@categoria"><p>Categoria :<xsl:value-of select="@categoria"/></p></xsl:if>
            <xsl:if test="@tempo"><p>Tempo :<xsl:value-of select="@tempo"/></p></xsl:if>
            <xsl:if test="@custo"><p>Custo :<xsl:value-of select="@custo"/></p></xsl:if>
        </ol>



        <xsl:apply-templates select="r:ingredientes"/>
            <xsl:apply-templates select="r:instruções"/>
        <hr/>
        <xsl:apply-templates select="r:receita" mode="normal"/>
    </xsl:template>
    <xsl:template match="r:ingredientes">
        <p>Ingredientes</p>
        <ol>
            <xsl:apply-templates select="r:ingrediente"/>
        </ol>
    </xsl:template>

    <xsl:template match="r:ingrediente">
        <xsl:variable name="id" select="@id"/>
        <li id="{$id}">
            <xsl:value-of select="." />
        </li>
    </xsl:template>



    <xsl:template match="r:instruções">
        <p>Instruções</p>
        <xsl:choose>
            <xsl:when test="r:texto-instrução"> <xsl:apply-templates select="r:texto-instrução"/></xsl:when>
            <xsl:otherwise>
                <ol type="1">
                    <xsl:apply-templates/>
                </ol>
            </xsl:otherwise>
        </xsl:choose>
        <xsl:if test="r:texto-instrução">
            <xsl:apply-templates/>
        </xsl:if>
    </xsl:template>

    <xsl:template match="r:texto-instrução">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="r:passo">
        <xsl:choose>
            <xsl:when test="@id">
                <xsl:variable name="id" select="@id"/>
                    <li id="{$id}">
                        <xsl:apply-templates/>
                    </li>
            </xsl:when>
            <xsl:otherwise>
                <li>
                    <xsl:apply-templates/>
                </li>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="r:passoRef">
        <xsl:variable name="ref" select="@ref"/>
        <xsl:variable name="step" select="//r:passo[@id = $ref]"/>
        <a href="#{@ref}">
            <li>
                <xsl:apply-templates select="$ref"/>
            </li>
        </a>
    </xsl:template>

    <xsl:template match="r:ingredienteRef">
        <xsl:variable name="ref" select="@ref"/>

        <a href="#{@ref}"><xsl:value-of select="//r:ingrediente[@id = $ref]"/></a>
    </xsl:template>
    <xsl:template match="r:receitaRef">
        <xsl:variable name="ref" select="@ref"/>
        <a href="#{@ref}"><xsl:value-of select="//r:receita[@id = $ref]"/></a>
    </xsl:template>


</xsl:stylesheet>

