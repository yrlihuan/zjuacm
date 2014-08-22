import org.scalatest.FunSuite

/**
 * Created by huan on 8/12/14.
 */
class HelloTest extends FunSuite {
  test("sayHello method works correctly") {
    val hello = new Hello
    assert(hello.sayHello("Scala") == "Hello, Scala!")
  }
}

class SumTest extends FunSuite {
  test("test sum") {
    val s = new Sum
    assert(s.sum(x => x)(1, 10) == 55)
    assert(s.sum(x => x)(1, 100) == 5050)
  }

  test("test square") {
    val s = new Sum
    assert(s.sum(x => x * x)(1, 10) == 385)
    assert(s.sum(x => x * x)(1, 100) == 338350)
  }

  test("test currying") {
    val s = new Sum
    val f = s.sum(x => x * x)

    assert(f(1, 10) == 385)
  }
}