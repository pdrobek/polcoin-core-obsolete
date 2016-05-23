DIST_SUBDIRS = secp256k1
AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS)


if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(CXXFLAGS) $(CPPFLAGS)"
endif

POLCOIN_CONFIG_INCLUDES=-I$(builddir)/config
POLCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

POLCOIN_INCLUDES += -I$(srcdir)/secp256k1/include

LIBPOLCOIN_SERVER=libpolcoin_server.a
LIBPOLCOIN_WALLET=libpolcoin_wallet.a
LIBPOLCOIN_COMMON=libpolcoin_common.a
LIBPOLCOIN_CLI=libpolcoin_cli.a
LIBPOLCOIN_UTIL=libpolcoin_util.a
LIBPOLCOIN_CRYPTO=crypto/libpolcoin_crypto.a
LIBPOLCOIN_UNIVALUE=univalue/libpolcoin_univalue.a
LIBPOLCOINQT=qt/libpolcoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libpolcoin_crypto.a \
  libpolcoin_util.a \
  libpolcoin_common.a \
  univalue/libpolcoin_univalue.a \
  libpolcoin_server.a \
  libpolcoin_cli.a
if ENABLE_WALLET
POLCOIN_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libpolcoin_wallet.a
endif

if BUILD_POLCOIN_LIBS
lib_LTLIBRARIES = libpolcoinconsensus.la
LIBPOLCOIN_CONSENSUS=libpolcoinconsensus.la
else
LIBPOLCOIN_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =

if BUILD_POLCOIND
  bin_PROGRAMS += polcoind
endif

if BUILD_POLCOIN_UTILS
  bin_PROGRAMS += polcoin-cli polcoin-tx
endif

.PHONY: FORCE
# polcoin core #
POLCOIN_CORE_H = \
  addrman.h \
  alert.h \
  allocators.h \
  amount.h \
  base58.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compressor.h \
  primitives/block.h \
  primitives/transaction.h \
  core_io.h \
  crypter.h \
  db.h \
  eccryptoverify.h \
  ecwrapper.h \
  hash.h \
  init.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  merkleblock.h \
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  noui.h \
  pow.h \
  protocol.h \
  pubkey.h \
  random.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  script/interpreter.h \
  script/script.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/script_error.h \
  serialize.h \
  streams.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilstrencodings.h \
  utilmoneystr.h \
  utiltime.h \
  version.h \
  wallet.h \
  wallet_ismine.h \
  walletdb.h \
  compat/sanity.h

JSON_H = \
  json/json_spirit.h \
  json/json_spirit_error_position.h \
  json/json_spirit_reader.h \
  json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h \
  json/json_spirit_utils.h \
  json/json_spirit_value.h \
  json/json_spirit_writer.h \
  json/json_spirit_writer_template.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libpolcoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between polcoind and polcoin-qt
libpolcoin_server_a_CPPFLAGS = $(POLCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS)
libpolcoin_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  txdb.cpp \
  txmempool.cpp \
  $(JSON_H) \
  $(POLCOIN_CORE_H)

# wallet: shared between polcoind and polcoin-qt, but only linked
# when wallet enabled
libpolcoin_wallet_a_CPPFLAGS = $(POLCOIN_INCLUDES)
libpolcoin_wallet_a_SOURCES = \
  db.cpp \
  crypter.cpp \
  rpcdump.cpp \
  rpcwallet.cpp \
  wallet.cpp \
  wallet_ismine.cpp \
  walletdb.cpp \
  $(POLCOIN_CORE_H)

# crypto primitives library
crypto_libpolcoin_crypto_a_CPPFLAGS = $(POLCOIN_CONFIG_INCLUDES)
crypto_libpolcoin_crypto_a_SOURCES = \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/hmac_sha256.cpp \
  crypto/rfc6979_hmac_sha256.cpp \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/common.h \
  crypto/sha256.h \
  crypto/sha512.h \
  crypto/hmac_sha256.h \
  crypto/rfc6979_hmac_sha256.h \
  crypto/hmac_sha512.h \
  crypto/sha1.h \
  crypto/ripemd160.h

# univalue JSON library
univalue_libpolcoin_univalue_a_SOURCES = \
  univalue/univalue.cpp \
  univalue/univalue_read.cpp \
  univalue/univalue_write.cpp \
  univalue/univalue_escapes.h \
  univalue/univalue.h

