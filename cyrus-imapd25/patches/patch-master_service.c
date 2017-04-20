$NetBSD$

--- master/service.c.orig	2015-11-30 22:59:09.000000000 +0000
+++ master/service.c
@@ -382,10 +382,13 @@ int main(int argc, char **argv, char **e
     }
     id = atoi(p);
 
-    /* pick a random timeout between reuse_timeout -> 2*reuse_timeout
-     * to avoid massive IO overload if the network connection goes away */
     srand(time(NULL) * getpid());
-    reuse_timeout = reuse_timeout + (rand() % reuse_timeout);
+
+    /* if timeout is enabled, pick a random timeout between reuse_timeout
+     * and 2*reuse_timeout to avoid massive IO overload if the network
+     * connection goes away */
+    if (reuse_timeout)
+        reuse_timeout = reuse_timeout + (rand() % reuse_timeout);
 
     extern const int config_need_data;
     cyrus_init(alt_config, service, 0, config_need_data);
