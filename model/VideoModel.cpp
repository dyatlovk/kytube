#include "VideoModel.h"

#include <QTime>
#include <algorithm>
#include <curl/curl.h>
#include <fstream>
#include <iostream>

struct MemoryStruct
{
  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

VideoModel::VideoModel(QObject *parent)
    : QAbstractTableModel(parent)
    , parsedData(nullptr)
{
}

int VideoModel::rowCount(const QModelIndex &parent) const
{
  const auto count = m_data.count();
  return count;
}

int VideoModel::columnCount(const QModelIndex &parent) const
{
  return 5;
}

QVariant VideoModel::data(const QModelIndex &index, int role) const
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
      return QString(dataRow.channelTitle.c_str());
    }
    if (col == 4)
    {
      return (dataRow.isStream) ? QString("+") : QString("");
    }
  }

  return {};
}

QVariant VideoModel::headerData(int section, Qt::Orientation orientation, int role) const
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
      return QString("Channel");
    case 4:
      return QString("Live");
    default:
      return QString("");
    }
  }

  return {};
}

auto VideoModel::AppendData(const RowData &data) -> void
{
  int row = rowCount(QModelIndex());
  beginInsertRows(QModelIndex(), row, row);
  m_data.push_back(data);
  endInsertRows();
}

auto VideoModel::ResetModel() -> void
{
  m_data.clear();
  this->beginResetModel();
  this->resetInternalData();
  this->endResetModel();
}

auto VideoModel::FindDataByIndex(const QModelIndex &index) -> const RowData
{
  const auto found = m_data.at(index.row());

  return found;
}

auto VideoModel::RequestApi(std::basic_string<char> q, int max, const std::string &key, const std::string &api,
    const std::string &pageToken) -> void
{
  currentQuery = q;
  std::replace(q.begin(), q.end(), ' ', '+');
  std::string url = api + "?q=" + q + "&part=snippet&maxResults=" + std::to_string(max) + "&order=date&key=" + key;
  if (!pageToken.empty())
  {
    url.append("&pageToken=" + pageToken);
  }

  CURL *curl_handle;
  CURLcode res;

  struct MemoryStruct chunk
  {
  };

  chunk.memory = static_cast<char *>(malloc(1)); /* will be grown as needed by the realloc above */
  chunk.size = 0; /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers do not like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  /* get it! */
  res = curl_easy_perform(curl_handle);

  const auto youtube = new youtube::api();
  /* check for errors */
  if (res != CURLE_OK)
  {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }
  else
  {
    std::ofstream file;
    file.open("./api_log.json", std::ios_base::out | std::ios_base::ate);
    file << chunk.memory;
    file.close();
    youtube->MakeFromString(chunk.memory);
    const auto parsed = youtube->getParsedData();
    parsedData = const_cast<youtube::api::Main *>(parsed);
    for (const auto &item : parsed->items)
    {
      std::string id;
      if (item.id->videoId)
      {
        id.append(*item.id->videoId);
      }
      if (item.id->channelId)
      {
        id.append(*item.id->channelId);
      }
      if (item.id->playlistId)
      {
        id.append(*item.id->playlistId);
      }
      bool isStream = item.snippet->leaveBroadcastContent == "live";
      AppendData({item.snippet->title, item.snippet->publishedAt, item.snippet->channelTitle, id, isStream});
    }
  }
  delete youtube;

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  free(chunk.memory);

  /* we are done with libcurl, so clean it up */
  curl_global_cleanup();
}

auto VideoModel::HasNextPage() const -> bool
{
  if (!parsedData)
    return false;

  return !parsedData->nextPageToken.empty();
}

auto VideoModel::HasPrevPage() const -> bool
{
  if (!parsedData)
    return false;

  return !parsedData->prevPageToken.empty();
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  auto *mem = (struct MemoryStruct *)userp;

  char *ptr = static_cast<char *>(realloc(mem->memory, mem->size + realsize + 1));
  if (!ptr)
  {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}
