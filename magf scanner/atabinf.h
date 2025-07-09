#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <memory> // For std::shared_ptr
#include <sstream>
#include <vector>
#include <functional>
#include <type_traits>
#include <any>
#include <variant> // For RowWrapper::get return type
#include <optional> // Potentially useful, but variant is better here
#include <map>      // For name-to-index lookup

// --- SQLite Headers ---
#include <winsqlite/winsqlite3.h>

// --- Forward Declarations ---
class DBHandler;
template <typename... ColTypes>
class TableHandler;
template <typename... ColTypes>
class QueryBuilder;
template <typename... ColTypes>
class RowWrapper; // New Row Wrapper

// --- Basic Exception Class ---
class SqliteException : public std::runtime_error {
public:
    SqliteException(const std::string& msg, int rc = -1)
        : std::runtime_error(
            msg +
            (rc != -1 ? " (SQLite code: " + std::to_string(rc) + ")" : "")
        ) {}
};

// --- RAII Wrapper for sqlite3 handle (Unchanged) ---
class SqliteConnection {
    sqlite3* db_ = nullptr;

public:
    SqliteConnection(const std::string& dbPath) {
        int rc = sqlite3_open(dbPath.c_str(), &db_);
        if (rc != SQLITE_OK) {
            std::string errMsg = sqlite3_errmsg(db_);
            sqlite3_close(db_); // Attempt to close even on open failure
            throw SqliteException("Cannot open database: " + errMsg, rc);
        }
        std::cout << "Database opened successfully: " << dbPath << std::endl;
    }

    ~SqliteConnection() {
        if (db_) {
            sqlite3_close(db_);
            std::cout << "Database closed." << std::endl;
        }
    }

    SqliteConnection(const SqliteConnection&) = delete;
    SqliteConnection& operator=(const SqliteConnection&) = delete;
    SqliteConnection(SqliteConnection&& other) noexcept : db_(other.db_) {
        other.db_ = nullptr;
    }
    SqliteConnection& operator=(SqliteConnection&& other) noexcept {
        if (this != &other) {
            if (db_) sqlite3_close(db_);
            db_ = other.db_;
            other.db_ = nullptr;
        }
        return *this;
    }
    sqlite3* get() const { return db_; }

    // Helper to execute simple SQL (used for CREATE TABLE)
    void execute(const std::string& sql) {
        //std::cout << "Executing SQL: " << sql << std::endl;
        char* errMsg = nullptr;
        int rc = sqlite3_exec(get(), sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::string err = errMsg ? errMsg : "Unknown error";
            sqlite3_free(errMsg);
            throw SqliteException("Failed to execute SQL: " + err, rc);
        }
        //std::cout << "Execution successful." << std::endl;
    }
};


// --- Type Mapping & Binding Helpers (Unchanged) ---
namespace Detail {

