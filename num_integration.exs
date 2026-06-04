# Mariana Ramirez
# A01787819

defmodule NumericalIntegration do

  def f(x) do
    4.0 / (1.0 + x * x)
  end

  def sequential(n) do
    width = 1.0 / n

    sum =
      Enum.reduce(0..(n - 1), 0.0, fn i, acc ->
        mid = (i + 0.5) * width
        acc + f(mid)
      end)

    sum * width
  end

  def sum_range(start, finish, n) do
    width = 1.0 / n

    Enum.reduce(start..finish, 0.0, fn i, acc ->
      mid = (i + 0.5) * width
      acc + f(mid)
    end)
  end

  def parallel(n) do
    cores = System.schedulers_online()
    size = div(n, cores)

    sum =
      1..cores
      |> Enum.map(fn i ->
        start = (i - 1) * size

        finish =
          if i == cores do
            n - 1
          else
            i * size - 1
          end

        Task.async(fn -> sum_range(start, finish, n) end)
      end)
      |> Enum.map(fn task -> Task.await(task, :infinity) end)
      |> Enum.sum()

    sum * (1.0 / n)
  end

  def run(n) do
    seq = sequential(n)
    par = parallel(n)

    IO.puts("Sequential pi: #{seq}")
    IO.puts("Parallel pi: #{par}")
    IO.puts("Reference pi: 3.141592653589793")
  end
end

n =  case System.argv() do
    [value] -> String.to_integer(value)
    _ -> 1_000_000
  end

NumericalIntegration.run(n)