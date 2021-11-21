<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="yes" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="busbar" urn="urn:adsk.eagle:library:112">
<description>&lt;b&gt;Schroff Current Bus Bars for 19-Inch Racks &lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="TE3/4" urn="urn:adsk.eagle:footprint:5156/1" library_version="1">
<description>&lt;b&gt;BUS BAR&lt;/b&gt;&lt;p&gt;
4 connections, grid 3 TE</description>
<wire x1="-20.828" y1="0.508" x2="-9.652" y2="0.508" width="0.1524" layer="21"/>
<wire x1="-20.828" y1="-0.508" x2="-9.652" y2="-0.508" width="0.1524" layer="21"/>
<wire x1="-9.652" y1="-0.508" x2="-5.588" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="-5.588" y1="0.508" x2="-9.652" y2="0.508" width="0.1524" layer="51"/>
<wire x1="-20.828" y1="0.508" x2="-24.892" y2="0.508" width="0.1524" layer="51"/>
<wire x1="-24.892" y1="0.508" x2="-24.892" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="-24.892" y1="-0.508" x2="-20.828" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="24.892" y1="-0.508" x2="24.892" y2="0.508" width="0.1524" layer="51"/>
<wire x1="24.892" y1="0.508" x2="20.828" y2="0.508" width="0.1524" layer="51"/>
<wire x1="20.828" y1="-0.508" x2="24.892" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="5.588" y1="-0.508" x2="9.652" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="5.588" y1="0.508" x2="9.652" y2="0.508" width="0.1524" layer="51"/>
<wire x1="-5.588" y1="0.508" x2="5.588" y2="0.508" width="0.1524" layer="21"/>
<wire x1="-5.588" y1="-0.508" x2="5.588" y2="-0.508" width="0.1524" layer="21"/>
<wire x1="9.652" y1="-0.508" x2="20.828" y2="-0.508" width="0.1524" layer="21"/>
<wire x1="9.652" y1="0.508" x2="20.828" y2="0.508" width="0.1524" layer="21"/>
<pad name="1" x="-22.86" y="0" drill="1.6002" diameter="3.1496" shape="octagon"/>
<pad name="2" x="-7.62" y="0" drill="1.6002" diameter="3.1496" shape="octagon"/>
<pad name="3" x="7.62" y="0" drill="1.6002" diameter="3.1496" shape="octagon"/>
<pad name="4" x="22.86" y="0" drill="1.6002" diameter="3.1496" shape="octagon"/>
<text x="-20.32" y="0.889" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-5.08" y="0.889" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
</packages>
<packages3d>
<package3d name="TE3/4" urn="urn:adsk.eagle:package:5181/1" type="box" library_version="1">
<description>BUS BAR
4 connections, grid 3 TE</description>
<packageinstances>
<packageinstance name="TE3/4"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="4" urn="urn:adsk.eagle:symbol:5155/1" library_version="1">
<wire x1="7.62" y1="0" x2="6.985" y2="0.635" width="0.254" layer="94"/>
<wire x1="6.985" y1="0.635" x2="3.175" y2="0.635" width="0.254" layer="94"/>
<wire x1="3.175" y1="0.635" x2="2.54" y2="0" width="0.254" layer="94"/>
<wire x1="2.54" y1="0" x2="1.905" y2="0.635" width="0.254" layer="94"/>
<wire x1="1.905" y1="0.635" x2="-1.905" y2="0.635" width="0.254" layer="94"/>
<wire x1="-1.905" y1="0.635" x2="-2.54" y2="0" width="0.254" layer="94"/>
<wire x1="-2.54" y1="0" x2="-3.175" y2="0.635" width="0.254" layer="94"/>
<wire x1="-3.175" y1="0.635" x2="-6.985" y2="0.635" width="0.254" layer="94"/>
<wire x1="-6.985" y1="0.635" x2="-7.62" y2="0" width="0.254" layer="94"/>
<circle x="-7.62" y="0" radius="0.254" width="0.4064" layer="94"/>
<circle x="-2.54" y="0" radius="0.254" width="0.4064" layer="94"/>
<circle x="2.54" y="0" radius="0.254" width="0.4064" layer="94"/>
<circle x="7.62" y="0" radius="0.254" width="0.4064" layer="94"/>
<text x="-7.62" y="1.27" size="1.778" layer="95">&gt;NAME</text>
<text x="0" y="1.27" size="1.778" layer="96">&gt;VALUE</text>
<pin name="P@1" x="-7.62" y="-2.54" visible="pad" length="short" direction="pas" swaplevel="1" rot="R90"/>
<pin name="P@2" x="-2.54" y="-2.54" visible="pad" length="short" direction="pas" swaplevel="1" rot="R90"/>
<pin name="P@3" x="2.54" y="-2.54" visible="pad" length="short" direction="pas" swaplevel="1" rot="R90"/>
<pin name="P@4" x="7.62" y="-2.54" visible="pad" length="short" direction="pas" swaplevel="1" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="TE3/4" urn="urn:adsk.eagle:component:5197/1" prefix="BB" uservalue="yes" library_version="1">
<description>&lt;b&gt;BUS BAR&lt;/b&gt;&lt;p&gt;
4 connections, grid 3 TE</description>
<gates>
<gate name="1" symbol="4" x="0" y="0"/>
</gates>
<devices>
<device name="" package="TE3/4">
<connects>
<connect gate="1" pin="P@1" pad="1"/>
<connect gate="1" pin="P@2" pad="2"/>
<connect gate="1" pin="P@3" pad="3"/>
<connect gate="1" pin="P@4" pad="4"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:5181/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="BB1" library="busbar" library_urn="urn:adsk.eagle:library:112" deviceset="TE3/4" device="" package3d_urn="urn:adsk.eagle:package:5181/1"/>
<part name="BB2" library="busbar" library_urn="urn:adsk.eagle:library:112" deviceset="TE3/4" device="" package3d_urn="urn:adsk.eagle:package:5181/1"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="BB1" gate="1" x="33.02" y="81.28" smashed="yes" rot="R270">
<attribute name="NAME" x="34.29" y="88.9" size="1.778" layer="95" rot="R270"/>
<attribute name="VALUE" x="34.29" y="81.28" size="1.778" layer="96" rot="R270"/>
</instance>
<instance part="BB2" gate="1" x="68.58" y="81.28" smashed="yes" rot="R270">
<attribute name="NAME" x="69.85" y="88.9" size="1.778" layer="95" rot="R270"/>
<attribute name="VALUE" x="69.85" y="81.28" size="1.778" layer="96" rot="R270"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="AC-LINE" class="0">
<segment>
<pinref part="BB1" gate="1" pin="P@1"/>
<wire x1="30.48" y1="88.9" x2="20.32" y2="88.9" width="0.1524" layer="91"/>
<label x="7.62" y="88.9" size="1.778" layer="95"/>
</segment>
</net>
<net name="AC-NEUT" class="0">
<segment>
<pinref part="BB2" gate="1" pin="P@1"/>
<wire x1="66.04" y1="88.9" x2="58.42" y2="88.9" width="0.1524" layer="91"/>
<label x="45.72" y="88.9" size="1.778" layer="95"/>
</segment>
</net>
<net name="SURGE-L" class="0">
<segment>
<pinref part="BB1" gate="1" pin="P@2"/>
<wire x1="30.48" y1="83.82" x2="20.32" y2="83.82" width="0.1524" layer="91"/>
<label x="7.62" y="83.82" size="1.778" layer="95"/>
</segment>
</net>
<net name="SURGE-N" class="0">
<segment>
<pinref part="BB2" gate="1" pin="P@2"/>
<wire x1="66.04" y1="83.82" x2="58.42" y2="83.82" width="0.1524" layer="91"/>
<label x="45.72" y="83.82" size="1.778" layer="95"/>
</segment>
</net>
<net name="PS-L" class="0">
<segment>
<pinref part="BB1" gate="1" pin="P@3"/>
<wire x1="30.48" y1="78.74" x2="20.32" y2="78.74" width="0.1524" layer="91"/>
<label x="12.7" y="78.74" size="1.778" layer="95"/>
</segment>
</net>
<net name="PS-N" class="0">
<segment>
<pinref part="BB2" gate="1" pin="P@3"/>
<wire x1="66.04" y1="78.74" x2="58.42" y2="78.74" width="0.1524" layer="91"/>
<label x="50.8" y="78.74" size="1.778" layer="95"/>
</segment>
</net>
<net name="FAN" class="0">
<segment>
<pinref part="BB2" gate="1" pin="P@4"/>
<wire x1="66.04" y1="73.66" x2="58.42" y2="73.66" width="0.1524" layer="91"/>
<label x="50.8" y="73.66" size="1.778" layer="95"/>
</segment>
</net>
<net name="FAN-SSR" class="0">
<segment>
<pinref part="BB1" gate="1" pin="P@4"/>
<wire x1="30.48" y1="73.66" x2="20.32" y2="73.66" width="0.1524" layer="91"/>
<label x="7.62" y="73.66" size="1.778" layer="95"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
