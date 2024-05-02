#include "search.hpp"

#include <future>
#include <string>

#include "core/network/request.hpp"
#include "core/providers/piped/Common.hpp"
#include "core/providers/piped/PipedException.h"
#include "core/providers/piped/Search.h"

namespace models
{
  search::search(QObject *parent)
      : QAbstractTableModel(parent)
      , currentQuery("")
      , parsedData_()
  {
  }

  search::~search() {}

  int search::rowCount(const QModelIndex &parent) const
  {
    const auto count = m_data.count();
    return count;
  }

  int search::columnCount(const QModelIndex &parent) const
  {
    return 5;
  }

  QVariant search::data(const QModelIndex &index, int role) const
  {
    int row = index.row();
    int col = index.column();
    const auto dataRow = m_data.at(row);

    if (role == Qt::DisplayRole)
    {
      if (col == 0)
      {
        return row + 1;
      }
      if (col == 1)
      {
        return QString(dataRow.title.c_str());
      }
      if (col == 2)
      {
        return QString(dataRow.created.c_str());
      }
      if (col == 3)
      {
        return QString(dataRow.uploader.c_str());
      }
      if (col == 4)
      {
        return QString(dataRow.type.c_str());
      }
    }

    return {};
  }

  QVariant search::headerData(int section, Qt::Orientation orientation, int role) const
  {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
      switch (section)
      {
      case 0:
        return QString("#");
      case 1:
        return QString("Title");
      case 2:
        return QString("Published");
      case 3:
        return QString("Uploader");
      case 4:
        return QString("Type");
      default:
        return QString("");
      }
    }

    return {};
  }

  auto search::AppendData(const RowData &data) -> void
  {
    int row = rowCount(QModelIndex());
    beginInsertRows(QModelIndex(), row, row);
    m_data.push_back(data);
    endInsertRows();
  }

  auto search::ResetModel() -> void
  {
    m_data.clear();
    this->beginResetModel();
    this->resetInternalData();
    this->endResetModel();
  }

  auto search::FindDataByIndex(const QModelIndex &index) const -> RowData
  {
    const auto found = m_data.at(index.row());
    return found;
  }

  auto search::Search(const std::string &url, const std::string &query) -> void
  {
    currentQuery = query;
    const auto request = new network::request();
    const auto searchProvider = new piped::search();
    const auto response = request->Get(url);
    try
    {
      searchProvider->MakeFromString(response);
    }
    catch (piped::Exception &e)
    {
      std::cout << e.what() << std::endl;
      return;
    }
    const auto parsed = searchProvider->getParsedData();
    parsedData_ = parsed;

    for (const auto &item : parsedData_.items)
    {
      const std::string id = piped::FindVideoId(item.url);
      std::string uploadedDate = "";
      if (item.uploadedDate != nullptr)
      {
        uploadedDate = *item.uploadedDate;
      }
      AppendData({item.url, item.title, uploadedDate, item.uploaderName, id, item.type});
    }

    delete searchProvider;
    delete request;

    emit searchComplete();
  }

  auto search::SearchAsync(const std::string &url, const std::string &query) -> void
  {
    auto futureResult = std::async(std::launch::async, &search::Search, this, url, query);
    asyncResult = std::move(futureResult);
  }
} // namespace models
