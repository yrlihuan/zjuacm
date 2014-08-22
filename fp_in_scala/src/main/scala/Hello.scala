/**
 * Created by huan on 8/12/14.
 */
class Hello {
  def sayHello(name: String) = s"Hello, $name!"
}

class Sum {
  def sum(f: Int => Int): (Int, Int) => Int = {
    def sumF(from: Int, to: Int):Int =
      if (from > to) 0
      else f(from) + sum(f)(from + 1, to)

    sumF
  }
}
