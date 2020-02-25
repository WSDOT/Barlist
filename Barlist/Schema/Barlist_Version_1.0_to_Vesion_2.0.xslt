<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml"/>

  <!-- 
This transform maps Barlist version 2 files to Barlist version 3 files.
This transform can be safely applied to Barlist version 3 files
-->

  <!-- This template matches the document level element -->
  <xsl:template match="/">
    <xsl:choose>
      <!-- 
When the Barlist data is at version 2, perform the transform otherwise do an identity transform
-->
      <xsl:when test="Barlist/@version">
        <!-- Barlist data has the version attribute so it is at least version 2-->
        <!-- Do an identity transform-->
        <xsl:copy-of select="." />
      </xsl:when>
      <xsl:otherwise>
        <!-- otherwise, write the Barlist element with the version=2 attribute-->
        <Barlist version="2">
          <xsl:apply-templates/>
        </Barlist>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Remove Quantities Not Included -->
  <xsl:template match="QNI">
  </xsl:template>

  <!-- Copy Properties into the new document (no changes) -->
  <xsl:template match="Properties">
    <xsl:copy-of select="." />
  </xsl:template>

  <!-- Rebuild the Group record by modifying each BarRecord element -->
  <!-- Add Material, default A706_Grade60 to all bar records -->
  <!-- Keep everything else the same-->
  <xsl:template match="Group">
    <Group>
    <Name>
      <xsl:value-of select="Name"/>
    </Name>
    <xsl:for-each select="BarRecord">
      <BarRecord>
        <Material>A706_Grade60</Material>
        <xsl:copy-of select="Mark"/>
        <xsl:copy-of select="Location"/>
        <xsl:copy-of select="Size"/>
        <xsl:copy-of select="NoReqd"/>
        <xsl:copy-of select="BendType"/>
        <xsl:copy-of select="Use"/>
        <!-- <xsl:copy-of select="LumpSum"/> dropping lump sum -->
        <xsl:copy-of select="Substructure"/>
        <xsl:copy-of select="Epoxy"/>
        <xsl:copy-of select="Varies"/>
        <xsl:copy-of select="NoEach"/>
        <xsl:copy-of select="PrimaryBend"/>
        <xsl:copy-of select="VariesBend"/>
      </BarRecord>
    </xsl:for-each>
    </Group>
  </xsl:template>

</xsl:stylesheet>