    // Bind a value to a prepared statement
    template <typename T>
    void bindValue(sqlite3_stmt* stmt, int index, const T& value);
    template <>
    void bindValue<int>(sqlite3_stmt* stmt, int index, const int& value) {
        sqlite3_bind_int(stmt, index, value);
    }
    template <>
    void bindValue<long long>(
        sqlite3_stmt* stmt, int index, const long long& value
    ) {
        sqlite3_bind_int64(stmt, index, value);
    }
    template <>
    void bindValue<double>(sqlite3_stmt* stmt, int index, const double& value) {
        sqlite3_bind_double(stmt, index, value);
    }
    template <>
    void bindValue<float>(sqlite3_stmt* stmt, int index, const float& value) {
        sqlite3_bind_double(stmt, index, static_cast<double>(value));
    }
    template <>
    void bindValue<std::string>(
        sqlite3_stmt* stmt, int index, const std::string& value
    ) {
        sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    // Get a value from a result column
    template <typename T>
    T getColumnValue(sqlite3_stmt* stmt, int index);
    template <>
    int getColumnValue<int>(sqlite3_stmt* stmt, int index) {
        return sqlite3_column_int(stmt, index);
    }
    template <>
    long long getColumnValue<long long>(sqlite3_stmt* stmt, int index) {
        return sqlite3_column_int64(stmt, index);
    }
    template <>
    double getColumnValue<double>(sqlite3_stmt* stmt, int index) {
        return sqlite3_column_double(stmt, index);
    }
    template <>
    float getColumnValue<float>(sqlite3_stmt* stmt, int index) {
        return static_cast<float>(sqlite3_column_double(stmt, index));
    }
    template <>
    std::string getColumnValue<std::string>(sqlite3_stmt* stmt, int index) {
        const unsigned char* text = sqlite3_column_text(stmt, index);
        return text ? reinterpret_cast<const char*>(text) : "";
    }

    // Helper to bind a tuple of values
    template <size_t Index = 0, typename... ColTypes>
    typename std::enable_if<Index == sizeof...(ColTypes)>::type bindTuple(
        sqlite3_stmt* /*stmt*/, const std::tuple<ColTypes...>& /*t*/
    ) {}

    template <size_t Index = 0, typename... ColTypes>
    typename std::enable_if < Index<sizeof...(ColTypes)>::type bindTuple(
        sqlite3_stmt* stmt, const std::tuple<ColTypes...>& t
    ) {
        bindValue(stmt, Index + 1, std::get<Index>(t));
        bindTuple<Index + 1>(stmt, t);
    }

    // Helper to create a tuple from a result row
    template <typename... ColTypes, std::size_t... Indices>
    std::tuple<ColTypes...> createTupleFromRow(
        sqlite3_stmt* stmt, std::index_sequence<Indices...>
    ) {
        return std::make_tuple(getColumnValue<ColTypes>(stmt, Indices)...);
    }

    // --- NEW: C++ Type to SQLite Type String Mapping ---
    template <typename T>
    struct SqliteTypeString; // Forward declaration

    template <>
    struct SqliteTypeString<int> {
        static constexpr const char* value = "INTEGER";
    };
    template <>
    struct SqliteTypeString<long long> {
        static constexpr const char* value = "INTEGER";
    };
    template <>
    struct SqliteTypeString<float> {
        static constexpr const char* value = "REAL";
    };
    template <>
    struct SqliteTypeString<double> {
        static constexpr const char* value = "REAL";
    };
    template <>
    struct SqliteTypeString<std::string> {
        static constexpr const char* value = "TEXT";
    };
    // Add more specializations (e.g., for bool -> INTEGER, custom types -> BLOB/TEXT)

    // Helper to build column definitions for CREATE TABLE
    template <typename Tuple, std::size_t... Indices>
    std::string buildColumnDefinitions(
        const std::vector<std::string>& names, std::index_sequence<Indices...>
    ) {
        std::stringstream ss;
        (
            (ss << (Indices == 0 ? "" : ", ") << names[Indices] << " "
                << SqliteTypeString<std::tuple_element_t<Indices, Tuple>>::value),
            ...
            );
        return ss.str();
    }

} // namespace Detail


// --- NEW: Row Wrapper ---
template <typename... ColTypes>
class RowWrapper {
public:
    using TupleType = std::tuple<ColTypes...>;
    using VariantType = std::variant<ColTypes...>;

private:
    TupleType data_;
    // Store shared_ptr to names/map for efficiency and lifetime safety
    std::shared_ptr<const std::vector<std::string>> colNamesPtr_;
    std::shared_ptr<const std::map<std::string, size_t>> nameToIndexMapPtr_;

    // Helper to create the variant based on runtime index
    template <size_t I = 0>
    VariantType getVariantByIndex(size_t index) const {
        if constexpr (I < sizeof...(ColTypes)) {
            if (I == index) {
                return VariantType{ std::in_place_index<I>, std::get<I>(data_) }; // Implicitly constructs variant
            }
            else {
                return getVariantByIndex<I + 1>(index);
            }
        }
        else {
            // This should be unreachable if index is valid
            throw std::logic_error("Invalid index in getVariantByIndex");
        }
    }

public:
    RowWrapper(
        TupleType data,
        std::shared_ptr<const std::vector<std::string>> colNames,
        std::shared_ptr<const std::map<std::string, size_t>> nameToIndexMap
    )
        : data_(std::move(data)),
        colNamesPtr_(std::move(colNames)),
        nameToIndexMapPtr_(std::move(nameToIndexMap)) {}

