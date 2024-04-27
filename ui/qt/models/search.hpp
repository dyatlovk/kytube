#pragma once

#include <QAbstractTableModel>

#include "core/providers/piped/Search.h"

namespace models
{
  class search : public QAbstractTableModel
  {
    struct RowData
    {
      std::string url;
      std::string title;
      std::string created;
      std::string uploader;
      std::string videoId;
      std::string type;
    };

    Q_OBJECT

  public:
    explicit search(QObject *parent = nullptr);

    ~search();

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    auto AppendData(const RowData &data) -> void;
    auto ResetModel() -> void;
    [[nodiscard]] auto FindDataByIndex(const QModelIndex &index) const -> RowData;
    auto Search(const std::string &url, const std::string &query) -> void;
    auto GetQuery() -> std::string & { return currentQuery; };
    auto GetParsedData() -> piped::search::Response { return parsedData_; };

  private:
    QVector<RowData> m_data;
    std::string currentQuery;
    piped::search::Response parsedData_;
  };
} // namespace models
