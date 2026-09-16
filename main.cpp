#include <iostream>
#include <thread>
#include <functional>
#include <vector>

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

int main()
{
  size_t threadCount = 4;

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