    // Access by compile-time index (like std::get)
    template <size_t Index>
    const std::tuple_element_t<Index, TupleType>& get() const {
        static_assert(Index < sizeof...(ColTypes), "Index out of bounds");
        return std::get<Index>(data_);
    }

    // Access by runtime string name
    VariantType get(const std::string& columnName) const {
        if (!nameToIndexMapPtr_) {
            throw std::runtime_error(
                "Column name map is not available in RowWrapper."
            );
        }
        auto it = nameToIndexMapPtr_->find(columnName);
        if (it == nameToIndexMapPtr_->end()) {
            throw std::out_of_range(
                "Column name not found: " + columnName
            );
        }
        size_t index = it->second;
        return getVariantByIndex(index);
    }

    // Allow access to the underlying tuple if needed
    const TupleType& getTuple() const { return data_; }
};


// --- Query Builder (Modified to return RowWrapper) ---
template <typename... ColTypes>
class QueryBuilder {
public:
    // Use RowWrapper instead of raw tuple
    using WrappedRowType = RowWrapper<ColTypes...>;

private:
    sqlite3* db_;
    std::string tableName_;
    // Use shared_ptr for column names and the map for sharing with RowWrapper
    std::shared_ptr<const std::vector<std::string>> colNamesPtr_;
    std::shared_ptr<const std::map<std::string, size_t>> nameToIndexMapPtr_;

    std::string whereClause_;
    std::vector<std::any> params_; // Keep using std::any for simplicity here

    // Constructor takes shared pointers now
    QueryBuilder(
        sqlite3* db,
        std::string tableName,
        std::shared_ptr<const std::vector<std::string>> colNames,
        std::shared_ptr<const std::map<std::string, size_t>> nameToIndexMap
    )
        : db_(db),
        tableName_(std::move(tableName)),
        colNamesPtr_(std::move(colNames)),
        nameToIndexMapPtr_(std::move(nameToIndexMap)) {}

    friend class TableHandler<ColTypes...>;

    QueryBuilder& addCondition(
        size_t colIndex, const std::string& op, std::any value
    ) {
        if (!colNamesPtr_ || colIndex >= colNamesPtr_->size()) {
            throw std::out_of_range("Column index out of range for filtering.");
        }
        if (!whereClause_.empty()) {
            whereClause_ += " AND ";
        }
        whereClause_ += (*colNamesPtr_)[colIndex] + " " + op + " ?";
        params_.push_back(std::move(value));
        return *this;
    }

public:
    // Filter methods use std::tuple_element_t based on the original ColTypes
    template <size_t ColIndex>
    QueryBuilder& filterEq(
        const std::tuple_element_t<ColIndex, std::tuple<ColTypes...>>& value
    ) {
        static_assert(
            ColIndex < sizeof...(ColTypes), "Column index out of bounds"
            );
        return addCondition(ColIndex, "=", value);
    }

    template <size_t ColIndex>
    QueryBuilder& filterNotEq(
        const std::tuple_element_t<ColIndex, std::tuple<ColTypes...>>& value
    ) {
        static_assert(
            ColIndex < sizeof...(ColTypes), "Column index out of bounds"
            );
        return addCondition(ColIndex, "!=", value);
    }

    template <size_t ColIndex>
    QueryBuilder& filterGreaterThan(
        const std::tuple_element_t<ColIndex, std::tuple<ColTypes...>>& value
    ) {
        static_assert(
            ColIndex < sizeof...(ColTypes), "Column index out of bounds"
            );
        return addCondition(ColIndex, ">", value);
    }

    template <size_t ColIndex>
    QueryBuilder& filterLessThan(
        const std::tuple_element_t<ColIndex, std::tuple<ColTypes...>>& value
    ) {
        static_assert(
            ColIndex < sizeof...(ColTypes), "Column index out of bounds"
            );
        return addCondition(ColIndex, "<", value);
    }
    // Add more filter types...

