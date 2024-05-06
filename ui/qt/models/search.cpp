#include "search.hpp"

#include <future>
#include <qabstractitemmodel.h>
#include <qicon.h>
#include <qpixmap.h>
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
    return 6;
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
      if (col == 2)
      {
        return QString(dataRow.title.c_str());
      }
      if (col == 3)
      {
        return QString(dataRow.created.c_str());
      }
      if (col == 4)
      {
        return QString(dataRow.uploader.c_str());
      }
      if (col == 5)
      {
        return QString(dataRow.type.c_str());
      }
    }

    if (role == Qt::DecorationRole && col == 1)
    {
      return QIcon(dataRow.thumb);
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
        return QString("Thumb");
      case 2:
        return QString("Title");
      case 3:
        return QString("Published");
      case 4:
        return QString("Uploader");
      case 5:
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
    const auto imgPlaceholder = this->createThumbPlaceholder();
    const auto parsed = searchProvider->getParsedData();
    parsedData_ = parsed;

    int c = 0;
    for (const auto &item : parsedData_.items)
    {
      const std::string id = piped::FindVideoId(item.url);
      std::string uploadedDate = "";
      if (item.uploadedDate != nullptr)
      {
        uploadedDate = *item.uploadedDate;
      }
      AppendData({imgPlaceholder, item.url, item.title, uploadedDate, item.uploaderName, id, item.type});
      this->loadImageAsync(item.thumbnail, c);
      c++;
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

  auto search::createThumbPlaceholder() -> QPixmap
  {
    QPixmap placeholder(178, 100);
    placeholder.fill(Qt::lightGray);
    return placeholder;
  }

  auto search::loadImage(const std::string &url, int index) -> void
  {
    network::request request;
    const auto response = request.Get(url);

    QByteArray bytes(response.c_str(), response.length());
    QPixmap image;
    const auto imageLoaded = image.loadFromData(bytes);
    if (!imageLoaded)
    {
      std::cout << "image loading error: " << url << std::endl;
      return;
    }

    QModelIndex idx = this->index(index, 1);
    if (!idx.isValid())
    {
      std::cout << "index not valid: " << idx.row() << std::endl;
      return;
    }

    m_data[index].thumb = image;
  }

  auto search::loadImageAsync(const std::string &url, int index) -> void
  {
    auto future = std::async(std::launch::async, &search::loadImage, this, url, index);
    asyncImagesResult = std::move(future);
  }
} // namespace models
