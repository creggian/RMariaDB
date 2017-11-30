#ifndef __RMARIADB_MARIA_RESULT_PREP__
#define __RMARIADB_MARIA_RESULT_PREP__

#include <boost/noncopyable.hpp>
#include "DbConnection.h"
#include "MariaBinding.h"
#include "DbResult.h"
#include "MariaRow.h"
#include "MariaTypes.h"
#include "MariaUtils.h"

class MariaResultPrep : boost::noncopyable, public DbResult {
  MYSQL_STMT* pStatement_;
  MYSQL_RES* pSpec_;
  uint64_t rowsAffected_, rowsFetched_;

  int nCols_, nParams_;
  bool bound_, complete_;
  bool is_statement_;

  std::vector<MariaFieldType> types_;
  std::vector<std::string> names_;
  MariaBinding bindingInput_;
  MariaRow bindingOutput_;

public:
  MariaResultPrep(DbConnectionPtr conn, bool is_statement = false);
  ~MariaResultPrep();

public:
  virtual void send_query(const std::string& sql);
  virtual void close();

  virtual void bind(List params);

  virtual List get_column_info();

  virtual List fetch(int n_max = -1);

  virtual int n_rows_affected();
  virtual int n_rows_fetched();
  virtual bool complete();

public:
  class UnsupportedPS : public std::exception {};

private:
  void execute();

  bool has_result() const;
  bool step();
  bool fetch_row();
  void throw_error();

private:
  void cache_metadata();
};

#endif