    // --- Execution (Returns vector of RowWrappers) ---
    std::vector<WrappedRowType> compute() {
        std::vector<WrappedRowType> results;
        if (!colNamesPtr_ || !nameToIndexMapPtr_) {
            throw std::logic_error("Column metadata pointers are null in QueryBuilder.");
        }

        std::string sql = "SELECT ";
        for (size_t i = 0; i < colNamesPtr_->size(); ++i) {
            sql += (*colNamesPtr_)[i] +
                (i == colNamesPtr_->size() - 1 ? "" : ", ");
        }
        sql += " FROM " + tableName_;
        if (!whereClause_.empty()) {
            sql += " WHERE " + whereClause_;
        }
        sql += ";";

        //std::cout << "Executing SQL: " << sql << std::endl;

        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            throw SqliteException(
                "Failed to prepare statement: " + std::string(sqlite3_errmsg(db_)),
                rc
            );
        }
        std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt_ptr(
            stmt, sqlite3_finalize
        );

        // Bind parameters (same potentially brittle logic as before)
        for (int i = 0; i < params_.size(); ++i) {
            const auto& param = params_[i];
            // --- TEMPORARY BINDING PLACEHOLDER ---
            if (param.type() == typeid(int)) {
                Detail::bindValue(
                    stmt_ptr.get(), i + 1, std::any_cast<int>(param)
                );
            }
            else if (param.type() == typeid(double)) {
                Detail::bindValue(
                    stmt_ptr.get(), i + 1, std::any_cast<double>(param)
                );
            }
            else if (param.type() == typeid(float)) {
                Detail::bindValue(
                    stmt_ptr.get(), i + 1, std::any_cast<float>(param)
                );
            }
            else if (param.type() == typeid(std::string)) {
                Detail::bindValue(
                    stmt_ptr.get(), i + 1, std::any_cast<std::string>(param)
                );
            }
            else if (param.type() == typeid(long long)) {
                Detail::bindValue(
                    stmt_ptr.get(), i + 1, std::any_cast<long long>(param)
                );
            }
            else {
                throw std::runtime_error(
                    "Unsupported parameter type for binding"
                );
            }
        }

        // Execute and fetch results into RowWrappers
        while ((rc = sqlite3_step(stmt_ptr.get())) == SQLITE_ROW) {
            auto rowTuple = Detail::createTupleFromRow<ColTypes...>(
                stmt_ptr.get(), std::make_index_sequence<sizeof...(ColTypes)>{}
            );
            // Construct RowWrapper with the tuple and shared pointers to metadata
            results.emplace_back(
                std::move(rowTuple), colNamesPtr_, nameToIndexMapPtr_
            );
        }

        if (rc != SQLITE_DONE) {
            throw SqliteException(
                "Failed to execute statement: " + std::string(sqlite3_errmsg(db_)),
                rc
            );
        }

        return results;
    }
};


// --- Table Handler (Modified for shared metadata) ---
template <typename... ColTypes>
class TableHandler {
public:
    using RowType = std::tuple<ColTypes...>;
    using WrappedRowType = RowWrapper<ColTypes...>; // Expose wrapper type

private:
    sqlite3* db_;
    std::string tableName_;
    // Store column names and the name->index map in shared_ptrs
    std::shared_ptr<const std::vector<std::string>> colNamesPtr_;
    std::shared_ptr<const std::map<std::string, size_t>> nameToIndexMapPtr_;

    // Private constructor, takes shared pointers
    TableHandler(
        sqlite3* db,
        std::string tableName,
        std::shared_ptr<const std::vector<std::string>> colNames,
        std::shared_ptr<const std::map<std::string, size_t>> nameToIndexMap
    )
        : db_(db),
        tableName_(std::move(tableName)),
        colNamesPtr_(std::move(colNames)),
        nameToIndexMapPtr_(std::move(nameToIndexMap)) {
        // Basic validation moved to DBHandler::openTable
    }

    friend class DBHandler;

public:
    void insert(const ColTypes&... values) {
        insert(std::make_tuple(values...));
    }

