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
                    @import url("https://fonts.googleapis.com/css2?family=Literata");

                    * {
                    font-family: "Literata";
                    }

                    .center
                    {
                    text-align: center;
                    }

                    .centerImage {
                    display: block;
                    margin-left: auto;
                    margin-right: auto;
                    width: 50%;
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
        <ul>
            <xsl:apply-templates select="r:parte"  mode="tabela-conteudo"/>
            <xsl:apply-templates select="r:seção" mode="tabela-conteudo"/>
        </ul>
    </xsl:template>

    <xsl:template match="r:cabecalho">
        <div class="center">
            <h1> <xsl:value-of select="r:titulo"/></h1>
            <h3>
                <xsl:for-each select="r:autor">
                    <xsl:value-of select="."/> ;
                </xsl:for-each>
            </h3>
            <h3> <xsl:value-of select="r:data-publicacao"/></h3>
            <p><xsl:value-of select="r:resumo"/></p>
        </div>
    </xsl:template>

    <xsl:template match="r:parte" mode="tabela-conteudo">
        <li>
            <a href="#{generate-id()}">Parte <xsl:number format="1."/> -
                <xsl:value-of select="r:titulo"/></a>
            <ul>
                <xsl:apply-templates select="r:receita" mode="tabela-conteudo"/>
                <xsl:apply-templates select="r:seção" mode="tabela-conteudo"/>
            </ul>
        </li>
    </xsl:template >

    <xsl:template match="r:parte" mode="normal">
        <hr/>
        <h2 id="#{generate-id()}" class="center">Parte <xsl:number format="1."/> -
            <xsl:value-of select="r:titulo"/> </h2>
        <xsl:if test="@ilustração"><img src="{@ilustração}" alt="imagem" class="centerImage"/></xsl:if>
        <p class="centerImage"><xsl:value-of select="r:texto-introdutorio"/></p>
        <hr/>
        <xsl:apply-templates select="r:receita" mode="normal"/>
        <xsl:apply-templates select="r:seção" mode="normal"/>
    </xsl:template >

    <xsl:template match="r:seção" mode="tabela-conteudo">
        <li>
            <a href="#{generate-id()}">Seção <xsl:number format="1." count="r:receita | r:parte |r:seção " level="multiple"/> -
                <xsl:value-of select="r:titulo"/></a>
            <!-- FALTA O HREF -->
            <ul>
                <xsl:apply-templates select="r:receita" mode="tabela-conteudo"/>
                <xsl:apply-templates select="r:sub-seção" mode="tabela-conteudo"/>
            </ul>
        </li>
    </xsl:template>

    <xsl:template match="r:seção" mode="normal">
        <hr/>
        <h3 id="{generate-id()}" class="center">Seção <xsl:number format="1." count="r:receita | r:parte |r:seção " level="multiple"/> -
            <xsl:value-of select="r:titulo"/> </h3>
        <xsl:if test="@ilustração"><img src="{@ilustração}" alt="imagem" class="centerImage"/></xsl:if>
        <p class="centerImage"><xsl:value-of select="r:texto-introdutorio"/></p>
        <hr/>
        <xsl:apply-templates select="r:receita" mode="normal"/>
        <xsl:apply-templates select="r:sub-seção" mode="normal"/>
    </xsl:template>


    <xsl:template match="r:sub-seção" mode="tabela-conteudo">
        <li>
            <a href="#{generate-id()}">Sub-secção <xsl:number format="1." count="r:receita |r:parte |r:seção | r:sub-seção" level="multiple"/> -
                <xsl:value-of select="r:titulo"/></a>
            <!-- FALTA O HREF -->
            <ul>
                <xsl:apply-templates select="r:receita" mode="tabela-conteudo"/>
            </ul>
        </li>
    </xsl:template>

    <xsl:template match="r:sub-seção" mode="normal">
        <hr/>
        <h4 id="{generate-id()}" class="center">Sub-secção <xsl:number format="1." count="r:receita | r:parte |r:seção | r:sub-seção" level="multiple"/> -
            <xsl:value-of select="r:titulo"/></h4>
        <xsl:if test="@ilustração"><img src="{@ilustração}" alt="imagem" class="centerImage"/></xsl:if>
        <p class="centerImage"><xsl:value-of select="r:texto-introdutorio"/></p>
        <hr/>
        <xsl:apply-templates select="r:receita" mode="normal"/>
    </xsl:template>

    <xsl:template match="r:receita" mode="tabela-conteudo">

        <li>
            <xsl:choose>
                <xsl:when test="@id"><a href="#{@id}">
                    Receita - <xsl:number format="1." count="r:receita | r:parte |r:seção | r:sub-seção" level="multiple"/> -
                    <xsl:value-of select="r:nome"/></a></xsl:when>
                <xsl:otherwise> <a href="#{generate-id()}">Receita - <xsl:number format="1." count="r:receita | r:parte |r:seção | r:sub-seção" level="multiple"/> -
                    <xsl:value-of select="r:nome"/></a></xsl:otherwise>
            </xsl:choose>

        </li>
    </xsl:template>

    <xsl:template match="r:receita" mode="normal">
        <hr/>
        <xsl:choose>
            <xsl:when test="@id"><h4  id="{@id}" class="center">Receita - <xsl:number format="1." count="r:receita | r:parte |r:seção | r:sub-seção" level="multiple"/> -
                <xsl:value-of select="r:nome"/></h4></xsl:when>
            <xsl:otherwise><h4  id="{generate-id()}" class="center">Receita - <xsl:number format="1." count="r:receita | r:parte |r:seção | r:sub-seção" level="multiple"/> -
                <xsl:value-of select="r:nome"/></h4></xsl:otherwise>
        </xsl:choose>
        <xsl:if test="@ilustração"><img src="{@ilustração}" alt="imagem" class="centerImage"/></xsl:if>

        <div class="centerImage">
            <xsl:if test="@dificuldade"><p><b>Dificuldade</b> :<xsl:value-of select="@dificuldade"/></p></xsl:if>
            <xsl:if test="@categoria"><p><b>Categoria </b> :<xsl:value-of select="@categoria"/></p></xsl:if>
            <xsl:if test="@tempo"><p><b>Tempo </b> :<xsl:value-of select="@tempo"/></p></xsl:if>
            <xsl:if test="@custo"><p><b>Custo</b> :<xsl:value-of select="@custo"/></p></xsl:if>
        </div>


        <xsl:apply-templates select="r:ingredientes"/>
        <xsl:apply-templates select="r:instruções"/>
        <hr/>
    </xsl:template>
    <xsl:template match="r:ingredientes">
        <div class="centerImage">
            <p><b>Ingredientes</b></p>
            <ol>
                <xsl:apply-templates select="r:ingrediente"/>
            </ol>
        </div>
    </xsl:template>

    <xsl:template match="r:ingrediente">
        <xsl:variable name="id" select="@id"/>
        <li id="{$id}">
            <xsl:value-of select="." />
            <xsl:if test="@quantidade | @unidade-medida" >
                <ul >
                    <xsl:if test="@quantidade"><li>Quantidade : <xsl:value-of select="@quantidade"/></li></xsl:if>
                    <xsl:if test="@unidade-medida"><li>Unidade de medida : <xsl:value-of select="@unidade-medida"/></li></xsl:if>
                </ul>
            </xsl:if>
        </li>
        <xsl:if test="not(//r:ingredienteRef[@ref = $id])"><xsl:message>Ingrediente criado que não é utilizado na receita</xsl:message></xsl:if>
    </xsl:template>



    <xsl:template match="r:instruções">
        <div class="centerImage">
            <p><b>Instruções</b></p>

            <xsl:choose>
                <xsl:when test="r:texto-instrução"> <xsl:apply-templates select="r:texto-instrução"/></xsl:when>
                <xsl:otherwise>
                    <ol>
                        <xsl:apply-templates/>
                    </ol>
                </xsl:otherwise>
            </xsl:choose>
        </div>
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
                    <xsl:if test="@ilustração"><img src="{@ilustração}" alt="imagem" class="centerImage"/></xsl:if>
                </li>
            </xsl:when>
            <xsl:otherwise>
                <li>
                    <xsl:apply-templates/>
                    <xsl:if test="@ilustração"><img src="{@ilustração}" alt="imagem" class="centerImage"/></xsl:if>
                </li>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>


    <xsl:template match="r:passoRef">
        <xsl:variable name="ref" select="@ref"/>
        <xsl:choose>
            <xsl:when test="//r:passo[@id = $ref]">
                <li>
                    <a href="#{$ref}">
                        <xsl:value-of select="."/>
                    </a>
                </li></xsl:when>
            <xsl:otherwise><xsl:message>O passo referenciado não existe</xsl:message></xsl:otherwise>
        </xsl:choose>
    </xsl:template>



    <xsl:template match="r:ingredienteRef">
        <xsl:variable name="ref" select="@ref"/>

        <xsl:choose>
            <xsl:when test="../../../r:ingredientes/r:ingrediente[@id = $ref]"><b><a href="#{@ref}"><xsl:value-of select="../../../r:ingredientes/r:ingrediente[@id = $ref]"/></a></b></xsl:when>
            <xsl:otherwise><xsl:message >O ingrediente referenciado não existe</xsl:message></xsl:otherwise>
        </xsl:choose>

    </xsl:template>
    <xsl:template match="r:receitaRef">
        <xsl:variable name="ref" select="@ref"/>
        <xsl:choose>
            <xsl:when test="//r:receita[@id = $ref]">
                <a href="#{@ref}"><xsl:value-of select="."/></a>
            </xsl:when>
            <xsl:otherwise><xsl:message >A receita referenciada não existe</xsl:message></xsl:otherwise>
        </xsl:choose>


    </xsl:template>

</xsl:stylesheet>

