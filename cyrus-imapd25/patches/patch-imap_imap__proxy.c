$NetBSD$

--- imap/imap_proxy.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/imap_proxy.c
@@ -93,6 +93,7 @@ struct protocol_t imap_protocol =
 	  { "IDLE", CAPA_IDLE },
 	  { "MUPDATE", CAPA_MUPDATE },
 	  { "MULTIAPPEND", CAPA_MULTIAPPEND },
+	  { "METADATA", CAPA_METADATA },
 	  { "RIGHTS=kxte", CAPA_ACLRIGHTS },
 	  { "LIST-EXTENDED", CAPA_LISTEXTENDED },
 	  { "SASL-IR", CAPA_SASL_IR },
@@ -1248,7 +1249,9 @@ int annotate_fetch_proxy(const char *ser
 			 const strarray_t *attribute_pat)
 {
     struct backend *be;
-    int i;
+    int x, y;
+    int use_metadata = 0;
+
     char mytag[128];
     
     assert(server && mbox_pat && entry_pat && attribute_pat);
@@ -1256,17 +1259,61 @@ int annotate_fetch_proxy(const char *ser
     be = proxy_findserver(server, &imap_protocol,
 			  proxy_userid, &backend_cached,
 			  &backend_current, &backend_inbox, imapd_in);
+
     if (!be) return IMAP_SERVER_UNAVAILABLE;
 
+    // See if we would want to use METADATA at all, by testing the 'attribute
+    // pattern' to be either the draft ANNOTATEMORE or the METADATA RFC.
+    for (x = 0 ; x < attribute_pat->count ; x++) {
+	if (!strncmp(attribute_pat->data[x], "/shared", 7)) {
+	    use_metadata = 1;
+	}
+
+	else if (!strncmp(attribute_pat->data[x], "/private", 8)) {
+	    use_metadata = 1;
+	}
+    }
+
     /* Send command to remote */
     proxy_gentag(mytag, sizeof(mytag));
-    prot_printf(be->out, "%s GETANNOTATION \"%s\" (", mytag, mbox_pat);
-    for (i = 0 ; i < entry_pat->count ; i++) {
-	prot_printf(be->out, "%s\"%s\"", i ? " " : "", entry_pat->data[i]);
-    }
-    prot_printf(be->out, ") (");
-    for (i = 0 ; i < attribute_pat->count ; i++) {
-	prot_printf(be->out, "%s\"%s\"", i ? " " : "", attribute_pat->data[i]);
+
+    if (!CAPA(be, CAPA_METADATA) && use_metadata) {
+	syslog(
+		LOG_WARNING,
+		"METADATA requested, but '%s' does not have the capability",
+		server
+	    );
+    }
+
+    if (CAPA(be, CAPA_METADATA) && use_metadata) {
+	prot_printf(be->out, "%s GETMETADATA (DEPTH infinity) ", mytag);
+	prot_printf(be->out, "\"%s\" (", mbox_pat);
+
+	for (x = 0 ; x < attribute_pat->count ; x++) {
+	    for (y = 0 ; y < entry_pat->count ; y++) {
+		prot_printf(
+			be->out,
+			"%s\"%s%s\"",
+			(x || y) ? " ": "",
+			attribute_pat->data[x],
+			entry_pat->data[y]
+		    );
+	    }
+	}
+    }
+
+    else {
+	prot_printf(be->out, "%s GETANNOTATION \"%s\" (", mytag, mbox_pat);
+
+	for (y = 0 ; y < entry_pat->count ; y++) {
+	    prot_printf(be->out, "%s\"%s\"", y ? " " : "", entry_pat->data[y]);
+	}
+
+	prot_printf(be->out, ") (");
+
+	for (x = 0 ; x < attribute_pat->count ; x++) {
+	    prot_printf(be->out, "%s\"%s\"", x ? " " : "", attribute_pat->data[x]);
+	}
     }
     prot_printf(be->out, ")\r\n");
     prot_flush(be->out);
@@ -1369,22 +1416,25 @@ static void proxy_part_filldata(partlist
     if (be) {
 	uint64_t server_available = 0;
 	uint64_t server_total = 0;
+	const char *annot =
+	    (part_list->mode == PART_SELECT_MODE_FREESPACE_MOST) ?
+	    "freespace/total" : "freespace/percent/most";
+	struct buf cmd = BUF_INITIALIZER;
 	int c;
 
 	/* fetch annotation from remote */
 	proxy_gentag(mytag, sizeof(mytag));
-	if (part_list->mode == PART_SELECT_MODE_FREESPACE_MOST) {
-	    prot_printf(be->out,
-		"%s GETANNOTATION \"\" "
-		"\"/vendor/cmu/cyrus-imapd/freespace/total\" "
-		"\"value.shared\"\r\n", mytag);
+
+	if (CAPA(be, CAPA_METADATA)) {
+	    buf_printf(&cmd, "METADATA \"\" (\"/shared/vendor/cmu/cyrus-imapd/%s\"",
+			annot);
 	}
 	else {
-	    prot_printf(be->out,
-		"%s GETANNOTATION \"\" "
-		"\"/vendor/cmu/cyrus-imapd/freespace/percent/most\" "
-		"\"value.shared\"\r\n", mytag);
+	    buf_printf(&cmd, "ANNOTATION \"\" \"/vendor/cmu/cyrus-imapd/%s\" "
+			"(\"value.shared\"", annot);
 	}
+
+	prot_printf(be->out, "%s GET%s)\r\n", mytag, buf_cstring(&cmd));
 	prot_flush(be->out);
 
 	for (/* each annotation response */;;) {
@@ -1394,17 +1444,8 @@ static void proxy_part_filldata(partlist
 	    c = prot_getc(be->in);
 	    if (c != ' ') { /* protocol error */ c = EOF; break; }
 
-	    if (part_list->mode == PART_SELECT_MODE_FREESPACE_MOST) {
-		c = chomp(be->in,
-		    "ANNOTATION \"\" "
-		    "\"/vendor/cmu/cyrus-imapd/freespace/total\" "
-		    "(\"value.shared\" ");
-	    } else {
-		c = chomp(be->in,
-		    "ANNOTATION \"\" "
-		    "\"/vendor/cmu/cyrus-imapd/freespace/percent/most\" "
-		    "(\"value.shared\" ");
-	    }
+	    c = chomp(be->in, buf_cstring(&cmd));
+	    if (c == ' ') c = prot_getc(be->in);
 	    if ((c == EOF) || (c != '\"')) {
 		/* we don't care about this response */
 		eatline(be->in, c);
@@ -1420,6 +1461,7 @@ static void proxy_part_filldata(partlist
 	    if (c != '\"') { c = EOF; break; }
 	    eatline(be->in, c); /* we don't care about the rest of the line */
 	}
+	buf_free(&cmd);
 	if (c != EOF) {
 	    prot_ungetc(c, be->in);
 
