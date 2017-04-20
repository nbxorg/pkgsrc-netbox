$NetBSD$

--- imap/annotate.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/annotate.c
@@ -2115,16 +2115,25 @@ EXPORTED int annotate_state_fetch(annota
 	int attribcount;
 
 	/*
-	 * TODO: this is bogus.  The * and % wildcard characters applied
-	 * to attributes in the early drafts of the ANNOTATEMORE
-	 * extension, but not in later drafts where those characters are
-	 * actually illegal in attribute names.
+	 * Ensure this function can be called with '/private' and '/shared' as
+	 * is used in the RFC, and 'value.priv' and 'value.shared' as
+	 * implemented in the RFC's drafts -- without requiring changes to
+	 * /etc/imapd.annotations.conf.
 	 */
-	g = glob_init(s, GLOB_HIERARCHY);
-	
+	if (!strncmp(s, "/private", 8)) {
+	    g = glob_init("value.priv", GLOB_HIERARCHY);
+	}
+	else if (!strncmp(s, "/shared", 7)) {
+	    g = glob_init("value.shared", GLOB_HIERARCHY);
+	}
+	else {
+	    g = glob_init(s, GLOB_HIERARCHY);
+	}
+
 	for (attribcount = 0;
 	     annotation_attributes[attribcount].name;
 	     attribcount++) {
+
 	    if (GLOB_TEST(g, annotation_attributes[attribcount].name) != -1) {
 		if (annotation_attributes[attribcount].entry & ATTRIB_DEPRECATED) {
 		    if (strcmp(s, "*"))
@@ -2136,7 +2145,7 @@ EXPORTED int annotate_state_fetch(annota
 		    state->attribs |= annotation_attributes[attribcount].entry;
 	    }
 	}
-	
+
 	glob_free(&g);
     }
 
