<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="html" version="4.01" encoding="UTF-8"/>
    <xsl:variable name="cor-base">white</xsl:variable>
    <xsl:variable name="cor-fundo">lightGrey</xsl:variable>
    <xsl:template match="receitas">
        <HTML>
            <HEAD>
                <META http-equiv="Content-Type" content="text/html; charset=utf-8"/>
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
                <hr></hr>
                <TABLE BORDER="1" CELLPADDING="10" WIDTH="100%" BGCOLOR="{$cor-fundo}">
                    <TR><TD> <xsl:apply-templates select="cabecalho"/> </TD></TR>
                    <TR><TD><TABLE BORDER="1" WIDTH="100%">
                        <TR><TH>Quant.</TH><TH>Descrição</TH><TH>Preço</TH><TH>IVA</TH><TH>Total</TH></TR>
                        <xsl:apply-templates select="corpo"/>
                        <xsl:call-template name="rodape"/>
                    </TABLE></TD></TR>
                </TABLE>
            </BODY>
        </HTML>
    </xsl:template>
</xsl:stylesheet>