# common: shared between polcoind, and polcoin-qt and non-server tools
libpolcoin_common_a_CPPFLAGS = $(POLCOIN_INCLUDES)
libpolcoin_common_a_SOURCES = \
  allocators.cpp \
  amount.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  core_read.cpp \
  core_write.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  pubkey.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/sign.cpp \
  script/standard.cpp \
  script/script_error.cpp \
  $(POLCOIN_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libpolcoin_util_a_CPPFLAGS = $(POLCOIN_INCLUDES)
libpolcoin_util_a_SOURCES = \
  compat/strnlen.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  random.cpp \
  rpcprotocol.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilstrencodings.cpp \
  utilmoneystr.cpp \
  utiltime.cpp \
  $(POLCOIN_CORE_H)

if GLIBC_BACK_COMPAT
libpolcoin_util_a_SOURCES += compat/glibc_compat.cpp
libpolcoin_util_a_SOURCES += compat/glibcxx_compat.cpp
endif

# cli: shared between polcoin-cli and polcoin-qt
libpolcoin_cli_a_CPPFLAGS = $(POLCOIN_INCLUDES)
libpolcoin_cli_a_SOURCES = \
  rpcclient.cpp \
  $(POLCOIN_CORE_H)

nodist_libpolcoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# polcoind binary #
polcoind_LDADD = \
  $(LIBPOLCOIN_SERVER) \
  $(LIBPOLCOIN_COMMON) \
  $(LIBPOLCOIN_UNIVALUE) \
  $(LIBPOLCOIN_UTIL) \
  $(LIBPOLCOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_WALLET
polcoind_LDADD += libpolcoin_wallet.a
endif
polcoind_SOURCES = polcoind.cpp
#

if TARGET_WINDOWS
polcoind_SOURCES += polcoind-res.rc
endif

polcoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS)
polcoind_CPPFLAGS = $(POLCOIN_INCLUDES)
polcoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

# polcoin-cli binary #
polcoin_cli_LDADD = \
  $(LIBPOLCOIN_CLI) \
  $(LIBPOLCOIN_UTIL) \
  $(BOOST_LIBS) \
  $(SSL_LIBS) \
  $(CRYPTO_LIBS)

polcoin_cli_SOURCES = \
  polcoin-cli.cpp

polcoin_cli_CPPFLAGS = $(POLCOIN_INCLUDES)
#

# polcoin-tx binary #
polcoin_tx_LDADD = \
  $(LIBPOLCOIN_UNIVALUE) \
  $(LIBPOLCOIN_COMMON) \
  $(LIBPOLCOIN_UTIL) \
  $(LIBPOLCOIN_CRYPTO) \
  $(LIBSECP256K1) \
  $(BOOST_LIBS) \
  $(CRYPTO_LIBS)

polcoin_tx_SOURCES = polcoin-tx.cpp
polcoin_tx_CPPFLAGS = $(POLCOIN_INCLUDES)
#
polcoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
polcoin_cli_SOURCES += polcoin-cli-res.rc
endif
polcoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if BUILD_POLCOIN_LIBS
include_HEADERS = script/polcoinconsensus.h
libpolcoinconsensus_la_SOURCES = \
  primitives/transaction.cpp \
  crypto/hmac_sha512.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/ripemd160.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  pubkey.cpp \
  script/script.cpp \
  script/interpreter.cpp \
  script/polcoinconsensus.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libpolcoinconsensus_la_SOURCES += compat/glibc_compat.cpp
  libpolcoinconsensus_la_SOURCES += compat/glibcxx_compat.cpp
endif

libpolcoinconsensus_la_LDFLAGS = -no-undefined $(RELDFLAGS)
libpolcoinconsensus_la_LIBADD = $(CRYPTO_LIBS)
libpolcoinconsensus_la_CPPFLAGS = $(CRYPTO_CFLAGS) -I$(builddir)/obj -DBUILD_POLCOIN_INTERNAL
if USE_LIBSECP256K1
libpolcoinconsensus_la_LIBADD += secp256k1/libsecp256k1.la
endif
endif

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(CXXFLAGS)  -c -o $@ $<

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