    void clear() {
        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(db_, ("DELETE FROM " + tableName_ + ";").c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            throw SqliteException(
                "Failed to prepare delete statement: " +
                std::string(sqlite3_errmsg(db_)),
                rc
            );
        }
        std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt_ptr(
            stmt, sqlite3_finalize
        );
        rc = sqlite3_step(stmt_ptr.get());
        if (rc != SQLITE_DONE) {
            throw SqliteException(
                "Failed to execute delete statement: " +
                std::string(sqlite3_errmsg(db_)),
                rc
            );
        }
    }

    void insert(const RowType& row) {
        if (!colNamesPtr_) {
            throw std::logic_error("Column names pointer is null in TableHandler.");
        }
        std::string sql = "INSERT INTO " + tableName_ + " (";
        std::string placeholders = "";
        for (size_t i = 0; i < colNamesPtr_->size(); ++i) {
            sql += (*colNamesPtr_)[i] +
                (i == colNamesPtr_->size() - 1 ? "" : ", ");
            placeholders += "?" +
                std::string(i == colNamesPtr_->size() - 1 ? "" : ", ");
        }
        sql += ") VALUES (" + placeholders + ");";

        //std::cout << "Executing SQL: " << sql << std::endl;

        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            throw SqliteException(
                "Failed to prepare insert statement: " +
                std::string(sqlite3_errmsg(db_)),
                rc
            );
        }
        std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt_ptr(
            stmt, sqlite3_finalize
        );

        Detail::bindTuple(stmt_ptr.get(), row);

        rc = sqlite3_step(stmt_ptr.get());
        if (rc != SQLITE_DONE) {
            throw SqliteException(
                "Failed to execute insert statement: " +
                std::string(sqlite3_errmsg(db_)),
                rc
            );
        }
        std::cout << "Insertion successful." << std::endl;
    }

    // Returns a QueryBuilder
    QueryBuilder<ColTypes...> select() {
        // Pass the shared pointers to the builder
        return QueryBuilder<ColTypes...>(
            db_, tableName_, colNamesPtr_, nameToIndexMapPtr_
        );
    }

    const std::string& getTableName() const { return tableName_; }
    const std::vector<std::string>& getColumnNames() const {
        if (!colNamesPtr_) static std::vector<std::string> empty; // Should not happen
        return *colNamesPtr_;
    }
};


// --- Database Handler (Modified for CREATE TABLE IF NOT EXISTS) ---
class DBHandler {
public:
    SqliteConnection connection_; // Owns the connection

    DBHandler(const std::string& dbPath) : connection_(dbPath) {}

    template <typename... ColTypes>
    TableHandler<ColTypes...> openTable(
        const std::string& tableName, std::initializer_list<std::string> colNamesVec
    ) {
        // 1. Validate inputs
        if (colNamesVec.size() != sizeof...(ColTypes)) {
            throw std::invalid_argument(
                "Number of column names (" + std::to_string(colNamesVec.size()) +
                ") does not match number of template type arguments (" +
                std::to_string(sizeof...(ColTypes)) + ")"
            );
        }
        if (tableName.empty()) {
            throw std::invalid_argument("Table name cannot be empty.");
        }
        // Could add validation for column names (e.g., not empty, valid chars)

        // 2. Create Table If Not Exists
        try {
            using TupleType = std::tuple<ColTypes...>;
            std::string colsDef = Detail::buildColumnDefinitions<TupleType>(
                colNamesVec, std::make_index_sequence<sizeof...(ColTypes)>{}
            );
            std::string createSql = "CREATE TABLE IF NOT EXISTS " + tableName +
                " (" + colsDef + ");";
            connection_.execute(createSql); // Use connection's execute method
        }
        catch (const SqliteException& e) {
            // More specific error handling might be needed
            throw SqliteException(
                "Failed to ensure table '" + tableName + "' exists: " + e.what()
            );
        }
        catch (const std::exception& e) {
            throw std::runtime_error(
                "Error during table creation setup for '" + tableName + "': " + e.what()
            );
        }


        // 3. Create shared metadata
        auto colNamesPtr =
            std::make_shared<const std::vector<std::string>>(std::move(colNamesVec));
        auto nameToIndexMapPtr =
            std::make_shared<std::map<std::string, size_t>>();
        for (size_t i = 0; i < colNamesPtr->size(); ++i) {
            (*nameToIndexMapPtr)[(*colNamesPtr)[i]] = i;
        }
        // Make map const after population (though shared_ptr already points to const map)
        // For true constness, copy to a const map if needed, but shared_ptr<const map> is fine.

        // 4. Create and return the handler
        return TableHandler<ColTypes...>(
            connection_.get(), tableName, std::move(colNamesPtr), std::move(nameToIndexMapPtr)
        );
    }

