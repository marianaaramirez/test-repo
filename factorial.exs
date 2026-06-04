#Mariana Ramirez
#A01787819

defmodule Paralel do
  def multiply_range({start,finish})do
    Enum.product(start..finish)
  end

  def greet(id),do: IO.puts("Helo from task #{id}")

  def testTask(n) do
    1..n
    |>Enum.map(&Task.async(fn ->greet(&1) end))
    |>Enum.map(&Task.await(&1))
  end

  def factorial_range({start,stop})do
    Enum.reduce(start..stop,1,&(&1*&2))
  end

  def paralel_factorial(n,task \\ System.schedulers)do
    step = ceil(n/ task)
    start = [1 | Enum.to_list(step+1..n//step)]
    finishes = Enum.to_list(step..n//step)

    Enum.zip(start,finishes)
    |>IO.inspect()
    |>Enum.map(&Task.async(fn -> factorial_range(&1)end))
    |>Enum.map(&Task.await(&1,:infinity))

    |>Enum.reduce(1,&(&1*&2))
  end
end