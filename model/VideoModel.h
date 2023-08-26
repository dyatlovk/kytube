#pragma once

#include <QAbstractTableModel>

#include "../youtube/YoutubeApi.h"

class VideoModel : public QAbstractTableModel
{
  struct RowData
  {
    std::string title;
    std::string created;
    std::string channelTitle;
    std::string videoId;
    bool isStream;
  };

  Q_OBJECT
public:
  explicit VideoModel(QObject *parent = nullptr);
  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
  [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
  [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  auto AppendData(const RowData &data) -> void;
  auto ResetModel() -> void;
  auto FindDataByIndex(const QModelIndex &index) -> const RowData;
  auto RequestApi(std::basic_string<char> uri, int max = 1, const std::string &key = "", const std::string &api = "", const std::string &pageToken = "") -> void;
  auto getParsedData() -> youtube::api::Main * { return parsedData; };
  auto GetQuery() -> const std::string & { return currentQuery; };

private:
  QVector<RowData> m_data;
  youtube::api::Main *parsedData;
  std::string currentQuery;
};
