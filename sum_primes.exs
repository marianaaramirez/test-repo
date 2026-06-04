# Mariana Ramirez
# A01787819

defmodule PrimeSum do

  def prime?(number) do
    cond do
      number < 2 ->
        false

      number == 2 ->
        true

      rem(number, 2) == 0 ->
        false

      true ->
        limit = :math.sqrt(number) |> trunc()
        check_divisors(number, 3, limit)
    end
  end

  def check_divisors(_number, divisor, limit) when divisor > limit do
    true
  end

  def check_divisors(number, divisor, limit) do
    if rem(number, divisor) == 0 do
      false
    else
      check_divisors(number, divisor + 2, limit)
    end
  end

  def sequential_sum(n) do
    Enum.reduce(2..n, 0, fn number, total ->
      if prime?(number) do
        total + number
      else
        total
      end
    end)
  end

  def sum_interval(first, last) do
    Enum.reduce(first..last, 0, fn number, total ->
      if prime?(number) do
        total + number
      else
        total
      end
    end)
  end

  def create_ranges(n, tasks) do
    size = ceil(n / tasks)

    Enum.map(0..(tasks - 1), fn i ->
      first = i * size + 1
      last = min((i + 1) * size, n)
      {first, last}
    end)
  end

  def parallel_sum(n, tasks \\ System.schedulers_online()) do
    ranges = create_ranges(n, tasks)

    task_list =
      Enum.map(ranges, fn range ->
        Task.async(fn ->
          {first, last} = range
          sum_interval(first, last)
        end)
      end)

    results =
      Enum.map(task_list, fn task ->
        Task.await(task, :infinity)
      end)

    Enum.sum(results)
  end

  def run(n) do
    sequential = sequential_sum(n)
    parallel = parallel_sum(n)

    IO.puts("Sequential result: #{sequential}")
    IO.puts("Parallel result: #{parallel}")
    IO.puts("Same result? #{sequential == parallel}")
  end
end

n =
  case System.argv() do
    [value] -> String.to_integer(value)
    _ -> 1_000_000
  end

PrimeSum.run(n)