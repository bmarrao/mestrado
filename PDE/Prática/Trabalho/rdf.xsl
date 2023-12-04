<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
                xmlns:dc="http://purl.org/dc/elements/1.1/"  xmlns:rep="trabalhoPDE">

    <xsl:template match="/">
        <rdf:RDF xml:base="trabalhoPDE">
            <xsl:apply-templates/>
        </rdf:RDF>      
    </xsl:template>

    

</xsl:stylesheet>
