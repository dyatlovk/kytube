#pragma once

#include <QAbstractTableModel>

class LogModel : public QAbstractTableModel
{
  struct RowData
  {
    std::string date;
    std::string reason;
    std::string message;
  };

  Q_OBJECT
public:
  explicit LogModel(QObject *parent = nullptr);
  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
  [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
  [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  auto AppendData(const RowData &data) -> void;
  auto ResetModel() -> void;
  auto FindDataByIndex(const QModelIndex &index) -> const RowData;

private:
  QVector<RowData> m_data;
};
