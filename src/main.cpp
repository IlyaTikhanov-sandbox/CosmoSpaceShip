#include <glog/logging.h>
#include <iostream>

int
main(int argc, char* argv[])
{
#pragma region "Init"
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = true;
  FLAGS_log_dir = "..\\..\\..\\..\\log";
  LOG(INFO) << "ZARA SEE YOU!";
#pragma endregion

  return 0;
}