    // Overload for C-style string literals for column names
    template <typename... ColTypes, size_t N>
    TableHandler<ColTypes...> openTable(
        const std::string& tableName, const char* (&colNamesArr)[N]
    ) {
        std::vector<std::string> colNamesVec(colNamesArr, colNamesArr + N);
        return openTable<ColTypes...>(tableName, std::move(colNamesVec));
    }

    // Allow direct SQL execution if needed (delegated to connection_)
    void execute(const std::string& sql) { connection_.execute(sql); }
};

DBHandler db("local_storage.db");


namespace Serialization {

template<typename T>
struct TypeSerializer;

} // namespace Serialization


template<class val, class key>
class kvtable {
private:
    // The underlying table handler.
    // For a key `pos` (3x long long) and value `chsaveable` (serialized to string/blob),
    // the table structure is <long long, long long, long long, std::string>.
    TableHandler<long long, long long, long long, std::string> tableHandler_;
    std::string tableName_;

public:
    // Constructor takes the table name and initializes the handler.
    kvtable(std::string tableName)
        : tableName_((tableName)),
        tableHandler_(db.openTable<long long, long long, long long, std::string>(
            tableName,
            { "key_x", "key_y", "key_z", "value_blob" }
        ))
    {
        // We can also create a primary key on the key columns for efficiency.
        // This only runs once if the table is new.
        std::string sql = "CREATE UNIQUE INDEX IF NOT EXISTS idx_pos ON " +
            tableName_ + " (key_x, key_y, key_z);";
        db.execute(sql);
    }

    // Writes (inserts or replaces) a key-value pair.
    void write(const key& k, const val& v) {
        // First, delete any existing entry with the same key to avoid conflicts.
        // This makes the write operation idempotent (like INSERT OR REPLACE).
        // A more optimized way would be a single "INSERT OR REPLACE" SQL command,
        // but this approach uses the existing TableHandler API.

        // A simple DELETE statement
        std::string deleteSql = "DELETE FROM " + tableName_ +
            " WHERE key_x = ? AND key_y = ? AND key_z = ?;";
        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(db.connection_.get(), deleteSql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) throw SqliteException("Failed to prepare delete statement", rc);
        std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt_ptr(stmt, sqlite3_finalize);

        sqlite3_bind_int64(stmt_ptr.get(), 1, k.x);
        sqlite3_bind_int64(stmt_ptr.get(), 2, k.y);
        sqlite3_bind_int64(stmt_ptr.get(), 3, k.z);

        rc = sqlite3_step(stmt_ptr.get());
        if (rc != SQLITE_DONE) {
            throw SqliteException("Failed to execute delete for write operation", rc);
        }

        // Now, insert the new record.
        std::string blob = Serialization::TypeSerializer<val>::serialize(v);
        tableHandler_.insert(k.x, k.y, k.z, blob);
    }

    // Reads a value by its key.
    // The signature matches your request: it fills pointers for the value and existence.
    // Note: A more modern C++ approach would be to return std::optional<val>.
    void read(const key& k, val* out_val, bool* out_exist) {
        if (!out_val || !out_exist) {
            throw std::invalid_argument("Output pointers for read must not be null.");
        }

        auto results = tableHandler_.select()
            .filterEq<0>(k.x)
            .filterEq<1>(k.y)
            .filterEq<2>(k.z)
            .compute();

        if (results.empty()) {
            *out_exist = false;
        }
        else {
            *out_exist = true;
            // Get the blob from the 4th column (index 3)
            const std::string& blob = results.front().get<3>();
            // Deserialize the blob into the user-provided struct
            Serialization::TypeSerializer<val>::deserialize(blob, *out_val);
        }
    }
};