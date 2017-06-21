#define SQLITE_STATIC
#define SOCI_LIB_PREFIX "libsoci_"
#define SOCI_LIB_SUFFIX ".lib"

#include "backend-loader.cpp"
#include "blob.cpp"
#include "connection-parameters.cpp"
#include "connection-pool.cpp"
#include "error.cpp"
#include "into-type.cpp"
#include "once-temp-type.cpp"
#include "prepare-temp-type.cpp"
#include "procedure.cpp"
#include "ref-counted-prepare-info.cpp"
#include "ref-counted-statement.cpp"
#include "row.cpp"
#include "rowid.cpp"
#include "session.cpp"
#include "soci-simple.cpp"
#include "statement.cpp"
#include "transaction.cpp"
#include "use-type.cpp"
#include "values.cpp"

//Backends
#include "../backends/sqlite3/blob.cpp"
#include "../backends/sqlite3/common.cpp"
#include "../backends/sqlite3/error.cpp"
#include "../backends/sqlite3/factory.cpp"
#include "../backends/sqlite3/row-id.cpp"
#include "../backends/sqlite3/session.cpp"
#include "../backends/sqlite3/standard-into-type.cpp"
#include "../backends/sqlite3/standard-use-type.cpp"
#include "../backends/sqlite3/statement.cpp"
#include "../backends/sqlite3/vector-into-type.cpp"
#include "../backends/sqlite3/vector-use-type.cpp"
