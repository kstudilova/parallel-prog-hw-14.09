#include <iostream>
#include <functional>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <future>

#include "clicker.hpp"

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

value_t sumPart(const data_t& data, size_t begin, size_t end)
{
  value_t result = 0;
  for (size_t i = begin; i < end; ++i)
  {
    result += data[i];
  }
  return result;
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
  catch(const std::exception&)
  {
    std::cerr << "Invalid input" << '\n';
    return 1;
  }

  constexpr size_t size = 1'000'000'000;
  data_t data(size);

  for (size_t i = 0; i < size; ++i)
  {
    data[i] = 1;
  }

  std::vector< double > times(5);

  value_t res = 0;
  for (size_t attempt = 0; attempt < 5; ++attempt)
  {
    Clicker clicker;

    size_t partSize = size / threadCount;

    std::vector< std::future< value_t > > futures;
    futures.reserve(threadCount);

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
      futures.push_back(std::async(std::launch::async, sumPart, std::cref(data), begin, end));
    }

    res = 0;
    for (size_t i = 0; i < futures.size(); ++i)
    {
      res += futures[i].get();
    }

    times[attempt] = clicker.millisec();
  }

  std::sort(times.begin(), times.end());

  for (size_t i = 0; i < times.size(); ++i)
  {
    std::cout << times[i] << " ms\n";
  }

  std::cout << "Median: " << times[2] << " ms\n";
  std::cout << "Total sum: " << res << '\n';
  return 0;
}
