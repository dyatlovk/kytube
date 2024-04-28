#include "network/request.hpp"
#include "piped/search.hpp"
#include "test/datetime/datetime.hpp"

int main()
{
  TestNetwork::Get();
  TestNetwork::UrlDecode();
  TestNetwork::UrlEncode();
  TestNetwork::UrlEncodeCyrrylic();
  TestNetwork::UrlDecodeCyrrylic();
  TestPipedSearch::ParseFromDump();
  TestPipedSearch::ParseRequest();
  TestDateTime::Format();
  return 0;
}
