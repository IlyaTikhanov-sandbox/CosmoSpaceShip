#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <string>

struct Asset
{
  std::string name;
  std::string path;
};

class AssetManager
{
public:
  AssetManager(const std::string& dbPath)
    : database(dbPath)
  {
  }

  Asset GetAssetByName(const std::string& assetName)
  {
    Asset asset;

    try {
      SQLite::Database db(database);
      SQLite::Statement query(db, "SELECT name, path FROM Assets WHERE name = ?");
      query.bind(1, assetName);

      if (query.executeStep()) {
        asset.name = query.getColumn("name").getString();
        asset.path = query.getColumn("path").getString();
      }
    } catch (std::exception& e) {
      std::cerr << "SQLite exception: " << e.what() << std::endl;
    }

    return asset;
  }

private:
  std::string database;
};