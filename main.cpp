#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <stdexcept>

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

void sumPart(const data_t& data, size_t begin, size_t end, value_t& result)
{
  result = 0;
  for (size_t i = begin; i < end; ++i)
  {
    result += data[i];
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Wrong number of arguments\n";
    return 1;
  }

  if (argv[1][0] == '-')
  {
    std::cerr << "Threads number must be positive\n";
    return 1;
  }

  size_t threadCount = 0;
  try
  {
    threadCount = std::stoul(argv[1]);
    if (threadCount == 0)
    {
      throw std::invalid_argument("Number of threads can't be 0");
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << "Invalid input" << '\n';
    return 1;
  }

  data_t data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
  size_t size = data.size();

  size_t partSize = size / threadCount;

  std::vector< std::thread > threads;
  std::vector< value_t > sums(threadCount);

  for (size_t i = 0; i < threadCount; ++i)
  {
    size_t begin, end;
    begin = i * partSize;
    if (i == threadCount - 1)
    {
      end = size;
    }
    else
    {
      end = (i + 1) * partSize;
    }
    threads.push_back(std::thread(sumPart, std::cref(data), begin, end, std::ref(sums[i])));
  }

  for (size_t i = 0; i < threads.size(); ++i)
  {
    threads[i].join();
  }

  value_t res = 0;
  for (size_t i = 0; i < sums.size(); ++i)
  {
    res += sums[i];
  }

  std::cout << res << '\n';
  return 0;
}
