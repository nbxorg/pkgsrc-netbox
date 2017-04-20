$NetBSD$

--- imap/mboxname.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/mboxname.c
@@ -1275,13 +1275,10 @@ HIDDEN int mboxname_policycheck(const ch
 	    name++;		/* Skip over terminating '-' */
 	}
 	else {
-	    /* If we're using unixhierarchysep, DOTCHAR is allowed */
 	    if (!strchr(GOODCHARS, *name) &&
+		/* If we're using unixhierarchysep, DOTCHAR is allowed */
 		!(unixsep && *name == DOTCHAR))
 		return IMAP_MAILBOX_BADNAME;
-	    /* If we're not using virtdomains, '@' is not permitted in the mboxname */
-	    if (!config_virtdomains && *name == '@')
-		return IMAP_MAILBOX_BADNAME;
 	    name++;
 	    sawutf7 = 0;
 	}
