#include "log.hpp"

namespace models
{
  log::log(QObject *parent)
      : QAbstractTableModel(parent)
  {
  }

  int log::rowCount(const QModelIndex &parent) const
  {
    return m_data.count();
  }

  int log::columnCount(const QModelIndex &parent) const
  {
    return 3;
  }

  QVariant log::data(const QModelIndex &index, int role) const
  {
    int row = index.row();
    int col = index.column();
    const auto dataRow = m_data.at(row);

    if (role == Qt::DisplayRole)
    {
      if (col == 0)
      {
        return QString(dataRow.date.c_str());
      }
      if (col == 1)
      {
        return QString(dataRow.reason.c_str());
      }
      if (col == 2)
      {
        return QString(dataRow.message.c_str());
      }
    }

    return {};
  }

  QVariant log::headerData(int section, Qt::Orientation orientation, int role) const
  {
    switch (section)
    {
    case 0:
      return QString("Date");
    case 1:
      return QString("Reason");
    case 2:
      return QString("Message");
    }

    return {};
  }

  auto log::AppendData(const RowData &data) -> void
  {
    int row = rowCount(QModelIndex());
    beginInsertRows(QModelIndex(), row, row);
    m_data.push_back(data);
    endInsertRows();
  }

  auto log::ResetModel() -> void
  {
    m_data.clear();
    this->beginResetModel();
    this->resetInternalData();
    this->endResetModel();
  }
} // namespace models
