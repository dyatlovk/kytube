#pragma once

#include <QAbstractTableModel>
#include <core/providers/piped/Search.h>
#include <qpixmap.h>
#include <qstyleditemdelegate.h>

namespace TestQt::models
{
  class search : public QAbstractTableModel
  {
    Q_OBJECT

    struct RowData
    {
      QPixmap thumb;
      std::string url;
      std::string title;
      std::string created;
      std::string uploader;
      std::string videoId;
      std::string type;
    };

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
    auto createThumbPlaceholder() -> QPixmap;

  private:
    QVector<RowData> m_data;
    std::string currentQuery;
    piped::search::Response parsedData_;
  };
} // namespace TestQt::models
