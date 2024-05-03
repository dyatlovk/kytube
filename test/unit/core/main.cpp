#include "datetime/datetime.hpp"
#include "network/request.hpp"
#include "piped/search.hpp"
#include "piped/stream.hpp"
#include "sys/shell.hpp"

int main()
{
  TestNetwork::Get();
  TestNetwork::UrlDecode();
  TestNetwork::UrlEncode();
  TestNetwork::UrlEncodeCyrrylic();
  TestNetwork::UrlDecodeCyrrylic();
  TestPipedSearch::ParseFromDump();
  TestPipedSearch::ParseRequest();
  TestPipedSearch::GetVideId();
  TestPipedStream::ParseFromDump();
  TestDateTime::Format();
  TestCore::Run();
  return 0;
}