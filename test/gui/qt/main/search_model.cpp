#include "search_model.hpp"

#include <fstream>
#include <qicon.h>

#include "Config.hpp"

namespace TestQt::models
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
    { // Отображение иконок только для второго столбца
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
    std::string p(YoutubeQt::ROOT_DIR);
    std::ifstream f(p + "/test/Fixtures/piped/search.json");
    std::ostringstream response;
    response << f.rdbuf();
    f.close();

    currentQuery = query;
    std::ifstream fixture;

    const auto searchProvider = new piped::search();
    searchProvider->MakeFromString(response.str());
    const auto parsed = searchProvider->getParsedData();
    parsedData_ = parsed;

    const auto imgPlaceholder = this->createThumbPlaceholder();

    for (const auto &item : parsedData_.items)
    {
      std::string id;
      id.append(item.url);
      id.append(item.title);
      std::string uploadedDate = "";
      if (item.uploadedDate)
        uploadedDate = *item.uploadedDate;
      id.append(uploadedDate);
      id.append(item.uploaderName);
      id.append(item.url);
      id.append(item.type);
      AppendData({imgPlaceholder, item.url, item.title, uploadedDate, item.uploaderName, id, item.type});
    }

    delete searchProvider;
  }

  auto search::createThumbPlaceholder() -> QPixmap
  {
    QPixmap placeholder(200, 100);
    placeholder.fill(Qt::lightGray);
    return placeholder;
  }
} // namespace TestQt::models
