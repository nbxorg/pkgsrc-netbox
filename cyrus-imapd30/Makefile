# $NetBSD$

DISTNAME=	cyrus-imapd-3.0.1
CATEGORIES=	devel
MASTER_SITES=	ftp://ftp.cyrusimap.org/cyrus-imapd/

MAINTAINER=	yuka@netbox.org
HOMEPAGE=	ftp://ftp.cyrusimap.org/cyrus-imapd/
COMMENT=	Cyrus IMAP server
LICENSE=	bsd-original # like

#USE_TOOLS += pkg-config libtool autotool automake aclocal bison autoconf gmake perl:run makedepend
USE_TOOLS += pkg-config gmake bison perl:run makedepend
USE_LIBTOOL= yes
USE_PKGLOCALEDIR= yes
GNU_CONFIGURE= yes
GNU_LIBTOOL= yes
PERL5_CONFIGURE= no 

BUILD_DEFS+= VARBASE
USE_LANGUAGES+= c c++ C99
CHECK_PORTABILITY_SKIP= configure.lineno

#MAKE_JOBS_SAFE=no
#PKGCONFIG_OVERRIDE+= libcyrus_min.pc.in
#PKGCONFIG_OVERRIDE+= libcyrus_sieve.pc.in
#PKGCONFIG_OVERRIDE+= libcyrus.pc.in

CYRUS_USER?=	cyrus	
CYRUS_GROUP?=	mail

BUILD_DEFS+=	CYRUS_USER CYRUS_GROUP
FILES_SUBST+=	CYRUS_USER=${CYRUS_USER}
FILES_SUBST+=	CYRUS_GROUP=${CYRUS_GROUP}


.include "../../security/cyrus-sasl/buildlink3.mk"
.include "../../security/openssl/buildlink3.mk"
.include "../../textproc/jansson/buildlink3.mk"
.include "../../textproc/icu/buildlink3.mk"
.include "../../devel/flex/buildlink3.mk"
.include "../../lang/perl5/module.mk"

.include "../../mk/bsd.prefs.mk"


CONFIGURE_ARGS+= --libexecdir=${PREFIX}/libexec/cyrus
CONFIGURE_ARGS+= --with-cyrus-user=${CYRUS_USER}
CONFIGURE_ARGS+= --with-cyrus-group=${CYRUS_USER}
CONFIGURE_ARGS+= --with-sasl=${BUILDLINK_PREFIX.cyrus-sasl}
CONFIGURE_ARGS+= --with-perl=${PERL5:Q}

CONFIGURE_ARGS+= --with-pgsql=${BUILDLINK_PREFIX.postgresql96-client}
.include "../../databases/postgresql96-client/buildlink3.mk"

.include "../../databases/sqlite3/buildlink3.mk"
.include "../../time/libical/buildlink3.mk"
.include "../../textproc/libxml2/buildlink3.mk"
.include "../../www/nghttp2/buildlink3.mk"
CONFIGURE_ARGS+= --enable-http --enable-calalarmd

.include "../../mk/krb5.buildlink3.mk"

CONFIGURE_ARGS+= --enable-autocreate
CONFIGURE_ARGS+= --enable-idled

CONFIGURE_ARGS+= --enable-murder
CONFIGURE_ARGS+= --enable-replication

MAKE_ENV+= PERL=${PERL5:Q}
REPLACE_PERL+= tools/rehash

HTMLDIR= ${PREFIX}/share/doc/cyrus-imapd
EGDIR= ${PREFIX}/share/examples/cyrus-imapd
CONF_FILES= ${EGDIR}/imapd.conf ${PKG_SYSCONFDIR}/imapd.conf
#CONF_FILES+= ${EGDIR}/normal.conf ${PKG_SYSCONFDIR}/cyrus.conf
FILES_SUBST+= IMAPDCONF=${PKG_SYSCONFDIR}/imapd.conf

.include "../../mk/pthread.buildlink3.mk"

post-patch:
	cd ${WRKSRC}; \
	set --	deliver 8 fetchnews 8 idled 8 imapd 8 lmtpd 8 master 8 \
		nntpd 8 httpd 8 notifyd 8 pop3d 8 quota 8; \
	while [ $$# -gt 0 ]; do	\
		man=$$1.$$2; \
		for file in Makefile.in; do \
			sed	-e "s|\(\man/\)\($$man\)|\1cyrus-\2|g" \
				-e "s|\(\\\fB\)\($$1($$2)\\\fR\)|\1cyrus-\2|g" \
				$$file > $$file.fixed; \
			mv -f $$file.fixed $$file; \
		done; \
		if [ -f man/$${man} ]; then \
			mv man/$${man} man/cyrus-$${man}; \
		fi; \
		shift 2; \
	done
	find ${WRKSRC} -type -f -print | \
	xargs -n 1 ${GREP} -l -e /etc/imapd\.conf -e /etc/cyrus\.conf | \
	while read file; do \
		sed	-e "s|/etc/\(cyrus\.conf\)|${PKG_SYSCONFDIR}/\1|g" \
			-e "s|/etc/\(imapd\.conf\)|${PKG_SYSCONFDIR}/\1|g" \
			$${file} > $${file}.fixed; \
		mv -f $${file}.fixed $${file}; \
	done

post-install:
	${INSTALL_SCRIPT} ${WRKSRC}/tools/migrate-metadata ${DESTDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/tools/undohash ${DESTDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/tools/dohash ${DESTDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/tools/mkimap ${DESTDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/tools/rehash ${DESTDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/tools/upgradesieve ${DESTDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/tools/masssievec ${DESTDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/tools/mknewsgroups ${DESTDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/tools/translatesieve ${DESTDIR}${PREFIX}/bin

	${INSTALL_DATA_DIR} ${DESTDIR}${EGDIR}
	${INSTALL_DATA} ${FILESDIR}/imapd.conf ${DESTDIR}${EGDIR}
	${INSTALL_DATA_DIR} ${DESTDIR}${HTMLDIR}
	

.include "../../mk/bsd.pkg.mk"
